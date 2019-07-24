// DlgBs.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgBs.h"
#include "excel9.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBs dialog
CString strVal[128];
int flag_lac=-1;
int flag_cid=-1;
int flag_bsid=-1;
int flag_lng=-1;
int flag_lat=-1;
int flag_marker=-1;

CDlgBs *pBSDlg;
CDlgBs::CDlgBs(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBs)
	m_bDel = TRUE;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgBs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBs)

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBs, CDialog)
	//{{AFX_MSG_MAP(CDlgBs)

	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBs message handlers

BOOL CDlgBs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pBSDlg=this;
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBs::SetControlPostion()
{
/*	int nTmp,nTmp2;
	CRect rtClient; 
	
	GetWindowRect(&rtClient);
	int nW = rtClient.Width();
	int nH = rtClient.Height();
	
	GetClientRect(&rtClient);             //获得对话框客户区屏幕坐标   
    ClientToScreen(&rtClient); //映射为屏幕坐标   
	//	MoveWindow(CRect(0,0,nW,nH)); 
	
	CRect rtCaption;   
	CWnd *pWnd;
	int lx,ly,dx,dy;
	
	pWnd=&m_list;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,nW-8,nH-8));


	pWnd=&m_list2;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,dx-rtClient.left+rtCaption.Width(),nH-8));


	
	*/
}

