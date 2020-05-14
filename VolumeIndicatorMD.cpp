#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <time.h>


#include "VolumeIndicatorMD.h"
#include "readerwriterqueue.h"
#include "CTP-MFC.h"
#include "CTP-MFCDlg.h"
#include "ThreadWorkLogic.h"


using namespace moodycamel;
using namespace std;

HANDLE LoginSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE SubscribeSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE CloseSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE writeFileNameSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE ReceiveDepDataSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE writeDepDataSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE TickSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE ConDisconnetSignalReady = CreateEvent(NULL, false, false, NULL);
HANDLE MkdirSignalReady = CreateEvent(NULL, false, false, NULL);


MDTICKDATA structTickData;

ReaderWriterQueue<MDTICKDATA> tickDataQueue;

TThostFtdcMdcsvFileName     m_chMdcsvFileName = {0};
TThostFtdcMdPankouFileName  m_chMdPankouFileName = {0};
CHInstrumentID              g_chInstrumentIDFilestr = { 0 };
vector<string>      g_vcInstrumentIDFilterStr;
char targetpath[1024] = { 0 };


list<struct CMDPankoudata> listPankou;

/// ��Ա����
TThostFtdcBrokerIDType g_chBrokerID;
/// �����û�����
TThostFtdcUserIDType g_chUserID;
/// �����û�����
TThostFtdcPasswordType g_chPassword;
/// ����������
TThostFtdcExchangeIDType g_chExchangeID;
///��Լ����
TThostFtdcInstrumentIDType	g_chInstrumentID;
///Ͷ���ߴ���
TThostFtdcInvestorIDType g_chInvestorID;
///Ԥ�񳷵����
TThostFtdcParkedOrderActionIDType	g_chParkedOrderActionID1;
///Ԥ�񱨵����
TThostFtdcParkedOrderIDType	g_chParkedOrderID1;
///��������
TThostFtdcOrderRefType	g_chOrderRef;
///ǰ�ñ��
TThostFtdcFrontIDType	g_chFrontID;
///�Ự���
TThostFtdcSessionIDType	g_chSessionID;
///�������
TThostFtdcOrderSysIDType	g_chOrderSysID;
///ֹ���
TThostFtdcPriceType	g_chStopPrice;
///��������
TThostFtdcOrderRefType	g_chQuoteRef;

///��֤��
TThostFtdcAuthCodeType	g_chAuthCode;
///App����
TThostFtdcAppIDType	g_chAppID;

//==========
///ǰ�ñ��
TThostFtdcFrontIDType	g_NewFrontID;
///�Ự���
TThostFtdcSessionIDType	g_NewSessionID;


int CVolumeIndicatorMDSpi::m_nIndex = 0;

class CVolumeIndicatorMDApi;
CVolumeIndicatorMDSpi::CVolumeIndicatorMDSpi(CThostFtdcMdApi *pUserApi) : m_pUserMdApi(pUserApi) {

	memset(&m_preTickData, 0, sizeof(MDTICKDATA));
}


int CVolumeIndicatorMDSpi::ReqUserLogin()
{

	CThostFtdcReqUserLoginField reqUserLogin = { 0 };
	strcpy_s(reqUserLogin.BrokerID, getConfig("config", "BrokerID").c_str());
	strcpy_s(reqUserLogin.UserID, getConfig("config", "UserID").c_str());
	strcpy_s(reqUserLogin.Password, getConfig("config", "Password").c_str());
	for (int i = 0; i<5; i++)
	{
		int nResult = m_pUserMdApi->ReqUserLogin(&reqUserLogin, 0);
		if (nResult != 0)
		{
			Sleep(200);
		}
		else
		{
			return 0;
		}
	}

	return 0;
	
}

int  CVolumeIndicatorMDSpi::SubscribeMarketData()
{

	int md_num = 0;

	char **ppInstrumentID = new char*[5000];
	for (int count1 = 0; count1 <= md_InstrumentID.size() / 500; count1++)
	{
		if (count1 < md_InstrumentID.size() / 500)
		{
			int a = 0;
			for (a; a < 500; a++)
			{
				ppInstrumentID[a] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
				md_num++;
			}
			int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, a);
			//LOG((result == 0) ? "������������1......���ͳɹ�\n" : "������������1......����ʧ�ܣ��������=[%d]\n", result);
		}
		else if (count1 = md_InstrumentID.size() / 500)
		{
			int count2 = 0;
			for (count2; count2 < md_InstrumentID.size() % 500; count2++)
			{
				ppInstrumentID[count2] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
				md_num++;
			}
			int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, count2);
			//LOG((result == 0) ? "������������2......���ͳɹ�\n" : "������������2......����ʧ�ܣ��������=[%d]\n", result);
		}
	}





/*


int md_num = 0;
//char *ppInstrumentID[] = { "ag1912" };			// ���鶩���б�
//int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, 1);
//*
char **ppInstrumentID = new char*[5000];
for (int count1 = 0; count1 <= md_InstrumentID.size() / 500; count1++)
{
if (count1 < md_InstrumentID.size() / 500)
{
int a = 0;
for (a; a < 500; a++)
{
ppInstrumentID[a] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
md_num++;
}
int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, a);
//LOG((result == 0) ? "������������1......���ͳɹ�\n" : "������������1......����ʧ�ܣ��������=[%d]\n", result);
}
else if (count1 = md_InstrumentID.size() / 500)
{
int count2 = 0;
for (count2; count2 < md_InstrumentID.size() % 500; count2++)
{
ppInstrumentID[count2] = const_cast<char *>(md_InstrumentID.at(md_num).c_str());
md_num++;
}
int result = m_pUserMdApi->SubscribeMarketData(ppInstrumentID, count2);
//LOG((result == 0) ? "������������2......���ͳɹ�\n" : "������������2......����ʧ�ܣ��������=[%d]\n", result);
}
}


*/


	return 0;
}


void CVolumeIndicatorMDSpi::OnFrontConnected()
{
	//ReqUserLogin();
	SetEvent(LoginSignalReady);
}


void CVolumeIndicatorMDSpi::OnFrontDisconnected(int nReason)
{	
	SetEvent(ConDisconnetSignalReady);
 }


void CVolumeIndicatorMDSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
		if (pRspInfo->ErrorID != 0) {
			exit(-1);
		}

		if (bIsLast && ( !pRspInfo->ErrorID ))
		{
			
			sprintf(targetpath, "%s\\%s\\", targetpath, m_pUserMdApi->GetTradingDay());
			if (_access(targetpath, 0) == -1)
			{
				_mkdir(targetpath);
				SetEvent(MkdirSignalReady);
			}
			SetEvent(SubscribeSignalReady);
			memset(&m_preTickData, 0, sizeof(MDTICKDATA)); //�ٴε�¼��ʱ��,�������pre.
		}
}

void CVolumeIndicatorMDSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		if (pSpecificInstrument != NULL)
		{
			strcpy_s(g_chInstrumentIDFilestr, pSpecificInstrument->InstrumentID);
			g_vcInstrumentIDFilterStr.push_back(g_chInstrumentIDFilestr);
			
		}

		if (bIsLast)
		{

			theApp.GetMainWnd()->PostMessageW(WM_HEYUEPREPARED, 0, 0);

			SetEvent(writeFileNameSignalReady);
			SetEvent(ReceiveDepDataSignalReady);
			//SetEvent(writeDepDataSignalReady);
		}
	}
}

void CVolumeIndicatorMDSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	/*if (!m_bPankouData)
	{
		struct CMDPankoudata  structPankou;
		memset(&structPankou, 0, sizeof(struct CMDPankoudata));
		structPankou.dwPreSettlementPrice = pDepthMarketData->PreSettlementPrice;
		structPankou.dwPreOpenInterest = pDepthMarketData->PreOpenInterest;
		structPankou.dwUpperLimitPrice = pDepthMarketData->UpperLimitPrice;
		structPankou.dwLowerLimitPrice = pDepthMarketData->LowerLimitPrice;
		structPankou.dwOpenPrice = pDepthMarketData->OpenPrice;
		structPankou.dwHighestPrice = pDepthMarketData->HighestPrice;
		structPankou.dwLowestPrice = pDepthMarketData->LowestPrice;		
		listPankou.push_back(structPankou);
		m_bPankouData = true;

	}*/


 	if (pDepthMarketData)
	{
		/*if (pDepthMarketData->UpdateMillisec == m_preTickData.iMilitime && (m_preTickData.iVolume !=0)) //��ʧ������
		{
			if (m_preTickData.iMilitime == 0)
			{
				m_preTickData.iMilitime = 500;
			}
			else
			{
				m_preTickData.iMilitime = 0;
				strcpy_s(m_preTickData.cUpdatetime, pDepthMarketData->UpdateTime);
			}
			m_preTickData.nIndex = m_nIndex++;
			tickDataQueue.enqueue(m_preTickData);	

			PARAMTOCHARTS paramTrans;
			memset(&paramTrans, 0, sizeof(PARAMTOCHARTS));
			if(m_preTickData.iMilitime==0)
				paramTrans.strMessage = "Program Add a volume : 0";
			else
				paramTrans.strMessage = "Program Add a volume :500";			
			PostMessageToDlg(paramTrans);
		}*/

			memset(&structTickData, 0, sizeof(struct CMDTickdata));
			strcpy_s(structTickData.cInstrumentID, pDepthMarketData->InstrumentID);
			strcpy_s(structTickData.cUpdatetime, pDepthMarketData->UpdateTime);
			structTickData.iMilitime = pDepthMarketData->UpdateMillisec;
			structTickData.dwLastPrice = pDepthMarketData->LastPrice;
			structTickData.iVolume = pDepthMarketData->Volume;
			structTickData.dwOpenInterest = pDepthMarketData->OpenInterest;
			structTickData.dwBidPrice1 = pDepthMarketData->BidPrice1;
			structTickData.iBidVolume1 = pDepthMarketData->BidVolume1;
			structTickData.dwAskPrice1 = pDepthMarketData->AskPrice1;
			structTickData.iAskVolume1 = pDepthMarketData->AskVolume1;
			structTickData.nIndex = m_nIndex++;
			tickDataQueue.enqueue(structTickData);
		//memcpy(&m_preTickData, &structTickData, sizeof(MDTICKDATA));

	
	}		
	
	/*if (pDepthMarketData->UpdateMillisec == 0)
	{

		PARAMTOCHARTS paramTrans;
		memset(&paramTrans, 0, sizeof(PARAMTOCHARTS));
		paramTrans.strMessage = "0";
		PostMessageToDlg(paramTrans);

	}
	else
	{
		PARAMTOCHARTS paramTrans;
		memset(&paramTrans, 0, sizeof(PARAMTOCHARTS));
		paramTrans.strMessage = "500";
		PostMessageToDlg(paramTrans);		
	}*/		
}







