import string
from typing import List, Dict, Tuple, Optional
import itertools, re, os

def camel_to_snake(name: str):
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()

def walk_hpp(content: str, meta: Dict[str, 'ClsAst']):
    classstart = re.compile(r"class.*\s+(\w+)\s*\{")
    funccomment = re.compile(r"^\s*//(.*)\s*$")
    memberfunc = re.compile(r"^\s*(.*)\s+([\w*]+)\((.*)\)(.*);\s*$")
    match = classstart.search(content)
    while match != None:
        assert match is not None
        clsname, = match.groups()
        clsast = ClsAst(clsname)
        contents = content[match.end():].split("\n")
        curr = 0
        comments = []
        for line in contents:
            curr += 1
            if funccomment.match(line):
                match = funccomment.match(line)
                assert match is not None
                comments.append(match.groups()[0].lstrip("/"))
            elif memberfunc.match(line):
                match = memberfunc.match(line)
                assert match is not None
                rtn, funcname, args, decl = match.groups()

                left = sum(c == '{' for c in decl)
                right = sum(c == '}' for c in decl)
                assert left == right

                curr = 0
                for c in funcname:
                    if c != '*': break
                    curr += 1
                    rtn += '*'
                funcname = funcname[curr:]

                if rtn.strip().startswith("static"):
                    pass
                else:
                    clsast.funcs.append(ClsF(rtn, funcname, args, decl, comments))
                comments = []
            else:
                left = sum(c == '{' for c in line)
                right = sum(c == '}' for c in line)
                if left == 0 and right == 1: break

        meta[clsname] = clsast
        content = "\n".join(contents[curr:])
        match = classstart.search(content)


class ClsAst:
    name: str
    parent: List['ClsAst']
    funcs: List['ClsF']

    def __init__(self, name: str):
        self.name = name
        self.parent = []
        self.funcs = []

    # 只会产生cpp代码，cpp代码会被 bindgen 转录成对应的api
    def member_to_rust(self, meta: Dict[str, 'ClsAst'], is_m_member_owned: bool = True):
        rust_name = f"Rust_{self.name}"

        funcs = []
        impls = []
        for func in self.funcs:
            cdef, cimpl = func.member_to_rust(rust_name)
            impls.append(cimpl)
            funcs.append(cdef)

        funcs_repl = "\n".join([f"        {f}" for f in funcs])
        des = "delete m_member;" if is_m_member_owned else ""
        create_and_dest = [
            f"{rust_name}::{rust_name}({self.name} *member) : m_member(member) {{  }};",
            f"{rust_name}::~{rust_name}() {{ {des} }};"
        ]
        impls_repl = "\n".join(create_and_dest + impls)
        cdef = f"""
class {rust_name} {{
    public:
        {self.name} *m_member;
        {rust_name}({self.name} *member);
        ~{rust_name}();

{funcs_repl}
}};


{impls_repl}
"""
        #print(cdef)
        return(cdef)

    # 产生cpp代码，同时生成对应的rust代码
    def forward_to_rust(self, meta: Dict[str, 'ClsAst']) -> string:
        rust_name = f"Rust_{self.name}"

        funcs = []
        impls = []
        externcs = []
        for func in self.funcs:
            res = func.forward_to_rust(rust_name)
            if res is None: continue
            cdef, cimpl, externc = res
            impls.append(cimpl)
            funcs.append(cdef)
            externcs.append(externc)

        funcs_repl = "\n".join([f"        {f}" for f in funcs])
        impls_repl = "\n".join(impls)
        externcs_repl = "\n".join(externcs)
        cdef = f"""
class {rust_name} : {self.name} {{
    public:
        void *m_rust;
        {rust_name}(void *rust);
        ~{rust_name}();

{funcs_repl}
}};

{externcs_repl}
extern "C" void {rust_name}_Trait_Drop(void* m_rust);

{impls_repl}
{rust_name}::{rust_name}(void *rust) : m_rust(rust) {{}}
{rust_name}::~{rust_name}() {{ {rust_name}_Trait_Drop(m_rust); }}
"""

        #print(cdef)
        return(cdef)

class ClsF:
    def __init__(self, rtn, name, args, decl, comments):
        self.rtn = rtn
        self.name = name
        self.args = args
        self.decl = decl
        self.comments = comments

    def forward_to_rust(self, clsname: str) -> Optional[Tuple[str, str, str]]:
        rtn = self.rtn.strip()
        if rtn.startswith("virtual"):
            rtn = rtn.lstrip("virtual").strip()
        else:
            return None
        ARG_PATTERN = re.compile(r"^(.*)\s+([\w*]+)\s*([\[\]]*)$")

        args = []
        argvs = []

        for arg in self.args.split(","):
            arg = arg.strip()
            if len(arg) == 0: continue
            if arg.find("=") >= 0: arg = arg[:arg.find("=")]
            match = ARG_PATTERN.match(arg)
            assert match is not None
            a, v, b = match.groups()
            trim_star = 0
            for c in v:
                if c != '*': break
                trim_star += 1
                a += '*'
            v = v[trim_star:]
            args.append(f"{a} {v}{b}")
            argvs.append(v)

        args_repl = ", ".join(args)
        argvs_repl = ", ".join(["m_rust"] + argvs)
        args_repl_extern = ", ".join(["void* m_rust"] + args)

        trait_name = f"{clsname}_Trait"
        forward_func = f"{trait_name}_{self.name}"

        return (
            f"{rtn} {self.name}({args_repl}) override;",
            f"{rtn} {clsname}::{self.name}({args_repl}) {{ return {forward_func}({argvs_repl}); }}",
            f"extern \"C\" {rtn} {forward_func}({args_repl_extern});"
        )

    def member_to_rust(self, clsname: str) -> Tuple[str, str]:
        func_name_right = camel_to_snake(self.name)
        rtn = self.rtn.strip()
        if rtn.startswith("virtual"): rtn = rtn.lstrip("virtual").strip()
        ARG_PATTERN = re.compile(r"^(.*)\s+([\w*]+)\s*([\]\[]*)$")

        args = []
        argvs = []
        for arg in self.args.split(","):
            arg = arg.strip()
            if len(arg) == 0: continue
            if arg.find("=") >= 0: arg = arg[:arg.find("=")]
            match = ARG_PATTERN.match(arg)
            assert match is not None
            a, v, b = match.groups()
            trim_star = 0
            for c in v:
                if c != '*': break
                trim_star += 1
                a += '*'
            v = v[trim_star:]
            args.append(f"{a} {v}{b}")
            argvs.append(v)
        args_repl = ", ".join(args)
        argvs_repl = ", ".join(argvs)
        return (
            f"{rtn} {self.name}({args_repl});",
            f"{rtn} {clsname}::{self.name}({args_repl}) {{ return m_member->{self.name}({argvs_repl}); }}"
        )

def port_ctp_td():
    meta = {}
    walk_hpp(open("./shared/include/ThostFtdcTraderApi.h", encoding="utf-8").read(), meta)

    with open("./src/ctptd.cpp", "w", encoding="utf-8") as file: 
        file.writelines(["#include \"../shared/include/ThostFtdcTraderApi.h\""])
        
        c1 = meta["CThostFtdcTraderApi"]
        content = c1.member_to_rust(meta, is_m_member_owned=False)
        file.write(content)

        c2 = meta["CThostFtdcTraderSpi"]
        content = c2.forward_to_rust(meta)
        file.write(content)

def port_ctp_md():
    meta = {}
    walk_hpp(open("./shared/include/ThostFtdcMdApi.h", encoding="utf-8").read(), meta)

    with open("./src/ctpmd.cpp", "w", encoding="utf-8") as file: 
        file.writelines(["#include \"../shared/include/ThostFtdcMdApi.h\""])

        c1 = meta["CThostFtdcMdApi"]
        content = c1.member_to_rust(meta, is_m_member_owned=False)
        file.write(content)

        c2 = meta["CThostFtdcMdSpi"]
        content = c2.forward_to_rust(meta)
        file.write(content)


if __name__ == "__main__":
    port_ctp_md()
    port_ctp_td()
