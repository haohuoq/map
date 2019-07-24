// DlgLoad.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgLoad.h"
#include "CellLocatiomDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoad dialog
extern CCellLocatiomDlg * pMainDlg;
typedef BYTE  uchar ;
CDlgLoad * pLoadDlg;
extern int link_flag;
CDlgLoad::CDlgLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoad)
	m_nDelItem=-1;	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoad)
	DDX_Control(pDX, IDC_BUTTON3, m_btnTime);
	DDX_Control(pDX, IDC_STATIC_INFO, m_infost);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDown);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_STATIC_LINK, m_stLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoad, CDialog)
	//{{AFX_MSG_MAP(CDlgLoad)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(ID_REC_DELETE, OnRecDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoad message handlers

BOOL CDlgLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pLoadDlg = this;
	// TODO: Add extra initialization here
	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");
	
	GetDlgItem(IDC_STATIC_LINK)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_INFO)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK1)->SetFont(&m_font);
	GetDlgItem(IDC_LIST1)->SetFont(&m_font);


	m_list1.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list1.SetColumnHeader(_T("时间, 140;入库数, 60;记录名,120"));
	m_list1.SetGridLines(TRUE);
	m_list1.SetSortable(FALSE);
	//	m_list2.SetBkColor(RGB(200,235,200));
	//m_list1.SetImageList(IDB_BMP_LIST);

	
	LoadDB();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgLoad::LoadDB()
{
	m_list1.DeleteAllItems();
	CString strTime, strCount,strRemark;
	int nObjCount = 0;
	
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	strDBPath += "\\database.mdb";
	
	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
		
	}
	else
	{
		AfxMessageBox("数据库连接失败!");
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	m_pAdoRecordset->Open("record", adCmdTable);
	
	m_pAdoConn->BeginTrans();
	int i;
	CString tmp;
	nObjCount=m_pAdoRecordset->GetRecordCount();
	m_pAdoRecordset->MoveFirst();
	for(i=0;i<nObjCount;i++)
	{
		//m_pAdoRecordset->GetCollect("casename",strCaseName);
		m_pAdoRecordset->GetCollect("time",strTime);
		m_pAdoRecordset->GetCollect("count",strCount);
		m_pAdoRecordset->GetCollect("remark",strRemark);

		strTime.TrimRight();
		strCount.TrimRight();
	strRemark.TrimRight();
		m_list1.InsertItem(i,strTime);
		m_list1.SetItemText(i,1,strCount);
		m_list1.SetItemText(i,2,strRemark);
		
		m_pAdoRecordset->MoveNext();
	}
	
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;
	   
}

void CDlgLoad::OnButton9() 
{
	// TODO: Add your control notification handler code here
/*	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	int id; 
	CString tmp_file = strAppPath+"\\temp\\data.temp";

	FILE*fp=NULL;
	if ((fp=fopen(tmp_file,"rb"))!=NULL)
	{
			fclose(fp);
	}
	else
	{
		AfxMessageBox("临时数据文件不存在");
		return;
	}
	
	DWORD dwAG;
	CreateThread(NULL,0,SaveToDBThread,NULL,0,&dwAG);*/
}


void CDlgLoad::SetControlPostion()
{
	int nTmp,nTmp2;
	CRect rtClient; 
	
	pMainDlg->m_stTab.GetWindowRect(&rtClient);
	int nW = rtClient.Width();
	int nH = rtClient.Height();
	
	GetClientRect(&rtClient);             //获得对话框客户区屏幕坐标   
    ClientToScreen(&rtClient); //映射为屏幕坐标   
	//	MoveWindow(CRect(0,0,nW,nH)); 
	
	CRect rtCaption;   
	CWnd *pWnd;
	int lx,ly,dx,dy;
	
	pWnd=&m_list1;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,nW,nH));
	
	
}

void CDlgLoad::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	//if(link_flag==0)
	//{
	//	AfxMessageBox("此功能需要联网获取百度地图支持");
	//	return;
	//}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		
		CString strVal = m_list1.GetItemText(pNMListView->iItem,0);
	
		pMainDlg->m_dlgRst.LoadToList(strVal);

		
	/*	CString strFile = strVal.Replace(":","_");
		TCHAR AppPathName[MAX_PATH];
		GetModuleFileName(NULL,AppPathName,MAX_PATH);
		CString strAppPath = AppPathName;
		
		strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
		CString strDBPath = strAppPath;
		
		
		CString temp_file1 = strAppPath+"\\temp\\"+strFile+".db";
*/

		
	}
	
	*pResult = 0;
}

void CDlgLoad::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
		m_nDelItem = pNMListView->iItem;
		
		LPPOINT lpoint=new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu *menu;
		CMenu PopMenu;
		PopMenu.LoadMenu(IDR_MENU4);
		menu = PopMenu.GetSubMenu (0);
		
		menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//资源回收
		HMENU hmenu=menu->Detach();
		menu->DestroyMenu();
		delete lpoint;
		
	}
	
	*pResult = 0;
}

void CDlgLoad::OnRecDelete() 
{
	// TODO: Add your command handler code here
	CString tmm = m_list1.GetItemText(m_nDelItem,0);
	

	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	strDBPath += "\\database.mdb";
	

	


	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
		
	}
	else
	{
		AfxMessageBox("数据库连接失败!");
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	
	m_pAdoConn->BeginTrans();

	CString strSQL = "DELETE FROM gather WHERE savetime='"+tmm+"'";
	m_pAdoRecordset->Open(strSQL);
	strSQL = "DELETE FROM record WHERE time='"+tmm+"'";
	m_pAdoRecordset->Open(strSQL);

	
	m_pAdoRecordset->Update();
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;

	tmm.Replace(":","_"); 

	CString file = strAppPath+"\\temp\\"+tmm+".db";

	DeleteFile(file);

	m_list1.DeleteItem(m_nDelItem);

}
