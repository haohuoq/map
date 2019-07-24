// DlgDown.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgDown.h"
#include "DlgLoad.h"
#include "DlgBs.h"
#include "CellLocatiomDlg.h"
#include "myexcel.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDown dialog
extern CCellLocatiomDlg * pMainDlg;
DlgDown*pDlgDown;
extern CDlgLoad * pLoadDlg;
extern CDlgBs*pBSDlg;
#define DVC_NUM		10
BOOL g_bUDPThreadEnd = FALSE;


SOCKET server;
SOCKET_LIST  sock[DVC_NUM];  //默认10个设备
BOOL bSocketTerminated = FALSE;
int sockdvc1=-1;
#define MAX_DVC_COUNT 3

BYTE default_mnc=0;

int sockbk;




//DWORD  WINAPI  SaveToDBThread(void *p) 
int  SaveToDBThread(CString strMark)
{

	return 1;
}	



int  SaveToDBThread2(CString strFile,CString strMark)
{
	
	FILE*fp;
	CString temp_file1;
	int sss=0;
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	int id; 


	CTime tmm = CTime::GetCurrentTime();
	CString strTime = tmm.Format("%y-%m-%d %H_%M_%S");
	temp_file1 = strAppPath+"\\temp\\"+strTime+".db";
	strDBPath += "\\database.mdb";
		
	int total_count = 0;
	


	strTime.Replace("_",":");
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	

	//if(m_pAdoConn->IsOpen())
	//AfxMessageBox(strDBPath);
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		
	}
	else
	{
		int n = GetLastError();
		CString see;
		see.Format("%d",n);

		
		AfxMessageBox("数据库连接失败!");//""

	
		
		return 0;
	}

			
	
	if(m_pAdoRecordset->GetState())
		m_pAdoRecordset->Close();
	m_pAdoRecordset->SetCursorLocation();  //相当必要，否则会导致删除等操作时的莫名错误
				
	m_pAdoConn->BeginTrans();
	
	total_count=0;
	CString str1,str2,str3,str4,str5,str6,str7,str8,str9,strSQL;

	CString str[50];
//	table_name = "GSM1";
	

		int nfilesize;    //文件大小
		int nNumCnt = 0;						
		int i = 0;
		int count = 0;

		
		m_pAdoRecordset->Open("gather", adCmdTable);

		int cnt = 0;
		int nCounts = pMainDlg->m_list.GetItemCount();

		//FILE *pfsave  =  fopen(temp_file1,"wb");
		for(cnt=0;cnt<nCounts;cnt++)
		{
			for(int a = 0 ;a<49;a++)
				str[a] = pMainDlg->m_list.GetItemText(cnt,a);
			


			
				m_pAdoRecordset->AddNew();
				m_pAdoRecordset->GetAbsolutePosition();
				if (!m_pAdoRecordset->PutCollect("time", str[0])
					|| !m_pAdoRecordset->PutCollect("lat", str[1])
					|| !m_pAdoRecordset->PutCollect("lng", str[2])
					|| !m_pAdoRecordset->PutCollect("plmn", str[6])
					|| !m_pAdoRecordset->PutCollect("mode", str[7])
					|| !m_pAdoRecordset->PutCollect("lac", str[8])
					|| !m_pAdoRecordset->PutCollect("cid", str[9])
					|| !m_pAdoRecordset->PutCollect("arfcn", str[10])
						|| !m_pAdoRecordset->PutCollect("band", str[11])
					|| !m_pAdoRecordset->PutCollect("pci", str[12])
					|| !m_pAdoRecordset->PutCollect("rssi", str[13])
					|| !m_pAdoRecordset->PutCollect("lac1", str[14])
					|| !m_pAdoRecordset->PutCollect("cid1", str[15])
					|| !m_pAdoRecordset->PutCollect("arfcn1", str[16])
					|| !m_pAdoRecordset->PutCollect("band1", str[17])
					|| !m_pAdoRecordset->PutCollect("pci1", str[18])
					|| !m_pAdoRecordset->PutCollect("rssi1", str[19])
					|| !m_pAdoRecordset->PutCollect("lac2", str[20])
					|| !m_pAdoRecordset->PutCollect("cid2", str[21])
					|| !m_pAdoRecordset->PutCollect("arfcn2", str[22])
					|| !m_pAdoRecordset->PutCollect("band2", str[23])
					|| !m_pAdoRecordset->PutCollect("pci2", str[24])
					|| !m_pAdoRecordset->PutCollect("rssi2", str[25])
					|| !m_pAdoRecordset->PutCollect("lac3", str[26])
					|| !m_pAdoRecordset->PutCollect("cid3", str[27])
					|| !m_pAdoRecordset->PutCollect("arfcn3", str[28])
					|| !m_pAdoRecordset->PutCollect("band3", str[29])
					|| !m_pAdoRecordset->PutCollect("pci3", str[30])
					|| !m_pAdoRecordset->PutCollect("rssi3", str[31])
					|| !m_pAdoRecordset->PutCollect("lac4", str[32])
					|| !m_pAdoRecordset->PutCollect("cid4", str[33])
					|| !m_pAdoRecordset->PutCollect("arfcn4", str[34])
					|| !m_pAdoRecordset->PutCollect("band4", str[35])
					|| !m_pAdoRecordset->PutCollect("pci4", str[36])
					|| !m_pAdoRecordset->PutCollect("rssi4", str[37])
					|| !m_pAdoRecordset->PutCollect("lac5", str[38])
					|| !m_pAdoRecordset->PutCollect("cid5", str[39])
					|| !m_pAdoRecordset->PutCollect("arfcn5", str[40])
					|| !m_pAdoRecordset->PutCollect("band5", str[41])
					|| !m_pAdoRecordset->PutCollect("pci5", str[42])
					|| !m_pAdoRecordset->PutCollect("rssi5", str[43])
					|| !m_pAdoRecordset->PutCollect("lac6", str[44])
					|| !m_pAdoRecordset->PutCollect("cid6", str[45])
					|| !m_pAdoRecordset->PutCollect("arfcn6", str[46])
					|| !m_pAdoRecordset->PutCollect("band6", str[47])
					|| !m_pAdoRecordset->PutCollect("pci6", str[48])
					|| !m_pAdoRecordset->PutCollect("rssi6", str[49])
					|| !m_pAdoRecordset->PutCollect("lat2", str[3])
					|| !m_pAdoRecordset->PutCollect("lng2", str[4])
					|| !m_pAdoRecordset->PutCollect("type", str[5])
					|| !m_pAdoRecordset->PutCollect("savetime", strFile)
			
					)
				{
					AfxMessageBox("数据库添加失败1!");
					m_pAdoRecordset->Close();
					m_pAdoConn->Close();
					delete m_pAdoRecordset;
					delete m_pAdoConn;
					
					return 0;
				}


				
			
				total_count++;
				
/*				TSaveData sd;
				sd.base_lat = atof(str8);
				sd.base_long = atof(str9);
				sd.time1 = (long)(tmm.GetTime());
				sd.lac = atoi(str3);
				sd.cid = atoi(str4);
				sd.arfcn = atoi(str5);
				sd.PCI = atoi(str6);
				sd.rssi = atoi(str7);
				strcpy(sd.type,str1);
				
				fwrite(&sd,1,sizeof(TSaveData),pfsave);
*/		
		}
	
//		fclose(pfsave);
		if (!m_pAdoRecordset->Update())
		{
			
			AfxMessageBox("数据库更新失败");
			m_pAdoRecordset->Close();
			m_pAdoConn->Close();
			delete m_pAdoRecordset;
			delete m_pAdoConn;
			
			return 0;
		}
		



	pDlgDown->m_list.AddString("数据入库完成");
	CString strTotalCount;
	strTotalCount.Format("%d",total_count);
//	CTime timeNow=CTime::GetCurrentTime();	
//	CString strTime = timeNow.Format("%y-%m-%d %H:%M:%S");

	
	m_pAdoRecordset->Close();
	strSQL = "insert into record values ('"+strFile+"','"+strTotalCount+"','"+strMark+"')";
	m_pAdoRecordset->Open(strSQL);


	m_pAdoRecordset->Close();
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;

	pLoadDlg->LoadDB();
	
	pMainDlg->m_list.DeleteAllItems();
	return 1;
}	

UINT     WorkSocketThread(LPVOID   pParam) 
{ 

	return   0; 
}


DWORD  WINAPI TCPThread(void*arg)
{

	return 1;
	
}


DlgDown::DlgDown(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDown::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDown)
	m_nSel = 0;
	m_bChk = TRUE;
	m_strName2 = _T("");
	pfTmp=NULL;	// NOTE: the ClassWizard will add member initialization here
	m_strName1 = _T("");
	m_strName3 = _T("");
	m_strFile="";
	//}}AFX_DATA_INIT
}


void DlgDown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDown)
	DDX_Control(pDX, IDC_BUTTON3, m_btnTime);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDown);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Radio(pDX, IDC_RADIO1, m_nSel);
	DDX_Check(pDX, IDC_CHECK1, m_bChk);
	DDX_Text(pDX, IDC_EDIT2, m_strName2);
	DDX_Text(pDX, IDC_EDIT1, m_strName1);
	DDX_Text(pDX, IDC_EDIT6, m_strName3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDown, CDialog)
	//{{AFX_MSG_MAP(DlgDown)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDown message handlers

BOOL DlgDown::OnInitDialog() 
{
	
	CDialog::OnInitDialog();


	
	
	pDlgDown=this;
	// TODO: Add extra initialization here


	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");
	
	GetDlgItem(IDC_LIST1)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO1)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_3)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_JLM1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_JLM2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_JLM3)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_JLM4)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_JLM5)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT1)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT2)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT6)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT7)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT8)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_font);

	GetDlgItem(IDC_BUTTON17)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON18)->SetFont(&m_font);


	DWORD dwTcp;
	CreateThread(NULL,0,TCPThread,NULL,0,&dwTcp);

	OpenUnsaveData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DlgDown::DispRecvedMsg(int sock_id, char* byBuf)
{

	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\temp\\";
	

	TFileUploadStart * pfus;
	TDataContent* pdc;
	TFileUploadFinish* pfuf;

//	STRU_MSG_HEADER pht;
	
	CString  strNum;
	CString strInsert="",strTmp;
	int i;
	
	STRU_MSG_HEADER* pMsgHead = (STRU_MSG_HEADER*)byBuf;
	
	CTime time=CTime::GetCurrentTime();	
	CString strTime = time.Format("%H:%M:%S");
	CTime time2=CTime::GetCurrentTime();
	CTimeSpan myTs;
	
	switch(pMsgHead->u16MsgType)
	{
		
	case ARM_PC_FILE_START_IND:
		{
	//		GetDlgItem(IDC_STATIC_INFO)->SetWindowText("ARM_PC_FILE_START_IND");
	//		pfus = (TFileUploadStart*)(byBuf+sizeof(STRU_MSG_HEADER));
			pfTmp=NULL;
			pfTmp = fopen(strIniPath+"data.temp","wb");
			if(pfTmp!=NULL)
				m_list.AddString("开始路测数据下载.");


			

			
		}
		break;
	case ARM_PC_FILE_CONTENT:
		{
			//AfxMessageBox("");
			pdc = (TDataContent*)(byBuf+sizeof(STRU_MSG_HEADER));
			if(pfTmp)
				fwrite(pdc,1,sizeof(TDataContent),pfTmp);
		}
		break;
	case ARM_PC_FILE_FINISH_IND:
		{
			pfuf = (TFileUploadFinish*)(byBuf+sizeof(STRU_MSG_HEADER));
			if(pfTmp)
			{
				fclose(pfTmp);
				pfTmp=NULL;
			}
			
			if(pfuf->index==8)
			{
					
		//		DWORD dwAG;
		//		CreateThread(NULL,0,SaveToDBThread,NULL,0,&dwAG);
				
				m_list.AddString("结束，数据下载");
				m_btnDown.EnableWindow(TRUE);

				OpenUnsaveData();

			
			}
			else
			{
				m_list.AddString("结束，无数据下载");
				m_btnDown.EnableWindow(TRUE);	
			}


			

		}
		break;

	}
}

void DlgDown::SendCmdData(int sock_id, char* buf, int len)
{
	


	if(sock[sock_id].state==1)
	{

		FILE * pf;
		pf = fopen("TEST_W.txt","a");
	
		fprintf(pf,"----------------------------");
		
		for(int j = 0; j<len; j++)
		{
			if(j%8==0)
				fprintf(pf,"\n");
			fprintf(pf,"%02X ",(BYTE)(buf[j]));
		}
		fprintf(pf,"\n\n\n");
		fclose(pf);



		if(send(sock[sock_id].sock,buf,len,0)==SOCKET_ERROR)
		{
			sock[sock_id].state=0;
			pLoadDlg->m_stLink.SetWindowText("设备未连接");
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		}

	//	else
	//		AfxMessageBox("d");
	}
}



void DlgDown::SendDownCmd(int sock_id)
{
	char buf[256];
	STRU_MSG_HEADER pht;
	pht.u16MsgType = PC_ARM_UPLOAD_REQ;
	pht.u16MsgLen = 0;
	pht.u16SessionID = 0;
	
	
	memcpy(buf, &pht, sizeof(STRU_MSG_HEADER));
	//	memcpy(buf+sizeof(pkg_header_t), &ppar, sizeof(T_PAIF_POWER_ADJUST_REQ));
//	AfxMessageBox("ok");
	SendCmdData(sock_id,buf,sizeof(STRU_MSG_HEADER) );//+sizeof(T_PAIF_HEARTBEAT_IND)
}

void DlgDown::OnButton1() 
{


	return;

	
	
}

void DlgDown::OnButton3() 
{
	// TODO: Add your control notification handler code here
	char buf[256];
	STRU_MSG_HEADER pht;
	pht.u16MsgType = PC_ARM_TIME_ADJ_REQ;
	pht.u16MsgLen = 14;
	pht.u16SessionID = 0;
	
	
	CTime timeNow=CTime::GetCurrentTime();	
	CString strTime = timeNow.Format("%Y%m%d%H%M%S");
	char kkk[15];
	strcpy(kkk,strTime);
	
	memcpy(buf, &pht, sizeof(STRU_MSG_HEADER));
	memcpy(buf+sizeof(STRU_MSG_HEADER), kkk, 14);
	//	AfxMessageBox("ok");
	SendCmdData(0,buf,sizeof(STRU_MSG_HEADER)+14 );//+sizeof(T_PAIF_HEARTBEAT_IND)
	
}


void DlgDown::OpenUnsaveData()
{

}

void DlgDown::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	int n =pMainDlg->m_list.GetItemCount();
	if(n>0)
	{

		pMainDlg->m_list.SetCheck(-1,TRUE);
	}
}

void DlgDown::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	int n =pMainDlg->m_list.GetItemCount();
	if(n>0)
	{
		
		pMainDlg->m_list.SetCheck(-1,FALSE);
	}
}

void DlgDown::OnButton2() 
{
	// TODO: Add your control notification handler code here

	int n = pMainDlg->m_list.GetItemCount();
	if(n<1)
	{
		
		AfxMessageBox("没有需要保存的数据，请检查。");
		return;
	}


	UpdateData(TRUE);
	if(m_bChk)
	{
		CFileDialog fdlg(FALSE,".xls","cell_data",OFN_OVERWRITEPROMPT,"Worksheet Files (*.xls)|*.xls|Chart Files (*.xlc)|*.xlc|Data Files (*.xlc;*.xls)|*.xlc; *.xls|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CString filename="";
		if(fdlg.DoModal()==IDOK)
		{
			filename=fdlg.GetPathName();
		}
		else
			return;
		
		//	MessageBox(filename);
		if(filename.Find(".xls")>0||filename.Find(".xlc")>0||filename.Find(".xlsx")>0)
		{
			if(ExportToExcel(filename))
			{
				
				AfxMessageBox("导出Excel成功!");
			}
		}
		else if(filename.Find(".txt")>0)
		{
			if(ExportToTxt(filename))
				AfxMessageBox("导出Txt成功!");
		}
	}
SaveToDBThread(m_strName1);

	DWORD dwAG;
//	CreateThread(NULL,0,SaveToDBThread,NULL,0,&dwAG);
	//入库
}
bool DlgDown::ExportToExcel(CString filename)
{
	CMyExcel my;
	int i=0;
	int j=0;
	my.Open();
	my.AddSheet("cell_data");
	//m_listOpRst.GetHeaderText();
	my.SetColumnWidth(150);
	
	int columnCount=pMainDlg->m_list.GetColumnCount();

	
	MyNumberFormat Xnumber;
	Xnumber.GetText();
	my.SetNumberFormat(Xnumber);
	for(i=1;i<=columnCount;i++)
	{
		my.SetItemText(1,i,pMainDlg->m_list.GetHeaderText(i-1));
	}
	
	int countRows=	pMainDlg->m_list.GetItemCount();
	
	for(i=0;i<countRows;i++)
	{
		if((pMainDlg->m_dlgDown.m_nSel==1 &&pMainDlg->m_list.GetCheck(i)==TRUE) ||   pMainDlg->m_dlgDown.m_nSel == 0 )// if( || (pMainDlg->m_dlgDown.m_nSel==1 && pMainDlg->m_list.GetCheck(cnt)==TRUE)))
		{

			for(j=0;j<columnCount;j++)
			{
				CString tmp=pMainDlg->m_list.GetItemText(i,j);
				my.SetItemText(i+2,j+1,tmp);
			}
		
		}
		
	}
	
	my.SaveAs(filename);
	return true;
}

bool DlgDown::ExportToTxt(CString filename)
{
	
	
	int i=0;
	int j=0;
	
	ofstream fs;
	fs.open(filename);
	
	int columnCount=pMainDlg->m_list.GetColumnCount();
	
	char* seperator="		";
	for(i=1;i<=columnCount;i++)
	{
		fs<<pMainDlg->m_list.GetHeaderText(i-1).GetBuffer(0)<<seperator;
	}
	
	fs<<endl;
	int countRows=	pMainDlg->m_list.GetItemCount();
	for(i=0;i<countRows;i++)
	{
		if((pMainDlg->m_dlgDown.m_nSel==1 &&pMainDlg->m_list.GetCheck(i)==TRUE) ||   pMainDlg->m_dlgDown.m_nSel == 0 )
		{

			for(j=0;j<columnCount;j++)
			{
				fs<<pMainDlg->m_list.GetItemText(i,j).GetBuffer(0)<<seperator;
			}
			
			fs<<endl;
		}
	}
	
	fs.close();
	
	return true;
}

void DlgDown::OnButton7() 
{
	// TODO: Add your control notification handler code here
	LoadFromExcel() ;
	CTime tmm = CTime::GetCurrentTime();
	CString strTime = tmm.Format("%y%m%d%H%M%S");
	
	GetDlgItem(IDC_EDIT2)->SetWindowText(strTime);
	m_strFile = strTime;
	UpdateData(TRUE);
}

void Split (CString source , CString divKey, CStringArray & dest )

{

     dest .RemoveAll ();

     int pos = 0;

     int pre_pos = 0;

     while ( -1 != pos ){

         

         pos = source .Find (divKey ,(pos +1));

         dest .Add (source .Mid (pre_pos ,(pos -pre_pos )));

		 pre_pos = pos+1 ;

     }

}
void DlgDown::LoadFromExcel() 
{
	// TODO: Add your command handler code here
	//获取Excel文件路径
	CString filePath;
	CFileDialog dlg (TRUE, NULL, NULL,OFN_FILEMUSTEXIST, "FieldTest File(*.csv)|*.*||", this);


	if(dlg.DoModal()==IDOK)
	{
		filePath = dlg.GetPathName();
//		m_strFile = dlg.GetFileName();
		
		GetDlgItem(IDC_EDIT7)->SetWindowText(filePath);
//		UpdateData(FALSE);
	}
	else
	{
        return;
	}

	
	pMainDlg->m_list.DeleteAllItems();


	CStdioFile file;
	file.Open(filePath,CFile::modeRead);
	CString szLine="";
	CStringArray dest1;

	CStringArray &dest = dest1;
	int nLine =0;
	while(file.ReadString(szLine))
	{
		dest.RemoveAll();
		Split(szLine,",",dest);
		int size = dest.GetSize();

		if(size == 50)
		{
		//	AfxMessageBox("50");

		if(dest.GetAt(11)=="B0")
			dest.SetAt(11,"");
		if(dest.GetAt(17)=="B0")
			dest.SetAt(17,"");
		if(dest.GetAt(23)=="B0")
			dest.SetAt(23,"");
		if(dest.GetAt(29)=="B0")
			dest.SetAt(29,"");
		if(dest.GetAt(35)=="B0")
			dest.SetAt(35,"");
		if(dest.GetAt(41)=="B0")
			dest.SetAt(41,"");
		if(dest.GetAt(47)=="B0")
			dest.SetAt(47,"");


		pMainDlg->m_list.InsertItem(nLine,dest.GetAt(0));
		pMainDlg->m_list.SetItemText(nLine,1,dest.GetAt(1));
		pMainDlg->m_list.SetItemText(nLine,2,dest.GetAt(2));
		pMainDlg->m_list.SetItemText(nLine,3,dest.GetAt(4));
		pMainDlg->m_list.SetItemText(nLine,4,dest.GetAt(5));
		pMainDlg->m_list.SetItemText(nLine,5,dest.GetAt(6));
		pMainDlg->m_list.SetItemText(nLine,6,dest.GetAt(3));
		if(dest.GetAt(7)=="0")
			pMainDlg->m_list.SetItemText(nLine,7,"GSM");
		else if(dest.GetAt(7)=="1")
			pMainDlg->m_list.SetItemText(nLine,7,"CDMA");
		else if(dest.GetAt(7)=="2")
			pMainDlg->m_list.SetItemText(nLine,7,"WCDMA");
		else if(dest.GetAt(7)=="3")
			pMainDlg->m_list.SetItemText(nLine,7,"LTE");
		else
			pMainDlg->m_list.SetItemText(nLine,7,"-");


		pMainDlg->m_list.SetItemText(nLine,8,dest.GetAt(8));
		pMainDlg->m_list.SetItemText(nLine,9,dest.GetAt(9));


		pMainDlg->m_list.SetItemText(nLine,10,dest.GetAt(10));
		pMainDlg->m_list.SetItemText(nLine,11,dest.GetAt(11));
		pMainDlg->m_list.SetItemText(nLine,12,dest.GetAt(12));
		pMainDlg->m_list.SetItemText(nLine,13,dest.GetAt(13));
		pMainDlg->m_list.SetItemText(nLine,14,dest.GetAt(14));

		pMainDlg->m_list.SetItemText(nLine,15,dest.GetAt(15));
		pMainDlg->m_list.SetItemText(nLine,16,dest.GetAt(16));
		pMainDlg->m_list.SetItemText(nLine,17,dest.GetAt(17));
		pMainDlg->m_list.SetItemText(nLine,18,dest.GetAt(18));
		pMainDlg->m_list.SetItemText(nLine,19,dest.GetAt(19));

		pMainDlg->m_list.SetItemText(nLine,20,dest.GetAt(20));
		pMainDlg->m_list.SetItemText(nLine,21,dest.GetAt(21));
		pMainDlg->m_list.SetItemText(nLine,22,dest.GetAt(22));
		pMainDlg->m_list.SetItemText(nLine,23,dest.GetAt(23));
		pMainDlg->m_list.SetItemText(nLine,24,dest.GetAt(24));

		pMainDlg->m_list.SetItemText(nLine,25,dest.GetAt(25));
		pMainDlg->m_list.SetItemText(nLine,26,dest.GetAt(26));
		pMainDlg->m_list.SetItemText(nLine,27,dest.GetAt(27));
		pMainDlg->m_list.SetItemText(nLine,28,dest.GetAt(28));
		pMainDlg->m_list.SetItemText(nLine,29,dest.GetAt(29));


		pMainDlg->m_list.SetItemText(nLine,30,dest.GetAt(30));
		pMainDlg->m_list.SetItemText(nLine,31,dest.GetAt(31));
		pMainDlg->m_list.SetItemText(nLine,32,dest.GetAt(32));
		pMainDlg->m_list.SetItemText(nLine,33,dest.GetAt(33));
		pMainDlg->m_list.SetItemText(nLine,34,dest.GetAt(34));

		pMainDlg->m_list.SetItemText(nLine,35,dest.GetAt(35));
		pMainDlg->m_list.SetItemText(nLine,36,dest.GetAt(36));
		pMainDlg->m_list.SetItemText(nLine,37,dest.GetAt(37));
		pMainDlg->m_list.SetItemText(nLine,38,dest.GetAt(38));
		pMainDlg->m_list.SetItemText(nLine,39,dest.GetAt(39));
		pMainDlg->m_list.SetItemText(nLine,40,dest.GetAt(40));
		pMainDlg->m_list.SetItemText(nLine,41,dest.GetAt(41));

		pMainDlg->m_list.SetItemText(nLine,42,dest.GetAt(42));
		pMainDlg->m_list.SetItemText(nLine,43,dest.GetAt(43));
		pMainDlg->m_list.SetItemText(nLine,44,dest.GetAt(44));
		pMainDlg->m_list.SetItemText(nLine,45,dest.GetAt(45));
		pMainDlg->m_list.SetItemText(nLine,46,dest.GetAt(46));
		pMainDlg->m_list.SetItemText(nLine,47,dest.GetAt(47));
		pMainDlg->m_list.SetItemText(nLine,48,dest.GetAt(48));
		pMainDlg->m_list.SetItemText(nLine,49,dest.GetAt(49));
	

		}


		nLine++;


	}
	file.Close();



	/*for(int j=2; j<=iRowNum;j++)
	{
	
			pMainDlg->m_list.InsertItem(j-2,"");
			for(int i=1;i<=iColsNum;i++)
			{

		
			
				range.AttachDispatch(usedRange.GetItem(COleVariant((long)j), COleVariant((long)i)).pdispVal);
			
				vResult = range.GetText();
				pMainDlg->m_list.SetItemText(j-2,i-1,(CString)(vResult.bstrVal) );
			
			}
		

	
	}


    //关闭所有的book，退出Excel 
    book.Close (covOptional,COleVariant(filePath),covOptional);

    books.Close();      
    app.Quit();
	pMainDlg->m_list.SetCheck(-1,TRUE);*/
	//调用刷新函数
//	Refresh();

//	MessageBox("导入Excel成功！", "提示",MB_ICONINFORMATION);



	
	//创建对象
/*	_Application app;
    _Workbook book;
    _Worksheet sheet;
    Workbooks books;
    Worksheets sheets;
    Range range;
    LPDISPATCH lpDisp;
    COleVariant vResult;                              
    COleVariant covTrue((short)TRUE);
    COleVariant covFalse((short)FALSE);
    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);                        
    //COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 

    //启动Excel服务器
    if(!app.CreateDispatch("Excel.Application"))
    {
        AfxMessageBox("无法启动Excel服务器!");
        return;
    }

    app.SetVisible(FALSE);         
	
	//获取Excel文件路径
	CString filePath;
	CFileDialog dlg (TRUE, NULL, NULL,OFN_FILEMUSTEXIST, "Excel File(*.*)|*.*||", this);

	int structsize=0; 
    DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
    dwVersion = GetVersion(); 
    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
    dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
   
    if (dwVersion < 0x80000000)               
       structsize =88;                        
    else                                     
    structsize =76;                           
   
	dlg.m_ofn.lStructSize=structsize;
	if(dlg.DoModal()==IDOK)
	{
		filePath = dlg.GetPathName();
		GetDlgItem(IDC_EDIT7)->SetWindowText(filePath);
//		UpdateData(FALSE);
	}
	else
	{
        return;
	}

	//打开Excel文件
    books.AttachDispatch(app.GetWorkbooks());
    lpDisp = books.Open(filePath, covOptional, covFalse, covOptional, covOptional, covOptional, covOptional, 
         covOptional, covOptional, covOptional, covOptional, covOptional,covOptional);

    //得到Workbook
    book.AttachDispatch(lpDisp);

    //得到Worksheets 
    sheets.AttachDispatch(book.GetWorksheets()); 

    //得到当前活跃sheet
    lpDisp=book.GetActiveSheet();
    sheet.AttachDispatch(lpDisp);

    //读取已经使用区域的信息
    Range usedRange;
    usedRange.AttachDispatch(sheet.GetUsedRange());
  
	Range range_cols;
	range_cols.AttachDispatch(usedRange.GetColumns());
	long iColsNum = range_cols.GetCount();
	
	range.AttachDispatch(usedRange.GetRows());
   
	long iRowNum=range.GetCount();                               //已经使用的行数
    long iStartRow=usedRange.GetRow();                           //已使用区域的起始行，从第1行开始
	


	
	CString strss;
	strss.Format("%d   %d",iRowNum,iColsNum);

	if(iColsNum!=9)
	{
		book.Close (covOptional,COleVariant(filePath),covOptional);
		
		books.Close();      
		app.Quit();
		
		
		AfxMessageBox("Excel表格格式不满足导入标准.");


		return;
		
	}
		
	//先判断，防止胡乱导入！

	pMainDlg->m_list.DeleteAllItems();
	for(int j=2; j<=iRowNum;j++)
	{
	
			pMainDlg->m_list.InsertItem(j-2,"");
			for(int i=1;i<=iColsNum;i++)
			{

		
			
				range.AttachDispatch(usedRange.GetItem(COleVariant((long)j), COleVariant((long)i)).pdispVal);
			
				vResult = range.GetText();
				pMainDlg->m_list.SetItemText(j-2,i-1,(CString)(vResult.bstrVal) );
			
			}
		

	
	}


    //关闭所有的book，退出Excel 
    book.Close (covOptional,COleVariant(filePath),covOptional);

    books.Close();      
    app.Quit();
	pMainDlg->m_list.SetCheck(-1,TRUE);*/
	//调用刷新函数
//	Refresh();

//	MessageBox("导入Excel成功！", "提示",MB_ICONINFORMATION);


}

void DlgDown::OnButton8() 
{
	// TODO: Add your control notification handler code here

	int n = pMainDlg->m_list.GetItemCount();
	if(n<1)
	{
		
		AfxMessageBox("没有找到需要保存的数据，请检查文件格式是否正确。");
		return;
	}
	UpdateData(TRUE);
	SaveToDBThread2(m_strFile,m_strName2);
}

void DlgDown::OnButton18() 
{
	// TODO: Add your control notification handler code here

	int n = pMainDlg->m_list.GetItemCount();
	if(n<1)
	{

		AfxMessageBox("没有找到需要保存的数据，请检查文件格式是否正确。");
		return;
	}
	UpdateData(TRUE);
//	SaveToDBThread2(m_strName3);
}

void DlgDown::OnButton17() 
{

}

