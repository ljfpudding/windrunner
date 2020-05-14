
// CTP-MFCDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "CTP-MFC.h"
#include "CTP-MFCDlg.h"
#include "afxdialogex.h"
#include "VolumeIndicatorMD.h"
#include "ThreadWorkLogic.h"
#include "ChartCtrlLib\Util.h"
#include  "gdiplusenums.h"
#include   "wingdi.h"
#include "strategy.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCTPMFCDlg �Ի���
CCTPMFCDlg::CCTPMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTPMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LISTBOX_MESSAGE, m_lstMessage);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX, IDC_COMBO_MD, m_ComBoxCtrl);
	
	 
}

BEGIN_MESSAGE_MAP(CCTPMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MD_FETCH, &CCTPMFCDlg::OnBnClickedButtonFetchmd)
	ON_BN_CLICKED(IDC_BTN_MD_CLOSE, &CCTPMFCDlg::OnBnClickedBtnMdClose)
	ON_MESSAGE(WM_MYMSG, &CCTPMFCDlg::OnMyMsgHandler)
	ON_MESSAGE(WM_KDJMSG,&CCTPMFCDlg::OnKDJMsgHandler)
	ON_MESSAGE(WM_HEYUEPREPARED, &CCTPMFCDlg::OnHeYuePreparedHandler)
	ON_NOTIFY(CODE_VISIBILITY, IDC_CHARTCTRL, OnChartVisibilityChanged)
END_MESSAGE_MAP()

LRESULT CCTPMFCDlg::OnHeYuePreparedHandler(WPARAM w, LPARAM l)
{
	if (md_InstrumentID.size() != 0 && m_ComBoxCtrl.GetCount() == 0)
	{
		for (int i = 0; i < md_InstrumentID.size(); i++)
		{
			m_ComBoxCtrl.AddString(CString(md_InstrumentID[i].c_str()));
		}

		m_ComBoxCtrl.SetCurSel(0);

	}
	return 0;
}



void CCTPMFCDlg::OnTimer(UINT_PTR nIDEvent)
{


	CDialogEx::OnTimer(nIDEvent);
}



LRESULT CCTPMFCDlg::OnMyMsgHandler(WPARAM w, LPARAM l)
{

	PARAMTOCHARTS  *param = (PARAMTOCHARTS*)w;
	if (param->strMessage != "")
	{
		m_lstMessage.AddString(CString(param->strMessage.c_str()));

	}
	
	return 0;
}


LRESULT CCTPMFCDlg::OnKDJMsgHandler(WPARAM w, LPARAM l)
{





	return 0;
}


BOOL CCTPMFCDlg::PreTranslateMessage(MSG* pMsg)
{

	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);

}


// CCTPMFCDlg ��Ϣ�������

BOOL CCTPMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	ShowWindow(SW_NORMAL);


	EnableToolTips(TRUE);
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	
	CWnd* pW = GetDlgItem(IDC_LISTBOX_MESSAGE);
	if (pW != NULL)
	{
		m_tt.AddTool(pW, L"Message List Box");		
	}

	m_lstMessage.AddString(CString("This List Box Show Log Message"));
	m_lstMessage.AddString(CString("The Line Follow:"));

	m_lstMessage.SetHorizontalExtent(10000);

	CChartStandardAxis* pBottomAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(0, 500);
	CChartStandardAxis* pLeftAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0, 500);

	pCandleSerie = m_ChartCtrl.CreateCandlestickSerie(false, false);


	//void AddPoint(double XVal, double Low, double High, double Open, double Close);

	pCandleSerie->AddPoint(1, 10, 20, 18, 16);
	pCandleSerie->AddPoint(2, 12, 26, 18, 22);
	pCandleSerie->AddPoint(3, 8, 19, 15, 18);
	pCandleSerie->AddPoint(4, 12, 20, 12, 20);
	pCandleSerie->AddPoint(5, 6, 15, 15, 6);
	pCandleSerie->AddPoint(6, 9, 21, 14, 20);


	SetTimer(1, 5000, NULL);





	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CCTPMFCDlg::DrawInit()
{
	

}

void CCTPMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCTPMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCTPMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCTPMFCDlg::OnBnClickedButtonFetchmd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	hFetchMD = (HANDLE)_beginthreadex(NULL, 0, &fetchMDThreadProc, (LPVOID)this, 0, NULL);
	hWriteData = (HANDLE)_beginthreadex(NULL, 0, &writeDataThreadProc, (LPVOID)this, 0, NULL);
	hWaitForConDisconnect = (HANDLE)_beginthreadex(NULL, 0, &waitForConDisconnectThreadProc, (LPVOID)this, 0, NULL);
	hCalAnalysis = (HANDLE)_beginthreadex(NULL, 0, &CalculateAndAnalysisThreadProc, (LPVOID)this, 0, NULL);
	hStrategy = (HANDLE)_beginthreadex(NULL, 0, &CalculateStrategyThreadProc, (LPVOID)this, 0, NULL);
	hTradeThreadProc = (HANDLE)_beginthreadex(NULL, 0, &TradeThreadProc, (LPVOID)this, 0, NULL);	
}


void CCTPMFCDlg::OnBnClickedBtnMdClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetEvent(CloseSignalReady);	
	CloseHandle(hFetchMD);
	CloseHandle(hWriteData);
	CloseHandle(hWaitForConDisconnect);
	CloseHandle(hCalAnalysis);
	CloseHandle(hStrategy);
	CloseHandle(hTradeThreadProc);
}

void CCTPMFCDlg::OnChartVisibilityChanged(NMHDR* nmhdr, LRESULT* pRes)
{
	NMCHART* pNmChart = PNMCHART(nmhdr);

}
