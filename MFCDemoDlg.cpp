
// MFCDemoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "framework.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "ChartAxis.h"
#include "ChartCtrl.h"
#include "ChartAxisLabel.h"
#include "ChartLineSerie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define EVENT_SEND_BY_TIMER 0
//STX
BYTE bSTX[] = { 0x02 };

//CMD LAB1
BYTE bMOVL[] = { 0x4D,0x4F,0x56,0x4C };
BYTE bGPOS[] = { 0x47,0x50,0x4F,0x53 };
BYTE bGVEL[] = { 0x47,0x56,0x45,0x4C };
BYTE bSTT[] = { 0x47,0x53,0x54,0x54 };
//CMD LAB2
BYTE bSPID[] = { 0x53,0x50,0x49,0x44 }; //ASCII {SPID}
BYTE bGPID[] = { 0x47,0x50,0x49,0x44 }; //ASCII {GPIO}
BYTE bCTUN[] = { 0x43,0x54,0x55,0x4E }; //ASCII {CTUN}
BYTE bCRUN[] = { 0x43,0x52,0x55,0x4E }; //ASCII {CRUN}
BYTE bCSET[] = { 0x43,0x53,0x45,0x54 }; //ASCII {CSET}
BYTE bGRMS[] = { 0x47,0x52,0x4D,0x53 }; //ASCII {GRMS}


//OPTION
BYTE bOPT[] = { 0x0,0x0,0x00 };

//DATA
BYTE bDATA[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

//SYNC/ACK
BYTE bSYNC[] = { 0x16 };
BYTE bACK[] = { 0x06 };

//ETX
BYTE bETX[] = { 0x03 };

//RECEIVE
BYTE bProtocolDataBuffer[18] = { };
BYTE bProtocolData[8] = {};
BYTE bProtocolOption[3] = {};

int nLenGraphPID;
double dPIDPlotData[1000] = {};
double dPIDPlotTime[1000] = {};

double fPositionPlotData[1000] = {};
float fVelPlotData[1000] = {};
float fAccPlotData[1000] = {};
int nLenGraphOperation;
int nLengtPositionPlotData = 0;

VOID CMFCDemoDlg::DrawPIDGraph() {
	pChartPIDSeries->ClearSerie();
	for (int i = 0; i < nLenGraphPID; i++) {
		pChartPIDSeries->AddPoint(dPIDPlotTime[i], dPIDPlotData[i]);
	}
}

VOID CMFCDemoDlg::ProcessData(unsigned char* data, int inLength) {
	//CString Cmd;
	//STX CMD OPTION DATA ACK
	//8 9 10 11 12 13 14 15
	CString str;
	uint16_t nIndex = 0;
	uint16_t nPosition = 0;
	CString csCmd, csOption, csData;
	for (UINT i = 0; i < static_cast<UINT>(inLength); i++) {
		bProtocolDataBuffer[i] = static_cast<BYTE>(data[i]);
	}
	for (UINT i = 1; i <= 4; i++) {
		csCmd.AppendChar(static_cast<char>(bProtocolDataBuffer[i]));
	}
	for (UINT i = 5; i <= 7; i++) {
		bProtocolOption[i - 5] = bProtocolDataBuffer[i];
	}
	for (UINT i = 8; i <= 15; i++) {
		bProtocolData[i - 8] = bProtocolDataBuffer[i];
	}

	if (!csCmd.Compare(static_cast<CString>("SPID"))) {

	}
	if (!csCmd.Compare(static_cast<CString>("GPID"))) {
		//m_btnREQ
		nLenGraphPID = (bProtocolData[0] << 8) + bProtocolData[1];
		//TODO:
		nIndex = (bProtocolData[2] << 8) + bProtocolData[3];
		nPosition = (bProtocolData[6] << 8) + bProtocolData[7];
		dPIDPlotData[nIndex] = nPosition;
		if (nIndex == (bProtocolData[0] << 8) + bProtocolData[1]) {
			DrawPIDGraph();
		}
	}
	if (!csCmd.Compare(static_cast<CString>("CTUN"))) {
		if (bProtocolDataBuffer[16] == 0x06) {
			//Receive SPID OK
		}
	}
	if (!csCmd.Compare(static_cast<CString>("CRUN"))) {
		if (bProtocolDataBuffer[16] == 0x06) {
			//Receive SPID OK
		}
	}

	if (!csCmd.Compare(static_cast<CString>("GRMS"))) {
		if (bProtocolDataBuffer[16] == 0x06) {
			//Receive SPID OK
		}
	}
	if (!csCmd.Compare(static_cast<CString>("CSET"))) {
		if (bProtocolDataBuffer[16] == 0x06) {
			//Receive SPID OK
		}
	}
	if (!csCmd.Compare(static_cast<CString>("GRMS"))) {
		//if (bProtocolBuffer[16] == 0x06) {
			//Receive SPID OK
		nLengtPositionPlotData = (bProtocolData[0] << 8) + bProtocolData[1];
		//if ((bProtocolData[2] << 8) + bProtocolData[3] >= nLengtPositionPlotData) return;
		nIndex = (bProtocolData[2] << 8) + bProtocolData[3];
		nPosition = (bProtocolData[6] << 8) + bProtocolData[7];

		fPositionPlotData[nIndex] = nPosition;

		if (nLengtPositionPlotData == nIndex) {
			/*DrawOperationGraph();*/
			uint16_t nRms = (bProtocolData[4] << 8) + bProtocolData[5];

			str.Format(static_cast<CString>("%d "), nRms);

			m_stRms.SetWindowText(str);
		}
		//}
	}

}

VOID CMFCDemoDlg::DrawOperationGraph() {
	pChartPosSeriesRef = pChartCtrlPos.CreateLineSerie();
	pChartPosSeriesRef->SetColor(RGB(255, 0, 0));
	pChartPosSeries->SetWidth(3); //line width
	pChartPosSeriesRef->ClearSerie();

	for (int i = 0; i < nLengtPositionPlotData; i++) {
		pChartPosSeriesRef->AddPoint(dPIDPlotTime[i], fPositionPlotData[i]);
	}
	//pChartVelSeries->ClearSerie();
	//for (int i = 0; i < nLenGraphOperation; i++) {
	//	pChartVelSeries->AddPoint(fPIDPlotTime[i], fVelPlotData[i]);
	//}
	//pChartAccSeries->ClearSerie();
	//for (int i = 0; i < nLenGraphOperation; i++) {
	//	pChartAccSeries->AddPoint(fPIDPlotTime[i], fAccPlotData[i]);
	//}
}

// CAboutDlg dialog used for App About
void FloatToByteArray(double dNumber, BYTE bOut[], uint8_t* nLengthTithes)
{
	uint8_t nCountTithes = 0;
	bOut[0] = (BYTE)dNumber;
	float nTithes = (float)(dNumber - bOut[0]);
	do
	{
		nTithes *= 10;
		if (nCountTithes >> 3) break; // Xem lai cho nay

	} while (nTithes <= 1);
	*nLengthTithes = nCountTithes;
	bOut[1] = (BYTE)nTithes;
}

void FloatToByteArrayWithNipes(double fNumber, BYTE bOut[], uint8_t* nLengthTithes) {
	uint8_t nCountTithes = 5;
	bOut[0] = (BYTE)fNumber;
	double nTithes = (double)(fNumber - bOut[0]);

	nTithes = nTithes * pow(10, nCountTithes);
	while (nTithes > 255) {
		nTithes /= 10;
		nCountTithes--;
	}
	*nLengthTithes = nCountTithes;
	bOut[1] = (BYTE)nTithes;

}
class CAboutDlg : public CDialogEx, public CSerialIO
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CMFCDemoDlg dialog



CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIALNAME, m_ccbSerialName);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ccbBaudrate);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_EDIT_SEND1, m_ceSendCmd1);
	DDX_Control(pDX, IDC_BUTTON_SEND1, m_btnSendCmd1);
	DDX_Control(pDX, IDC_BUTTON_POS, mBtnPosition);
	DDX_Control(pDX, IDC_BUTTON_VEL, mBtnVelocity);
	DDX_Control(pDX, IDC_BUTTON_MOV, mBtnMove);
	DDX_Control(pDX, IDC_BUTTON_STT, mBtnStatus);
	DDX_Control(pDX, IDC_LIST_RECEIVEDATA, m_listboxRead);
	DDX_Control(pDX, IDC_CHECK_LED1, m_chLed1);
	DDX_Control(pDX, IDC_CHECK_GPIO1, m_chGPIO1);
	DDX_Control(pDX, IDC_CHECK_LED2, m_chLed2);
	DDX_Control(pDX, IDC_CHECK_GPIO2, m_chGPIO2);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btnSendPID);
	DDX_Control(pDX, IDC_BUTTON_TUNING, m_btnTuning);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_PID, m_btnREQ);
	DDX_Control(pDX, IDC_CHARTCTRL_TUNNING, pCharCtrlPID);
	DDX_Control(pDX, IDC_BUTTON_DRAW, m_btnProfileDraw);
	DDX_Control(pDX, IDC_BUTTON_SENDPOS, m_btnSendSetting);
	DDX_Control(pDX, IDC_BUTTON_OPRUN, m_btnSendRunPos);
	DDX_Control(pDX, IDC_BUTTON_GET_RMS, m_btnGETRMS);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_stRms);
	DDX_Control(pDX, IDC_CHARTCTRL_POS, pChartCtrlPos);
	DDX_Control(pDX, IDC_CHARTCTRL_VEL, pChartCtrlVel);
	DDX_Control(pDX, IDC_CHARTCTRL_ACC, pChartCtrlAcc);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_SERIALNAME, &CMFCDemoDlg::OnCbnDropdownComboSerialname)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCDemoDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &CMFCDemoDlg::OnBnClickedButtonSend1)
	ON_BN_CLICKED(IDC_BUTTON_POS, &CMFCDemoDlg::OnBnClickedButtonPos)
	ON_BN_CLICKED(IDC_BUTTON_VEL, &CMFCDemoDlg::OnBnClickedButtonVel)
	ON_BN_CLICKED(IDC_BUTTON_MOV, &CMFCDemoDlg::OnBnClickedButtonMov)
	ON_BN_CLICKED(IDC_BUTTON_STT, &CMFCDemoDlg::OnBnClickedButtonStt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_TIMER, &CMFCDemoDlg::OnBnClickedRadioTimer)
	ON_BN_CLICKED(IDC_RADIO_EVENT, &CMFCDemoDlg::OnBnClickedRadioEvent)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCDemoDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_TUNING, &CMFCDemoDlg::OnBnClickedButtonTuning)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_PID, &CMFCDemoDlg::OnBnClickedButtonRequestPid)
END_MESSAGE_MAP()


// CMFCDemoDlg message handlers

BOOL CMFCDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ccbBaudrate.InsertString(0, (CString)"4800");
	m_ccbBaudrate.InsertString(1, (CString)"9600");
	m_ccbBaudrate.InsertString(2, (CString)"19200");
	m_ccbBaudrate.InsertString(3, (CString)"115200");
	m_ccbBaudrate.SetCurSel(3);

	return TRUE;  // return TRUE  unless you set the focus to a control
	//
	//
	//

	for (int i = 0; i < 1000; i++)
	{
		if (i == 0)
		{
			dPIDPlotTime[i] = 0.01;
		}
		else dPIDPlotTime[i] = dPIDPlotTime[i - 1] + 0.01;
	}

	CChartCtrl ref;
	ref.RemoveAllSeries();
	pChartCtrlPID.EnableRefresh(true);
	pBottomAxis = pCharCtrlPID.CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis = pCharCtrlPID.CreateStandardAxis(CChartCtrl::LeftAxis);
	pBottomAxis->SetMinMax(0, 2);
	pLeftAxis->SetMinMax(0, 100);
	CChartAxisLabel* m_pAxisLabel = pLeftAxis->GetLabel();
	m_pAxisLabel->SetText("Position");
	m_pAxisLabel = pBottomAxis->GetLabel();
	m_pAxisLabel->SetText("Time");
	pBottomAxis->SetTickIncrement(true, 200);
	pBottomAxis->SetDiscrete(false);
	pBottomAxis->EnableScrollBar(true);

	pChartPIDSeries = pCharCtrlPID.CreateLineSerie();
	pChartPIDSeries->ClearSerie();
	pChartPIDSeries->SetWidth(5); //linewidth
	pChartPIDSeries->SetColor(RGB(0, 0, 255));


}

void CMFCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDemoDlg::OnCbnDropdownComboSerialname()
{
	// TODO: Add your control notification handler code here
	m_ccbSerialName.ResetContent();

	TCHAR lpTargetPath[5000]={};
	DWORD test;
	bool gotPort = 0;

	for (int i = 0; i < 255; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		CString ComName = CString("COM") + CString(str);

		test = QueryDosDevice(ComName, (LPSTR)lpTargetPath, 2000);

		if (test != 0)
		{
			m_ccbSerialName.AddString((CString)ComName);
			gotPort = 1;
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpTargetPath[2000];
			continue;
		}



	}
	if (!gotPort)
		m_ccbSerialName.AddString((CString)"No Active Ports Found");

}


void CMFCDemoDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	if (bPortOpened == FALSE)
	{
		CString csPortName;
		CString csBaudRate;
		m_ccbSerialName.GetLBText(m_ccbSerialName.GetCurSel(), csPortName);
		m_ccbBaudrate.GetLBText(m_ccbBaudrate.GetCurSel(), csBaudRate);

		OpenPort(csPortName, csBaudRate);

	}
	else
	{
		ClosePort();
	}
}

/**
* \brief Handle Open Serial Port Events
* \param bSuccess
*/
void CMFCDemoDlg::OnEventOpen(BOOL bSuccess)
{
}

/**
* \brief Handle Close Serial Port Events
* \param bSuccess
*/
void CMFCDemoDlg::OnEventClose(BOOL bSuccess)
{
}

/**
* \brief Handle Receive Data from Serial
* \param inPacket
* \param inLength
*/
void CMFCDemoDlg::OnEventRead(char* inPacket, int inLength)
{
	CString csInPacket;
	csInPacket.Format((CString)"Receive: ");
	m_listboxRead.InsertString(0, csInPacket);
	csInPacket.Empty();

	for (UINT i = 0; i < (UINT)inLength; i++) {
		csInPacket.AppendFormat((CString)"%02X ", inPacket[i]);
	}
	m_listboxRead.InsertString(0, csInPacket);

	ProcessData(inPacket, inLength);


	CString str;
	str.Format((LPCTSTR)"%d bytes read", inLength);

	m_staticInfo.SetWindowText(str);
}

/**
* \brief Handle Write Data
* \param nWritten
*/
void CMFCDemoDlg::OnEventWrite(int nWritten)
{
}



void CMFCDemoDlg::OnBnClickedButtonSend1()
{
	// TODO: Add your control notification handler code here
	if (!GetPortActivateValue()) return;

	CString strW;
	m_ceSendCmd1.GetWindowText(strW);
	Write((LPTSTR)(LPCTSTR)strW, strW.GetLength());
}


void CMFCDemoDlg::OnBnClickedButtonPos()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bGPOS, sizeof(bGPOS));
	index += sizeof(bGPOS);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "POS CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonVel()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bGVEL, sizeof(bGVEL));
	index += sizeof(bGVEL);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "VEL CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonMov()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bMOVL, sizeof(bMOVL));
	index += sizeof(bMOVL);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "MOV CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonStt()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bSTT, sizeof(bSTT));
	index += sizeof(bSTT);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "STT CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}

VOID CMFCDemoDlg::ProcessData(char* data, int inLength)
{
	CString cmd;
	for (UINT i = 0; i < (UINT)inLength; i++)
	{
		bProtocolDataBuffer[i] = (BYTE)data[i];
	}
	for (UINT i = 1; i <= 4; i++)
	{
		cmd.AppendChar((char)bProtocolDataBuffer[i]);
	}
	for (UINT i = 8; i <= 15; i++)
	{
		bProtocolData[i - 8] = bProtocolDataBuffer[i];
	}

	if (cmd.Compare((LPCSTR)"GPOS") == 0)
	{

	}

	else if (cmd.Compare((LPCSTR)"MOVL") == 0)
	{

	}
	else if (cmd.Compare((LPCSTR)"GVEL") == 0)
	{

	}
	else if (cmd.Compare((LPCSTR)"GSTT") == 0)
	{
		if (bProtocolDataBuffer[12] == 0x01) {
			m_chLed1.SetCheck(1);
		}
		else {
			m_chLed1.SetCheck(0);
		}
		if (bProtocolDataBuffer[13] == 0x01) {
			m_chGPIO1.SetCheck(1);
		}
		else {
			m_chGPIO1.SetCheck(0);
		}
		if (bProtocolDataBuffer[14] == 0x01) {
			m_chLed2.SetCheck(1);
		}
		else {
			m_chLed2.SetCheck(0);
		}
		if (bProtocolDataBuffer[15] == 0x01) {
			m_chGPIO2.SetCheck(1);
		}
		else {
			m_chGPIO2.SetCheck(0);
		}

	}
}

void CMFCDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	CString cmd;

	if (!GetPortActivateValue()) return;

	switch (nIDEvent) {
	case EVENT_SEND_BY_TIMER:
		memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
		index += sizeof(bSTX);
		memcpy(ProtocolFrame + index, bSTT, sizeof(bSTT));
		index += sizeof(bSTT);
		memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
		index += sizeof(bOPT);
		memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
		index += sizeof(bDATA);
		memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
		index += sizeof(bSYNC);
		memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
		index += sizeof(bETX);

		Write((char*)ProtocolFrame, index);
		cmd.Format((LPCTSTR)"%s", "TIMER STT CMD: ");
		m_listboxRead.InsertString(0, cmd);
		cmd.Empty();
		for (UINT i = 0; i < index; i++)
		{
			cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
		}
		m_listboxRead.InsertString(0, cmd);
		break;
	default:
		break;
	}
		__super::OnTimer(nIDEvent);
}


void CMFCDemoDlg::OnBnClickedRadioTimer()
{
	// TODO: Add your control notification handler code here
	SetTimer(EVENT_SEND_BY_TIMER, 1000, nullptr);
}


void CMFCDemoDlg::OnBnClickedRadioEvent()
{
	// TODO: Add your control notification handler code here
	KillTimer(EVENT_SEND_BY_TIMER);

	m_listboxRead.InsertString(0, reinterpret_cast<LPCTSTR>("Stop timer"));
}


void CMFCDemoDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	CString strData;
	// DWORD dwRetCode;
	memset(bDATA, '\0', 8);

	double dKp = (double) GetDlgItemInt(IDC_EDIT_KP); // Nho xem lai doan nay
	double dKi = (double) GetDlgItemInt(IDC_EDIT_KI);
	double dKd = (double) GetDlgItemInt(IDC_EDIT_KD);
	// TODO: Add your control handler code
	BYTE bprotocol[50] = {};
	UINT index = 0;

	uint8_t nLengTithes;
	BYTE bKp[2], bKi[2], bKd[3];
	
	FloatToByteArray(static_cast<float>(dKp), bKp, &nLengTithes);
	bDATA[0] = bKp[0];
	bDATA[1] = bKp[1];

	FloatToByteArray(static_cast<float>(dKi), bKi, &nLengTithes);
	bDATA[2] = bKi[0];
	bDATA[3] = bKi[1];

	FloatToByteArrayWithNipes(dKd, bKd, &nLengTithes);
	bKd[2] = nLengTithes;
	bDATA[4] = bKd[0];
	bDATA[5] = bKd[1];
	bDATA[6] = bKd[2];

	if (!GetPortActivateValue()) return;

	memcpy(bprotocol + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(bprotocol + index, bSPID, sizeof(bSPID));
	index += sizeof(bSPID);
	memcpy(bprotocol + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(bprotocol + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(bprotocol + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(bprotocol + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	Write((char*)bprotocol, index);
}


void CMFCDemoDlg::OnBnClickedButtonTuning()
{
	BYTE bProtocol[50] = {};
	UINT index = 0;
	// TODO: Add your control notification handler code here

	if (!GetPortActivateValue()) return;
	memset(bDATA, '\0', 8);

	memcpy(bProtocol + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(bProtocol + index, bCTUN, sizeof(bCTUN));
	index += sizeof(bCTUN);
	memcpy(bProtocol + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(bProtocol + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(bProtocol + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(bProtocol + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	Write((char*)bProtocol, index);

	//Show on text box
	CString cmd;
	cmd.Format(static_cast<CString>("CMD: "));
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
}


void CMFCDemoDlg::OnBnClickedButtonRequestPid()
{
	BYTE bProtocol[50] = {};
	UINT index = 0;
	// TODO: Add your control notification handler code here
	memset(bDATA, '\0', 8);
	if (!GetPortActivateValue()) return;
	
	memcpy(bProtocol + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(bProtocol + index, bGPID, sizeof(bGPID));
	index += sizeof(bGPID);
	memcpy(bProtocol + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(bProtocol + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(bProtocol + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(bProtocol + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	Write((char*)bProtocol, index);
}
