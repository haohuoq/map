// DlgPreview.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgPreview.h"
#include "CellLocatiomDlg.h"
#include <set>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPreview dialog
extern CCellLocatiomDlg * pMainDlg;

CDlgPreview::CDlgPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPreview)
	m_nSelectItem = -1;
	m_nDeleteItem = -1;
	m_nMapSwitch = 0; 
	//}}AFX_DATA_INIT
}


void CDlgPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPreview)
DDX_Control(pDX, IDC_LIST5, m_list5);
	DDX_Control(pDX, IDC_LIST4, m_list4);
	DDX_Control(pDX, IDC_LIST3, m_list3);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPreview, CDialog)
	//{{AFX_MSG_MAP(CDlgPreview)
	ON_NOTIFY(NM_RCLICK, IDC_LIST4, OnRclickList4)
	ON_COMMAND(MENU_CHOOSE_CELL, OnSelectCell)
	ON_NOTIFY(NM_RCLICK, IDC_LIST5, OnRclickList5)
	ON_COMMAND(MENU_DELETE_CELL, OnDeleteCell)
	ON_BN_CLICKED(IDC_BTN_MAP_SWITCH, OnBtnMapSwitch)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, OnClickList3)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnClickList2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPreview message handlers

BOOL CDlgPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");
	
	GetDlgItem(IDC_STATIC_T1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_T2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_T3)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_T4)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_T5)->SetFont(&m_font);
	GetDlgItem(IDC_LIST1)->SetFont(&m_font);
	GetDlgItem(IDC_LIST2)->SetFont(&m_font);
	GetDlgItem(IDC_LIST3)->SetFont(&m_font);
	GetDlgItem(IDC_LIST4)->SetFont(&m_font);
	GetDlgItem(IDC_LIST5)->SetFont(&m_font);
	GetDlgItem(IDC_BTN_MAP_SWITCH)->SetFont(&m_font);

	
	// TODO: Add extra initialization here

	m_list1.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list1.SetColumnHeader(_T("PLMN, 310"));
	m_list1.SetGridLines(TRUE);
	m_list1.SetSortable(FALSE);


	m_list2.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list2.SetColumnHeader(_T("LAC, 260"));
	m_list2.SetGridLines(TRUE);
	m_list2.SetSortable(FALSE);


	m_list3.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list3.SetColumnHeader(_T("MODE, 310"));
	m_list3.SetGridLines(TRUE);
	m_list3.SetSortable(FALSE);

	m_list5.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list5.SetColumnHeader(_T("CEllID, 310"));
	m_list5.SetGridLines(TRUE);
	m_list5.SetSortable(FALSE);

	m_list4.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
//	m_list4.SetColumnHeader(_T("CellID, 140;采集次数, 100;lng,120;lat,120"));


	m_list4.InsertColumn(0,_T("CellID"),LVCFMT_LEFT,140);
	m_list4.InsertColumn(1,_T("采集次数"),LVCFMT_LEFT,100);
	m_list4.InsertColumn(2,_T("lng"),LVCFMT_LEFT,120);
	m_list4.InsertColumn(3,_T("lat"),LVCFMT_LEFT,120);
	m_list4.SetGridLines(TRUE);
	m_list4.SetSortable(FALSE);
	for(int i = 0;i<5;i++)
//	  cell_select_list[i]=0;
/*	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);
	freopen("CONOUT$","w",stderr);*/
	LoadDB();
	CFile file1;
	file1.Open("1.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
	file1.SetLength(0);
	file1.Close();
	CFile file2;
	file2.Open("2.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
	file2.SetLength(0);
	file2.Close();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPreview::LoadDB()
{
	m_list1.DeleteAllItems();
	m_list3.DeleteAllItems();
	CString strPlmn,strMode;
	int nObjCount=0;
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
	m_pAdoRecordset->Open("gather", adCmdTable);

	m_pAdoConn->BeginTrans();
	int i,j,k;
	int flag1=0,flag2=0;
	CString tmp1(""),tmp2("");
	nObjCount=m_pAdoRecordset->GetRecordCount();
	m_pAdoRecordset->MoveFirst();
	for(i=0;i<nObjCount;i++)
	{
		tmp1="";
		flag1=0;
		tmp2="";
		flag2=0;
		//m_pAdoRecordset->GetCollect("casename",strCaseName);
		m_pAdoRecordset->GetCollect("plmn",strPlmn);
		m_pAdoRecordset->GetCollect("mode",strMode);
		
		strPlmn.TrimRight();
		strMode.TrimRight();
		for(j=0;j<m_list1.GetItemCount();j++)
		{
			tmp1=tmp1+m_list1.GetItemText(j,0);
			if(strPlmn==tmp1)
			{
				flag1=1;
				break;
			}
			tmp1="";
		}
		if(flag1==0)
		{
			m_list1.InsertItem(i,strPlmn);
		}
		 
		for(k=0;k<m_list3.GetItemCount();k++)
		{
			tmp2=tmp2+m_list3.GetItemText(k,0);
			if(strMode==tmp2)
			{
				flag2=1;
				break;
			}
			tmp2="";
		}
		if(flag2==0)
		{
			m_list3.InsertItem(i,strMode);
		}

	
		
		m_pAdoRecordset->MoveNext();
	}
	
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;

}


void CDlgPreview::SetControlPostion()
{
	int nTmp,nTmp2;
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
	
	pWnd=&m_list2;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,dx-rtClient.left+lx,nH-8));



	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);


	pWnd=GetDlgItem(IDC_STATIC_T4); 
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,nW-10,dy-rtClient.top+ly));



	pWnd=&m_list4;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	
	
	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,nW-10,nH-8));
	

	
	
}


BOOL CDlgPreview::isExistCellID(int cellid)
{
	if(cellid==0)
		return FALSE;
	for(int i=0;i<5;i++)
	{

		if(cellid== cell_select_list[i])
			return TRUE;
	}
	return FALSE;

}
void CDlgPreview::OnRclickList4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
		m_nSelectItem = pNMListView->iItem;
		
		LPPOINT lpoint=new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu *menu;
		CMenu PopMenu;
		int cellid = atoi(m_list4.GetItemText(m_nSelectItem,0));
		if(!isExistCellID(cellid))
			PopMenu.LoadMenu(IDR_MENU7);
		
		menu = PopMenu.GetSubMenu (0);
		

		menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//资源回收
		HMENU hmenu=menu->Detach();
		menu->DestroyMenu();
		delete lpoint;
		
	}
	*pResult = 0;
}

void CDlgPreview::OnSelectCell() 
{
	
	if(m_list5.GetItemCount()<5)
	{
		m_list5.InsertItem(m_list5.GetItemCount(),m_list4.GetItemText(m_nSelectItem,0));
		for(int i=0;i<5;i++)
		{

			if(0== cell_select_list[i])
			{
				cell_select_list[i] = atoi(m_list4.GetItemText(m_nSelectItem,0));
				break;
			}
		}
	}
}

void CDlgPreview::OnRclickList5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
		m_nDeleteItem = pNMListView->iItem;
		
		LPPOINT lpoint=new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu *menu;
		CMenu PopMenu;

		PopMenu.LoadMenu(IDR_MENU8);
		
		menu = PopMenu.GetSubMenu (0);
		

		menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//资源回收
		HMENU hmenu=menu->Detach();
		menu->DestroyMenu();
		delete lpoint;
		
	}
	*pResult = 0;	
	
}

void CDlgPreview::OnDeleteCell()   //从列表里删除基站，从地图上移除该基站数据
{

	
}

void CDlgPreview::OnBtnMapSwitch() 
{
	if(m_nMapSwitch ==0)
	{

		GetDlgItem(IDC_BTN_MAP_SWITCH)->SetWindowText("切换到列表");
		pMainDlg->SetControlPostion6();
		m_nMapSwitch =1;
	}
	else
	{
		GetDlgItem(IDC_BTN_MAP_SWITCH)->SetWindowText("切换到地图");
		pMainDlg->SetControlPostion5();
		m_nMapSwitch =0;

	}
	
}

void CDlgPreview::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		CString strVal = m_list1.GetItemText(pNMListView->iItem,0);
	//	printf(strVal);
		CFile file;
		file.Open("1.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
		file.Write(strVal,strlen(strVal));
		file.Close();
	}
	*pResult = 0;
}

void CDlgPreview::OnClickList3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		CString strVal = m_list3.GetItemText(pNMListView->iItem,0);
	//	printf(strVal);
		CFile file;
		file.Open("2.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
		file.SetLength(0);
		file.Write(strVal,strlen(strVal));
		file.Close();
	}
	*pResult = 0;
	}	
void CDlgPreview::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_list4.DeleteAllItems();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString strLac = m_list2.GetItemText(pNMListView->iItem,0),tmp("");
	int nObjCount=0;
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
	m_pAdoRecordset->Open("gather", adCmdTable);

	m_pAdoConn->BeginTrans();
	int i,j;
	int flag=0;
	multiset <CString> s;
	CString tmp1(""),tmp2(""),tmp3(""),tmp4("");
	nObjCount=m_pAdoRecordset->GetRecordCount();
	m_pAdoRecordset->MoveFirst();
	for(i=0;i<nObjCount;i++)
	{
		tmp="";
		flag=0;
		m_pAdoRecordset->GetCollect("lac",tmp1);
		m_pAdoRecordset->GetCollect("cid",tmp2);
		m_pAdoRecordset->GetCollect("lat2",tmp3);
		m_pAdoRecordset->GetCollect("lng2",tmp4);
		tmp1.TrimRight();
		tmp2.TrimRight();
		tmp3.TrimRight();
		tmp4.TrimRight();
		if(tmp1==strLac)
		{
		/*	m_list4.InsertItem(i,_T(tmp2));
			int nIndex =m_list4.InsertItem(i,_T(tmp2));
			m_list4.SetItemText(nIndex,1,_T(tmp1));
			m_list4.SetItemText(nIndex,2,_T(tmp3));
			m_list4.SetItemText(nIndex,3,_T(tmp4));
			printf(tmp3+" "+tmp4+"\n");*/
	
			s.insert(tmp2);
			for(j=0;j<m_list4.GetItemCount();j++)
			{
				tmp=tmp+m_list4.GetItemText(j,0);
				if(tmp2==tmp)
				{
					flag=1;
					break;
				}
				tmp="";
			}
			if(flag==0)
			{
				int nIndex=m_list4.InsertItem(i,_T(tmp2));
				m_list4.SetItemText(nIndex,2,_T(tmp3));
				m_list4.SetItemText(nIndex,3,_T(tmp4));

			}
		}
		m_pAdoRecordset->MoveNext();
	}
	for(j=0;j<m_list4.GetItemCount();j++)
	{
		tmp="";
		tmp=tmp+m_list4.GetItemText(j,0);
		printf("%d\n",s.count(tmp));
		int p=j;
		m_list4.SetItemText(p,1,_T(s.count(tmp)));

	}
	
/*	for(i=0;i<nObjCount;i++)
	{

		m_pAdoRecordset->GetCollect("lac",tmp1);
		m_pAdoRecordset->GetCollect("cid",tmp2);
		tmp1.TrimRight();
		tmp2.TrimRight();
		if(tmp1==strLac)
		{
			printf("%d",s.count(tmp2));
			break;
		}
		m_pAdoRecordset->MoveNext();
	}*/
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;
	*pResult = 0;
}

void CDlgPreview::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_list2.DeleteAllItems();
	m_list4.DeleteAllItems();
	CString strPlmn,strMode,strLac;
	int nObjCount=0;
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
	m_pAdoRecordset->Open("gather", adCmdTable);

	m_pAdoConn->BeginTrans();
	int i,j=0,flag=0,k=0;
	CString tmp1,tmp2,tmp3("");
	nObjCount=m_pAdoRecordset->GetRecordCount();
	m_pAdoRecordset->MoveFirst();
	CFile file1;
	file1.Open("1.txt",CFile::modeRead,NULL);
	CArchive ar1(&file1,CArchive::load);
	ar1.ReadString(tmp1);
	CFile file2;
	file2.Open("2.txt",CFile::modeRead,NULL);
	CArchive ar2(&file2,CArchive::load);
	ar2.ReadString(tmp2);
	for(i=0;i<nObjCount;i++)
	{
		m_pAdoRecordset->GetCollect("plmn",strPlmn);
		m_pAdoRecordset->GetCollect("mode",strMode);
		m_pAdoRecordset->GetCollect("lac",strLac);
		strPlmn.TrimRight();
		strMode.TrimRight();
		strLac.TrimRight();
		//printf(strPlmn+"\n");
		//printf(strMode+"\n");
		tmp3="";
		flag=0;
		if(strMode==tmp2&&strPlmn==tmp1)
		{
			for(j=0;j<m_list2.GetItemCount();j++)
			{
				tmp3=tmp3+m_list2.GetItemText(j,0);
				if(strLac==tmp3)
				{
					flag=1;
					break;
				}
				tmp3="";
				
			}
			if(flag==0)
			{
				m_list2.InsertItem(i,strLac);
				//printf(strPlmn+"\n");
				//printf(strMode+"\n");
			}
		}
		
		m_pAdoRecordset->MoveNext();
	}
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;
}
