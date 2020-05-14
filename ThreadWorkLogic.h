#pragma once

#include <string>
#include <map>
#include <atomic>
#include <mutex>
#include <iostream>
#include <fstream>
#include <ctime>

#include "readerwriterqueue.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThreadWorkLogic.h"
#include "VolumeIndicatorMD.h"
#include "traderspi.h"


using namespace moodycamel;
using namespace std;

extern HANDLE hFetchMD ; //��ȡ����Handle;
extern HANDLE hWriteData ;//дdata���ļ�;
extern HANDLE hWaitForConDisconnect;
extern HANDLE hTradeThreadProc;

extern HANDLE hTraderAuthSignalReady;//Trader Auth Ready
extern HANDLE hTraderLoginSignalReady;//Trader Login Ready
extern HANDLE hTraderConnectSignalReady;//Trader Connect Ready

extern unsigned int __stdcall fetchMDThreadProc(void * data);
extern unsigned int __stdcall writeDataThreadProc(void * data);
extern unsigned int __stdcall waitForConDisconnectThreadProc(void * data);
extern unsigned int __stdcall CalculateAndAnalysisThreadProc(void * data);
extern unsigned int __stdcall TradeThreadProc(void * data);

typedef struct parameterTransToChart
{
	string strMessage; // to list box
	double dwprice; // to price chart
	int nGap; // to gap chart
	int nIndex; //x-axis index
}PARAMTOCHARTS;

extern void PostMessageToDlg(PARAMTOCHARTS &param);
extern map<string, map<int, double>> g_mapPrice;
extern vector<string> md_InstrumentID;


enum open_interest_delta_forward_enum
{
	OPEN, CLOSE, EXCHANGE, NONE, OPENFWDOUBLE, CLOSEFWDOUBLE
};
enum order_forward_enum
{
	UP, DOWN, TICKMIDDLE
};
enum tick_type_enum
{
	OPENLONG, OPENSHORT, OPENDOUBLE,
	CLOSELONG, CLOSESHORT, CLOSEDOUBLE,
	EXCHANGELONG, EXCHANGESHORT,
	OPENUNKOWN, CLOSEUNKOWN, EXCHANGEUNKOWN,
	UNKOWN, NOCHANGE
};
enum tick_color_enum
{
	RED, GREEN, WHITE
};
enum tick_type_key_enum
{
	TICKTYPE, TICKCOLOR
};
enum opponent_key_enum
{
	OPPOSITE, SIMILAR
};


typedef map<tick_type_key_enum, int> TICK_TYPE;
typedef map<open_interest_delta_forward_enum, map<order_forward_enum, TICK_TYPE>> TICK_TYPE_DICT;
typedef map<tick_type_enum, map<opponent_key_enum, int> > HANDICAP_DICT;
typedef map<int, string> TICK_TYPE_STR_DICT;


typedef struct trendTickData
{
	int openshortNum; //�տ���
	int openlongNum; //�࿪��
	int closeshortNum; //��ƽ��
	int closelongNum;  //��ƽ��
	int exchangelongNum; //�໻��
	int exchangeshortNum; //�ջ���
	int opendoubleNum;
	int closedoubleNum;

}TRENDTICKDATA;



class spin_mutex {
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	spin_mutex() = default;
	spin_mutex(const spin_mutex&) = delete;
	spin_mutex& operator= (const spin_mutex&) = delete;
	void lock() {
		while (flag.test_and_set(std::memory_order_acquire))
			;
	}
	void unlock() {
		flag.clear(std::memory_order_release);
	}
};

extern spin_mutex sm;



//============================================================================================================

//������
class CSimpleHandler : public CTraderSpi
{
public:
	CSimpleHandler(CThostFtdcTraderApi *pUserApi);
	~CSimpleHandler() {}
	virtual void OnFrontConnected();


	//�ͻ�����֤
	void ReqAuthenticate(); 


	///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo,	int nRequestID, bool bIsLast);


	void RegisterFensUserInfo();

	/*virtual void OnFrontDisconnected(int nReason)
	{
	//LOG("<OnFrontDisconnected>\n");
	//LOG("\tnReason = %d\n", nReason);
	//LOG("</OnFrontDisconnected>\n");
	}*/

	void ReqUserLogin();
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);



	void ReqQryInstrument();
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void ReqUserLogout();

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����ȷ�Ͻ��㵥
	void ReqSettlementInfoConfirm();

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û������������
	void ReqUserPasswordUpdate();

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ʽ��˻������������
	void ReqTradingAccountPasswordUpdate();

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);






	private:
		CThostFtdcTraderApi *m_pUserApi;

};

