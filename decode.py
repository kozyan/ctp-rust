str = b"""
  = note: Non-UTF-8 output: mdapi.utd2sixe38no8j9.rcgu.o : error LNK2019: \xce\xde\xb7\xa8\xbd\xe2\xce\xf6\xb5\xc4\xcd\xe2\xb2\xbf\xb7\xfb\xba\xc5 \"public: void __cdecl Rust_CThostFtdcMdApi::`vbase destructor\'(void)\" (??_DRust_CThostFtdcMdApi@@QEAAXXZ)\xa3\xac\xba\xaf\xca\xfd _ZN6ctp_rs9generated20Rust_CThostFtdcMdApi8destruct17h172d1c59296d85abE \xd6\xd0\xd2\xfd\xd3\xc3\xc1\xcb\xb8\xc3\xb7\xfb\xba\xc5\r\nmdapi.utd2sixe38no8j9.rcgu.o : error LNK2019: \xce\xde\xb7\xa8\xbd\xe2\xce\xf6\xb5\xc4\xcd\xe2\xb2\xbf\xb7\xfb\xba\xc5 \"public: void __cdecl Rust_CThostFtdcMdSpi::`vbase destructor\'(void)\" (??_DRust_CThostFtdcMdSpi@@QEAAXXZ)\xa3\xac\xba\xaf\xca\xfd _ZN6ctp_rs9generated20Rust_CThostFtdcMdSpi8destruct17h4c0f843d52a6084cE \xd6\xd0\xd2\xfd\xd3\xc3\xc1\xcb\xb8\xc3\xb7\xfb\xba\xc5\r\nD:\\workspace\\rust\\ctp-rust\\target\\debug\\examples\\mdapi.exe : fatal error LNK1120: 2 \xb8\xf6\xce\xde\xb7\xa8\xbd\xe2\xce\xf6\xb5\xc4\xcd\xe2\xb2\xbf\xc3\xfc\xc1\xee\r\n
  = note: Non-UTF-8 output: tdapi.5elnlwst0yxlqz3.rcgu.o : error LNK2019: \xce\xde\xb7\xa8\xbd\xe2\xce\xf6\xb5\xc4\xcd\xe2\xb2\xbf\xb7\xfb\xba\xc5 \"public: void __cdecl Rust_CThostFtdcTraderApi::`vbase destructor\'(void)\" (??_DRust_CThostFtdcTraderApi@@QEAAXXZ)\xa3\xac\xba\xaf\xca\xfd _ZN6ctp_rs9generated24Rust_CThostFtdcTraderApi8destruct17h08823d2a71620f5cE \xd6\xd0\xd2\xfd\xd3\xc3\xc1\xcb\xb8\xc3\xb7\xfb\xba\xc5\r\nD:\\workspace\\rust\\ctp-rust\\target\\debug\\examples\\tdapi.exe : fatal error LNK1120: 1 \xb8\xf6\xce\xde\xb7\xa8\xbd\xe2\xce\xf6\xb5\xc4\xcd\xe2\xb2\xbf\xc3\xfc\xc1\xee\r\n
"""

print(str.decode("gbk"))