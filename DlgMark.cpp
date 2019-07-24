// DlgMark.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgMark.h"
#include "CellLocatiomDlg.h"
extern CCellLocatiomDlg * pMainDlg;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMark dialog


CDlgMark::CDlgMark(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMark::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMark)
	m_strMark = _T("");
	//}}AFX_DATA_INIT
}


void CDlgMark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMark)
	DDX_Text(pDX, IDC_EDIT1, m_strMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMark, CDialog)
	//{{AFX_MSG_MAP(CDlgMark)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMark message handlers

void CDlgMark::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	pMainDlg->m_dlgReal.m_strMark = m_strMark;
//	AfxMessageBox(m_strMark);
	CDialog::OnOK();
}
