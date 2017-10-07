// PortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StellaB2.h"
#include "PortDlg.h"


// CPortDlg dialog

IMPLEMENT_DYNAMIC(CPortDlg, CDialog)

CPortDlg::CPortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortDlg::IDD, pParent)
	, m_comport(0)
{

}

CPortDlg::~CPortDlg()
{
}

void CPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboPort);
}


BEGIN_MESSAGE_MAP(CPortDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPortDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPortDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPortDlg message handlers
BOOL CPortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitPortDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//Init the value of Combobox
void CPortDlg::InitPortDlg(void)
{ 
	m_cboPort.ResetContent();
	m_cboPort.InsertString(0,L"COM1");
	m_cboPort.InsertString(1,L"COM2");
	m_cboPort.InsertString(2,L"COM3");
	m_cboPort.InsertString(3,L"COM4");
	m_cboPort.InsertString(4,L"COM5");
	m_cboPort.InsertString(5,L"COM6");
	m_cboPort.InsertString(6,L"COM7");
	m_cboPort.InsertString(7,L"COM8");
	if(m_comport==0)
		m_cboPort.SetCurSel(0);
	else m_cboPort.SetCurSel(m_comport-1);
}

//Get the string in Combobox
CString CPortDlg::getCurStrInCombobox(const CComboBox &a)    
{
	CString str;
	a.GetLBText(a.GetCurSel(),str);
	return str;
}

void CPortDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_comport=m_cboPort.GetCurSel()+1;
	OnOK();
}

void CPortDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
