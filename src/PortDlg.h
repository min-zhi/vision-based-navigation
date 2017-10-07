#pragma once
#include "afxwin.h"


// CPortDlg dialog

class CPortDlg : public CDialog
{
	DECLARE_DYNAMIC(CPortDlg)

public:
	CPortDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPortDlg();

// Dialog Data
	enum { IDD = IDD_PORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_comport;
	CComboBox m_cboPort;
	void InitPortDlg(void);
	CString getCurStrInCombobox(const CComboBox &a);
	
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
