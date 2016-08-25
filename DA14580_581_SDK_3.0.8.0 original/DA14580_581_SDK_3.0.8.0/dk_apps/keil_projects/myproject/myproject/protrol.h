/*
 * bma250_driver.h
 *
 *  Created on: 2015 0909
 *      Author: leiyq
 */

#ifndef PROTROL_H
#define PROTROL_H
#include <stdbool.h>
#include <stdint.h>

#define PROTO_HEAD 0x11
#define PROTO_END 0x0055

#define		 PROTO_RESET_SFOTWARE 0x00
#define	   PROTO_CALL_IN     0X1A
#define		 PROTO_CALL_END    0X1B
#define    PROTO_RECV_CALL   0X2A
#define		 PROTO_REJECT_CALL 0X2B
#define	   PROTO_TAP_PHOTO   0X3A
#define	   PROTO_TAP_CALL    0X3B
#define		 PROTO_UP_MSG      0X4A
#define		 PROTO_SET_BMA_NORMAL		 0X5A	   
#define		 PROTO_SET_BMA_SUSPEND		 0X5B	   
typedef struct
{
	unsigned char p_head;
	unsigned char p_cmd;
	unsigned short p_end;	
}PROTROL_CONTENT;




int app_sent_cmd(unsigned cmd);//sent the cmd
int app_sent_data(unsigned cmd , unsigned cmd1);
unsigned char app_parase_cmd(unsigned char *strdata,unsigned char lenth);//parase cmd
void app_handle_cmd(unsigned char cmd);//handle cmd 


#endif /* PROTROL_H */
