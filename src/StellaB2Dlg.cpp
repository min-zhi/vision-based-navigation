// StellaB2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "StellaB2.h"
#include "PortDlg.h"
#include "StellaB2Dlg.h"
#include "StellaB2Function.h"
#include "SerialPort.h"

//open CV header
#include<cv.h>
#include<cxcore.h>
#include "CvvImage.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<highgui.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <ml.h>
#include<MMSystem.h>
#pragma comment(lib,"winmm.lib")
#include <stdio.h>

#define pi 3.141592

using namespace cv;
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvvImage m_CvvImage;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CRect rect;
//int t=0;
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStellaB2Dlg dialog




CStellaB2Dlg::CStellaB2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStellaB2Dlg::IDD, pParent)

	, m_strState(_T(""))
	, m_PortNumShow(_T(""))
	, m_leftTraveled(_T("0"))
	, m_rightTraveled(_T("0"))
	, m_StrCommand(_T("CPAR000DF000T000"))
	, m_strLeftSpeed(_T("0"))
	, m_strRightSpeed(_T("0"))
	, m_Sign(_T(""))
	, m_NextDist(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStellaB2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATE, m_state);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDX_Text(pDX, IDC_PORT_NUM, m_PortNumShow);
	DDX_Text(pDX, IDC_LEFT_TRAVEL, m_leftTraveled);
	DDX_Text(pDX, IDC_RIGHT_TRAVEL, m_rightTraveled);
	DDX_Text(pDX, IDC_COMMAND, m_StrCommand);
	//DDV_MaxChars(pDX, m_leftTraveled, 9);
	//DDV_MaxChars(pDX, m_rightTraveled, 9);
	DDX_Text(pDX, IDC_LEFT_SPEED, m_strLeftSpeed);
	DDX_Text(pDX, IDC_RIGHT_SPEED, m_strRightSpeed);
	DDX_Text(pDX, IDC_SIGN, m_Sign);
	DDX_Text(pDX, IDC_NEXT_SIGN_DISTANCE, m_NextDist);
}

BEGIN_MESSAGE_MAP(CStellaB2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FORWARD, &CStellaB2Dlg::OnBnClickedForward)
	ON_BN_CLICKED(IDC_BACKWARD, &CStellaB2Dlg::OnBnClickedBackward)
	ON_BN_CLICKED(IDC_LEFT, &CStellaB2Dlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CStellaB2Dlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_STOP, &CStellaB2Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CStellaB2Dlg::OnBnClickedProg1)
	ON_BN_CLICKED(IDC_PROGRAM2, &CStellaB2Dlg::OnBnClickedProgram2)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_EXIT, &CStellaB2Dlg::OnFileExit)
	ON_COMMAND(ID_HELP_ABOUT, &CStellaB2Dlg::OnHelpAbout)
	ON_COMMAND(ID_SETTINGS_COMPORT, &CStellaB2Dlg::OnSettingsComport)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CStellaB2Dlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_ERROR_SIGNAL, &CStellaB2Dlg::OnEnChangeErrorSignal)
	ON_BN_CLICKED(IDC_CAM_RUN, &CStellaB2Dlg::OnBnClickedCamRun)
END_MESSAGE_MAP()


// CStellaB2Dlg message handlers

BOOL CStellaB2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	pwnd = GetDlgItem(IDC_CAMERA_SCR);
	//pwnd->MoveWindow(35,30,387,318);
	pDC =pwnd->GetDC();
	hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	m_PortDlg.DoModal();
	m_PortNum = m_PortDlg.m_comport;
	m_PortNumShow.Format(_T("%d"),m_PortNum);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStellaB2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStellaB2Dlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStellaB2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void angle_distance(float* angle, float* distance, float width_rate, float height_rate, float M_distance, int x, int y, int width, int height);
float* HOG(IplImage* frame, int nwin_x, int nwin_y, int B);
float* HOG(IplImage* frame, int nwin_x, int nwin_y, int B){

	float* H;
	int L = frame->height;
	int C = frame->width;
	H = (float*)malloc((nwin_x*nwin_y*B)*sizeof(float));

	int step_x = floor((float)C/(nwin_x+1));
	int step_y = floor((float)L/(nwin_y+1));
	int cnt = 0;
	float** angles;
	float** magnit;
	angles = (float**)malloc(L*sizeof(float*));
	magnit = (float**)malloc(L*sizeof(float*));
	for(int i = 0;i<L;i++){
		angles[i] = (float*)malloc(C*sizeof(float));
		magnit[i] = (float*)malloc(C*sizeof(float));
	}

	IplImage *xsobel, *ysobel;
	xsobel = cvCreateImage(cvGetSize(frame),  IPL_DEPTH_16S, 1);
	ysobel = cvCreateImage(cvGetSize(frame),  IPL_DEPTH_16S, 1);

	cvSobel(frame, ysobel, 0,1,3);
	cvSobel(frame, xsobel, 1,0,3);

	int index;
	float y, x;
	for(int i=0;i<L;i++){
		for(int j=0;j<C;j++){
			index = j+i*ysobel->widthStep;
			y = ysobel->imageData[index];
			x = xsobel->imageData[index];
			angles[i][j] = atan2(y, x);
			magnit[i][j] = sqrt(y*y+x*x);
		}
	}
	float* tangles;
	float* tmagnit;
	float* H2;
	float S=0;
	int K = 2*step_y*2*step_x;
	int Kcnt, bin;
	tangles = (float*)malloc(K*sizeof(float));
	tmagnit = (float*)malloc(K*sizeof(float));
	H2 = (float*)malloc(B*sizeof(float));

	for(int i=0;i<B;i++){
		H2[i] = 0;
	}

	for(int i=0;i<=nwin_y-1;i++){
		for(int j=0;j<=nwin_x-1;j++){
			cnt = cnt+1;
			Kcnt = 0;
			for(int sy=i*step_y;sy<(i+2)*step_y;sy++){
				for(int sx=j*step_x;sx<(j+2)*step_x;sx++){
					tangles[Kcnt] = angles[sy][sx];
					tmagnit[Kcnt] = magnit[sy][sx];
					Kcnt = Kcnt+1;
				}
			}
			bin = 0;
			for(float ang_lim = -pi+(2*pi/(float)B); ang_lim<=pi+0.2;ang_lim+=(2*pi/(float)B)){
				for(int k=0;k<K;k++){
					if(tangles[k]<ang_lim){
						tangles[k] = 100;
						H2[bin]=H2[bin]+tmagnit[k];
					}
				}
				S+=H2[bin]*H2[bin];
				bin+=1;
			}
			S = sqrt(S);
			for(int i=0;i<B;i++){
				//printf("%lf, ", H2[i]);
				H2[i] = H2[i]/(S+0.01);
				//printf("%lf\n", H2[i]);
			}
			Kcnt = 0;
			for(int k=(cnt-1)*B;k<cnt*B;k++){
				H[k] = H2[Kcnt];
				Kcnt+=1;
			}
		}
	}
	for (int i=0; i<L; i++){
		free(angles[i]);
		free(magnit[i]);
	}
	free(angles);
	free(magnit);

	cvReleaseImage(&xsobel);
	cvReleaseImage(&ysobel);
	free(tangles);
	free(tmagnit);
	free(H2);
	return H;
}

void angle_distance(float* angle, float* distance, float width_rate, float height_rate, float M_distance, int x, int y, int width, int height)
{
	int Px = x-width/2;
	int Py = height-y;
	float Tdx = width_rate*Px;
	float Tdy = height_rate*Py;

	*distance = sqrt((Tdy+M_distance)*(Tdy+M_distance) + Tdx*Tdx);
	*angle = atan2(Tdx,(Tdy+M_distance))*180/pi;
}

void CStellaB2Dlg::InitPort(CSerialPort *_sp)
{
	_sp->Open(m_PortNum, 115200, CSerialPort::NoParity, 8, CSerialPort::OneStopBit, CSerialPort::NoFlowControl, NULL);
	_sp->SetTimeout (100, 100, 10);

}
void CStellaB2Dlg::InitStella(CStellaB2Function *_sf)
{
	_sf->Init ();
	_sf->Reset ();
}
void CStellaB2Dlg::OnBnClickedForward()
{	CString s;
CSerialPort *_sp = new CSerialPort ();
InitPort(_sp);
CStellaB2Function *_sf = new CStellaB2Function (_sp);
InitStella(_sf);
_sf->Forward();
SetTimer(ID_CLOCK_TIMER,1000,NULL);
SetTimer(ID_TRAVEL_TIMER,100,NULL);
delete _sp;
delete _sf;	

}

void CStellaB2Dlg::OnBnClickedBackward()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	_sf->Backward ();
	SetTimer(ID_TRAVEL_TIMER,200,NULL);

	delete _sf;
	delete _sp;	
}

void CStellaB2Dlg::OnBnClickedLeft()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);
	_sf->Left ();
	delete _sp;
	delete _sf;	
}

void CStellaB2Dlg::OnBnClickedRight()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);
	_sf->Right ();
	delete _sf;
	delete _sp;	
}


void CStellaB2Dlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);
	_sf->Stop (3);
	delete _sf;
	delete _sp;	
}

void CStellaB2Dlg::OnBnClickedProg1()  //command 10 seconds 3.14M moving in the right direction to the goal of a 360-degree rotation
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);
	_sf->Rotate360();
	delete _sf;
	delete _sp;	
}

void CStellaB2Dlg::OnBnClickedProgram2()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);
	_sf->Program2();
	delete _sf;
	delete _sp;		
}
CString StripExtraWhitespace (const CString & strInput)
{
	CString strOutput;
	LPTSTR o=strOutput.GetBuffer(1+strInput.GetLength());
	for(LPCTSTR c=strInput;*c;c++){
		if(_istspace(*c)){
			*o++=_TCHAR(' ');
			if (_istspace(c[1])){
				for(c=c+2;_istspace(*c);c++);
				c--;
			}
		} else {
			*o++=*c;
		}
	}
	*o=0;
	strOutput.ReleaseBuffer();
	strOutput.TrimLeft();
	strOutput.TrimRight();
	return strOutput;
}
void CStellaB2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	//InitStella(_sf);
	//_sf->Stop (3);
	char state,allFeedBack;
	unsigned char left[9]={0,},right[9]={0,},speedL[6]={0,},speedR[6]={0,};

	CString s;
	_sf->GetState (&state);
	_sf->GetPosition(right,left);
	_sf->GetSpeed(speedR,speedL);
	UpdateData();
	switch(nIDEvent)
	{
	case ID_CLOCK_TIMER:
		m_strState=_sf->GetStateString(state); //Display the state of robot;
		break;
	case ID_TRAVEL_TIMER:
		s.Format(_T("%S"),right);
		m_rightTraveled=StripExtraWhitespace(s);
		m_leftTraveled.Format(_T("%S"),left);

		m_strLeftSpeed.Format(_T("%S"),speedL);
		m_strRightSpeed.Format(_T("%S"),speedR);
		break;
	case ID_SPEED_TIMER:
		//m_strLeftSpeed.Format(_T("%S"),speedL);
		//m_strRightSpeed.Format(_T("%S"),speedR);
		break;
	case ID_SIGN_LEFT:
		m_Sign =_T("Turn LEFT");
		break;
	case ID_SIGN_RIGHT:
		m_Sign =_T("Turn RIGHT");
		break;
	case ID_SIGN_STRAIGHT:
		m_Sign =_T("STRAIGHT");
		break;
	case ID_NO_SIGN:
		m_Sign =_T("No sign detected");
		break;
	case ID_NEXT_DIST:
		UpdateData(false);
		break;
	}

	delete _sf;
	delete _sp;	
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}


void CStellaB2Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//cvReleaseCapture( &capture );
	cvDestroyAllWindows();
	OnOK();
	CDialog::OnClose();
}

void CStellaB2Dlg::OnFileExit()
{
	// TODO: Add your command handler code here
	//cvReleaseCapture( &capture );
	cvDestroyAllWindows();
	OnOK();
}

void CStellaB2Dlg::OnHelpAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	//show the about dialog
	dlgAbout.DoModal();
}

void CStellaB2Dlg::OnSettingsComport()
{
	// TODO: Add your command handler code here
	//CPortDlg dlgComPort;
	m_PortDlg.DoModal();
	m_PortNum = m_PortDlg.m_comport;
	m_PortNumShow.Format(_T("%d"),m_PortNum);
	UpdateData(FALSE);
}

void CStellaB2Dlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	InitStella(_sf);

	_sf->WriteCommand(m_StrCommand) ;
	//UpdateData(FALSE);
	SetTimer(ID_CLOCK_TIMER,1000,NULL);
	SetTimer(ID_TRAVEL_TIMER,200,NULL);

	delete _sf;
	delete _sp;	

}

void CStellaB2Dlg::OnEnChangeErrorSignal()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

bool Equal(float a, float b)
{
	float epsilon=0.00001;
	return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}


bool IsGreaterThan(float a, float b)
{
	float epsilon=0.00001;
	return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool IsLessThan(float a, float b)
{
	float epsilon=0.00001;
	return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

void CStellaB2Dlg::OnBnClickedCamRun()
{
	// TODO: Add your control notification handler code here
	DWORD Start = timeGetTime();     //Timer Count Start
	DWORD End = timeGetTime();
	DWORD Elapsed;
	BOOL last_Init=FALSE;
	BOOL first_Run=FALSE;
	int turning_right=1;
	int turning_left=1;
	int stop=1;
	int straight=1;
	float m_Flt_Travel,m_Flt_Next_Dist=0,m_Flt_Final_Dist=0;
	CString m_Final_Dist;
	char direction[20],direction2[20];
	//DWORD dwElapsed = dwEnd-dwStart;
	//printf("%d  time :%d \n", cnt, dwElapsed);
	//const char* path="test.txt";
	//LPCTSTR test123;
	CStdioFile 	angle_log( _T("test.txt"), CFile::modeCreate|CFile::modeWrite);
	angle_log.WriteString(_T("Hello world \n"));
	CvCapture* capture = cvCaptureFromCAM(1);   //highcam  //Video Initialize
	CvCapture* capture2 = cvCaptureFromCAM(0);   //lowcam
	if(!capture)
	{
		printf("You need to connect a camera to your computer");
		//return 0;
	}
	if(!capture2)
	{
		printf("You need to connect a camera to your computer");
		//return 0;
	}
	int widtht = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	int heightt = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);	
	IplImage* framet;
	IplImage* birds_image =  cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	IplImage* frame;
	int width = widtht/2;
	int height = heightt/2;
	int widtht2 = (int)cvGetCaptureProperty(capture2,CV_CAP_PROP_FRAME_WIDTH);
	int heightt2 = (int)cvGetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT);	
	IplImage* framet2;
	IplImage* frame2;
	int width2 = widtht2/2;
	int height2 = heightt2/2;


	int Gm = 400;
	int Lm = 300;
	int Rm = 300;
	int St = 300;
	int Cm = 300;
	int nm = 600;

	char str[400];


	float data[2200][32];	
	float Teacher[2200];
	float data2[1912][32];	
	float Teacher2[1912];
	int i,j;
	int index;
	int index2;
	int index_2;
	int index2_2;
	CvScalar CVX_RED = cvScalar(0,0,255);
	CvScalar CVX_BLUE = cvScalar(255,0,0);
	
	char d;
	int h, s, v, h2, s2, v2;
	IplImage* fGray;
	IplImage* Gray = 0;
	IplImage* test = 0;
	Gray = cvCreateImage(cvSize(width, height),  8, 1);
	fGray = cvCreateImage(cvSize(width, height), 8, 1);
	frame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	IplImage *hsv = cvCreateImage(cvSize(width, height), 8, 3);

	IplImage* fGray2;
	IplImage* Gray2 = 0;
	IplImage* test2 = 0;
	Gray2 = cvCreateImage(cvSize(width, height),  8, 1);
	fGray2 = cvCreateImage(cvSize(width, height), 8, 1);
	frame2 = cvCreateImage(cvSize(width2, height2), IPL_DEPTH_8U, 3);
	IplImage *hsv2 = cvCreateImage(cvSize(width, height), 8, 3);

	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	CvRect bbs[1000];
	
	CvMemStorage* storage2 = cvCreateMemStorage();
	CvSeq* first_contour2 = NULL;
	CvRect bbs2[1000];
	
	float* HO;
	float* HO2;
	int delay;
	int nwin_x = 2;
	int nwin_y = 2;
	int B = 8;
	int Dem = nwin_x*nwin_y*B; 
	char str2[400];  
	int cntt;
	float M_dist = 91.5;
	float width_rate = (float)5.7/22;
	float height_rate = (float)4.1/10;
	
	float M_dist2 = 24.5;
	float width_rate2 = (float)5.7/40;
	float height_rate2 = (float)4.1/45;
	int response;
	int response2;
	FILE* D;
	FILE* T;
	FILE* D2;
	FILE* T2;
	D = fopen("Data.txt", "r+");
	T = fopen("Teach.txt", "r+");
	D2 = fopen("Data2.txt", "r+");
	T2 = fopen("Teach2.txt", "r+");
	//font initialize
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4f, 0.4f);
	int Fc = Gm+Rm+Lm+St+Cm+nm;
	int Fc2 = Gm+Rm+Lm+St+nm;

	//SVM Classification Algorithm parpameters
	CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_EPS, 100, 1e-6);
	CvSVMParams param (CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);
	CvSVM SVM_S;
	CvSVM SVM_S2;

	//Load learning data
	for(int i=0;i<Fc;i++)
	{
		fscanf(T, "%f \n",&Teacher[i]);
		for(int j=0;j<(Dem);j++)
		{
			fscanf(D, "%f ",&data[i][j]);
		}
		fscanf(D, "\n");
	}
	for(int i=0;i<Fc2;i++)
	{
		fscanf(T2, "%f \n",&Teacher2[i]);
		for(int j=0;j<(Dem);j++)
		{
			fscanf(D2, "%f ",&data2[i][j]);
		}
		fscanf(D2, "\n");
	}
	//SVM algorithm Training
	Mat labels_S(Fc, 1, CV_32FC1, Teacher);
	Mat trainingData_S(Fc, Dem, CV_32FC1, data);
	SVM_S.train(trainingData_S, labels_S,Mat(), Mat(), param);

	Mat labels_S2(Fc2, 1, CV_32FC1, Teacher2);
	Mat trainingData_S2(Fc2, Dem, CV_32FC1, data2);
	SVM_S2.train(trainingData_S2, labels_S2,Mat(), Mat(), param);
	//for(int i = 0; i < Fc; i++)
	//{
	//	// 열의 메모리를 제거
	//	delete[] data[i];
	//	// 행 포인터 초기화
	//}
	//delete[] Teacher;
	fclose(D);
	fclose(T);
	fclose(D2);
	fclose(T2);
	HO2 = (float*)malloc(Dem*sizeof(float));
	cntt = 0;
	//DWORD End = timeGetTime();    //Timer Count END

	//Calibration Matrix
	// CvMat* intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	// CvMat* distortion = (CvMat*)cvLoad("Distortion.xml");
	// IplImage* mapx = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
	// IplImage* mapy = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
	// cvInitUndistortMap(intrinsic, distortion, mapx, mapy);
	//IplImage* birds_image;
	//Bird_eyes
	CvMat* H = (CvMat*)cvLoad("H.xml");
	float Z = 34;
	//Z = 0;
	//DWORD End = timeGetTime();    //Timer Count END
	float angle;
	float distance;
	int xp = width/2;
	int yp = height/2;
	int hs = 10;
	int ws = 10;
	int Nc,n,Nc2,n2;
	float angle2;
	float distance2;
	while(1)
	{
		//End = timeGetTime();    
		framet = cvQueryFrame( capture );   //Get a frame from Camera
		if(!framet ) 
			break;

		framet2 = cvQueryFrame( capture2 );   //Get a frame from Camera
		if(!framet2 ) 
			break;
		cvResize(framet2, frame2, CV_INTER_CUBIC);    //Image Resize
		//cvShowImage("Origin", framet);
		//cvResize(framet, frame, CV_INTER_CUBIC);    //Image Resize
		//cvRemap(framet, framet, mapx, mapy);
		//birds_image = cvCloneImage(framet);
		cvCopy(framet, birds_image);
		cvSetImageROI(framet, cvRect(0,100, framet->width, framet->height-100));

		CV_MAT_ELEM(*H, float, 2,2) = Z;
		//printf("%f \n", Z);
		cvWarpPerspective(framet, birds_image, H, CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);
		cvResetImageROI(framet);
		//cvShowImage("Video", frame);
		cvSetImageROI(birds_image, cvRect(130,0, birds_image->width-320, birds_image->height-20));
		//cvShowImage("Video2", birds_image);

		cvResize(birds_image, frame, CV_INTER_CUBIC);    //Image Resize
		cvResetImageROI(birds_image);
		cvSetImageROI(framet2, cvRect(0,0, framet2->width, framet2->height-170));
		cvResize(framet2, frame2, CV_INTER_CUBIC);    //Image
		//cvShowImage("Video", frame);
		//if(End-Start > 1000){
		cvCvtColor(frame, hsv, CV_BGR2HSV);          //Convert Image from RGB to HSV
		cvCvtColor(frame, fGray, CV_RGB2GRAY);       //Convert Image from RGB to GRAY
		cvZero(Gray);
		
		cvCvtColor(frame2, hsv2, CV_BGR2HSV);          //Convert Image from RGB to HSV
		cvCvtColor(frame2, fGray2, CV_RGB2GRAY);       //Convert Image from RGB to GRAY
		cvZero(Gray2);
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				index = 3*j+i*hsv->widthStep;
				index2 = j+i*Gray->widthStep;
				h = (unsigned char)hsv->imageData[index];
				s = (unsigned char)hsv->imageData[index+1];
				v = (unsigned char)hsv->imageData[index+2];
				
				index_2 = 3*j+i*hsv2->widthStep;
				index2_2 = j+i*Gray2->widthStep;
				h2 = (unsigned char)hsv2->imageData[index_2];
				s2 = (unsigned char)hsv2->imageData[index_2+1];
				v2 = (unsigned char)hsv2->imageData[index_2+2];

				if(((h<=10)||(h>=170))&&(s>120 && s<=240)){              // Extract Candidates by color information
					//if(((h>180*0.6)&&(s>255*0.2))){
					Gray->imageData[index2] = 255;
				}
				if(((h2<=10)||(h2>=170))&&(s2>120 && s2<=240)){              // Extract Candidates by color information
					//if(((h>180*0.6)&&(s>255*0.2))){
					Gray2->imageData[index2] = 255;
				
				}
			}
		}

		cvSmooth(Gray,Gray,CV_MEDIAN,3,3);         //Medina filter
		cvSmooth(Gray2,Gray2,CV_MEDIAN,3,3);         //Medina filter
		Nc = cvFindContours(Gray, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST);   //Find contour
		n = 0;
		response = 0;

		for(CvSeq* c=first_contour; c!=NULL; c=c->h_next)
		{
			cvDrawContours(frame, c, CVX_RED, CVX_BLUE, 0,2,8);          //draw contour
			bbs[n] = cvBoundingRect(c,0);                                

			// Extract effective Candidates base on size and position of the candidate
			if((bbs[n].width > 10 && bbs[n].height > 10)&&(bbs[n].width < 100 && bbs[n].height < 100))
			{
				cvRectangle(frame, cvPoint( bbs[n].x, bbs[n].y),cvPoint(bbs[n].x + bbs[n].width, bbs[n].y + bbs[n].height),CV_RGB(255,255,255),1);  //draw bounding box
				test = cvCreateImage(cvSize(bbs[n].width, bbs[n].height),  8, 1);   //Candidate Data space
				cvZero(test);
				for(int i=0;i<test->height;i++){
					for(int j=0;j<test->width;j++){
						index = j+i*test->widthStep;
						index2 = (bbs[n].x+j)+(bbs[n].y+i)*Gray->widthStep;
						if(Gray->imageData[index2] > 0)
							test->imageData[index] = fGray->imageData[index2];
					}
				}
				cvThreshold(test,test, 10, 255,CV_THRESH_BINARY);         //Image Binarization
				cvShowImage("fGray", test);
				HO = HOG(test, nwin_x, nwin_y, B);                         //HOG feature 
				Mat sampleMat(1, Dem, CV_32FC1, HO); 
				response = SVM_S.predict(sampleMat);                   //SVM predict
				sampleMat.release();
				cvReleaseImage(&test2);

				//init serial port
				CSerialPort *_sp = new CSerialPort ();
				InitPort(_sp);
				CStellaB2Function *_sf = new CStellaB2Function (_sp);
				if(last_Init==FALSE)
					InitStella(_sf);
				last_Init=TRUE;

				if(response==1){           //Straight

					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
					if(first_Run==FALSE)
						_sf->Velocity(15,15);
					else _sf->Velocity(25,25);
					first_Run=TRUE;
					

					cvPutText( frame, "Straight", cvPoint( bbs[n].x-20, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs[n].x + bbs[n].width/2;
					yp = bbs[n].y + bbs[n].height/2;
					cvRectangle(frame, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					//	printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
					angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
					sprintf(str2,"Angle : %f, Distance : %f", angle, distance);
					cvPutText( frame, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					
					if(IsGreaterThan(angle,-1)&&IsLessThan(angle,1))//(-1<angle<1)
					{	     
						    sprintf( direction,"Move Straight");
							_sf->Velocity(25,25); Sleep(1);
							
					}
					if(IsGreaterThan(angle,1)&&IsLessThan(angle,2))//(1<angle<2)
					{	
							sprintf( direction,"Move Right 1");
							_sf->Velocity(25,15); Sleep(1);
							
					}if(IsGreaterThan(angle,2)&&IsLessThan(angle,3))//(2<angle<3)
					{	
                            sprintf( direction,"Move Right 2"); 
							_sf->Velocity(35,15); Sleep(1);
							
					}if(IsGreaterThan(angle,3))//(angle>3)
					{	
						    sprintf( direction,"Move Right 3");
							_sf->Velocity(40,10); Sleep(1);
							
					}if(IsGreaterThan(angle,-2)&&IsLessThan(angle,-1))//(-1>angle>-2)
					{	
						    sprintf( direction,"Move Left 1");
							_sf->Velocity(15,25);Sleep(1);
							
					}if(IsGreaterThan(angle,-3)&&IsLessThan(angle,-2))//(-2>angle>-3)
					{	
						    sprintf( direction,"Move Left 2");
							_sf->Velocity(15,35);Sleep(1);
							
					}if(IsLessThan(angle,-3))//(angle<-3)
					{	
						    sprintf( direction,"Move Left 3");
							_sf->Velocity(10,40);Sleep(1);
							
					}else 
					{
							_sf->Velocity(25,25); Sleep(1);
					}
					_sf->Velocity(25,25); Sleep(1);
					cvPutText( frame, direction, cvPoint(0 , 200 ), &font, cvScalar( 255, 0, 0 ) );
				}else if(response==2){     // Right Turn
					//Start = timeGetTime();
					SetTimer(ID_CLOCK_TIMER,1000,NULL);
					SetTimer(ID_TRAVEL_TIMER,200,NULL);
					_sf->Velocity(25,25);
					
					cvPutText( frame, "Right turn", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs[n].x + bbs[n].width/2;
					yp = bbs[n].y + bbs[n].height/2;
					cvRectangle(frame, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					//	printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
					angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
					sprintf(str2,"Angle : %f, Distance : %f", angle, distance);
					cvPutText( frame, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					if(IsGreaterThan(angle,-1)&&IsLessThan(angle,1))//(-1<angle<1)
					{	     
						sprintf( direction,"Move Straight");
						_sf->Velocity(25,25); Sleep(1);

					}
					if(IsGreaterThan(angle,1)&&IsLessThan(angle,2))//(1<angle<2)
					{	
						sprintf( direction,"Move Right 1");
						_sf->Velocity(25,15); Sleep(1);

					}if(IsGreaterThan(angle,2)&&IsLessThan(angle,3))//(2<angle<3)
					{	
						sprintf( direction,"Move Right 2"); 
						_sf->Velocity(35,15); Sleep(1);

					}if(IsGreaterThan(angle,3))//(angle>3)
					{	
						sprintf( direction,"Move Right 3");
						_sf->Velocity(40,10); Sleep(1);

					}if(IsGreaterThan(angle,-2)&&IsLessThan(angle,-1))//(-1>angle>-2)
					{	
						sprintf( direction,"Move Left 1");
						_sf->Velocity(15,25);Sleep(1);

					}if(IsGreaterThan(angle,-3)&&IsLessThan(angle,-2))//(-2>angle>-3)
					{	
						sprintf( direction,"Move Left 2");
						_sf->Velocity(15,35);Sleep(1);

					}if(IsLessThan(angle,-3))//(angle<-3)
					{	
						sprintf( direction,"Move Left 3");
						_sf->Velocity(10,40);Sleep(1);

					}else 
					{
						_sf->Velocity(25,25); Sleep(1);
					}
					_sf->Velocity(25,25); Sleep(1);
					cvPutText( frame, direction, cvPoint(0 , 200 ), &font, cvScalar( 255, 0, 0 ) );
				}else if(response==3){     // Left
					SetTimer(ID_CLOCK_TIMER,1000,NULL);
					SetTimer(ID_TRAVEL_TIMER,200,NULL);
					_sf->Velocity(25,25);
					
					cvPutText( frame, "Left turn", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs[n].x + bbs[n].width/2;
					yp = bbs[n].y + bbs[n].height/2;
					cvRectangle(frame, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					//	printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
					angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
					sprintf(str2,"Angle : %f, Distance : %f", angle, distance);
					cvPutText( frame, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					if(IsGreaterThan(angle,-1)&&IsLessThan(angle,1))//(-1<angle<1)
					{	     
						sprintf( direction,"Move Straight");
						_sf->Velocity(25,25); Sleep(1);

					}
					if(IsGreaterThan(angle,1)&&IsLessThan(angle,2))//(1<angle<2)
					{	
						sprintf( direction,"Move Right 1");
						_sf->Velocity(25,15); Sleep(1);

					}if(IsGreaterThan(angle,2)&&IsLessThan(angle,3))//(2<angle<3)
					{	
						sprintf( direction,"Move Right 2"); 
						_sf->Velocity(35,15); Sleep(1);

					}if(IsGreaterThan(angle,3))//(angle>3)
					{	
						sprintf( direction,"Move Right 3");
						_sf->Velocity(40,10); Sleep(1);

					}if(IsGreaterThan(angle,-2)&&IsLessThan(angle,-1))//(-1>angle>-2)
					{	
						sprintf( direction,"Move Left 1");
						_sf->Velocity(15,25);Sleep(1);

					}if(IsGreaterThan(angle,-3)&&IsLessThan(angle,-2))//(-2>angle>-3)
					{	
						sprintf( direction,"Move Left 2");
						_sf->Velocity(15,35);Sleep(1);

					}if(IsLessThan(angle,-3))//(angle<-3)
					{	
						sprintf( direction,"Move Left 3");
						_sf->Velocity(10,40);Sleep(1);

					}else 
					{
						_sf->Velocity(25,25); Sleep(1);
					}
					_sf->Velocity(25,25); Sleep(1);
					cvPutText( frame, direction, cvPoint(0 , 200 ), &font, cvScalar( 255, 0, 0 ) );
				}else if(response==4){     // Stop
					SetTimer(ID_CLOCK_TIMER,10,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
				    _sf->Velocity(25,25);
					
					cvPutText( frame, "Stop", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs[n].x + bbs[n].width/2;
					yp = bbs[n].y + bbs[n].height/2;
					cvRectangle(frame, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					//	printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
					angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
					sprintf(str2,"Angle : %f, Distance : %f", angle, distance);
					cvPutText( frame, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					if(IsGreaterThan(angle,-1)&&IsLessThan(angle,1))//(-1<angle<1)
					{	     
						sprintf( direction,"Move Straight");
						_sf->Velocity(25,25); Sleep(1);

					}
					if(IsGreaterThan(angle,1)&&IsLessThan(angle,2))//(1<angle<2)
					{	
						sprintf( direction,"Move Right 1");
						_sf->Velocity(25,15); Sleep(1);

					}if(IsGreaterThan(angle,2)&&IsLessThan(angle,3))//(2<angle<3)
					{	
						sprintf( direction,"Move Right 2"); 
						_sf->Velocity(35,15); Sleep(1);

					}if(IsGreaterThan(angle,3))//(angle>3)
					{	
						sprintf( direction,"Move Right 3");
						_sf->Velocity(40,10); Sleep(1);

					}if(IsGreaterThan(angle,-2)&&IsLessThan(angle,-1))//(-1>angle>-2)
					{	
						sprintf( direction,"Move Left 1");
						_sf->Velocity(15,25);Sleep(1);

					}if(IsGreaterThan(angle,-3)&&IsLessThan(angle,-2))//(-2>angle>-3)
					{	
						sprintf( direction,"Move Left 2");
						_sf->Velocity(15,35);Sleep(1);

					}if(IsLessThan(angle,-3))//(angle<-3)
					{	
						sprintf( direction,"Move Left 3");
						_sf->Velocity(10,40);Sleep(1);

					}else 
					{
						_sf->Velocity(25,25); Sleep(1);
					}
					_sf->Velocity(25,25); Sleep(1);
					cvPutText( frame, direction, cvPoint(0 , 200 ), &font, cvScalar( 255, 0, 0 ) );
				}else if(response==5){     // robot
					//system("cls");
					cvPutText( frame, "Robot", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );

					xp = bbs[n].x + bbs[n].width/2;
					yp = bbs[n].y + bbs[n].height/2;
					cvRectangle(frame, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					//printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
					_sf->Velocity(0,0);
					angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
					/*sprintf(str2,"A:%f, D:%f \n", angle, distance);
					cvPutText(frame, str2, cvPoint(xp, yp+10 ), &font, cvScalar(255, 0, 0 ));*/
					//printf("angle : %f, Distance: %f \n", angle, distance);
				}else if(response==6){     // Negative features
					//printf("쓰레기\n");
					//system("cls");
					cvPutText( frame, "쓰레기", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );

				}


				//cvWaitKey(100);
				//sprintf(str2,"D:\\AGV\\Bird_eyes_real_camera\\Feature_Data\\video_%04d.jpg",cntt);
				//cntt+=1;
				//cvSaveImage(str2,test, 0);
				// Image Center position and Current Marker Position
				//printf("Center Position(x, y) -> (%d, %d),  Maker Position(x, y) -> (%d, %d) \n", width/2, height/2, bbs[n].x + bbs[n].width/2, bbs[n].y + bbs[n].height/2);
				//angle_distance(&angle, &distance,  width_rate, height_rate, M_dist,  bbs[n].x + bbs[n].width/2,  bbs[n].y + bbs[n].height/2,  width, height);
				//printf("angle : %f, Distance: %f \n", angle, distance);
				//system("cls");
				delete _sf;
				delete _sp;	
			}

			n++;
			//cvWaitKey(1);

		}
		//}
		cvReleaseImage(&test);
		cvDestroyWindow("fGray");


		Nc2 = cvFindContours(Gray2, storage2, &first_contour2, sizeof(CvContour), CV_RETR_LIST);   //Find contour
		n2 = 0;
		response2 = 0;
		
		for(CvSeq* c2=first_contour2; c2!=NULL; c2=c2->h_next)
		{
			cvDrawContours(frame2, c2, CVX_RED, CVX_BLUE, 0,2,8);          //draw contour
			bbs2[n] = cvBoundingRect(c2,0);                                

			// Extract effective Candidates base on size and position of the candidate
			if((bbs2[n].width >30 && bbs2[n].height > 30)&&(bbs2[n].width < 300 && bbs2[n].height < 400))
			{
				cvRectangle(frame2, cvPoint( bbs2[n].x, bbs2[n].y),cvPoint(bbs2[n].x + bbs2[n].width, bbs2[n].y + bbs2[n].height),CV_RGB(255,255,255),1);  //draw bounding box
				test2 = cvCreateImage(cvSize(bbs2[n].width, bbs2[n].height),  8, 1);   //Candidate Data space
				cvZero(test2);
				for(int i=0;i<test2->height;i++){
					for(int j=0;j<test2->width;j++){
						index_2 = j+i*test2->widthStep;
						index2_2 = (bbs2[n].x+j)+(bbs2[n].y+i)*Gray2->widthStep;
						if(Gray2->imageData[index2_2] > 0)
							test2->imageData[index_2] = fGray2->imageData[index2_2];
					}
				}
				cvThreshold(test2,test2, 10, 255,CV_THRESH_BINARY);         //Image Binarization
				cvShowImage("fGray2", test2);
				HO2 = HOG(test2, nwin_x, nwin_y, B);                         //HOG feature 
				Mat sampleMat2(1, Dem, CV_32FC1, HO2); 
				response2 = SVM_S2.predict(sampleMat2);                   //SVM predict
				sampleMat2.release();             //svm predict
				cvReleaseImage(&test2);

				// printf("%d " ,response2);
				// Feature 이미지 저장
			/*	sprintf(str2,"D:\\AGV\\new_test2\\Feature2\\video_%04d.jpg",cntt);
				cntt+=1;
				cvSaveImage(str2,test2, 0);*/
				CSerialPort *_sp = new CSerialPort ();
				InitPort(_sp);
				CStellaB2Function *_sf = new CStellaB2Function (_sp);
				if(last_Init==FALSE)
					InitStella(_sf);
				last_Init=TRUE;
				turning_left=1;
				turning_right=1;
				//stop=1;
				if(response2==1){           //straight
					//start = timegettime();
					// printf("straight\n");
					straight++;
					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
					if(first_Run==FALSE)
						_sf->Velocity(15,15);
					else _sf->Velocity(25,25);
					first_Run=TRUE;
					turning_left=1;
					turning_right=1;
					//stop=1;

					cvPutText( frame2, "Straight", cvPoint( bbs2[n].x-20, bbs2[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs2[n].x + bbs2[n].width/2;
					yp = bbs2[n].y + bbs2[n].height/2;
					cvRectangle(frame2, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					
					angle_distance(&angle2, &distance2,  width_rate2, height_rate2, M_dist2,  bbs2[n].x + bbs2[n].width/2,  bbs2[n].y + bbs2[n].height/2,  width2, height2);
					sprintf(str2,"Angle : %f, Distance : %f", angle2, distance2);
					cvPutText( frame2, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					if(IsGreaterThan(angle2,-1)&&IsLessThan(angle2,1))//(-1<angle<1)
					{	     
						sprintf( direction2,"Move Straight");
						_sf->Velocity(25,25); Sleep(1);

					}
					if(IsGreaterThan(angle2,1)&&IsLessThan(angle2,2))//(1<angle<2)
					{	
						sprintf( direction2,"Move Right 1");
						_sf->Velocity(25,15); Sleep(1);

					}if(IsGreaterThan(angle2,2)&&IsLessThan(angle2,3))//(2<angle<3)
					{	
						sprintf( direction2,"Move Right 2"); 
						_sf->Velocity(35,15); Sleep(1);

					}if(IsGreaterThan(angle2,3))//(angle>3)
					{	
						sprintf( direction2,"Move Right 3");
						_sf->Velocity(40,10); Sleep(1);

					}if(IsGreaterThan(angle2,-2)&&IsLessThan(angle2,-1))//(-1>angle>-2)
					{	
						sprintf( direction2,"Move Left 1");
						_sf->Velocity(15,25);Sleep(1);

					}if(IsGreaterThan(angle2,-3)&&IsLessThan(angle2,-2))//(-2>angle>-3)
					{	
						sprintf( direction2,"Move Left 2");
						_sf->Velocity(15,35);Sleep(1);

					}if(IsLessThan(angle2,-3))//(angle<-3)
					{	
						sprintf( direction2,"Move Left 3");
						_sf->Velocity(10,40);Sleep(1);

					}else 
					{
						_sf->Velocity(25,25); Sleep(1);
					}
					_sf->Velocity(25,25); Sleep(1);
					cvPutText( frame2, direction2, cvPoint(0 , 200 ), &font, cvScalar( 255, 0, 0 ) );
				}else if(response2==2){     // right turn
					//start = timegettime();
					
					End =timeGetTime();
					Elapsed=End-Start;
					if((turning_right==1)&&(Elapsed>3000)) {
					turning_right++;
					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
				/*
					m_Flt_Travel = _ttof(m_leftTraveled);
					m_leftTraveled.Format(_T("%+07.1f"),m_Flt_Travel);
					UpdateData(FALSE);
					m_NextDist.Format(_T("%+07.1f"),distance/100);
					m_Flt_Next_Dist = _ttof(m_NextDist);
					m_Flt_Final_Dist = m_Flt_Travel + m_Flt_Next_Dist;
					m_NextDist.Format(_T("%+07.1f"),m_Flt_Final_Dist);
					UpdateData(FALSE);
					if(m_Flt_Next_Dist>0)
						RunDistant(m_Flt_Travel,m_Flt_Final_Dist,_sf);
				*/
					_sf->Velocity(15,15);//Forward();
					Sleep(4000);
					_sf->Right();
					Sleep(4000);
					_sf->Velocity(25,25);
					
					}
					//stop=1;
					cvPutText( frame2, "Right turn", cvPoint( bbs2[n].x-5, bbs2[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs2[n].x + bbs2[n].width/2;
					yp = bbs2[n].y + bbs2[n].height/2;
					cvRectangle(frame2, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
				
					angle_distance(&angle2, &distance2,  width_rate2, height_rate2, M_dist2,  bbs2[n].x + bbs2[n].width/2,  bbs2[n].y + bbs2[n].height/2,  width2, height2);
					sprintf(str2,"Angle : %f, Distance : %f", angle2, distance2);
					cvPutText( frame2, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					Start = timeGetTime();
				}else if(response2==3){     // left turn
					//start = timegettime();
					End =timeGetTime();
					Elapsed=End-Start;
					if((turning_left==1)&&(Elapsed>3000)) {
					
					turning_left++;
					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
					_sf->Velocity(15,15);//Forward();
					Sleep(4000);
					_sf->Left();
					Sleep(4000);
					_sf->Velocity(25,25);
					
					}
					//stop=1;
					cvPutText( frame2, "Left turn", cvPoint( bbs2[n].x-5, bbs2[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs2[n].x + bbs2[n].width/2;
					yp = bbs2[n].y + bbs2[n].height/2;
					cvRectangle(frame2, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
		
					angle_distance(&angle2, &distance2,  width_rate2, height_rate2, M_dist2,  bbs2[n].x + bbs2[n].width/2,  bbs2[n].y + bbs2[n].height/2,  width2, height2);
					sprintf(str2,"Angle : %f, Distance : %f", angle2, distance2);
					cvPutText( frame2, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					Start = timeGetTime();
				}else if(response2==4){     // stop
					End =timeGetTime();
					Elapsed=End-Start;
					if(stop==1) {
					stop=0;
					//printf("Stop\n");
					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
					_sf->Velocity(15,15);//Forward();
					Sleep(4000);
					_sf->Velocity(0,0);
					}
					turning_left=1;
					turning_right=1;
					cvPutText( frame2, "Stop", cvPoint( bbs2[n].x-5, bbs2[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					xp = bbs2[n].x + bbs2[n].width/2;
					yp = bbs2[n].y + bbs2[n].height/2;
					cvRectangle(frame2, cvPoint(xp-ws/2,yp-hs/2),cvPoint(xp+ws/2, yp+hs/2),CV_RGB(0,255,255),1);  //draw bounding box
					angle_distance(&angle2, &distance2,  width_rate2, height_rate2, M_dist2,  bbs2[n].x + bbs2[n].width/2,  bbs2[n].y + bbs2[n].height/2,  width2, height2);
					sprintf(str2,"Angle : %f, Distance : %f", angle2, distance2);
					cvPutText( frame2, str2, cvPoint( 0, 20 ), &font, cvScalar( 255, 0, 0 ) );
					Start =timeGetTime();
				}else if(response2==5){     // negative features
					SetTimer(ID_CLOCK_TIMER,100,NULL);
					SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
					printf("쓰레기\n");
				//	_sf->Velocity(30,30);
					//cvPutText( frame2, "쓰레기", cvPoint( bbs[n].x-5, bbs[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					//system("cls");
					//cvPutText( frame2, "쓰레기", cvPoint( bbs2[n].x-5, bbs2[n].y-10 ), &font, cvScalar( 255, 0, 0 ) );
					
				}
				
				//turning_left=1;

				//turning_right=1;
				
				delete _sf;
				delete _sp;	
			}

			n2++;
			//cvWaitKey(1);

		}			
		//turning_left=1;
				//turning_right=1;
		//cvReleaseImage(&test2);
		//cvDestroyWindow("fGray2");

		//sprintf(str2,"r : %d, r2 : %d", turning_left, turning_right);//response, response2 );
		//cvPutText( frame2, str2, cvPoint( 120, 120 ), &font, cvScalar( 255, 0, 0 ) );
		
		cvShowImage("Video", frame);
		cvShowImage("Video2",  frame2);
		m_CvvImage.CopyOf(framet,1);
		m_CvvImage.DrawToHDC(hDC, &rect);//display the camera on screen of main dialog

		char chKey = cvWaitKey(1);
		if( chKey == 27 ) // Esc
			break;

		//cvShowImage("Video", frame);
	}


	cvReleaseCapture( &capture );
	cvReleaseCapture( &capture2 );
	CSerialPort *_sp = new CSerialPort ();
	InitPort(_sp);
	CStellaB2Function *_sf = new CStellaB2Function (_sp);
	_sf->Velocity(0,0);
	delete _sp;
	delete _sf;
	cvDestroyAllWindows();
	//	return 0;
}

void CStellaB2Dlg::RunDistant(float m_crt_pst, float m_final_distant,CStellaB2Function *_sf)
{   CString m_strFinalDist,m_strCrtPst;
unsigned char left[9]={0,},right[9]={0,};
boolean flag1 = TRUE;
//SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
while(flag1)
{
	//SetTimer(ID_TRAVEL_TIMER,1/1000000,NULL);
	_sf->GetPosition(right,left);
	//UpdateData();
	m_leftTraveled.Format(_T("%S"),left);
	m_crt_pst = _ttof(m_leftTraveled);
	m_strFinalDist.Format(_T("%+07.1f"),m_final_distant);
	m_strCrtPst.Format(_T("%+07.1f"),m_crt_pst);
	m_leftTraveled.Format(_T("%+07.1f"),m_crt_pst);
	//UpdateData(FALSE);
	m_NextDist.Format(_T("%+07.1f"),m_final_distant);
	//UpdateData(FALSE);
	if (m_strCrtPst ==m_strFinalDist)
	{
		flag1=FALSE;
		_sf->Velocity(0,0);
		//Sleep(100000);
	}
}
}
