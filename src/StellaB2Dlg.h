// StellaB2Dlg.h : header file
//

#pragma once
#include "portdlg.h"
#include "SerialPort.h"
#include "StellaB2Function.h"
#include "afxwin.h"


// CStellaB2Dlg dialog
class CStellaB2Dlg : public CDialog
{
// Construction
public:
	CStellaB2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STELLAB2_DIALOG };
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
	afx_msg void OnBnClickedForward();
	afx_msg void OnBnClickedBackward();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedStop();
	CPortDlg m_PortDlg;
	int m_PortNum;

	afx_msg void OnBnClickedProg1();
	afx_msg void OnBnClickedProgram2();
	void InitPort(CSerialPort *_sp);
	void InitStella(CStellaB2Function *_sf);
	CEdit m_state;
	CString m_strState;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	afx_msg void OnSettingsComport();
	CString m_PortNumShow;
	CString m_leftTraveled;
	CString m_rightTraveled;
	CString m_StrCommand;
	afx_msg void OnBnClickedButtonSend();
	CString m_strLeftSpeed;
	CString m_strRightSpeed;
	afx_msg void OnEnChangeErrorSignal();
	afx_msg void OnBnClickedCamRun();
	CString m_Sign;
	CString m_NextDist;
private:
	void RunDistant(float m_crt_pst, float m_final_distant,CStellaB2Function *_sf);
};
