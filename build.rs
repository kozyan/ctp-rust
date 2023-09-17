#![allow(dead_code)]
#![allow(unused_variables)]

use lazy_static::lazy_static;
use regex::Regex;
use std::path::{Path, PathBuf};
use std::{env, fs};

fn main() {
    //fast_log::init(fast_log::Config::new().console()).expect("rbatis init fail");

    let root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let platform = if cfg!(target_family = "windows") {
        "windows"
    } else {
        "unix"
    };
    let arch = if cfg!(target_arch = "x86_64") {
        "x86_64"
    } else if cfg!(target_arch = "x86") {
        "x86"
    } else {
        panic!("can not build on this platform.")
    };

    println!("Path: {}", format!("{}.{}", platform, arch));

    cc::Build::new()
        .cpp(true)
        .file("src/wrapper.cpp")
        .flag_if_supported("-std=c++17")
        .flag_if_supported("-w")
        .compile("wrapper");

    println!(
        "cargo:rustc-link-search={}",
        root.join("shared/md")
            .join(format!("{}.{}", platform, arch))
            .display()
    );
    println!(
        "cargo:rustc-link-search={}",
        root.join("shared/td")
            .join(format!("{}.{}", platform, arch))
            .display()
    );
    println!(
        "cargo:rustc-link-search={}",
        root.join("shared/data_collect")
            .join(format!("{}.{}", platform, arch))
            .display()
    );

    if platform == "unix" {
        println!("cargo:rustc-link-lib=dylib=LinuxDataCollect");
    } else {
        println!("cargo:rustc-link-lib=dylib=WinDataCollect");
    }
    println!("cargo:rustc-link-lib=dylib=thostmduserapi_se");
    println!("cargo:rustc-link-lib=dylib=thosttraderapi_se");

    // Tell cargo to invalidate the built crate whenever the wrapper changes
    println!("cargo:rerun-if-changed=src/ctpmd.cpp");
    println!("cargo:rerun-if-changed=src/ctptd.cpp");
    println!("cargo:rerun-if-changed=src/wrapper.cpp");

    // ctp api header is clean enough, we will use blacklist instead whitelist
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header("src/wrapper.cpp")
        // Tell cargo to invalidate the built crate whenever any of the
        // included header files changed.
        // .derive_debug(true)
        .derive_default(false)
        // make output smaller
        .layout_tests(false)
        .generate_comments(true)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        // we will handle class mannually by `autobind.py`
        // function defined in rust
        .opaque_type("CThostFtdcTraderApi")
        .opaque_type("CThostFtdcTraderSpi")
        .opaque_type("CThostFtdcMdApi")
        .opaque_type("CThostFtdcMdSpi")
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");

    //NOTE: 产生 bindgen 文件，由于 rust-bindgen 有 bug，先 mark 掉这段代码以使 generated/mod.rs 不会被覆盖。+++++++++++++
    // let outdir = PathBuf::from("src").join("generated");
    // std::fs::create_dir_all(outdir).expect("Couldn't create dir generated!");
    // let outfile = PathBuf::from("src").join("generated/mod123.rs");
    // // std::fs::write(&outfile, &output).expect("Couldn't write bindings!");
    // bindings
    //     .write_to_file(&outfile)
    //     .expect("Couldn't write bindings!");
    // let buf = replace_trait(&outfile, &["Rust_CThostFtdcMdSpi_Trait", "Rust_CThostFtdcTraderSpi_Trait"]).
    //     expect("Fail to replace trait!");
    // std::fs::write(&outfile, &buf)
    //     .expect("Fail to write converted bindings!");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //copy_resource_file_to_output(format!("{}.{}", platform, arch));
}

fn camel_to_snake<'t>(name: &'t str) -> String {
    lazy_static! {
        static ref PATTERN1: Regex = Regex::new(r"(.)([A-Z][a-z]+)").unwrap();
        static ref PATTERN2: Regex = Regex::new(r"([a-z0-9])([A-Z])").unwrap();
    }
    PATTERN2
        .replace_all(
            PATTERN1.replace_all(name, r"${1}_${2}").as_ref(),
            r"${1}_${2}",
        )
        .to_lowercase()
}

fn replace_trait(fname: &Path, traits: &[&str]) -> Result<String, Box<dyn std::error::Error>> {
    let mut buf = std::fs::read_to_string(fname)?;
    for trait_extern in traits {
        let pattern = Regex::new(&format!(
            r#"extern \s*"C"\s*\{{\s*pub\s+fn\s+{}_(\w+)\s*\(([^)]*)\)([^;]*);\s*}}\s*"#,
            trait_extern
        ))
        .unwrap();
        let pattern_arg = Regex::new(r"\s*(\w+)\s*:\s*(.*)\s*").unwrap();

        let mut exports = vec![];
        let mut traitfuns = vec![];
        assert!(
            pattern.captures(&buf).is_some(),
            "{}",
            format!("`{}` not found in source code", trait_extern)
        );
        for cap in pattern.captures_iter(&buf) {
            let fname = cap.get(1).unwrap().as_str().trim();
            let args: Vec<_> = cap
                .get(2)
                .unwrap()
                .as_str()
                .split(",")
                .filter(|s| s.trim().len() > 0)
                .map(|s| {
                    let c = pattern_arg.captures(s).unwrap();
                    (c.get(1).unwrap().as_str(), c.get(2).unwrap().as_str())
                })
                .collect();
            let rtn = cap.get(3).unwrap().as_str();
            let fname_camel = camel_to_snake(fname);
            if fname_camel == "drop" {
                continue;
            }
            assert!(args[0].1.trim().ends_with("c_void"));

            let mut tmp = args[1..]
                .iter()
                .map(|s| format!("{}: {}", s.0, s.1))
                .collect::<Vec<_>>();
            tmp.insert(0, "trait_obj: *mut ::std::os::raw::c_void".into());
            let args_repl = tmp.join(", ");
            let argv_repl = args[1..].iter().map(|s| s.0).collect::<Vec<_>>().join(", ");

            let export = format!(
                r#"#[no_mangle]
pub extern "C" fn {trait_extern}_{fname}({args_repl}){rtn} {{
    let trait_obj = trait_obj as *mut Box<dyn {trait_extern}>;
    let trait_obj: &mut dyn {trait_extern} = unsafe {{ &mut **trait_obj }};
    trait_obj.{fname_camel}({argv_repl})
}}
"#,
                trait_extern = trait_extern,
                fname = fname,
                args_repl = args_repl,
                rtn = rtn,
                fname_camel = fname_camel,
                argv_repl = argv_repl
            );
            exports.push(export);

            let mut tmp = args[1..]
                .iter()
                .map(|s| format!("{}: {}", s.0, s.1))
                .collect::<Vec<_>>();
            tmp.insert(0, "&mut self".into());
            let args_repl = tmp.join(", ");
            let traitfun = format!(
                r"    fn {fname_camel}({args_repl}){rtn} {{  }}",
                fname_camel = fname_camel,
                args_repl = args_repl,
                rtn = rtn
            );
            traitfuns.push(traitfun);
        }

        let exports_repl = exports.join("\n");
        let traitfuns_repl = traitfuns.join("\n");

        buf = format!(
            r#"{ori}
#[allow(unused)]
pub trait {trait_extern} {{
{traitfuns_repl}
}}

{exports_repl}
#[no_mangle]
pub extern "C" fn {trait_extern}_Drop(trait_obj: *mut ::std::os::raw::c_void) {{
    let trait_obj = trait_obj as *mut Box<dyn {trait_extern}>;
    let _r: Box<Box<dyn {trait_extern}>> = unsafe {{ Box::from_raw(trait_obj) }};
}}
"#,
            ori = pattern.replace_all(&buf, "").to_string(),
            exports_repl = exports_repl,
            trait_extern = trait_extern,
            traitfuns_repl = traitfuns_repl
        );
    }

    Ok(buf)
}

fn get_output_path() -> PathBuf {
    //<root or manifest path>/target/<profile>/
    let manifest_dir_string = env::var("CARGO_MANIFEST_DIR").unwrap();
    let build_type = env::var("PROFILE").unwrap(); //debug or release
    let path = Path::new(&manifest_dir_string)
        .join("target")
        .join(build_type);

    println!("{}", path.display());

    return PathBuf::from(path);
}

fn copy_resource_file_to_output(version: String) {
    let file_paths = vec![
        FilePath {
            file_path: format!("shared/data_collect/{version}/WinDataCollect.dll"),
            target_path: "WinDataCollect.dll",
        },
        FilePath {
            file_path: format!("shared/md/{version}/thostmduserapi_se.dll"),
            target_path: "thostmduserapi_se.dll",
        },
        FilePath {
            file_path: format!("shared/td/{version}/thosttraderapi_se.dll"),
            target_path: "thosttraderapi_se.dll",
        },
    ];

    //let out_dir = env::var("OUT_DIR").unwrap();
    let target_dir = get_output_path();

    for p in file_paths.iter() {
        let src = Path::join(&env::current_dir().unwrap(), p.file_path.as_str());
        let dest = Path::join(Path::new(&target_dir), Path::new(p.target_path));

        //trace!("Commencing yak shaving");

        fs::copy(src, dest).unwrap();
    }
}

struct FilePath<'a> {
    file_path: String,
    target_path: &'a str,
}
