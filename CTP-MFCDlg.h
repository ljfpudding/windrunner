
// CTP-MFCDlg.h : ͷ�ļ�
//

#pragma once



#define WM_MYMSG   WM_USER+88
#define WM_KDJMSG  WM_USER+89
#define WM_HEYUEPREPARED	WM_USER+90 //��Լ����
#define WM_READTESTDATA     WM_USER+91//��ȡTest Data



#include "HighSpeedChartCtrl/ChartCtrl.h"
#include "HighSpeedChartCtrl/ColourPicker.h"
#include "HighSpeedChartCtrl/ChartLineSerie.h"
#include "HighSpeedChartCtrl/ChartCandlestickSerie.h"

using namespace std;

typedef struct
{
	int funcID;
	double lastX;
	double deltaX;
	double alpha;
	double multY;
	size_t pntsNmb;
	size_t period;
} STRUCT_FUNCDATA;


// CCTPMFCDlg �Ի���
class CCTPMFCDlg : public CDialogEx
{
// ����
public:
	CCTPMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MD };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFetchmd();
	afx_msg void OnBnClickedBtnMdClose();
	afx_msg LRESULT OnMyMsgHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnKDJMsgHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnHeYuePreparedHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnReadTestDataHandler(WPARAM, LPARAM);
	afx_msg void OnChartVisibilityChanged(NMHDR*, LRESULT*);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelComChange();
	afx_msg void OnSelComChangeServer();
	afx_msg void OnSelComChangeTimeInterval();

public:
	void DrawInit();


public:
	CString m_strSelInstrument;
	BOOL	m_bSel;

public:
	CListBox m_lstMessage;
	CComboBox  m_ComBoxCtrl;
	CToolTipCtrl m_tt;
	CComboBox  m_ServerComBoxCtrl;
	CComboBox  m_TimeIntervalComBoxCtrl;

protected:
	CChartCtrl m_ChartCtrl;
	CChartCandlestickSerie *pCandleSerie;
	CChartLineSerie	*m_pLineSerie;
	CChartStandardAxis* m_pBottomAxis;
	CChartStandardAxis* m_pLeftAxis;


};
