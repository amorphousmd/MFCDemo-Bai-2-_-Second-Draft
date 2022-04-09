
// MFCDemoDlg.h : header file
//

#pragma once
#include "SerialCtrl.h"
#include "afxwin.h"
#include "ChartCtrl.h"
// CMFCDemoDlg dialog

class CMFCDemoDlg : public CDialogEx, public CSerialIO
{
// Construction
public:
	CMFCDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef pchart
	enum { IDD = IDD_MFCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ccbSerialName;
	afx_msg void OnCbnDropdownComboSerialname();
	CComboBox m_ccbBaudrate;
	CButton m_btnOpen;
	afx_msg void OnBnClickedButtonOpen();

	//Add by User
	BOOL bPortOpened = 0;

	virtual void OnEventOpen(BOOL bSuccess);
	virtual void OnEventClose(BOOL bSuccess);
	virtual void OnEventRead(char* inPacket, int inLength);
	virtual void OnEventWrite(int nWritten);


	CEdit m_ceSendCmd1;
	CButton m_btnSendCmd1;
	afx_msg void OnBnClickedButtonSend1();
	CButton mBtnPosition;
	CButton mBtnVelocity;
	CButton mBtnMove;
	CButton mBtnStatus;
	afx_msg void OnBnClickedButtonPos();
	CListBox m_listboxRead;
	afx_msg void OnBnClickedButtonVel();
	afx_msg void OnBnClickedButtonMov();
	afx_msg void OnBnClickedButtonStt();

	VOID ProcessData(char* data, int inLength);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioTimer();
	afx_msg void OnBnClickedRadioEvent();
	CButton m_chLed1;
	CButton m_chGPIO1;
	CButton m_chLed2;
	CButton m_chGPIO2;
	CStatic m_staticInfo;
	CButton m_btnSendPID;
	afx_msg void OnBnClickedButtonSend();
	CButton m_btnTuning;
	afx_msg void OnBnClickedButtonTuning();
	CButton m_btnREQ;
	afx_msg void OnBnClickedButtonRequestPid();
	CChartCtrl pCharCtrlPID;

	CChartStandardAxis* pBottomAxis;
	CChartStandardAxis* pLeftAxis;
	CChartCtrl pChartCtrlPID;
	CChartLineSerie* pChartPIDSeries;
	CButton m_btnProfileDraw;
	CButton m_btnSendSetting;
	CButton m_btnSendRunPos;
	CButton m_btnGETRMS;
	CStatic m_stRms;
	CChartCtrl pChartCtrlPos;
	CChartCtrl pChartCtrlVel;
	CChartCtrl pChartCtrlAcc;
	VOID DrawPIDGraph();
	VOID ProcessData(unsigned char* data, int inLength);
	VOID DrawOperationGraph();
	CChartLineSerie* pChartPosSeries;
	CChartLineSerie* pChartPosSeriesRef;
};
