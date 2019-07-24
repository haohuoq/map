#ifndef MSG_STRU_H
#define MSG_STRU_H

#pragma	pack(1)

typedef struct 
{
	char catch_time[15];
	char lng[10];
	char lat[10];
	char yd_lac[6];
	char yd_cid[6];
	char yd_chn[4];
	char yd_marker[20];

	char lt_lac[6];
	char lt_cid[6];
	char lt_chn[4];
	char lt_marker[20];
	
	char w_lac[6];
	char w_cid[6];
	char w_marker[20];

	char t_lac[6];
	char t_cid[6];
	char t_marker[20];

	char f_4g_1[32];
	char f_4g_2[32];
	char f_4g_3[32];

	
}cell_info_t;




#pragma	pack(0)

#endif