// DlgEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgEdit.h"
#include "CellLocatiomDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEdit dialog
extern CCellLocatiomDlg * pMainDlg;

CDlgEdit::CDlgEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEdit)
	m_strMark = _T("");
	//}}AFX_DATA_INIT
}


void CDlgEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEdit)
	DDX_Control(pDX, IDC_STATIC_LNG, m_stLng);
	DDX_Control(pDX, IDC_STATIC_LAT, m_stLat);
	DDX_Text(pDX, IDC_EDIT1, m_strMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgEdit)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEdit message handlers

void CDlgEdit::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	pMainDlg->AddMark(m_strMark);
	ShowWindow(SW_HIDE);
}

BOOL CDlgEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow(); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
