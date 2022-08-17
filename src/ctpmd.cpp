#include "../shared/include/ThostFtdcMdApi.h"
class Rust_CThostFtdcMdApi {
    public:
        CThostFtdcMdApi *m_member;
        Rust_CThostFtdcMdApi(CThostFtdcMdApi *member);
        ~Rust_CThostFtdcMdApi();

        void Release();
        void Init();
        int Join();
        const char* GetTradingDay();
        void RegisterFront(char* pszFrontAddress);
        void RegisterNameServer(char* pszNsAddress);
        void RegisterFensUserInfo(CThostFtdcFensUserInfoField* pFensUserInfo);
        void RegisterSpi(CThostFtdcMdSpi* pSpi);
        int SubscribeMarketData(char* ppInstrumentID[], int nCount);
        int UnSubscribeMarketData(char* ppInstrumentID[], int nCount);
        int SubscribeForQuoteRsp(char* ppInstrumentID[], int nCount);
        int UnSubscribeForQuoteRsp(char* ppInstrumentID[], int nCount);
        int ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID);
        int ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID);
        int ReqQryMulticastInstrument(CThostFtdcQryMulticastInstrumentField* pQryMulticastInstrument, int nRequestID);
};


Rust_CThostFtdcMdApi::Rust_CThostFtdcMdApi(CThostFtdcMdApi *member) : m_member(member) {  };
Rust_CThostFtdcMdApi::~Rust_CThostFtdcMdApi() {  };
void Rust_CThostFtdcMdApi::Release() { return m_member->Release(); }
void Rust_CThostFtdcMdApi::Init() { return m_member->Init(); }
int Rust_CThostFtdcMdApi::Join() { return m_member->Join(); }
const char* Rust_CThostFtdcMdApi::GetTradingDay() { return m_member->GetTradingDay(); }
void Rust_CThostFtdcMdApi::RegisterFront(char* pszFrontAddress) { return m_member->RegisterFront(pszFrontAddress); }
void Rust_CThostFtdcMdApi::RegisterNameServer(char* pszNsAddress) { return m_member->RegisterNameServer(pszNsAddress); }
void Rust_CThostFtdcMdApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField* pFensUserInfo) { return m_member->RegisterFensUserInfo(pFensUserInfo); }
void Rust_CThostFtdcMdApi::RegisterSpi(CThostFtdcMdSpi* pSpi) { return m_member->RegisterSpi(pSpi); }
int Rust_CThostFtdcMdApi::SubscribeMarketData(char* ppInstrumentID[], int nCount) { return m_member->SubscribeMarketData(ppInstrumentID, nCount); }
int Rust_CThostFtdcMdApi::UnSubscribeMarketData(char* ppInstrumentID[], int nCount) { return m_member->UnSubscribeMarketData(ppInstrumentID, nCount); }
int Rust_CThostFtdcMdApi::SubscribeForQuoteRsp(char* ppInstrumentID[], int nCount) { return m_member->SubscribeForQuoteRsp(ppInstrumentID, nCount); }
int Rust_CThostFtdcMdApi::UnSubscribeForQuoteRsp(char* ppInstrumentID[], int nCount) { return m_member->UnSubscribeForQuoteRsp(ppInstrumentID, nCount); }
int Rust_CThostFtdcMdApi::ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID) { return m_member->ReqUserLogin(pReqUserLoginField, nRequestID); }
int Rust_CThostFtdcMdApi::ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID) { return m_member->ReqUserLogout(pUserLogout, nRequestID); }
int Rust_CThostFtdcMdApi::ReqQryMulticastInstrument(CThostFtdcQryMulticastInstrumentField* pQryMulticastInstrument, int nRequestID) { return m_member->ReqQryMulticastInstrument(pQryMulticastInstrument, nRequestID); }

class Rust_CThostFtdcMdSpi : CThostFtdcMdSpi {
    public:
        void *m_rust;
        Rust_CThostFtdcMdSpi(void *rust);
        ~Rust_CThostFtdcMdSpi();

        void OnFrontConnected() override;
        void OnFrontDisconnected(int nReason) override;
        void OnHeartBeatWarning(int nTimeLapse) override;
        void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override;
        void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) override;
};

extern "C" void Rust_CThostFtdcMdSpi_Trait_OnFrontConnected(void* m_rust);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnFrontDisconnected(void* m_rust, int nReason);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnHeartBeatWarning(void* m_rust, int nTimeLapse);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspUserLogin(void* m_rust, CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspUserLogout(void* m_rust, CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspQryMulticastInstrument(void* m_rust, CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspError(void* m_rust, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspSubMarketData(void* m_rust, CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspUnSubMarketData(void* m_rust, CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspSubForQuoteRsp(void* m_rust, CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRspUnSubForQuoteRsp(void* m_rust, CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRtnDepthMarketData(void* m_rust, CThostFtdcDepthMarketDataField* pDepthMarketData);
extern "C" void Rust_CThostFtdcMdSpi_Trait_OnRtnForQuoteRsp(void* m_rust, CThostFtdcForQuoteRspField* pForQuoteRsp);
extern "C" void Rust_CThostFtdcMdSpi_Trait_Drop(void* m_rust);

void Rust_CThostFtdcMdSpi::OnFrontConnected() { return Rust_CThostFtdcMdSpi_Trait_OnFrontConnected(m_rust); }
void Rust_CThostFtdcMdSpi::OnFrontDisconnected(int nReason) { return Rust_CThostFtdcMdSpi_Trait_OnFrontDisconnected(m_rust, nReason); }
void Rust_CThostFtdcMdSpi::OnHeartBeatWarning(int nTimeLapse) { return Rust_CThostFtdcMdSpi_Trait_OnHeartBeatWarning(m_rust, nTimeLapse); }
void Rust_CThostFtdcMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspUserLogin(m_rust, pRspUserLogin, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspUserLogout(m_rust, pUserLogout, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspQryMulticastInstrument(m_rust, pMulticastInstrument, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspError(m_rust, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspSubMarketData(m_rust, pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspUnSubMarketData(m_rust, pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspSubForQuoteRsp(m_rust, pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) { return Rust_CThostFtdcMdSpi_Trait_OnRspUnSubForQuoteRsp(m_rust, pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }
void Rust_CThostFtdcMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) { return Rust_CThostFtdcMdSpi_Trait_OnRtnDepthMarketData(m_rust, pDepthMarketData); }
void Rust_CThostFtdcMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) { return Rust_CThostFtdcMdSpi_Trait_OnRtnForQuoteRsp(m_rust, pForQuoteRsp); }
Rust_CThostFtdcMdSpi::Rust_CThostFtdcMdSpi(void *rust) : m_rust(rust) {}
Rust_CThostFtdcMdSpi::~Rust_CThostFtdcMdSpi() { Rust_CThostFtdcMdSpi_Trait_Drop(m_rust); }
