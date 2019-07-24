// CellLocatiomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "CellLocatiomDlg.h"
#include <atlbase.h>
#include "Psapi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
extern Point * wgs_gcj_encrypts(double wgLat, double wgLon) ;
extern Point *  gcj_wgs_decrypt(double gcjLat, double  gcjLon);
CCellLocatiomDlg * pMainDlg;
CString strSaveFilePath;
CString strIcon[11];
int mark_flag = 0;

int link_flag = 0;

typedef  unsigned char uchar;
typedef  unsigned char uint8;
typedef unsigned int  uint32 ;
typedef unsigned short uint16 ;
#pragma pack(1)

typedef struct {
	uint32 msgtype;
	uint16 msglen;
	uint16 transid;  //事务ID,一组设备组成的系统分配一个事务ID，范围10000-20000之间
	uint16 sourceid; // 发送者ID，一般终端范围1000-8999;管理终端9000-9999
	uint16 targetid; // 目标ID，指定目标范围1000-9999;仅服务器收不转 0 ;群发所有同一事务下的所有上线目标 1
}pkg_header_t;

typedef struct{
	BYTE mode;
	char lac[6];
	char cid[12];
	char arfcn[6];
	char pci[6];
	char rssi[6];
	double lat;
	double lng;

}lc_data_t;
typedef struct SNumber
{
	uint8 type;  //1G,2W,3F
	
    char imsi[15];
	
    char imei[15];          
	
    char time[17];	
	
 }SNumber;


#pragma pack()


SOCKET main_sock = INVALID_SOCKET;
BOOL g_bQuitSocket = FALSE;

BOOL g_bLock = FALSE;

DWORD ResetMainSocket()
{

	CString strip="139.196.18.2";
	
	int port=10110;
	


	if(main_sock!=INVALID_SOCKET)
	{
		closesocket(main_sock);
		main_sock= INVALID_SOCKET;
	}


	
	
	if ((main_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("main_sock");
		main_sock= INVALID_SOCKET;
		return 0;
	}

	

	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr.s_addr = inet_addr(strip);

	if (connect(main_sock, (struct sockaddr *) &dest, sizeof(dest)) != 0) 
	{
		closesocket(main_sock);
		main_sock= INVALID_SOCKET;
		return 0;
	}

	return 1;
}

UINT MainSockThread(LPVOID pr)
{	
	CString str;
	char csBuffer[1024]={0};
	char csbuffHead[12]={0};
	char csBuffBody[1000]={0};
	int nReadLen=0;
	int nReadCount = 0;
	pkg_header_t msg_head_;
	

	WSADATA	wsd;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
	{
		return FALSE;
	}
	int		ret = 0;
	int		nStr = 0;
TDD_SOCKET_THREAD_HEAD:
	while(ResetMainSocket()==0)
	{
	
		link_flag = 0;
		pMainDlg->SetWindowText("路测数据管理系统——离线");
		Sleep(3000);
	}
	

	//pDlg->AddLogToList("tdd连接成功");
	pMainDlg->SetWindowText("路测数据管理系统——在线");
	pMainDlg->SendLogOn();
	link_flag = 1;
	while( !g_bQuitSocket )
	{
		memset(&msg_head_,0,sizeof(pkg_header_t));
		memset(csBuffer,0,1024);
		memset(csbuffHead,0,12);
		memset(csBuffBody,0,1000);

		nReadLen = recv(main_sock,csbuffHead,12,false);
		if(nReadLen==0)
		{
			continue;
		}
		if(nReadLen==-1)
		{
			//pDlg->AddLogToList("e ");
			goto TDD_SOCKET_THREAD_HEAD;
		}
		if(nReadLen!=12)
		{
			CString strr;
			strr.Format("%d",nReadLen);
			
			AfxMessageBox("数据长度错误");
			goto TDD_SOCKET_THREAD_HEAD;
		}
		memcpy( &msg_head_, csbuffHead,12);
		
		nReadCount = msg_head_.msglen;
		
		memcpy(csBuffer,csbuffHead,12);

		if(nReadCount == 0)
		{

			pMainDlg->DispRecvedMsg(csBuffer);
		}
		else if(nReadCount > 0)
		{
			int nread = 0;
			do 
			{
			
				nReadLen = recv(main_sock,csBuffBody+nread,nReadCount-nread,false);
				if(nReadLen>0)
					nread+= nReadLen;
				else
				{
					goto TDD_SOCKET_THREAD_HEAD;
				}

			
			} while(nread< nReadCount);
			
			if(nread == nReadCount)
			{
				
				//if(LOG_OUTPUT==2)
				
				
				memcpy(csBuffer+12,csBuffBody, nReadCount);
				
				
				pMainDlg->DispRecvedMsg(csBuffer);
			}
			else
			{
				//pDlg->AddLogToList("a");
				goto TDD_SOCKET_THREAD_HEAD;

			}
			
		}
		else
		{
			//pDlg->AddLogToList("b");
			goto TDD_SOCKET_THREAD_HEAD;

		}
	
		
	} 
	
	closesocket( main_sock );
	return 0;
}

void SendData(char* buf, int len)
{
	
	if(main_sock!=INVALID_SOCKET)
	{
		if(send(main_sock,buf,len,0)==SOCKET_ERROR)
		{
			//AddLogToList("send data error");
			closesocket(main_sock);
			main_sock = INVALID_SOCKET;
		}
	}
}


DWORD FindProcess(char *strProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    char szProcessName[MAX_PATH];
	
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )  return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
        //_tprintf(_T("%d\t"), aProcesses[i]);
        hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
        GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
        
        if(strstr(szProcessName, strProcessName))
        {
            //_tprintf(_T("%s;"), szProcessName);
            return(aProcesses[i]);
        }
        //_tprintf(_T("\n"));
    }
	
	
    return 0;
}



//
// Function: ErrorForce
// 此函数中用上面的 FindProcess 函数获得你的目标进程的ID
// 用WIN API OpenPorcess 获得此进程的句柄，再以TerminateProcess
// 强制结束这个进程
//

VOID KillProcess()
{
	
	
	
	DWORD pid =  FindProcess("httpsvr.exe");
	

	HANDLE h;
	
	if(pid>0)	
		h=OpenProcess(PROCESS_ALL_ACCESS,TRUE,pid);
	if(h!=NULL)
	{
		TerminateProcess(h,0);
	}
	
	
	
	
	
    return;
}



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellLocatiomDlg dialog

CCellLocatiomDlg::CCellLocatiomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCellLocatiomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCellLocatiomDlg)
	m_pIHTMLDoc		= NULL;	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);m_nTabID=1;m_bCanShow=TRUE;
	m_strLat = "";

}

void CCellLocatiomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCellLocatiomDlg)
	DDX_Control(pDX, IDC_COMBO2, m_cbList);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_stBtm);
	DDX_Control(pDX, IDC_STATIC_TAB, m_stTab);
	DDX_Control(pDX, IDC_EXPLORER1, m_brow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCellLocatiomDlg, CDialog)
	//{{AFX_MSG_MAP(CCellLocatiomDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(IDC_MENU_DEL, OnMenuDel)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellLocatiomDlg message handlers

BOOL CCellLocatiomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;

	strSaveFilePath = GetInstallPath()+"\\temp\\data.temp";
//	AfxMessageBox(strSaveFilePath);
	WinExec(strAppPath+"\\httpsvr.exe",SW_HIDE);
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
	
	pMainDlg = this;
    ShowWindow(SW_SHOWMAXIMIZED);
//	SetControlPostion();
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list.SetColumnHeader(_T("时间,150;Lat1,120;Lng1,120;Lat2,120;Lng2,120;标记,100;PLMN,100;制式,100;LAC,150;CID,150;ARFCN,100;Band,100;PCI,100;RSSI,150;LAC1,150;CID1,150;ARFCN1,100;Band1,100;PCI1,100;RSSI1,150;LAC2,150;CID2,150;ARFCN2,100;Band2,100;PCI2,100;RSSI2,150;LAC3,150;CID3,150;ARFCN3,100;Band3,100;PCI3,100;RSSI3,150;LAC4,150;CID4,150;ARFCN4,100;Band4,100;PCI4,100;RSSI4,150;LAC5,150;CID5,150;ARFCN5,100;Band5,100;PCI5,100;RSSI5,150;LAC6,150;CID6,150;ARFCN6,100;Band6,100;PCI6,100;RSSI6,150"));
	m_list.SetGridLines(TRUE);
	m_list.SetSortable(FALSE);
	m_list.SetCheckboxeStyle();
	m_list.ShowWindow(SW_HIDE);
	

	
	CString strPath = GetInstallPath() +"\\maps\\map.html";//"\\maps\\a.html";
	m_brow.Navigate(strPath, NULL, NULL, NULL, NULL);
	
//	m_brow.SetSilent(TRUE);
	
	m_webPage.SetDocument(m_brow.GetDocument());
	
	
	
	font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");

	
	CRect rect;
	CWnd* pWnd=GetDlgItem(IDC_STATIC_REAL);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_dlgReal.Create(IDD_DLG_REALTIME, this);
    m_dlgReal.MoveWindow(&rect);
    m_dlgReal.ShowWindow(SW_HIDE);	


	m_dlgQry.Create(IDD_DLG_QRY, this);
    m_dlgQry.MoveWindow(&rect);
    m_dlgQry.ShowWindow(SW_HIDE);

	m_dlgDown.Create(IDD_DLG_DOWN, this);
    m_dlgDown.MoveWindow(&rect);
    m_dlgDown.ShowWindow(SW_HIDE);


	m_dlgLoad.Create(IDD_DLG_LOAD, this);
    m_dlgLoad.MoveWindow(&rect);
    m_dlgLoad.ShowWindow(SW_HIDE);

	m_dlgRst.Create(IDD_DLG_RST, this);
    m_dlgRst.MoveWindow(&rect);
    m_dlgRst.ShowWindow(SW_HIDE);

	m_dlgPreview.Create(IDD_DLG_PREVIEW, this);
    m_dlgPreview.MoveWindow(&rect);
    m_dlgPreview.ShowWindow(SW_HIDE);


	m_dlgBs.Create(IDD_DLG_BS_LOAD, this);
	m_dlgBs.ShowWindow(SW_HIDE);




	GetDlgItem(IDC_LIST1)->SetFont(&font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&font);
	GetDlgItem(IDC_BUTTON12)->SetFont(&font);
	GetDlgItem(IDC_BUTTON13)->SetFont(&font);
	GetDlgItem(IDC_BUTTON14)->SetFont(&font);
	GetDlgItem(IDC_BUTTON15)->SetFont(&font);
	GetDlgItem(IDC_STATIC_Local)->SetFont(&font);
	GetDlgItem(IDC_COMBO2)->SetFont(&font);




/*	SetControlPostion1();
	m_dlgReal.SetControlPostion();
	m_dlgReal.ShowWindow(SW_SHOW);	*/

	m_nTabID=0;
//	m_webPage.CallJScript("ClearChangeMode");
	SetControlPostion();

	m_list.ShowWindow(SW_HIDE);


//strIcon[0]="images/mk1.png";
	strIcon[0]="images/mk1.png";
	strIcon[2]="images/mk2.png";
	strIcon[1]="images/star.png";

	strIcon[3]="images/push_pin0.png";
	strIcon[4]="images/push_pin1.png";
	strIcon[5]="images/push_pin2.png";
	strIcon[6]="images/push_pin3.png";
	strIcon[7]="images/push_pin4.png";
	strIcon[8]="images/push_pin5.png";
	strIcon[9]="images/push_pin6.png";
strIcon[10]="images/tower.png";
	memset(&m_curRealData,0,sizeof(TRealtimeData));




	m_pDlgEdit= new CDlgEdit;
	m_pDlgEdit->Create(IDD_DLG_EDIT);


	SetTimer(1,5000,NULL);
	//LoadLocList();
	AfxBeginThread( MainSockThread, this );
//	pMainDlg->m_webPage.CallJScript("MakeCenter", "118.784223","32.041586" );

//	
	
	SetTimer(2,5000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CCellLocatiomDlg::OnTabSelected(TabCtrl *pCtrl, HTAB hTab)
{
/*if(IsExist1(&m_dlgCatchCatch)==false) m_TabCtrl1.Add(m_dlgCatchCatch,_T("GSM"),-1);
if(IsExist1(&m_dlgWcdma)==false) m_TabCtrl1.Add(m_dlgWcdma,_T("WCDMA"),-1);
if(IsExist1(&m_dlgTD)==false) m_TabCtrl1.Add(m_dlgTD,_T("TD-SCDMA"),-1);
	if(IsExist1(&m_dlgCDMA)==false) m_TabCtrl1.Add(m_dlgCDMA,_T("CDMA2000"),-1);*/
	if(pCtrl->GetTabWnd(hTab)==m_dlgQry)
	{
		//OnRadio3();
		//m_nListID = 2;
		SetControlPostion();
		m_dlgReal.ShowWindow(SW_HIDE);	
		m_nTabID=0;
			pMainDlg->m_webPage.CallJScript("ClearMarker");
		pMainDlg->m_webPage.CallJScript("ClearLocations");
	}
	if(pCtrl->GetTabWnd(hTab)==m_dlgLoad)
	{	
		//OnRadio2();
		//m_nListID = 1;
//		SetControlPostionHide();
		m_dlgReal.ShowWindow(SW_SHOW);	
m_nTabID=1;
pMainDlg->m_webPage.CallJScript("ClearMarker");
		pMainDlg->m_webPage.CallJScript("ClearLocations");
	}
	
	UpdateData(FALSE);
}

void CCellLocatiomDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCellLocatiomDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCellLocatiomDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCellLocatiomDlg::SetTabsPosition(int n)
{	CWnd *pBaseWnd;

pBaseWnd = GetDlgItem(IDC_STATIC_TAB);

if(pBaseWnd!=NULL)
{	
	CRect rcTab, rcDlg;
	pBaseWnd->GetWindowRect(&rcTab);
	ScreenToClient(&rcTab);
	GetClientRect(&rcDlg);
	
	
	m_TabCtrl1.MoveWindow(&rcTab);
	
}
}
void CCellLocatiomDlg::SetImages1()
{	
	CImageList imagelist,imagelistDis;
	CBitmap bmp,bmpDis;
	// 
	imagelist.Create(16,16,ILC_COLOR24 | ILC_MASK,4,0);
	bmp.LoadBitmap(IDB_BITMAP1);
	imagelist.Add(&bmp,RGB(255,0,255));
	// 
	imagelistDis.Create(16,16,ILC_COLOR24 | ILC_MASK,4,0);
	bmpDis.LoadBitmap(IDB_BITMAP2);
	imagelistDis.Add(&bmpDis,RGB(255,0,255));
	// 
	m_TabCtrl1.SetImageLists(&imagelist,&imagelistDis);
}

bool CCellLocatiomDlg::IsExist1(CWnd *pWnd) const
{	
	for(int i=0, c=m_TabCtrl1.GetCount(); i<c; ++i)
		if(m_TabCtrl1.GetTabWnd( m_TabCtrl1.GetTab(i) )==pWnd->m_hWnd)
			return true;
		return false;
}


void CCellLocatiomDlg::OnCloseButtonClicked(TabCtrl * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/)
{	::MessageBox(m_hWnd,_T("DemoDlg::OnCloseButtonClicked"),_T("DemoDlg"),MB_OK);
}
void CCellLocatiomDlg::OnMenuButtonClicked(TabCtrl * /*pCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/)
{	::MessageBox(m_hWnd,_T("DemoDlg::OnMenuButtonClicked"),_T("DemoDlg"),MB_OK);
}
void CCellLocatiomDlg::OnDrag(TabCtrl *pCtrl, HTAB hTab, CPoint /*ptScr*/, bool outside)
{	if(outside==true && m_bDelDragOutside==true)
{	pCtrl->Delete(hTab);
pCtrl->Update();
}
}


CString CCellLocatiomDlg::GetInstallPath(void)
{
	CString strAppPath = _T("");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	
	return strAppPath;
}


void CCellLocatiomDlg::InvokeFunc()
{

	/*
	

	CString ch_query;
	
	ch_query="select * from mark";


	CString strAppPath = _T("");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	strAppPath = AppPathName;
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath+"\\database.mdb";
//	AfxMessageBox(strDBPath);
	CString str11,str12,str13,str4,str5,str6,str7;
		CString str21,str22,str23;
		CString str31,str32,str33;
		CString str41,str42,str43;

		CString str51,str52,str53;
		CString str61,str62,str63;
		CString str71,str72,str73;
		CString str81,str82,str83;

	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoConn->BeginTrans();
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
	}
	else
	{
		AfxMessageBox("数据库打开失败!");
		return;
	}
		
	m_pAdoRecordset->Open(ch_query);
		
	
	int num = m_pAdoRecordset->GetRecordCount();


	
	if(num<1)
	{
		m_dlgQry.m_btnExp.EnableWindow(FALSE);
		AfxMessageBox("没找到数据");
		return;
	}


	m_database.clear();
	m_webPage.CallJScript("ClearMarkerEvent");
	
	
	m_webPage.CallJScript("ClearLocations");
	
	

	m_dlgRst.m_list.DeleteAllItems();
	m_dlgQry.m_btnExp.EnableWindow(TRUE);
	int nItemCount = 0;
	m_pAdoRecordset->MoveFirst();
	//Here: bug  显示同一数据
	CString strData,strName;int ncnt;BYTE tp;
	while(!m_pAdoRecordset->IsEOF())
	{
	//	AfxMessageBox("a");	
		m_pAdoRecordset->GetCollect((LPCSTR)"lac1", str11);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid1", str12);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid1", str13);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac2", str21);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid2", str22);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid2", str23);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac3", str31);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid3", str32);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid3", str33);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac4", str41);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid4", str42);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid4", str43);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac5", str51);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid5", str52);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid5", str53);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac6", str61);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid6", str62);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid6", str63);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac7", str71);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid7", str72);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid7", str73);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac8", str81);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid8", str82);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid8", str83);

		m_pAdoRecordset->GetCollect((LPCSTR)"base_long", str4);
		m_pAdoRecordset->GetCollect((LPCSTR)"base_lat", str5);
		m_pAdoRecordset->GetCollect((LPCSTR)"mark", str6);
		m_pAdoRecordset->GetCollect((LPCSTR)"time", str7);


		

	//	AfxMessageBox(str7+ " "+ str6);
		ncnt = m_database.size();
		
	
		
		int ext = 0;
		for(int z=0;z<ncnt;z++)
		{
			if((atof(str4)== m_database[z].long1) && (atof(str5)== m_database[z].lat1))
			{
				strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";//str11+"-"+str12+"-#";
				m_database[z].m_g1 = strName;
				
				strName =	"<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
				m_database[z].m_g2 = strName;
				
				strName =	"<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";//str31+"-"+str32+"-"+str33;
				m_database[z].m_c = strName;
				
				strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
				m_database[z].m_w = strName;
				
				strName = "<tr align=\"center\">  <td移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
				m_database[z].m_t = strName;
				
				strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
				m_database[z].m_l1 = strName;
				
				strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
				m_database[z].m_l2 = strName;
				
				strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
				m_database[z].m_l3 = strName;
				
				ext=1;
				break;
			}
		}
		if(ext==1)
		{
			m_pAdoRecordset->MoveNext ();
			continue;
		}
		CInfoData infodata;
		infodata.long1 = atof(str4);
		infodata.lat1 = atof(str5);
		
		strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";
		infodata.m_g1 = strName;
		
		strName = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		infodata.m_g2 = strName;
		
		strName = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";
		infodata.m_c = strName;
		
		strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		infodata.m_w = strName;
		
		strName = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		infodata.m_t = strName;
		
		strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		infodata.m_l1 = strName;
		
		strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		infodata.m_l2 = strName;
		
		strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		infodata.m_l3 = strName;
		


		CTime tmm(atof(str7));
		
		str7 = tmm.Format("%y-%m-%d %H:%M:%S");
	//	if(str7.Left(2)!="15")
	//	{
	//		m_pAdoRecordset->MoveNext ();
	//		continue;
	//	}

		
		infodata.m_tm = "<h1 align=\"right\">      时间:"+str7+"</h1>";
		infodata.m_mk = "<h1>备注:"+str6+"</h1>";
		m_database.push_back( infodata );

//4546.40891/100=45.4640891可以直接读出45度, 4546.40891–45*100=46.40891, 可以直接读出46分

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_dlgRst.m_nIdx = 0;
		m_dlgRst.UpdateData(FALSE);
		m_dlgRst.m_list.InsertItem(ncnt,"");


	
		double long1 = atof(str4);
		str4.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
		double lat1 = atof(str5);
		str5.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
		


		
		
		m_dlgRst.m_list.SetItemText(ncnt,0,str7);
		m_dlgRst.m_list.SetItemText(ncnt,1,str4);
		m_dlgRst.m_list.SetItemText(ncnt,2,str5);
		m_dlgRst.m_list.SetItemText(ncnt,3,str11);
		m_dlgRst.m_list.SetItemText(ncnt,4,str12);
		m_dlgRst.m_list.SetItemText(ncnt,5,str21);
		m_dlgRst.m_list.SetItemText(ncnt,6,str22);
		m_dlgRst.m_list.SetItemText(ncnt,7,str31);
		m_dlgRst.m_list.SetItemText(ncnt,8,str33);
		m_dlgRst.m_list.SetItemText(ncnt,9,str41);
		m_dlgRst.m_list.SetItemText(ncnt,10,str42);
		m_dlgRst.m_list.SetItemText(ncnt,11,str51);
		m_dlgRst.m_list.SetItemText(ncnt,12,str52);
		m_dlgRst.m_list.SetItemText(ncnt,13,str61);
		m_dlgRst.m_list.SetItemText(ncnt,14,str62);
		m_dlgRst.m_list.SetItemText(ncnt,15,str71);
		m_dlgRst.m_list.SetItemText(ncnt,16,str72);
		m_dlgRst.m_list.SetItemText(ncnt,17,str81);
		m_dlgRst.m_list.SetItemText(ncnt,18,str82);
			
			
			
			
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
		m_pAdoRecordset->MoveNext ();
	}
	m_pAdoRecordset->Close();
	m_pAdoConn->Close();

	delete m_pAdoRecordset;
	delete m_pAdoConn;


	int nn = m_dlgRst.m_list.GetItemCount();
	if(nn>0)
		mark_flag = 1;

	ncnt = m_database.size();
	CString strlong2,strlat2,strLng,strLat;

	CString lll;
	lll.Format("%d",ncnt);
//	AfxMessageBox(lll);
	for(int k=0;k<ncnt;k++)
	{


		double long1 = m_database[k].long1;
		strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
		double lat1 = m_database[k].lat1;
		strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
		
		
		Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));
		
		strlong2.Format("%f", p->lng );
		strlat2.Format("%f", p->lat );

		CString strContent =m_database[k].m_mk+ "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
	//	strContent+=	"<tr align=\"center\">  <td>"+strZs+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
		strContent+=m_database[k].m_g1+m_database[k].m_g2+m_database[k].m_c+m_database[k].m_w+m_database[k].m_t+m_database[k].m_l1+m_database[k].m_l2+m_database[k].m_l3+"</table>";
		strContent+=m_database[k].m_tm;
		strData = strlong2+","+strlat2+","+strContent+","+strIcon[1];





		//strData = strlong2+","+strlat2+","+m_database[k].m_g1+"_"+m_database[k].m_g2+"_"+m_database[k].m_c+"_"+m_database[k].m_w+"_"+m_database[k].m_t+"_"+m_database[k].m_l1+"_"+m_database[k].m_l2+"_"+m_database[k].m_l3;
		
//		AfxMessageBox(strData);
		if(k==0)
		{
			strLng = strlong2;
			strLat = strlat2;
		}
		
		//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";
		
		m_webPage.CallJScript("AddLocation",strData);
		
		
		
		
		nItemCount++;
	}
//AfxMessageBox(strLng);

	pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
	if(nItemCount>0)
		pMainDlg->m_webPage.CallJScript("ShowLocations");

*/
}


void CCellLocatiomDlg::InvokeFuncWithParams(const CString& strParam1, int iParam2)
{
	CString		str;
	str.Format("第一个参数:%s,第二个参数:%d", strParam1, iParam2);
	MessageBox(str);
}


void CCellLocatiomDlg::SetControlPostion2()
{
	
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
/*	
	int dy = rtCaption.top;
	m_dlgLoad.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12-175));
	GetDlgItem(IDC_STATIC_TAB)->MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12-175));
	
	
	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12-175));
	pWndLeft->ShowWindow(SW_SHOW);
*/

	int dy = rtCaption.top;
	m_dlgLoad.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12));
	
	
	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-15,rtClient.Height()-15));
	pWndLeft->ShowWindow(SW_SHOW);
	

/*	pWndLeft=&m_dlgRst;
	pWndLeft->MoveWindow(CRect(15,rtClient.Height()-7-170,rtClient.Width()-15,rtClient.Height()-12));
	GetDlgItem(IDC_STATIC_BOTTOM)->MoveWindow(CRect(15,rtClient.Height()-7-170,rtClient.Width()-15,rtClient.Height()-12));
	m_dlgRst.ShowWindow(SW_SHOW);
	
	m_dlgQry.ShowWindow(SW_HIDE);

*/
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_SHOW);
	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgPreview.ShowWindow(SW_HIDE);
}


void CCellLocatiomDlg::SetControlPostion1()
{

	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
	
	int dy = rtCaption.top;
	m_dlgReal.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12));
	
	
	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-15,rtClient.Height()-15));
	pWndLeft->ShowWindow(SW_SHOW);
	

	
	pWnd = GetDlgItem(IDC_STATIC_K);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-15-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));
	
	pWnd = GetDlgItem(IDC_STATIC_Local);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-170-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-170,rtCaption.bottom-rtScreen.top));
	
	pWnd = GetDlgItem(IDC_COMBO2);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-30-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-30,rtCaption.bottom-rtScreen.top));
	
	pWnd = GetDlgItem(IDC_STATIC_HL);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(15,rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));

	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_SHOW);
	m_dlgQry.ShowWindow(SW_HIDE);
	//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
m_dlgPreview.ShowWindow(SW_HIDE);

/*	
	CRect rect;
//	CWnd* pWnd=


	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	

	
	CRect rtCaption;   
	CWnd *pWndCaption = GetDlgItem(IDC_EXPLORER1);//pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int dy = rtCaption.top;

	pWndCaption->MoveWindow(CRect(15,dy-rtScreen.top,rtClient.Width()-15,rtClient.Height()-12-170)); //移动button  









	CWnd* pWnd = GetDlgItem(IDC_STATIC_K);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-15-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));

	pWnd = GetDlgItem(IDC_STATIC_Local);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-170-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-170,rtCaption.bottom-rtScreen.top));

	pWnd = GetDlgItem(IDC_COMBO2);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-30-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-30,rtCaption.bottom-rtScreen.top));

	pWnd = GetDlgItem(IDC_STATIC_HL);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(15,rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));

//	CWnd* pWnd = GetDlgItem(IDC_STATIC_K);
//	pWnd->GetWindowRect(&rtCaption);
//	pWnd->MoveWindow(CRect(rtClient.Width()-15-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));

	
	CWnd* pWndLeft;//=GetDlgItem(IDC_STATIC_TAB);
	
//	pWndCaption->MoveWindow(CRect(dx-rtClient.left,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12-250));


	pWndLeft=&m_dlgReal;
	pWndLeft->MoveWindow(CRect(15,rtClient.Height()-7-170,rtClient.Width()-15,rtClient.Height()-12));
	GetDlgItem(IDC_STATIC_BOTTOM)->MoveWindow(CRect(15,rtClient.Height()-7-170,rtClient.Width()-15,rtClient.Height()-12));
	m_dlgReal.ShowWindow(SW_SHOW);

	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);

	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);*/
}


void CCellLocatiomDlg::SetControlPostion3()
{
	
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
	
	int dy = rtCaption.top;
	m_dlgDown.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12));
	
	

	CWnd *pWndR=GetDlgItem(IDC_LIST1);
	pWndR->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	


	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(2000,dy-rtScreen.top,2000+rtClient.Width()-10,rtClient.Height()-12));
	pWndLeft->ShowWindow(SW_SHOW);
	
	m_list.ShowWindow(SW_SHOW);
	m_dlgDown.ShowWindow(SW_SHOW);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_HIDE);
	//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgPreview.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
}

void CCellLocatiomDlg::SetControlPostion4()
{
	
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
	
	int dy = rtCaption.top;
	m_dlgBs.MoveWindow(CRect(15,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	

	CWnd *pWndR=GetDlgItem(IDC_LIST1);
	pWndR->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	
	m_list.ShowWindow(SW_HIDE);
	m_list2.ShowWindow(SW_HIDE);
	
	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(2000,dy-rtScreen.top,2000+rtClient.Width()-10,rtClient.Height()-12));
	pWndLeft->ShowWindow(SW_SHOW);
	
	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_HIDE);
	//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgPreview.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_SHOW);
}
void CCellLocatiomDlg::SetControlPostion5()
{
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
	
	int dy = rtCaption.top;
	m_dlgPreview.MoveWindow(CRect(15,dy-rtScreen.top,rtClient.right-15,rtClient.Height()-12));
	
	

//	CWnd *pWndR=GetDlgItem(IDC_LIST1);
//	pWndR->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	


	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(2000,dy-rtScreen.top,2000+rtClient.Width()-10,rtClient.Height()-12));
	pWndLeft->ShowWindow(SW_SHOW);
	
	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_HIDE);
	//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
	m_dlgPreview.ShowWindow(SW_SHOW);

}

void CCellLocatiomDlg::SetControlPostion6()
{
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   
	
	int dy = rtCaption.top;
	m_dlgPreview.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12));
	
	

//	CWnd *pWndR=GetDlgItem(IDC_LIST1);
//	pWndR->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	

	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-15,rtClient.Height()-15));
	pWndLeft->ShowWindow(SW_SHOW);
	
//	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
//	pWndLeft->MoveWindow(CRect(2000,dy-rtScreen.top,2000+rtClient.Width()-10,rtClient.Height()-12));
//	pWndLeft->ShowWindow(SW_SHOW);
	
	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_HIDE);
	//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
	m_dlgPreview.ShowWindow(SW_SHOW);

}



void CCellLocatiomDlg::SetControlPostion()
{
	
	CRect rect;
	CWnd* pWnd= GetDlgItem(IDC_STATIC_TAB);//&m_dlgQry;//GetDlgItem(IDC_EXPLORER1);
	
	CRect rtClient;   
	CRect rtScreen;
	GetClientRect(&rtScreen);             //获得对话框客户区屏幕坐标   
	rtClient.left = rtScreen.left;
	rtClient.top = rtScreen.top;
	rtClient.right = rtScreen.right;
	rtClient.bottom = rtScreen.bottom;
	
    ClientToScreen(&rtScreen); //映射为屏幕坐标  
	
	
	CRect rtCaption;   
	CWnd *pWndCaption = pWnd;   
    pWndCaption->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	int lx=rtCaption.Width();                 //button长度   
	int ly=rtCaption.Height();   //button高度   

	int dy = rtCaption.top;
	m_dlgQry.MoveWindow(CRect(15,dy-rtScreen.top,15+lx,rtClient.Height()-12));
	
		
	pWnd = GetDlgItem(IDC_STATIC_K);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-15-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-15,rtCaption.bottom-rtScreen.top));
	
	pWnd = GetDlgItem(IDC_STATIC_Local);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-170-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-170,rtCaption.bottom-rtScreen.top));
	
	pWnd = GetDlgItem(IDC_COMBO2);
	pWnd->GetWindowRect(&rtCaption);
	pWnd->MoveWindow(CRect(rtClient.Width()-30-rtCaption.Width(),rtCaption.top-rtScreen.top,rtClient.Width()-30,rtCaption.bottom-rtScreen.top));
	
	
	
	CWnd *pWndLeft=GetDlgItem(IDC_EXPLORER1);
	pWndLeft->MoveWindow(CRect(15+lx+10,dy-rtScreen.top,rtClient.Width()-10,rtClient.Height()-12));
	pWndLeft->ShowWindow(SW_SHOW);

	m_list.ShowWindow(SW_HIDE);
	m_dlgDown.ShowWindow(SW_HIDE);
	m_dlgReal.ShowWindow(SW_HIDE);
	m_dlgQry.ShowWindow(SW_SHOW);
//	m_dlgQry.ShowWindow(SW_HIDE);
	m_dlgRst.ShowWindow(SW_HIDE);
	m_dlgLoad.ShowWindow(SW_HIDE);
	m_dlgBs.ShowWindow(SW_HIDE);
}
void CCellLocatiomDlg::QryData2(DWORD a,DWORD b)
{
	m_database.clear();
	CString ch_query,stra,strb,strLng,strLat;
	stra.Format("%d",a);
	strb.Format("%d",b);
	ch_query="select * from main where time > "+stra+" and time < "+strb+" order by time";

//	AfxMessageBox(ch_query);
		CString strAppPath = _T("");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	strAppPath = AppPathName;
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath+"\\database.mdb";
//	AfxMessageBox(strDBPath);
	CString str11,str12,str13,str4,str5,str6,str7;
		CString str21,str22,str23;
		CString str31,str32,str33;
		CString str41,str42,str43;

		CString str51,str52,str53;
		CString str61,str62,str63;
		CString str71,str72,str73;
		CString str81,str82,str83;

	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoConn->BeginTrans();
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
	}
	else
	{
		AfxMessageBox("数据库打开失败!");
		return;
	}
		
	m_pAdoRecordset->Open(ch_query);
		
	
	int num = m_pAdoRecordset->GetRecordCount();

	CString stt;
	stt.Format("%d",num);
//	AfxMessageBox(stt);
	
	if(num<1)
	{
		AfxMessageBox("没找到数据");
		return;
	}
	int nItemCount = 0;
	m_pAdoRecordset->MoveFirst();
	//Here: bug  显示同一数据
	CString strData,strName;int ncnt;BYTE tp;
	while(!m_pAdoRecordset->IsEOF())
	{
			
		m_pAdoRecordset->GetCollect((LPCSTR)"lac1", str11);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid1", str12);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid1", str13);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac2", str21);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid2", str22);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid2", str23);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac3", str31);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid3", str32);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid3", str33);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac4", str41);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid4", str42);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid4", str43);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac5", str51);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid5", str52);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid5", str53);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac6", str61);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid6", str62);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid6", str63);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac7", str71);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid7", str72);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid7", str73);

		m_pAdoRecordset->GetCollect((LPCSTR)"lac8", str81);
		m_pAdoRecordset->GetCollect((LPCSTR)"cid8", str82);
		m_pAdoRecordset->GetCollect((LPCSTR)"bsid8", str83);

		m_pAdoRecordset->GetCollect((LPCSTR)"base_long", str4);
		m_pAdoRecordset->GetCollect((LPCSTR)"base_lat", str5);
//		m_pAdoRecordset->GetCollect((LPCSTR)"type", str6);



		ncnt = m_database.size();
		int ext = 0;
		for(int z=0;z<ncnt;z++)
		{
			if((atof(str4)== m_database[z].long1) && (atof(str5)== m_database[z].lat1))
			{
				strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";//str11+"-"+str12+"-#";
				m_database[z].m_g1 = strName;
				
				strName =	"<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
				m_database[z].m_g2 = strName;
				
				strName =	"<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";//str31+"-"+str32+"-"+str33;
				m_database[z].m_c = strName;
				
				strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
				m_database[z].m_w = strName;
				
				strName = "<tr align=\"center\">  <td移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
				m_database[z].m_t = strName;
				
				strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
				m_database[z].m_l1 = strName;
				
				strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
				m_database[z].m_l2 = strName;
				
				strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
				m_database[z].m_l3 = strName;
				
				ext=1;
				break;
			}
		}
		if(ext==1)
		{
			m_pAdoRecordset->MoveNext ();
			continue;
		}
		CInfoData infodata;
		infodata.long1 = atof(str4);
		infodata.lat1 = atof(str5);
		
		strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";
		infodata.m_g1 = strName;
		
		strName = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		infodata.m_g2 = strName;
		
		strName = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";
		infodata.m_c = strName;
		
		strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		infodata.m_w = strName;
		
		strName = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		infodata.m_t = strName;
		
		strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		infodata.m_l1 = strName;
		
		strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		infodata.m_l2 = strName;
		
		strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		infodata.m_l3 = strName;
		

		m_database.push_back( infodata );

//4546.40891/100=45.4640891可以直接读出45度, 4546.40891–45*100=46.40891, 可以直接读出46分

   
		m_pAdoRecordset->MoveNext ();
	}
	m_pAdoRecordset->Close();
	m_pAdoConn->Close();

	delete m_pAdoRecordset;
	delete m_pAdoConn;



	ncnt = m_database.size();
	CString strlong2,strlat2;

	CString lll;
	lll.Format("%d",ncnt);
//	AfxMessageBox(lll);
	for(int k=0;k<ncnt;k++)
	{


		double long1 = m_database[k].long1;
		strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
		double lat1 = m_database[k].lat1;
		strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
		
		
		Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));
		
		strlong2.Format("%f", p->lng );
		strlat2.Format("%f", p->lat );

		CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
	//	strContent+=	"<tr align=\"center\">  <td>"+strZs+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
		strContent+=m_database[k].m_g1+m_database[k].m_g2+m_database[k].m_c+m_database[k].m_w+m_database[k].m_t+m_database[k].m_l1+m_database[k].m_l2+m_database[k].m_l3+"</table>";
		strData = strlong2+","+strlat2+","+strContent+","+strIcon[0];





		//strData = strlong2+","+strlat2+","+m_database[k].m_g1+"_"+m_database[k].m_g2+"_"+m_database[k].m_c+"_"+m_database[k].m_w+"_"+m_database[k].m_t+"_"+m_database[k].m_l1+"_"+m_database[k].m_l2+"_"+m_database[k].m_l3;
		
//		AfxMessageBox(strData);
		if(k==0)
		{
			strLng = strlong2;
			strLat = strlat2;
		}
		
		//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";
		
		m_webPage.CallJScript("AddLocation",strData);
		
		
		
		
		nItemCount++;
	}


	pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
	if(nItemCount>0)
		pMainDlg->m_webPage.CallJScript("ShowLocations");

	
}

void CCellLocatiomDlg::QryData3(double min_lat,double min_lng,double max_lat,double max_lng)
{
/*	m_database.clear();
	m_webPage.CallJScript("ClearMarkerEvent");


	m_webPage.CallJScript("ClearLocations");

	Point * p1 =  gcj_wgs_decrypt(min_lat, min_lng);
	Point * p2 =  gcj_wgs_decrypt(max_lat, max_lng);



	CString lat_up,lat_down,lng_up,lng_down,strLng,strLat;
	double dlat_up,dlat_down,dlng_up,dlng_down;
	dlat_up = p1->lat;
	dlat_down = p2->lat;
	dlng_up = p1->lng;
	dlng_down = p2->lng;


	dlat_up = ((int)dlat_up)*100 + 60*(dlat_up-(int)(dlat_up));
	dlat_down = ((int)dlat_down)*100 + 60*(dlat_down-(int)(dlat_down));
	dlng_up = ((int)dlng_up)*100 + 60*(dlng_up-(int)(dlng_up));
	dlng_down = ((int)dlng_down)*100 + 60*(dlng_down-(int)(dlng_down));
	lat_up.Format("%f",dlat_up);
	lat_down.Format("%f",dlat_down);
	lng_up.Format("%f",dlng_up);
	lng_down.Format("%f",dlng_down);
	CString ch_query;
	
	ch_query="select * from main where base_lat between "+lat_up+" and "+lat_down+" and base_long between "+lng_up+" and "+lng_down;

//	AfxMessageBox(ch_query);
		CString strAppPath = _T("");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	strAppPath = AppPathName;
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath+"\\database.mdb";
//	AfxMessageBox(strDBPath);
	CString str11,str12,str13,str4,str5,str6,str7;
		CString str21,str22,str23;
		CString str31,str32,str33;
		CString str41,str42,str43;

		CString str51,str52,str53;
		CString str61,str62,str63;
		CString str71,str72,str73;
		CString str81,str82,str83;

	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoConn->BeginTrans();
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
	}
	else
	{
		AfxMessageBox("数据库打开失败!");
		return;
	}
		
	m_pAdoRecordset->Open(ch_query);
		
	CString strData,strName;int ncnt;BYTE tp;
	int num = m_pAdoRecordset->GetRecordCount();

	CString stt;
	stt.Format("查询到数据：%d 条",num);
	m_dlgQry.m_edtRst.SetWindowText(stt);
	
	if(num<1)
	{
		m_dlgQry.m_btnExp.EnableWindow(FALSE);
	
	}

	else
	{

	
		mark_flag = 0;
		m_dlgRst.m_list.DeleteAllItems();
		m_dlgQry.m_btnExp.EnableWindow(TRUE);
		int nItemCount = 0;
		m_pAdoRecordset->MoveFirst();
		//Here: bug  显示同一数据
		
		while(!m_pAdoRecordset->IsEOF())
		{
				
			m_pAdoRecordset->GetCollect((LPCSTR)"lac1", str11);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid1", str12);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid1", str13);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac2", str21);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid2", str22);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid2", str23);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac3", str31);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid3", str32);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid3", str33);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac4", str41);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid4", str42);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid4", str43);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac5", str51);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid5", str52);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid5", str53);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac6", str61);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid6", str62);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid6", str63);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac7", str71);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid7", str72);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid7", str73);

			m_pAdoRecordset->GetCollect((LPCSTR)"lac8", str81);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid8", str82);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid8", str83);

			m_pAdoRecordset->GetCollect((LPCSTR)"base_long", str4);
			m_pAdoRecordset->GetCollect((LPCSTR)"base_lat", str5);
	//		m_pAdoRecordset->GetCollect((LPCSTR)"type", str6);
			m_pAdoRecordset->GetCollect((LPCSTR)"time", str7);


			

			ncnt = m_database.size();
			int ext = 0;
			for(int z=0;z<ncnt;z++)
			{
				if((atof(str4)== m_database[z].long1) && (atof(str5)== m_database[z].lat1))
				{
					strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";//str11+"-"+str12+"-#";
					m_database[z].m_g1 = strName;
					
					strName =	"<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
					m_database[z].m_g2 = strName;
					
					strName =	"<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";//str31+"-"+str32+"-"+str33;
					m_database[z].m_c = strName;
					
					strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
					m_database[z].m_w = strName;
					
					strName = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
					m_database[z].m_t = strName;
					
					strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
					m_database[z].m_l1 = strName;
					
					strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
					m_database[z].m_l2 = strName;
					
					strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
					m_database[z].m_l3 = strName;
					
					ext=1;
					break;
				}
			}
			if(ext==1)
			{
				m_pAdoRecordset->MoveNext ();
				continue;
			}
			CInfoData infodata;
			infodata.long1 = atof(str4);
			infodata.lat1 = atof(str5);
			
			strName = 	"<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";
			infodata.m_g1 = strName;
			
			strName = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
			infodata.m_g2 = strName;
			
			strName = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>"+str32+"</td> <td>"+str33+"</td> </tr>";
			infodata.m_c = strName;
			
			strName = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
			infodata.m_w = strName;
			
			strName = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
			infodata.m_t = strName;
			
			strName = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
			infodata.m_l1 = strName;
			
			strName = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
			infodata.m_l2 = strName;
			
			strName = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
			infodata.m_l3 = strName;
			
			CTime tmm(atof(str7));
			
			str7 = tmm.Format("%y-%m-%d %H:%M:%S");

			infodata.m_tm = "<h1 align=\"right\">      时间:"+str7+"</h1>";
			m_database.push_back( infodata );

	//4546.40891/100=45.4640891可以直接读出45度, 4546.40891–45*100=46.40891, 可以直接读出46分

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m_dlgRst.m_nIdx = 0;
			m_dlgRst.UpdateData(FALSE);
			m_dlgRst.m_list.InsertItem(ncnt,"");



	//		if(str7.Left(2)!="15")
	//			continue;
			double long1 = atof(str4);
			str4.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
			double lat1 = atof(str5);
			str5.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
			

			CString strNCNT;
			strNCNT.Format("%d",ncnt);
	//		AfxMessageBox(str7.Left(2));
			
			m_dlgRst.m_list.SetItemText(ncnt,0,str7);
			m_dlgRst.m_list.SetItemText(ncnt,1,str4);
			m_dlgRst.m_list.SetItemText(ncnt,2,str5);
			m_dlgRst.m_list.SetItemText(ncnt,3,str11);
			m_dlgRst.m_list.SetItemText(ncnt,4,str12);
			m_dlgRst.m_list.SetItemText(ncnt,5,str21);
			m_dlgRst.m_list.SetItemText(ncnt,6,str22);
			m_dlgRst.m_list.SetItemText(ncnt,7,str31);
			m_dlgRst.m_list.SetItemText(ncnt,8,str33);
			m_dlgRst.m_list.SetItemText(ncnt,9,str41);
			m_dlgRst.m_list.SetItemText(ncnt,10,str42);
			m_dlgRst.m_list.SetItemText(ncnt,11,str51);
			m_dlgRst.m_list.SetItemText(ncnt,12,str52);
			m_dlgRst.m_list.SetItemText(ncnt,13,str61);
			m_dlgRst.m_list.SetItemText(ncnt,14,str62);
			m_dlgRst.m_list.SetItemText(ncnt,15,str71);
			m_dlgRst.m_list.SetItemText(ncnt,16,str72);
			m_dlgRst.m_list.SetItemText(ncnt,17,str81);
			m_dlgRst.m_list.SetItemText(ncnt,18,str82);
				
				
				
				
				
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
			m_pAdoRecordset->MoveNext ();
		}

	}
	m_pAdoRecordset->Close();



//------------------------------------------------------------------------------------------------------------------------------------------






	int	nItemCount=0;


	lat_up.Format("%f",min_lat);
	lat_down.Format("%f",max_lat);
	lng_up.Format("%f",min_lng);
	lng_down.Format("%f",max_lng);


	
	ch_query="select * from basestation where lat between "+lat_up+" and "+lat_down+" and lng between "+lng_up+" and "+lng_down;


//	AfxMessageBox(ch_query);
	m_pAdoRecordset->Open(ch_query);
		
	
	num = m_pAdoRecordset->GetRecordCount();



	if(num>=1)
	{
	

		m_dlgRst.m_list.DeleteAllItems();
		m_dlgQry.m_btnExp.EnableWindow(TRUE);
		
		m_pAdoRecordset->MoveFirst();
		//Here: bug  显示同一数据
		
		CString stra,strb,strc,strd,stre,strf,strg,strh,stri;
		while(!m_pAdoRecordset->IsEOF())
		{
				
			m_pAdoRecordset->GetCollect((LPCSTR)"lac", stra);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid", strb);
			m_pAdoRecordset->GetCollect((LPCSTR)"bsid", strc);
			m_pAdoRecordset->GetCollect((LPCSTR)"marker", strd);
			m_pAdoRecordset->GetCollect((LPCSTR)"type", stre);
			m_pAdoRecordset->GetCollect((LPCSTR)"lng", strf);
			m_pAdoRecordset->GetCollect((LPCSTR)"lat", strg);
			m_pAdoRecordset->GetCollect((LPCSTR)"reserve1", strh);


			if(stre=="0")
			{

				stri="移动GSM";
			}
			else if(stre=="1")
			{
				
				stri="联通GSM";
			}
			else if(stre=="2")
			{
				
				stri="电信CDMA";
			}
			else if(stre=="3")
			{
				
				stri="联通WCDMA";
			}
			else if(stre=="4")
			{
				
				stri="移动TD";
			}
			else if(stre=="5")
			{
				
				stri="移动LTE";
			}
			else if(stre=="6")
			{
				
				stri="联通LTE";
			}
			else if(stre=="7")
			{
				
				stri="电信LTE";
			}

			CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
			if(stre!="2")
				strName = 	"<tr align=\"center\">  <td>"+stri+"</td> <td>"+stra+"</td> <td>"+strb+"</td> <td>#</td> </tr>";
			else
				strName = 	"<tr align=\"center\">  <td>"+stri+"</td> <td>"+stra+"</td> <td>"+strb+"</td> <td>"+strc+"</td> </tr>";
			
			
			//	strContent+=	"<tr align=\"center\">  <td>"+strZs+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
			strContent+=strName+"</table>";
			strContent+= "<h1 align=\"right\">      备注:"+strd+"</h1>";
			strData = strf+","+strg+","+strContent+","+strIcon[10];
	
		//	AfxMessageBox(strData);
			m_webPage.CallJScript("AddLocation",strData);


			//break;
			nItemCount++;



			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
			m_pAdoRecordset->MoveNext ();
		}


	}






//-------------------------------------------------------------------------------------------------------------------------------------------

	m_pAdoConn->Close();

	delete m_pAdoRecordset;
	delete m_pAdoConn;



	ncnt = m_database.size();
	CString strlong2,strlat2;


	for(int k=0;k<ncnt;k++)
	{


		double long1 = m_database[k].long1;
		strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
		double lat1 = m_database[k].lat1;
		strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
		
		
		Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));
		
		strlong2.Format("%f", p->lng );
		strlat2.Format("%f", p->lat );

		CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
	//	strContent+=	"<tr align=\"center\">  <td>"+strZs+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
		strContent+=m_database[k].m_g1+m_database[k].m_g2+m_database[k].m_c+m_database[k].m_w+m_database[k].m_t+m_database[k].m_l1+m_database[k].m_l2+m_database[k].m_l3+"</table>";
		strContent+=m_database[k].m_tm;
		strData = strlong2+","+strlat2+","+strContent+","+strIcon[0];





		//strData = strlong2+","+strlat2+","+m_database[k].m_g1+"_"+m_database[k].m_g2+"_"+m_database[k].m_c+"_"+m_database[k].m_w+"_"+m_database[k].m_t+"_"+m_database[k].m_l1+"_"+m_database[k].m_l2+"_"+m_database[k].m_l3;
		
//		AfxMessageBox(strData);
//		if(k==0)
//		{
//			strLng = strlong2;
//			strLat = strlat2;
//		}
		
		//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";
		
		m_webPage.CallJScript("AddLocation",strData);
		
		nItemCount++;
	}



	


//	pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
	if(nItemCount>0)
	{
		
		pMainDlg->m_webPage.CallJScript("ShowLocations");
	}
*/
//	AfxMessageBox("ggg");
}


struct Loc
{
	double Lon;			//坐标的经度
	double Lat;			//坐标的纬度
};
 extern Loc bd_encrypt(Loc gg);//将谷歌坐标转换为百度坐标
extern Loc transform(Loc gps);//将GPS坐标转换为google地图
 
void CCellLocatiomDlg::QryData(int cb_sel,int lac,int cid)
{

	
	m_database.clear();
	CString ch_query;
	CString strType;
	CString strLng,strLat;
	CString strLac,strCid;
	strLac.Format("%d",lac);
	strCid.Format("%d",cid);
	strType.Format("%d",cb_sel);
	CString table_name;
	
	
	if(cb_sel==0 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'GSM' and plmn = '46000' and cid = '"+strCid+"'";
		else
			ch_query="select * from gather where mode = 'GSM' and plmn = '46000' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==1 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'GSM' and plmn = '46001' and cid = '"+strCid+"'";
		else
			ch_query="select * from gather where mode = 'GSM' and plmn = '46001' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==2 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'CDMA' and plmn = '46003' and cid = '"+strCid+"'";
		else
			ch_query="select * from gather where mode = 'CDMA' and plmn = '46003' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==3 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'WCDMA' and plmn = '46001' and cid = '"+strCid+"'";
		else
			ch_query="select * from gather where mode = 'WCDMA' and plmn = '46001' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==4 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'LTE' and plmn = '46000' and cid = '"+strCid+"'";
		else
			ch_query="select * from gather where mode = 'LTE' and plmn = '46000' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==5 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'LTE' and plmn = '46001' and cid = '"+strCid;
		else
			ch_query="select * from gather where mode = 'LTE' and plmn = '46001' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==6 )
	{
		
		if(lac==0)
			ch_query="select * from gather where mode = 'LTE' and plmn = '46011' and cid = '"+strCid;
		else
			ch_query="select * from gather where mode = 'LTE' and plmn = '46011' and cid = '"+strCid+"' and lac = '"+strLac+"'";
	}
	if(cb_sel==7 )
	{
		
		if(lac==0)
			ch_query="select * from gather where cid = '"+strCid+"'";
		else
			ch_query="select * from gather where cid = '"+strCid+"' and lac = '"+strLac+"'";
	}


	
//	AfxMessageBox(ch_query);

	CString strAppPath = _T("");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	strAppPath = AppPathName;
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath+"\\database.mdb";
//	AfxMessageBox(strDBPath);
	CString str1,str2,str3,str4,str5,str6;
	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoConn->BeginTrans();
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
	}
	else
	{
		AfxMessageBox("数据库打开失败!");
		return;
	}
//	AfxMessageBox(ch_query);
	m_pAdoRecordset->Open(ch_query);
		
	
	int num = m_pAdoRecordset->GetRecordCount();
	int nItemCount = 0;
	CString stt;
	stt.Format("共找到记录%d个",num);
	
	CString strData;
	CString strZs;
	if(num<1)
	{
		AfxMessageBox("路测数据库中没找到数据");
		//return;

		m_pAdoRecordset->Close();
		m_pAdoConn->Close();
		
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	else
	{

//		AfxMessageBox(stt);
		pMainDlg->m_webPage.CallJScript("ClearLocations");
		m_pAdoRecordset->MoveFirst();
		//Here: bug  显示同一数据

		while(!m_pAdoRecordset->IsEOF())
		{
			
/*			if(cb_sel==0)
			{
				strZs = "移动GSM";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac1", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid1", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid1", str3);
			}
			else if(cb_sel==1)
			{
				strZs = "联通GSM";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac2", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid2", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid2", str3);
			}
			else if(cb_sel==2)
			{
				strZs = "电信CDMA";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac3", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid3", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid3", str3);
			}
			else if(cb_sel==3)
			{
				strZs = "联通WCDMA";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac4", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid4", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid4", str3);
			}
			else if(cb_sel==4)
			{
				strZs = "移动TD";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac5", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid5", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid5", str3);
			}
			else if(cb_sel==5)
			{
				strZs = "移动LTE";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac6", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid6", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid6", str3);
			}
			else if(cb_sel==6)
			{
				strZs = "联通LTE";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac7", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid7", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid7", str3);
			}
			else if(cb_sel==7)
			{
				strZs = "电信GSM";
				m_pAdoRecordset->GetCollect((LPCSTR)"lac8", str1);
				m_pAdoRecordset->GetCollect((LPCSTR)"cid8", str2);
				m_pAdoRecordset->GetCollect((LPCSTR)"bsid8", str3);
			}
*/
			m_pAdoRecordset->GetCollect((LPCSTR)"lac", str1);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid", str2);
			m_pAdoRecordset->GetCollect((LPCSTR)"plmn", strZs);
			m_pAdoRecordset->GetCollect((LPCSTR)"rssi", str3);
			m_pAdoRecordset->GetCollect((LPCSTR)"mode", str6);
 
			m_pAdoRecordset->GetCollect((LPCSTR)"lng", str4);
			m_pAdoRecordset->GetCollect((LPCSTR)"lat", str5);

	//4546.40891/100=45.4640891可以直接读出45度, 4546.40891–45*100=46.40891, 可以直接读出46分

			double long1 = atof(str4);
			//str4.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
			double lat1 = atof(str5);
			//str5.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);

			if(lat1==0.0)
			{
				m_pAdoRecordset->GetCollect((LPCSTR)"lng2", str4);
				m_pAdoRecordset->GetCollect((LPCSTR)"lat2", str5);
				long1 = atof(str4);
				lat1 = atof(str5);

			}
		Point * p =  wgs_gcj_encrypts(lat1, long1);
		
		str4.Format("%f", p->lng );
		str5.Format("%f", p->lat );


		CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(TAC)</th><th>CID</th> <th>RSSI</th> </tr>";
		strContent+=	"<tr align=\"center\">  <td>"+strZs+" "+str6+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
		
	/*		Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
			
			str4.Format("%f", p->lng );
			str5.Format("%f", p->lat );


			CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
			strContent+=	"<tr align=\"center\">  <td>"+strZs+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";
			
			strData = str4+","+str5+","+strContent+","+strIcon[0];
*/
	/*
			
			  '<table width="300" align="center" border="1" cellspacing="0">  <tr bgcolor="#A7DCE0"> <th>网络制式</th> <th>LAC(SID)</th> <th>CID(NID)</th> <th>(BSID)</th> </tr> '+
			  '<tr align="center">  <td>移动GSM</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>联通GSM</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>电信CDMA</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>联通WCDMA</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>移动TD</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>移动LTE</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
			  '<tr align="center">  <td>电信LTE</td> <td>2012-5-10</td> <td>标题1</td> <td>标题1</td> </tr>' +
		'<tr align="center">  <td>联通LTE</td> <td>2012-5-20</td> <td>标题1</td> <td>标题2</td> </tr> </table>'
			
			*/		
		//	if(cb_sel!=2)
		//		strData = str4+","+str5+","+str1+"_"+str2;
		//	else
		//		strData = str4+","+str5+","+str1+"_"+str2+"_"+str3;
			
			
			
			
			//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";


			Loc demo,bd;
				demo.Lat = atof(str5);
				demo.Lon = atof(str4);
if(atof(str5)!=0.0)
{
			//	bd = (transform(demo));//bd_encrypt
			//	str4.Format("%.5f",bd.Lon);
			//	str5.Format("%.5f",bd.Lat);



			//	AfxMessageBox(str4+","+str5);


		


	
			if(nItemCount==0)
			{
				strLng = str4;
				strLat = str5;
			}	
		
			strData = str4+","+str5+","+strContent+","+strIcon[0];
		//	AfxMessageBox(strData);
			//m_webPage.CallJScript("addMarker",str4,str5);AddLocation
			m_webPage.CallJScript("AddLocation",strData);

}
			
			nItemCount++;   
			m_pAdoRecordset->MoveNext ();
		}
	}




	m_pAdoRecordset->Close();
	m_pAdoConn->Close();

	delete m_pAdoRecordset;
	delete m_pAdoConn;
	



	/*if(mysql_real_query(&mysql,ch_query,(UINT)strlen(ch_query))!=0)
	{ 
		AfxMessageBox("数据库中表格出错"); 
	}
	CString str,strTmp="",strData="";
	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!(result=mysql_use_result(&mysql)))
	{ 
		AfxMessageBox("读取数据集失败");
	}
	int i=0;
	while(row=mysql_fetch_row(result))
	{
		strData = "";
		str.Format("%s",row[2]);
		strData += str;
		strData += ",";

		if(i==0)
			strLng = str;
		str.Format("%s",row[1]);
		strData += str;
		strData += ",";

		if(i==0)
			strLat = str;

		str.Format("%s#%s#%s#%s#%s_0_0",row[2],row[3],row[4],row[5],row[6]);
		strData += str;
		m_list.AddString(strData);
		

	//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";

		m_webPage.CallJScript("AddLocation",strData);

		i++;
	}
	*/
	pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
	if(nItemCount>0)
		pMainDlg->m_webPage.CallJScript("ShowLocations");

	
	
	//mysql_free_result(result);
}


void CCellLocatiomDlg::ShowModalDlg(double dbLat, double dbLng)
{
//	CModalDlg dlg;
//	dlg.SetDataInfo(strTitle, dbLng, dbLat);
//	dlg.DoModal();


	Point * p1 =  gcj_wgs_decrypt(dbLat, dbLng);
//	Point * p2 =  gcj_wgs_decrypt(max_lat, max_lng);

	CString strTmp1,strTmp2;
	strTmp2.Format("纬度：%f", p1->lat);
	strTmp1.Format("经度：%f", p1->lng);
//	AfxMessageBox(strTmp);

	m_dSelectedLng = p1->lng;
	m_dSelectedLat = p1->lat;
	
	m_pDlgEdit->m_stLng.SetWindowText(strTmp1);
	m_pDlgEdit->m_stLat.SetWindowText(strTmp2);
	m_pDlgEdit->ShowWindow(SW_SHOW);
}

void CCellLocatiomDlg::ShowLatLng(double dbLng, double dbLat)
{
	m_dSelLat = dbLat;
	m_dSelLong = dbLng;
	SetTimer(1,1000,NULL);
	CString strTmp;
	strTmp.Format("中心点经纬度：%.5f    %.5f",dbLng, dbLat);
	m_dlgQry.m_stCur.SetWindowText(strTmp);
//	AfxMessageBox(strTmp);
}

void CCellLocatiomDlg::ShowCurLevel(int level)
{	CString strTmp;
	strTmp.Format("%d",level);
	AfxMessageBox(strTmp);
}

void CCellLocatiomDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{

		LoadLocList();
		KillTimer(1);
//		QryData3();
		
	}

	if(nIDEvent==2)
	{

		if(m_strLat!="" && m_dlgReal.m_nLockItem!=-1)
		{
			pMainDlg->m_webPage.CallJScript("SetMarker", m_strLng,m_strLat );
			m_strLat="";
		}

	}
	CDialog::OnTimer(nIDEvent);
}


#define HEAD0 0x00
#define HEAD1 0x01
#define HEAD2 0x02
#define HEAD3 0x03
#define HEAD4 0x04
#define HEAD5 0x05
#define HEAD6 0x06
#define HEAD7 0x07

#define DATA1 0x15
#define DATA2 0x25
#define DATA3 0x35
#define DATA4 0x45
#define DATA5 0x55
#define DATA6 0x65
#define DATA7 0x75
#define LEN1 0x09
#define LEN2 0x07
#define LEN3 0x07
#define DATA 0x08

char csData1[12]={0};
char csData2[12]={0};
char csData3[12]={0};
char csData4[12]={0};
char csData5[12]={0};

BYTE real_data[2048];
int real_data_pos=0;

FILE*pfTmpBT;

void CCellLocatiomDlg::RecvReal()
{
//	if(m_nTabID==1 && m_bCanShow)
//		m_dlgReal.ShowReal(&m_curRealData);
}


void CCellLocatiomDlg::SaveMark(CString strMark)
{

	int i= 0;
	CTime tmm = CTime::GetCurrentTime();
	CString strTime = tmm.Format("%y%m%d %H%M%S");
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	int id; 
	
	strDBPath += "\\"+strTime+".txt";
	FILE *pf = fopen(strMark,"a");
	fprintf(pf,"================================================\n");
	
	fprintf(pf,"保存时间:   %s \n",strTime);

	CString strlong2,strlat2;
	double long1 = m_curRealData.base_long;
	strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
	double lat1 = m_curRealData.base_lat;
	strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
	
	
	Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));

	fprintf(pf,"经纬度  :   %f \%f \n",p->lng,p->lat);
	fprintf(pf,"------------------------------------------------\n");

	if(m_curRealData.gsm1.srv_cell.arfcn!=257)
	{


		fprintf(pf,"网络    :   移动GSM \n");
		fprintf(pf,"LAC     :   %d \n",((m_curRealData.gsm1.lac[0]<<8)|(m_curRealData.gsm1.lac[1])));
		fprintf(pf,"CID     :   %d \n",m_curRealData.gsm1.cell_id);
		fprintf(pf,"当前小区:   %d  %d\n",m_curRealData.gsm1.srv_cell.arfcn,m_curRealData.gsm1.srv_cell.rssi);
		fprintf(pf,"邻区数  :   %d \n",m_curRealData.gsm1.nghbor_cell_num);
		for(i=0;i<m_curRealData.gsm1.nghbor_cell_num;i++)
		{
			fprintf(pf,"邻区%d     :   %d  %d\n",i+1,m_curRealData.gsm1.nghbor_cell[i].arfcn,m_curRealData.gsm1.nghbor_cell[i].rssi);
		}
		
		
		fprintf(pf,"------------------------------------------------\n");
	}



	if(m_curRealData.gsm2.srv_cell.arfcn!=257)
	{
		fprintf(pf,"网络    :   联通GSM \n");
		fprintf(pf,"LAC     :   %d \n",((m_curRealData.gsm2.lac[0]<<8)|(m_curRealData.gsm2.lac[1])));
		fprintf(pf,"CID     :   %d \n",m_curRealData.gsm2.cell_id);
		fprintf(pf,"当前小区:   %d  %d\n",m_curRealData.gsm2.srv_cell.arfcn,m_curRealData.gsm2.srv_cell.rssi);
		fprintf(pf,"邻区数  :   %d \n",m_curRealData.gsm2.nghbor_cell_num);
		for(i=0;i<m_curRealData.gsm2.nghbor_cell_num;i++)
		{
			fprintf(pf,"邻区%d   :   %d  %d\n",i+1,m_curRealData.gsm2.nghbor_cell[i].arfcn,m_curRealData.gsm2.nghbor_cell[i].rssi);
		}

		fprintf(pf,"------------------------------------------------\n");
	}


	if(m_curRealData.cdma.sid!=257)
	{	
		if(m_curRealData.cdma.pilot_num>6)
			m_curRealData.cdma.pilot_num=6;
		
		CString strNb="",strTmp;
		for(i=0;i<m_curRealData.cdma.pilot_num;i++)
		{
			strTmp.Format("%d ",m_curRealData.cdma.pilot_sets[i]);
			strNb+=strTmp;
			
		}

		fprintf(pf,"网络    :   电信CDMA2000 \n");
		fprintf(pf,"SID     :   %d \n",m_curRealData.cdma.sid);
		fprintf(pf,"NID     :   %d \n",m_curRealData.cdma.nid);
		fprintf(pf,"BSID    :   %d \n",m_curRealData.cdma.base_id);
		fprintf(pf,"频点    :   %d \n",m_curRealData.cdma.cdma_ch);
		fprintf(pf,"当前PN  :   %d \n",m_curRealData.cdma.cdma_pn);
		fprintf(pf,"邻近PN  :   %s \n",strNb);
		
		fprintf(pf,"------------------------------------------------\n");
	}



	if(m_curRealData.wcdma.srv_cell.UARFCN!=257)
	{

	
		fprintf(pf,"网络    :   联通WCDMA \n");
		fprintf(pf,"LAC     :   %d \n",((m_curRealData.wcdma.lac[0]<<8)|(m_curRealData.wcdma.lac[1])));
		fprintf(pf,"CID     :   %d \n",(m_curRealData.wcdma.cell_id%0x10000));  //prd->td.cid%0x10000
		fprintf(pf,"当前小区:   %d  %d  %d \n",m_curRealData.wcdma.srv_cell.UARFCN,m_curRealData.wcdma.srv_cell.PSC,m_curRealData.wcdma.srv_cell.RSSI);
		fprintf(pf,"邻区数  :   %d \n",m_curRealData.wcdma.neighbor_cell_num);
		for(i=0;i<m_curRealData.wcdma.neighbor_cell_num;i++)
		{
			fprintf(pf,"邻区%d   :   %d  %d  %d\n",i+1,m_curRealData.wcdma.neighbor_cell[i].UARFCN,m_curRealData.wcdma.neighbor_cell[i].PSC,m_curRealData.wcdma.neighbor_cell[i].RSSI);
		}
		
		fprintf(pf,"------------------------------------------------\n");
	}


	if(m_curRealData.td.lmi.tac!=257)
	{

	
		fprintf(pf,"网络    :   移动TD \n");
		fprintf(pf,"LAC     :   %d \n",m_curRealData.td.lmi.tac);
		fprintf(pf,"CID     :   %d \n",(m_curRealData.td.lmi.global_ci%0x10000));
		fprintf(pf,"频点    :   %d \n",m_curRealData.td.lmi.earfcn);
		if(m_curRealData.td.lmi.rscp-115<=-110)
			m_curRealData.td.lmi.rscp=115;
		fprintf(pf,"信号强度:   %d \n",m_curRealData.td.lmi.rscp-115);
		for(i=0;i<6;i++)
		{
			if(m_curRealData.td.lni[i].tac!=0 && m_curRealData.td.lni[i].tac!=257)
			{
				
				fprintf(pf,"邻区%d   :   %d  %d  %d\n",i+1,m_curRealData.td.lni[i].tac,m_curRealData.td.lni[i].earfcn,m_curRealData.td.lni[i].rscp);
			}
		}
		fprintf(pf,"------------------------------------------------\n");
	}
	if(m_curRealData.lte1.lmi.tac!=257)
	{

		fprintf(pf,"网络    :   移动LTE \n");
		fprintf(pf,"TAC     :   %d \n",m_curRealData.lte1.lmi.tac);
		fprintf(pf,"CID     :   %d(%x) \n",m_curRealData.lte1.lmi.global_ci,m_curRealData.lte1.lmi.global_ci);
		fprintf(pf,"频点    :   %d \n",m_curRealData.lte1.lmi.earfcn);
		if(m_curRealData.lte1.lmi.rscp-140<=-115)
			m_curRealData.lte1.lmi.rscp=140;
		fprintf(pf,"信号强度:   %d \n",m_curRealData.lte1.lmi.rscp-140);

		for(i=0;i<6;i++)
		{
			if(m_curRealData.lte1.lni[i].tac!=0 && m_curRealData.lte1.lni[i].tac!=257)
			{
				
				fprintf(pf,"邻区%d   :   %d  %d  %d\n",i+1,m_curRealData.lte1.lni[i].tac,m_curRealData.lte1.lni[i].earfcn,m_curRealData.lte1.lni[i].rscp);
			}
		}
		fprintf(pf,"------------------------------------------------\n");
	}
	if(m_curRealData.lte2.lmi.tac!=257)
	{
		fprintf(pf,"网络    :   联通LTE \n");
		fprintf(pf,"TAC     :   %d \n",m_curRealData.lte2.lmi.tac);
		fprintf(pf,"CID     :   %d(%x) \n",m_curRealData.lte2.lmi.global_ci,m_curRealData.lte2.lmi.global_ci);
		fprintf(pf,"频点    :   %d \n",m_curRealData.lte2.lmi.earfcn);
		if(m_curRealData.lte2.lmi.rscp-140<=-115)
			m_curRealData.lte2.lmi.rscp=140;
		fprintf(pf,"信号强度:   %d \n",m_curRealData.lte2.lmi.rscp-140);

		for(i=0;i<6;i++)
		{
			if(m_curRealData.lte2.lni[i].tac!=0 && m_curRealData.lte2.lni[i].tac!=257)
			{
				
				fprintf(pf,"邻区%d   :   %d  %d  %d\n",i+1,m_curRealData.lte2.lni[i].tac,m_curRealData.lte2.lni[i].earfcn,m_curRealData.lte2.lni[i].rscp);
			}
		}
		fprintf(pf,"------------------------------------------------\n");
	}

	if(m_curRealData.lte3.lmi.tac!=257)
	{

		fprintf(pf,"网络    :   电信LTE \n");
		fprintf(pf,"TAC     :   %d \n",m_curRealData.lte3.lmi.tac);
		fprintf(pf,"CID     :   %d(%x) \n",m_curRealData.lte3.lmi.global_ci,m_curRealData.lte3.lmi.global_ci);
		fprintf(pf,"频点    :   %d \n",m_curRealData.lte3.lmi.earfcn);
		if(m_curRealData.lte3.lmi.rscp-140<=-115)
			m_curRealData.lte3.lmi.rscp=140;
		fprintf(pf,"信号强度:   %d \n",m_curRealData.lte3.lmi.rscp-140);

		for(i=0;i<6;i++)
		{
			if(m_curRealData.lte3.lni[i].tac!=0 && m_curRealData.lte3.lni[i].tac!=257)
			{
				
				fprintf(pf,"邻区%d   :   %d  %d  %d\n",i+1,m_curRealData.lte3.lni[i].tac,m_curRealData.lte3.lni[i].earfcn,m_curRealData.lte3.lni[i].rscp);
			}
		}
	}

	
	fclose(pf);
	
	AfxMessageBox("保存完毕");
}


bool CCellLocatiomDlg::ExportToExcel(CString filename)
{
/*	CMyExcel my;
	int i=0;
	int j=0;
	my.Open();
	my.AddSheet("GSM");
	//m_listOpRst.GetHeaderText();
	my.SetColumnWidth(150);
	
	int columnCount=m_listOpRst.GetColumnCount();
	MyNumberFormat Xnumber;
	Xnumber.GetText();
	my.SetNumberFormat(Xnumber);
	for(i=1;i<=columnCount;i++)
	{
		my.SetItemText(1,i,m_listOpRst.GetHeaderText(i-1));
	}
	if(columnCount>6) //要加案件名和地址
	{
		my.SetItemText(1,i,"备注");
	}
	int countRows=	m_listOpRst.GetItemCount();
	for(i=0;i<countRows;i++)
	{
		for(j=0;j<columnCount;j++)
		{
			CString tmp=m_listOpRst.GetItemText(i,j);
			my.SetItemText(i+2,j+1,tmp);
		}
		my.SetItemText(i+2,j+1,m_strCaseRmk);
	}
	
	my.SaveAs(filename);*/
	return true;
}

void CCellLocatiomDlg::ShowMapData(double d1,double d2,double d3,double d4)
{
	QryData3(d1,d2,d3,d4);

}

void CCellLocatiomDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
m_nTabID=0;
//	m_webPage.CallJScript("ClearChangeMode");
	SetControlPostion();
}

void CCellLocatiomDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
//	m_webPage.CallJScript("ClearChangeMode");

	SetControlPostion1();
	m_dlgReal.SetControlPostion();
	m_nTabID=1;
}

void CCellLocatiomDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	m_nTabID=0;
//	m_webPage.CallJScript("ChangeMode");
	SetControlPostion2();

	m_dlgLoad.SetControlPostion();
//	m_dlgRst.SetControlPostion();
}


void CCellLocatiomDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
//	m_nTabID=0;

	SetControlPostion5();
	m_dlgPreview.SetControlPostion();
//	SetControlPostion4();
//	m_dlgBs.SetControlPostion();
//	pMainDlg->m_webPage.CallJScript("MakeCenter", "118.784223","32.041586" );
}

void CCellLocatiomDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	SetControlPostion3();

/*	if(m_bSerialPortOpened)
	{
		
		m_dlgDown.m_list.AddString("设备已连接(蓝牙)");
	}*/
}

void CCellLocatiomDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your_ control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
			m_nDelItem = pNMListView->iItem;
			
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//得到鼠标位置
			CMenu *menu;
			CMenu PopMenu;
			PopMenu.LoadMenu(IDR_MENU2);
			menu = PopMenu.GetSubMenu (0);
			
			menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
			//资源回收
			HMENU hmenu=menu->Detach();
			menu->DestroyMenu();
			delete lpoint;
		
	}
	*pResult = 0;
}

void CCellLocatiomDlg::OnMenuDel() 
{
	// TODO: Add your command handler code here
	m_list.DeleteItem(m_nDelItem);
}

void CCellLocatiomDlg::OnButton15() 
{


	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CCellLocatiomDlg::AddMark(CString strMk)
{

/*	CString strLng,strLat;
	int n = m_dlgRst.m_list.GetItemCount();


	if(n>0)
	{
		CString strlngTmp,strlatTmp;
		strlngTmp.Format("%.6f",m_dSelectedLng);
		strlatTmp.Format("%.6f",m_dSelectedLat);
		Point * p =  wgs_gcj_encrypts(m_dSelectedLat, m_dSelectedLng);
		
		CString strlong2,strlat2;
		
		strlong2.Format("%f", p->lng );
		strlat2.Format("%f", p->lat );

		m_webPage.CallJScript("ChangeMarkerInfo",strlong2,strlat2,strMk);

		for(int i=0;i<n;i++)
		{
			strLat = m_dlgRst.m_list.GetItemText(i,2);
			strLng = m_dlgRst.m_list.GetItemText(i,1);

		


			if((strLng==strlngTmp)  && (strLat==strlatTmp))
			{

				
				
				CTime tmSave = CTime::GetCurrentTime();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
					
				}
				else
				{
					AfxMessageBox("数据库连接失败!");
					return ;
				}

			
	
				if(m_pAdoRecordset->GetState())
					m_pAdoRecordset->Close();
				m_pAdoRecordset->SetCursorLocation();  //相当必要，否则会导致删除等操作时的莫名错误
							
				m_pAdoConn->BeginTrans();
	
	
				CString str1,str2,str3,str4,str5,str6="0",str7,str8,strSQL;
				CString str11,str12,str13;
				CString str21,str22,str23;
				CString str31,str32,str33;
				CString str41,str42,str43;
				CString str51,str52,str53;
				CString str61,str62,str63;
				CString str71,str72,str73;
				CString str81,str82,str83;
	
				m_pAdoRecordset->Open("mark", adCmdTable);
		
				str11= m_dlgRst.m_list.GetItemText(i,3);
				str12= m_dlgRst.m_list.GetItemText(i,4);
				str13= "0";

				str21= m_dlgRst.m_list.GetItemText(i,5);
				str22= m_dlgRst.m_list.GetItemText(i,6);
				str23= "0";

				str31= m_dlgRst.m_list.GetItemText(i,7);
				str32= "0";
				str33= m_dlgRst.m_list.GetItemText(i,8);

				str41= m_dlgRst.m_list.GetItemText(i,9);
				str42= m_dlgRst.m_list.GetItemText(i,10);
				str43="0";

				str51= m_dlgRst.m_list.GetItemText(i,11);
				str52= m_dlgRst.m_list.GetItemText(i,12);
				str53="0";

				str61= m_dlgRst.m_list.GetItemText(i,13);
				str62= m_dlgRst.m_list.GetItemText(i,14);
				str63="0";

				str71= m_dlgRst.m_list.GetItemText(i,15);
				str72= m_dlgRst.m_list.GetItemText(i,16);
				str73="0";

				str81= m_dlgRst.m_list.GetItemText(i,17);
				str82= m_dlgRst.m_list.GetItemText(i,18);
				str83="0";


				str4 = m_dlgRst.m_list.GetItemText(i,1);
				str5 = m_dlgRst.m_list.GetItemText(i,2);
				str7 = m_dlgRst.m_list.GetItemText(i,0);
				

				int y = 2000+atoi(str7.Left(2));
				int m = atoi(str7.Mid(3,2));
				int d = atoi(str7.Mid(6,2));
				int H = atoi(str7.Mid(9,2));
				int M = atoi(str7.Mid(12,2));
				int S = atoi(str7.Mid(15,2));
				CTime tmm(y,m,d,H,M,S);
				
				str7.Format("%d",tmm.GetTime());
				str8.Format("%d",tmSave.GetTime());	

				double dlng = ((int)atof(str4))*100 + 60*(atof(str4)-(int)(atof(str4)));
				double dlat = ((int)atof(str5))*100 + 60*(atof(str5)-(int)(atof(str5)));
				
				str4.Format("%f",dlng);
				str5.Format("%f",dlat);


			//	AfxMessageBox(str12+" "+str22+" "+str32+" "+str42+" "+str52+" "+str62+" "+str72+" "+str82);
				
				m_pAdoRecordset->AddNew();
				m_pAdoRecordset->GetAbsolutePosition();
				if (!m_pAdoRecordset->PutCollect("type", str6)
					|| !m_pAdoRecordset->PutCollect("lac1", str11)
					|| !m_pAdoRecordset->PutCollect("cid1", str12)
					|| !m_pAdoRecordset->PutCollect("bsid1", str13)
					|| !m_pAdoRecordset->PutCollect("lac2", str21)
					|| !m_pAdoRecordset->PutCollect("cid2", str22)
					|| !m_pAdoRecordset->PutCollect("bsid2", str23)
					|| !m_pAdoRecordset->PutCollect("lac3", str31)
					|| !m_pAdoRecordset->PutCollect("cid3", str32)
					|| !m_pAdoRecordset->PutCollect("bsid3", str33)
					|| !m_pAdoRecordset->PutCollect("lac4", str41)
					|| !m_pAdoRecordset->PutCollect("cid4", str42)
					|| !m_pAdoRecordset->PutCollect("bsid4", str43)
					|| !m_pAdoRecordset->PutCollect("lac5", str51)
					|| !m_pAdoRecordset->PutCollect("cid5", str52)
					|| !m_pAdoRecordset->PutCollect("bsid5", str53)
					|| !m_pAdoRecordset->PutCollect("lac6", str61)
					|| !m_pAdoRecordset->PutCollect("cid6", str62)
					|| !m_pAdoRecordset->PutCollect("bsid6", str63)
					|| !m_pAdoRecordset->PutCollect("lac7", str71)
					|| !m_pAdoRecordset->PutCollect("cid7", str72)
					|| !m_pAdoRecordset->PutCollect("bsid7", str73)
					|| !m_pAdoRecordset->PutCollect("lac8", str81)
					|| !m_pAdoRecordset->PutCollect("cid8", str82)
					|| !m_pAdoRecordset->PutCollect("bsid8", str83)
					|| !m_pAdoRecordset->PutCollect("base_long", str4)
					|| !m_pAdoRecordset->PutCollect("base_lat", str5)
					|| !m_pAdoRecordset->PutCollect("time", str7)
					|| !m_pAdoRecordset->PutCollect("mark", strMk)
					|| !m_pAdoRecordset->PutCollect("savetime", str8)
					)
				{
					AfxMessageBox("数据库添加失败1!");
					m_pAdoRecordset->Close();
					m_pAdoConn->Close();
					delete m_pAdoRecordset;
					delete m_pAdoConn;
					
					return ;
				}
				if (!m_pAdoRecordset->Update())
				{
					
					AfxMessageBox("数据库更新失败");
					m_pAdoRecordset->Close();
					m_pAdoConn->Close();
					delete m_pAdoRecordset;
					delete m_pAdoConn;
					
					return ;
				}
				m_pAdoRecordset->Close();
				m_pAdoConn->CommitTrans();
				m_pAdoConn->Close();
				delete m_pAdoRecordset;
				delete m_pAdoConn;


				AfxMessageBox("保存完毕");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		}
	}*/
}


void CCellLocatiomDlg::OnDestroy() 
{

	KillProcess();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CCellLocatiomDlg::LoadLocList()
{
//	AfxMessageBox("LoadLocList");

	m_cbList.ResetContent();
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\list.ini";
	
	char arfcn[2048];
	CString strcount,strTmp;
	GetPrivateProfileString("default","count",NULL,strcount.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	int ncount = atoi(strcount);

	GetPrivateProfileString("default","list",NULL,strTmp.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	strcpy(arfcn,strTmp);
	
	char *p = strtok(arfcn,"/");
	int pos=0;
	while(p!=NULL)
	{
		m_cbList.AddString((CString)p);
		
		p = strtok(NULL,"/");
	}


	int n= m_cbList.GetCount();

	CString strNum;
	strNum.Format("%d",n);
//	AfxMessageBox(strNum);
	if(n>0)
	{
		CString strTmp,strText,strSel,strTmp2,str0;
			int p;

		GetPrivateProfileString("default","db",NULL,strTmp2.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);

		/**江苏		jiangsu
		山东		shandong
		山西		shanxi
		陕西		shanx
		北京		beijing
		上海		shanghai
		河南		henan
		河北		hebei
		湖南		hunan
		湖北		hubei
		安徽		anhui
		浙江		zhejiang
		江西		jiangxi
		福建		fujian
		广东		guangdong
		广西		guangxi
		黑龙江		heilongjiang
		吉林		jilin
		辽宁		liaoning
		内蒙古		neimeng
		甘肃		gansu
		宁夏		ningxia
		青海		qinghai
		新疆		xinjiang
		四川		sichuan
		重庆		chongqing
		云南		yunnan
		贵州		guizhou
西藏		xizang*/

		
		if(strTmp2 == "jiangsu.db")
		{

			strSel = "江苏";
			pMainDlg->m_webPage.CallJScript("MakeCenter", "118.784223","32.041586" );

		}
		else if (strTmp2 == "gangzhuao.db")
		{
			strSel = "港珠澳";
			pMainDlg->m_webPage.CallJScript("MakeCenter", "114.17421","22.31689" );
			
		}
		else if (strTmp2 == "gangzhuao_s.db")
		{
			strSel = "港珠澳卫星图";
			pMainDlg->m_webPage.CallJScript("MakeCenter", "114.17421","22.31689" );
			
		}
		else if (strTmp2 == "shandong.db")
		{
			
			strSel = "山东";
			pMainDlg->m_webPage.CallJScript("MakeCenter", "117.03435","36.66601" );
		}
		else if (strTmp2 == "shanxi.db")
		{
			
			strSel = "山西";
			pMainDlg->m_webPage.CallJScript("setMapCenter", "112.55201","37.84870" );
		}
		else if (strTmp2 == "shanx.db")
		{
			
			strSel = "陕西";
			pMainDlg->m_webPage.CallJScript("setMapCenter", "108.94592","34.26970" );
		}
		else if (strTmp2 == "beijing.db")
		{
		
			strSel = "北京";
			pMainDlg->m_webPage.CallJScript("setMapCenter", "116.39671","39.90789" );
		}
		else if (strTmp2 == "shanghai.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "121.53488","31.21354" );
			strSel = "上海";
		}
		else if (strTmp2 == "henan.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "113.66618","34.75276" );
			strSel = "河南";
		}
		else if (strTmp2 == "hebei.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "114.49196","38.04431" );
			strSel = "河北";
		}
		else if (strTmp2 == "hunan.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "113.03215","28.18567" );
			strSel = "湖南";
		}
		else if (strTmp2 == "hubei.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "114.36047","30.56758" );
			strSel = "湖北";
		}
		else if (strTmp2 == "anhui.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "117.27236","31.84738" );
			strSel = "安徽";
		}
		else if (strTmp2 == "zhejiang.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "120.15644","30.25996" );
			strSel = "浙江";
		}
		else if (strTmp2 == "jiangxi.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "115.96207","28.68268" );
			strSel = "江西";
		}
		else if (strTmp2 == "fujian.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "119.30586","26.06388" );
			strSel = "福建";
		}
		else if (strTmp2 == "guangdong.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "113.32466","23.13799" );
			strSel = "广东";
		}
		else if (strTmp2 == "guangxi.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "108.36708","22.81638" );
			strSel = "广西";
		}
		else if (strTmp2 == "heilongjiang.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "126.62481","45.76740" );
			strSel = "黑龙江";
		}
		else if (strTmp2 == "jilin.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "125.30972","43.87810" );
			strSel = "吉林";
		}
		else if (strTmp2 == "liaoning.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "123.40410","41.80856" );
			strSel = "辽宁";
		}
		else if (strTmp2 == "neimeng.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "111.67740","40.82381" );
			strSel = "内蒙古";
		}
		else if (strTmp2 == "gansu.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "103.82423","36.05798" );
			strSel = "甘肃";
		}
		else if (strTmp2 == "ningxia.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "106.26732","38.47590" );
			strSel = "宁夏";
		}
		else if (strTmp2 == "qinghai.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "101.76189","36.63785" );
			strSel = "青海";
		}
		else if (strTmp2 == "sichuan.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "104.07366","30.66287" );
			strSel = "四川";
		}
		else if (strTmp2 == "xinjiang.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "87.60429","43.79985" );
			strSel = "新疆";
		}
		else if (strTmp2 == "chongqing.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "106.54713","29.54977" );
			strSel = "重庆";
		}
		else if (strTmp2 == "yunnan.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "102.66449","25.03864" );
			strSel = "云南";
		}
		else if (strTmp2 == "guizhou.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "106.71398","26.57778" );
			strSel = "贵州";
		}
		else if (strTmp2 == "xizang.db")
		{
			pMainDlg->m_webPage.CallJScript("setMapCenter", "91.11923","29.65449" );
			strSel = "西藏";
		}
		
		for(int i=0;i<n;i++)
		{

			p = m_cbList.GetLBTextLen(i);
			m_cbList.GetLBText(i, strTmp.GetBuffer(p) );
			strTmp.ReleaseBuffer();
			
			strText.Format("%s", strTmp.GetBuffer(0));

			//AfxMessageBox(strSel+" " +strText);
			if(i==0)
				str0 = strText;
			if(strText == strSel)
			{
				m_cbList.SetCurSel(i);
				

				return;

			}
	

		}

		m_cbList.SetCurSel(0);


/*		江苏		jiangsu
			山东		shandong
			山西		shanxi
			陕西		shanx
			北京		beijing
			上海		shanghai
			河南		henan
			河北		hebei
			湖南		hunan
			湖北		hubei
			安徽		anhui
			浙江		zhejiang
			江西		jiangxi
			福建		fujian
			广东		guangdong
			广西		guangxi
			黑龙江		heilongjiang
			吉林		jilin
			辽宁		liaoning
			内蒙古		neimeng
			甘肃		gansu
			宁夏		ningxia
			青海		qinghai
			新疆		xinjiang
			四川		sichuan
			重庆		chongqing
			云南		yunnan
			贵州		guizhou
西藏		xizang*/
		if(str0 == "江苏")
		{
			WritePrivateProfileString("default","db","jiangsu.db",strIniPath);
		}
		else if(str0 == "港珠澳")
		{
			WritePrivateProfileString("default","db","gangzhuao.db",strIniPath);
		}
		else if(str0 == "港珠澳卫星图")
		{
			WritePrivateProfileString("default","db","gangzhuao_s.db",strIniPath);
		}
		else if(str0 == "山东")
		{
			WritePrivateProfileString("default","db","shandong.db",strIniPath);
		}
		
		else if(str0 == "河南")
		{
			WritePrivateProfileString("default","db","henan.db",strIniPath);
		}
		else if(str0 == "北京")
		{
			WritePrivateProfileString("default","db","beijing.db",strIniPath);
		}
		else if(str0 == "河北")
		{
			WritePrivateProfileString("default","db","hebei.db",strIniPath);
		}
		else if(str0 == "浙江")
		{
			WritePrivateProfileString("default","db","zhejiang.db",strIniPath);
		}
		else if(str0 == "陕西")
		{
			WritePrivateProfileString("default","db","shanx.db",strIniPath);
		}
		else if(str0 == "山西")
		{
			WritePrivateProfileString("default","db","shanxi.db",strIniPath);
		}
		else if(str0 == "上海")
		{
			WritePrivateProfileString("default","db","shanghai.db",strIniPath);
		}
		else if(str0 == "湖南")
		{
			WritePrivateProfileString("default","db","hunan.db",strIniPath);
		}
		else if(str0 == "湖北")
		{
			WritePrivateProfileString("default","db","hunbei.db",strIniPath);
		}
		else if(str0 == "安徽")
		{
			WritePrivateProfileString("default","db","anhui.db",strIniPath);
		}
		else if(str0 == "江西")
		{
			WritePrivateProfileString("default","db","jiangxi.db",strIniPath);
		}
		else if(str0 == "福建")
		{
			WritePrivateProfileString("default","db","fujian.db",strIniPath);
		}
		else if(str0 == "广东")
		{
			WritePrivateProfileString("default","db","guangdong.db",strIniPath);
		}
		else if(str0 == "广西")
		{
			WritePrivateProfileString("default","db","guangxi.db",strIniPath);
		}
		else if(str0 == "黑龙江")
		{
			WritePrivateProfileString("default","db","heilongjiang.db",strIniPath);
		}
		else if(str0 == "吉林")
		{
			WritePrivateProfileString("default","db","jilin.db",strIniPath);
		}
		else if(str0 == "辽宁")
		{
			WritePrivateProfileString("default","db","liaoning.db",strIniPath);
		}
		else if(str0 == "内蒙古")
		{
			WritePrivateProfileString("default","db","neimeng.db",strIniPath);
		}
		else if(str0 == "甘肃")
		{
			WritePrivateProfileString("default","db","gansu.db",strIniPath);
		}
		else if(str0 == "宁夏")
		{
			WritePrivateProfileString("default","db","ningxia.db",strIniPath);
		}
		else if(str0 == "青海")
		{
			WritePrivateProfileString("default","db","qinghai.db",strIniPath);
		}
		else if(str0 == "新疆")
		{
			WritePrivateProfileString("default","db","xinjiang.db",strIniPath);
		}
		else if(str0 == "四川")
		{
			WritePrivateProfileString("default","db","sichuan.db",strIniPath);
		}
		else if(str0 == "云南")
		{
			WritePrivateProfileString("default","db","yunnan.db",strIniPath);
		}
		else if(str0 == "重庆")
		{
			WritePrivateProfileString("default","db","chongqing.db",strIniPath);
		}
		else if(str0 == "贵州")
		{
			WritePrivateProfileString("default","db","guizhou.db",strIniPath);
		}

	}

	
}


void CCellLocatiomDlg::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strTmp,strText;
	int n = m_cbList.GetLBTextLen(m_cbList.GetCurSel());
	m_cbList.GetLBText( m_cbList.GetCurSel(), strTmp.GetBuffer(n) );
	strTmp.ReleaseBuffer();
	
    strText.Format("%s", strTmp.GetBuffer(0));
	
	

	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\list.ini";


	if(strText == "江苏")
	{
		pMainDlg->m_webPage.CallJScript("MakeCenter", "118.784223","32.041586" );
		WritePrivateProfileString("default","db","jiangsu.db",strIniPath);
	}
	if(strText == "港珠澳")
	{
		pMainDlg->m_webPage.CallJScript("MakeCenter", "114.17421","22.31689" );
		WritePrivateProfileString("default","db","gangzhuao.db",strIniPath);
	}
	if(strText == "港珠澳卫星图")
	{
		pMainDlg->m_webPage.CallJScript("MakeCenter", "114.17421","22.31689" );
		WritePrivateProfileString("default","db","gangzhuao_s.db",strIniPath);
	}
	else if(strText == "山东")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "117.03435","36.66601" );
		WritePrivateProfileString("default","db","shandong.db",strIniPath);
	}
	else if(strText == "河南")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "113.66618","34.75276" );
		WritePrivateProfileString("default","db","henan.db",strIniPath);
	}
	else if(strText == "北京")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "116.39671","39.90789" );
		WritePrivateProfileString("default","db","beijing.db",strIniPath);
	}
	else if(strText == "河北")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "114.49196","38.04431" );
		WritePrivateProfileString("default","db","hebei.db",strIniPath);
	}
	else if(strText == "浙江")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "120.15644","30.25996" );
		WritePrivateProfileString("default","db","zhejiang.db",strIniPath);
	}
	else if(strText == "陕西")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "108.94592","34.26970" );
		WritePrivateProfileString("default","db","shanx.db",strIniPath);
	}
	else if(strText == "山西")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "112.55201","37.84870" );
		WritePrivateProfileString("default","db","shanxi.db",strIniPath);
	}
	else if(strText == "上海")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "121.53488","31.21354" );
		WritePrivateProfileString("default","db","shanghai.db",strIniPath);
	}
	else if(strText == "湖南")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "113.03215","28.18567" );
		WritePrivateProfileString("default","db","hunan.db",strIniPath);
	}
	else if(strText == "湖北")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "114.36047","30.56758" );
		WritePrivateProfileString("default","db","hunbei.db",strIniPath);
	}
	else if(strText == "安徽")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "117.27236","31.84738" );
		WritePrivateProfileString("default","db","anhui.db",strIniPath);
	}
	else if(strText == "江西")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "115.96207","28.68268" );
		WritePrivateProfileString("default","db","jiangxi.db",strIniPath);
	}
	else if(strText == "福建")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "119.30586","26.06388" );
		WritePrivateProfileString("default","db","fujian.db",strIniPath);
	}
	else if(strText == "广东")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "113.32466","23.13799" );
		WritePrivateProfileString("default","db","guangdong.db",strIniPath);
	}
	else if(strText == "广西")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "108.36708","22.81638" );
		WritePrivateProfileString("default","db","guangxi.db",strIniPath);
	}
	else if(strText == "黑龙江")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "126.62481","45.76740" );
		WritePrivateProfileString("default","db","heilongjiang.db",strIniPath);
	}
	else if(strText == "吉林")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "125.30972","43.87810" );
		WritePrivateProfileString("default","db","jilin.db",strIniPath);
	}
	else if(strText == "辽宁")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "123.40410","41.80856" );
		WritePrivateProfileString("default","db","liaoning.db",strIniPath);
	}
	else if(strText == "内蒙古")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "111.67740","40.82381" );
		WritePrivateProfileString("default","db","neimeng.db",strIniPath);
	}
	else if(strText == "甘肃")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "103.82423","36.05798" );
		WritePrivateProfileString("default","db","gansu.db",strIniPath);
	}
	else if(strText == "宁夏")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "106.26732","38.47590" );
		WritePrivateProfileString("default","db","ningxia.db",strIniPath);
	}
	else if(strText == "青海")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "101.76189","36.63785" );
		WritePrivateProfileString("default","db","qinghai.db",strIniPath);
	}
	else if(strText == "新疆")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "87.60429","43.79985" );
		WritePrivateProfileString("default","db","xinjiang.db",strIniPath);
	}
	else if(strText == "四川")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "104.07366","30.66287" );
		WritePrivateProfileString("default","db","sichuan.db",strIniPath);
	}
	else if(strText == "云南")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "102.66449","25.03864" );
		WritePrivateProfileString("default","db","yunnan.db",strIniPath);
	}
	else if(strText == "重庆")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "106.54713","29.54977" );
		WritePrivateProfileString("default","db","chongqing.db",strIniPath);
	}
	else if(strText == "贵州")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "106.71398","26.57778" );
		WritePrivateProfileString("default","db","guizhou.db",strIniPath);
		}
	else if(strText == "西藏")
	{
		pMainDlg->m_webPage.CallJScript("setMapCenter", "91.11923","29.65449" );
		WritePrivateProfileString("default","db","xizang.db",strIniPath);
	}


}

void CCellLocatiomDlg::OnButtonTest() 
{
//	CString strTmp,strText;
//	int n = m_cbList.GetLBTextLen(m_cbList.GetCurSel());
//	m_cbList.GetLBText( m_cbList.GetCurSel(), strTmp.GetBuffer(n) );
//	strTmp.ReleaseBuffer();
	
 //   strText.Format("%s", strTmp.GetBuffer(0));
	
	

//	if(strText == "江苏")
//	{
	//	pMainDlg->m_webPage.CallJScript("setMapCenter", "118.784223","32.041586" );
	//	pMainDlg->m_webPage.CallJScript("setMapCenter", m_strLng,m_strLat );
//		pMainDlg->m_webPage.CallJScript("addMarker","118.784223","32.041586"  );
		//WritePrivateProfileString("default","db","jiangsu.db",strIniPath);
//	}

}



void CCellLocatiomDlg::DispRecvedMsg(char*buf)
{
	
	CString strTest;
	pkg_header_t* pMsgHead = (pkg_header_t*)buf;
	CTime time=CTime::GetCurrentTime();	
	CString strTime = time.Format("%H:%M:%S");
	CTime time2=CTime::GetCurrentTime();
	CTimeSpan myTs;
	char msg_body[256];
	uint16 msg_type;
	double lat,lng;
	CString strTID;
	lc_data_t*plc;
	//
	switch(pMsgHead->msgtype)
	{
		
	case 2000: //上线
		{

		//Log("pMsgHead->msgtype = %d",pMsgHead->msgtype);
			strTID.Format("%d",pMsgHead->targetid);
		

			int n =m_dlgReal.m_listModule.GetItemCount();
			if(n>0)
			{
				for(int i=0;i<n;i++)
				{

					if(m_dlgReal.m_listModule.GetItemText(i,0)==strTID)
						return;
				}

				m_dlgReal.m_listModule.InsertItem(n,strTID);

				
			}
			else
				m_dlgReal.m_listModule.InsertItem(0,strTID);
			//m_btn2.EnableWindow(TRUE);
//			m_btn2.SetIcon(IDI_ICON2);
		}

		break;
		
	case 2001: //XIA线
		{

	
		strTID.Format("%d",pMsgHead->targetid);
			
			int n =m_dlgReal.m_listModule.GetItemCount();
			if(n>0)
			{
				for(int i=n-1;i>=0;i--)
				{
					
					if(m_dlgReal.m_listModule.GetItemText(i,0)==strTID)
					{
						if(i== m_dlgReal.m_nLockItem)
						{
							m_dlgReal.m_nLockItem=-1;
							m_dlgReal.m_nLockSN = 0 ;
						}
						m_dlgReal.m_listModule.DeleteItem(i);
					}
				}
				
				
			}	
		}
		break;
		
	case 3001: //来自服务器心跳
		{
		//	Log("HeartBeat from %d",pMsgHead->sourceid);
			
			
		}
		break;
	case 4001: //中转数据
		{
			
		//	Log("recv trans_data from %d, to %d",pMsgHead->sourceid,pMsgHead->targetid);
			
			
			if(pMsgHead->targetid==9000)
			{
				
				if(pMsgHead->msglen == 18)
				{
					if(pMsgHead->sourceid==m_dlgReal.m_nLockSN)
					{

					
						memcpy(&lat,buf+sizeof(pkg_header_t)+2,8);
						memcpy(&lng,buf+sizeof(pkg_header_t)+10,8);
						Log("lat = %f, lng = %f",lat,lng);

						double a = lat;
						double b = lng;
					//	CString strLat="",strLng="";
						m_strLat.Format("%f",a);
						m_strLng.Format("%f",b);
					}
				//	if(test==0)
				//	pMainDlg->m_webPage.CallJScript("setMapCenter", m_strLng,m_strLat );
				//	test = 1;
				//	m_webPage.CallJScript("SetMapCenter",st1 , st2);
					//GetNumber(msg_body,pMsgHead->msglen,pMsgHead->sourceid);
				}
				else if(pMsgHead->msglen==55)
				{

					plc = (lc_data_t*)(buf+sizeof(pkg_header_t)+2);
					CString strMode="";
					CString strBlank="";
					switch(plc->mode)
					{
					case 1:
						strMode="移动2G";
						break;
					case 2:
						strMode="联通2G";
						break;
					case 3:
						strMode="电信2G";
						break;
					case 4:
						strMode="联通3G";
						break;
					case 5:
						strMode="移动4G";
						break;
					case 6:
						strMode="联通4G";
						break;
					case 7:
						strMode="电信4G";
						break;

						
					}

					int n = pMainDlg->m_dlgReal.m_listData.GetItemCount();
					if(n<10)
					{
						pMainDlg->m_dlgReal.m_listData.InsertItem(n,strMode);
						pMainDlg->m_dlgReal.m_listData.SetItemText(n,1,(CString)(plc->lac));
						pMainDlg->m_dlgReal.m_listData.SetItemText(n,2,(CString)(plc->cid));
						pMainDlg->m_dlgReal.m_listData.SetItemText(n,3,(CString)(plc->arfcn));
						pMainDlg->m_dlgReal.m_listData.SetItemText(n,4,(CString)(plc->pci));
						pMainDlg->m_dlgReal.m_listData.SetItemText(n,5,(CString)(plc->rssi));
					}
					else
					{
						pMainDlg->m_dlgReal.m_listData.DeleteItem(0);
						pMainDlg->m_dlgReal.m_listData.InsertItem(9,strMode);
						pMainDlg->m_dlgReal.m_listData.SetItemText(9,1,(CString)(plc->lac));
						pMainDlg->m_dlgReal.m_listData.SetItemText(9,2,(CString)(plc->cid));
						pMainDlg->m_dlgReal.m_listData.SetItemText(9,3,(CString)(plc->arfcn));
						pMainDlg->m_dlgReal.m_listData.SetItemText(9,4,(CString)(plc->pci));
						pMainDlg->m_dlgReal.m_listData.SetItemText(9,5,(CString)(plc->rssi));

					}
				}
			}
			
		}
		break;
		
	}
	
	
	
}

void CCellLocatiomDlg::SendLogOn()
{
	
/*msgtype = 1000
msglen = 0
transid = xxxxxx
sourceid = xxxx
targetid = 0
	*/
	
	pkg_header_t pht;
	pht.msgtype = 1000;
	pht.msglen=0;
	pht.transid = 10000;
	pht.sourceid=9000;
	pht.targetid = 0;
	
	char buf[128];
	
	memcpy(buf, &pht, sizeof(pkg_header_t));
	
	SendData(buf,sizeof(pkg_header_t));
}

BOOL CCellLocatiomDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->hwnd == m_cbList.m_hWnd && pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
