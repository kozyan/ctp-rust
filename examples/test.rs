
use std::env;
use std::time::{Duration, Instant};
use std::io::{Write, Read};
use std::os::raw::*;
use std::ffi::{CStr, CString};
use std::collections::{HashMap, HashSet};
use std::path::Path;
use std::sync::{Arc, Mutex, Condvar};

use log::*;
use crossbeam::{channel::{self, Sender, Receiver}, select};
use serde::{Serialize, Deserialize};

fn main() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let platform = if cfg!(target_family = "windows") { "windows" } else { "unix" };
    let arch = if cfg!(target_arch = "x86_64") {
        "x86_64"
    } else if cfg!(target_arch = "x86") {
        "x86"
    } else {
        panic!("can not build on this platform.")
    };

    println!("Path: {}", format!("{}.{}", platform, arch));

    let path = env!("OUT_DIR");
    println!("out_dir: {}", path);
}