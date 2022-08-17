查看dll文件内函数入口名称
1、使用VS自带工具：

（1）进入VS开发环境，然后Tools -> Visual studio 2015 Command Prompt，打开兼容工具命令提示符，

（2）cd到dll所在目录，输入命令 dumpbin /exports a.dll  回车即可看到a.dll的所有导出函数。

　　或 dumpbin /exports “C:\Windows\System32\user32.dll”  （输入dll的完整路径）

 

 　　如果是查看lib文件的导出函数，命令如下：

　　dumpbin /disasm /LINKERMEMBER aa.lib

 

2、使用IDA，或DLL Export Viewer 软件

-------------------------------------------
bindgen 在产生代码时遇到 destructor 函数时有 bug，参考如下：
https://github.com/rust-lang/rust-bindgen/issues/1843
ctp 例子有人也发现：
https://github.com/rust-lang/rust-bindgen/issues/1725
https://www.editcode.net/issues/46405

-------------------------------------------

通过 dumpbin /disasm 后可知释构函数的 link 名称，如下列4行代码，在 src/generated/mod.rs 中找到并替换即可
??1Rust_CThostFtdcMdApi@@QEAA@XZ (public: __cdecl Rust_CThostFtdcMdApi::~Rust_CThostFtdcMdApi(void)):
??1Rust_CThostFtdcMdSpi@@QEAA@XZ (public: __cdecl Rust_CThostFtdcMdSpi::~Rust_CThostFtdcMdSpi(void)):
??1Rust_CThostFtdcTraderApi@@QEAA@XZ (public: __cdecl Rust_CThostFtdcTraderApi::~Rust_CThostFtdcTraderApi(void)):
??1Rust_CThostFtdcTraderSpi@@QEAA@XZ (public: __cdecl Rust_CThostFtdcTraderSpi::~Rust_CThostFtdcTraderSpi(void)):