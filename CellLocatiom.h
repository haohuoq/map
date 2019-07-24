// CellLocatiom.h : main header file for the CELLLOCATIOM application
//

#if !defined(AFX_CELLLOCATIOM_H__A2811990_2815_40AE_9EFF_5B98A30B752A__INCLUDED_)
#define AFX_CELLLOCATIOM_H__A2811990_2815_40AE_9EFF_5B98A30B752A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCellLocatiomApp:
// See CellLocatiom.cpp for the implementation of this class
//



#define PC_ARM_UPLOAD_REQ  1
#define ARM_PC_FILE_START_IND  2
#define ARM_PC_FILE_CONTENT  3
#define ARM_PC_FILE_FINISH_IND  4
#define PC_ARM_TIME_ADJ_REQ  5
#pragma pack(1)

typedef struct 
{
	WORD dvc_id;
	WORD	active_flag;
	DWORD state;  //状态
	DWORD sock;
	unsigned long addr;//BoardIPAddr.sin_addr.s_addr
	WORD toport;
}SOCKET_LIST;


typedef struct
{
    WORD		u16MsgType;    /* 消息类型 */
    WORD		u16MsgLen;     /* 消息体长度，字节，不含包头 */
    WORD		u16SessionID;   /* 消息会话标识 */
}STRU_MSG_HEADER;




typedef struct NetInfo
{	
	WORD lac; 	//数据类型
	WORD cid;
	WORD bsid;
}TNetInfo;


typedef struct SaveData
{	
	char type[10];
	WORD lac; 	//数据类型
	WORD cid;
	WORD arfcn;
	WORD PCI;
	int rssi;
	double base_long;
	double base_lat;
	long time1;
}TSaveData;



typedef struct FileUploadStart
{
	BYTE index;    //0-8  0 没数据
}TFileUploadStart;


typedef struct DataContent
{
	TSaveData sd[20];
}TDataContent;


typedef struct FileUploadFinish
{
	BYTE index;    //1-8  
}TFileUploadFinish;


typedef struct TimeAdj
{
	char tms[12];    //1-8  
}TTimeAdj;







typedef struct TSTRU_GSMCellInfo
{
	WORD	arfcn;			
	short	rssi;		
}STRU_GSMCellInfo;

typedef  struct TSTRU_GSMWlsEnvParam
{	
	BYTE     plmn_id[3];  
				
	BYTE lac[2];  
	
	STRU_GSMCellInfo srv_cell;
	DWORD cell_id;
	BYTE nghbor_cell_num;
	STRU_GSMCellInfo nghbor_cell[6];
	
}STRU_GSMWlsEnvParam;


typedef struct T_pmsmgr_wls_env_param_rpt_type
{
	WORD	msg_type;
	
	WORD    sid;                   /* System identification */
	WORD    nid;     		   /* Network identification */	
	WORD	base_id;              
	DWORD base_long;            /* Base station longitude */
	DWORD base_lat;             /* Base station latitude */
	
	WORD cdma_ch;       /* Frequency assignment */
	WORD cdma_pn;        /* Pilot PN sequence offset index */
	BYTE  pilot_strength;
	char rx_pwr;
	
	BYTE     num_freq;
	WORD	cdma_freq[7];
	
	BYTE  pilot_num;
	WORD   pilot_sets[20];
	
	
}pmsmgr_wls_env_param_rpt_type;

typedef struct TSTRU_WCDMACellInfo
{
	WORD		UARFCN;							//频率UARFCN
	WORD		PSC;						//PSC
	short		RSSI;							//强度
}STRU_WCDMACellInfo;



typedef struct TSTRU_WDMAWlsEnvParam
{
	BYTE     plmn_id[3];   /*64 f0 00（移动），64 f0 10（联通）*/
	
	BYTE lac[2];  
	
	STRU_WCDMACellInfo 	srv_cell;	/*服务小区信息*/
	
	DWORD       cell_id;
	
	BYTE 		neighbor_cell_num; /*邻区个数*/
	
	STRU_WCDMACellInfo 	neighbor_cell[6]; /*邻区信息*/
	
}STRU_WDMAWlsEnvParam;


typedef struct STRU_TD_LTE_SysInfo
{
	BYTE srv_status;   							
	BYTE sys_mode;
	WORD mcc;
	BYTE mnc;
	WORD channel;
	short rssi;
	short rscp;
	DWORD lac;
	DWORD cid;
	DWORD lte_tac;
	DWORD lte_rac;
}TD_LTE_SysInfo;


typedef struct STRU_LTE_MAININFO
{
	WORD earfcn;
	WORD rscp;
	WORD plmn;
	DWORD global_ci;
	WORD tac;
}LTE_MAININFO;


typedef struct STRU_LTE_NBINFO
{
	WORD tac;
	WORD earfcn;
	WORD rscp;
}LTE_NBINFO;


typedef struct STRU_TD_LTE_CellInfo
{ 
	LTE_MAININFO lmi;
	LTE_NBINFO lni[8];							//强度
}TD_LTE_CellInfo;



typedef struct RealData
{	
	STRU_GSMWlsEnvParam gsm1;
	STRU_GSMWlsEnvParam gsm2;
	pmsmgr_wls_env_param_rpt_type cdma;
	STRU_WDMAWlsEnvParam wcdma;
	TD_LTE_CellInfo td;
	TD_LTE_CellInfo lte1;
	TD_LTE_CellInfo lte2;
	TD_LTE_CellInfo lte3;
	double base_long;
	double base_lat;
	long time1;
}TRealtimeData;




#pragma pack()



class CImpIDispatch;
class CCellLocatiomApp : public CWinApp
{
public:
	CCellLocatiomApp();
CImpIDispatch*	m_pCustDisp;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellLocatiomApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCellLocatiomApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};
	extern CCellLocatiomApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLLOCATIOM_H__A2811990_2815_40AE_9EFF_5B98A30B752A__INCLUDED_)
