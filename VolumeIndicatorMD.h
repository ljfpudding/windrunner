#pragma once

#include <map>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <io.h>
#include <direct.h>

#include "CtpAPILibrary\ThostFtdcTraderApi.h"
#include "CtpAPILibrary\ThostFtdcMdApi.h"
#include "CtpAPILibrary\ThostFtdcUserApiDataType.h"
#include "getconfig.h"
#include "readerwriterqueue.h"


extern HANDLE LoginSignalReady;
extern HANDLE SubscribeSignalReady;
extern HANDLE CloseSignalReady;
extern HANDLE writeFileNameSignalReady;
extern HANDLE ReceiveDepDataSignalReady;
extern HANDLE writeDepDataSignalReady;
extern HANDLE TickSignalReady;
extern HANDLE ConDisconnetSignalReady;
extern HANDLE hCalAnalysis;
extern HANDLE MkdirSignalReady;


//�̿����ݽṹ
struct CMDPankoudata
{
	double dwPreSettlementPrice;
	double dwPreOpenInterest;
	double dwUpperLimitPrice;
	double dwLowerLimitPrice;
	double dwOpenPrice;
	double dwHighestPrice;
	double dwLowestPrice;
};

//tick���ݽṹ
typedef struct CMDTickdata
{
	char	cInstrumentID[31];
	char	cUpdatetime[9];
	int		iMilitime;
	double	dwLastPrice;
	int		iVolume;
	double  dwOpenInterest;
	double	dwBidPrice1;
	int		iBidVolume1;
	double	dwAskPrice1;
	int		iAskVolume1;
	int		iOpenDelta;
	char	cInformation[50];
	int		nIndex;//Price������Ϊ�˼������ָ�귽�㡣
}MDTICKDATA;

using namespace std;
using namespace moodycamel;

typedef char  TThostFtdcMdcsvFileName[256];  //�洢������ϸ���ļ���
typedef char TThostFtdcMdPankouFileName[256]; //�洢�̿ڵ��ļ���  ��Ҫ��������  ��ͣ��  ��ͣ��
typedef char CHInstrumentID[31];

extern char targetpath[1024];
extern vector<string>      g_vcInstrumentIDFilterStr;
extern TThostFtdcMdcsvFileName     m_chMdcsvFileName;
extern TThostFtdcMdPankouFileName  m_chMdPankouFileName;
extern CHInstrumentID              g_chInstrumentIDFilestr;
extern list<struct CMDPankoudata> listPankou;
extern MDTICKDATA structTickData;
extern ReaderWriterQueue<MDTICKDATA> tickDataQueue;


/// ��Ա����
extern TThostFtdcBrokerIDType g_chBrokerID;
/// �����û�����
extern TThostFtdcUserIDType g_chUserID;
/// �����û�����
extern TThostFtdcPasswordType g_chPassword;
/// ����������
extern TThostFtdcExchangeIDType g_chExchangeID;
///��Լ����
extern TThostFtdcInstrumentIDType	g_chInstrumentID;
///Ͷ���ߴ���
extern TThostFtdcInvestorIDType g_chInvestorID;
///Ԥ�񳷵����
extern TThostFtdcParkedOrderActionIDType	g_chParkedOrderActionID1;
///Ԥ�񱨵����
extern TThostFtdcParkedOrderIDType	g_chParkedOrderID1;
///��������
extern TThostFtdcOrderRefType	g_chOrderRef;
///ǰ�ñ��
extern TThostFtdcFrontIDType	g_chFrontID;
///�Ự���
extern TThostFtdcSessionIDType	g_chSessionID;
///�������
extern TThostFtdcOrderSysIDType	g_chOrderSysID;
///ֹ���
extern TThostFtdcPriceType	g_chStopPrice;
///��������
extern TThostFtdcOrderRefType	g_chQuoteRef;

///��֤��
extern TThostFtdcAuthCodeType	g_chAuthCode;
///App����
extern TThostFtdcAppIDType	g_chAppID;

//==========
///ǰ�ñ��
extern TThostFtdcFrontIDType	g_NewFrontID;
///�Ự���
extern TThostFtdcSessionIDType	g_NewSessionID;





class CVolumeIndicatorMDSpi : public CThostFtdcMdSpi
{

public:
	// ���캯������Ҫһ����Ч��ָ��CThostFtdcMduserApiʵ����ָ��
	CVolumeIndicatorMDSpi(CThostFtdcMdApi *pUserApi);
	~CVolumeIndicatorMDSpi() {}
	virtual void OnFrontConnected();
	// ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
	virtual void OnFrontDisconnected(int nReason);
	// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);	
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);


	int ReqUserLogin();
	int SubscribeMarketData();

private:
	// ָ��CThostFtdcMduserApiʵ����ָ��
	CThostFtdcMdApi *m_pUserMdApi;
	bool m_bPankouData = false;
	MDTICKDATA m_preTickData;//��������ʱ�򣬳����Զ�����һ����ʱ���ݡ�
	static int m_nIndex;

};

class CVolumeIndicatorMDApi : public CThostFtdcMdApi
{
   
public:
	CVolumeIndicatorMDApi() {}
	~CVolumeIndicatorMDApi() {}

private:
	CThostFtdcMdApi *m_pApi;

public:
	CThostFtdcMdApi *CreateFtdcMdApi(const char *pszFlowPath = "", const bool bIsUsingUdp = false, const bool bIsMulticast = false)
	{
		return m_pApi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);
	 }

	///��ȡAPI�İ汾��Ϣ
	///@retrun ��ȡ���İ汾��
	const char *GetApiVersion()
	{
		return m_pApi->GetApiVersion();
	}


	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount)
	{
		return m_pApi->SubscribeMarketData(ppInstrumentID, nCount);
	}

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
	{
		return  m_pApi->UnSubscribeMarketData(ppInstrumentID, nCount);
	}

	///����ѯ�ۡ�
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount)
	{
		return m_pApi->SubscribeForQuoteRsp(ppInstrumentID, nCount);
	}

	///�˶�ѯ�ۡ�
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount)
	{
		return m_pApi->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
	}

	///�û���¼����
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
	{
		return m_pApi->ReqUserLogin(pReqUserLoginField, nRequestID);
	}

	///�ǳ�����
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
	{

		return m_pApi->ReqUserLogout(pUserLogout, nRequestID);
	}


	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release()
	{
		m_pApi->Release();
	}

	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init()
	{
		m_pApi->Init();
	}

	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join()
	{
		return m_pApi->Join();
	}

	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay()
	{
		return m_pApi->GetTradingDay();
	}

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress)
	{
		m_pApi->RegisterFront(pszFrontAddress);
	}

	///ע�����ַ����������ַ
	///@param pszNsAddress�����ַ����������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:12001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����12001������������˿ںš�
	///@remark RegisterNameServer������RegisterFront
	virtual void RegisterNameServer(char *pszNsAddress)
	{
		m_pApi->RegisterNameServer(pszNsAddress);
	}

	///ע�����ַ������û���Ϣ
	///@param pFensUserInfo���û���Ϣ��
	virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo)
	{
		m_pApi->RegisterFensUserInfo(pFensUserInfo);
	}

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CThostFtdcMdSpi *pSpi)
	{
		m_pApi->RegisterSpi(pSpi);
	}


};


//============================================================================================

