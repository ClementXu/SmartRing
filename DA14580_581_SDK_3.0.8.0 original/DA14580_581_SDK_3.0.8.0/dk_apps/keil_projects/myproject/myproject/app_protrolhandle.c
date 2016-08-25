/**
 ****************************************************************************************
 *
 * @file app_protrolhandle.c
 *
 * @brief protrol handle
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

 
#include "protrol.h"
#include <streamdatad.h>
#include <gpio.h> 
 extern int wakeupflag;
 extern int intflag;
 int bma_state;
 extern unsigned char ledflag;
 extern sleep_mode_t sleep_mode;
#if 0
 int app_sent_cmd(unsigned cmd)
 {
	  PROTROL_CONTENT protrosent;
	  protrosent.p_head = PROTO_HEAD;
	  protrosent.p_cmd = cmd;
		protrosent.p_end  = PROTO_END;
	 
		attmdb_att_set_value(STREAMDATAD_DIR_VAL_HANDLE(0), sizeof(PROTROL_CONTENT), (uint8_t*)&(protrosent));
		prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_DIR_VAL_HANDLE(0));
	 
	  return 0;

 }

 
 int app_sent_cmd(unsigned cmd)
 {
	  unsigned char temp;
	  unsigned char sentbuf[8]={'1','1',0,0,'0','0','8','8'};
		temp = cmd/16;
    sentbuf[2]=temp+48;
		temp = cmd%16;
		sentbuf[3]=temp+55;
	 
		attmdb_att_set_value(STREAMDATAD_DIR_VAL_HANDLE(0), sizeof(sentbuf), sentbuf);
		prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_DIR_VAL_HANDLE(0));
	 
	  return 0;

 }
#endif
int app_sent_data(unsigned cmd , unsigned cmd1)
{


	  unsigned char temp;
	  unsigned char sentbuf[8]={'1','1',0,0,0,0,'8','8'};
		temp = cmd/16;
    sentbuf[2]=temp+48;
		temp = cmd%16;
		sentbuf[3]=temp+55;
		
		temp = cmd1/16;
    sentbuf[4]=temp+48;
		temp = cmd1%16;
		sentbuf[5]=temp+48;		
	 
		attmdb_att_set_value(STREAMDATAD_DIR_VAL_HANDLE(0), sizeof(sentbuf), sentbuf);
		prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_DIR_VAL_HANDLE(0));
	
	
	
			return 0;
}
 
 #if 0
 unsigned char app_parase_cmd(unsigned char *strdata,unsigned char lenth)
 {
	 unsigned char cmd = 0;;
	 if((strdata== NULL)||lenth!=4)
		  return 0;
	 if((strdata[0]!=0x11)|| (strdata[2]!=0x00) || strdata[3]!=0x55)
		 return 0;
	 cmd = strdata[1];
	 return cmd;
		
	
 }
#endif

 unsigned char app_parase_cmd(unsigned char *strdata,unsigned char lenth)
 {
		unsigned char cmd = 0;;
		if((strdata[2]=='1')&& (strdata[3]=='A'))
			cmd = PROTO_CALL_IN;
		else if((strdata[2] =='1')&&(strdata[3]=='B'))
			cmd = PROTO_CALL_END;
		else if((strdata[2]=='4')&&(strdata[3]=='A'))
			cmd = PROTO_UP_MSG;
		else if((strdata[2] =='5')&&(strdata[3]=='A'))
			cmd = PROTO_SET_BMA_NORMAL;
		else if((strdata[2] =='5')&&(strdata[3]=='B'))
			cmd = PROTO_SET_BMA_SUSPEND;
	 return cmd;
		
	
 } 
 

 void app_handle_cmd(unsigned char cmd)
{
	switch(cmd)
	{
		case PROTO_CALL_IN:
			//turn on led£»
			wakeupflag=1;
			hal_bma250_init();
			delay(200);
			ledflag =1;
			break;
		case PROTO_CALL_END:
			//turn off led
			ledflag = 0;
			wakeupflag=0;
			hal_bma250_init();
			break;
		case PROTO_UP_MSG:
			ledflag = 0;
			break;
		case PROTO_SET_BMA_NORMAL:
			bma_state=1;
			wakeupflag=1;
			hal_bma250_init();
			break;
		case PROTO_SET_BMA_SUSPEND:
			bma_state=0;
			wakeupflag=0;
			ledflag =0;
			intflag=0;
			hal_bma250_init();
			break;
		case PROTO_RESET_SFOTWARE:
			SetWord16(SYS_CTRL_REG,SW_RESET);
			break;
		default:
			break;
	
	}
}
 