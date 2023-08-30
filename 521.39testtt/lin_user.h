#ifndef _lin_user_h
#define _lin_user_h

#include "stdint.h"

typedef struct
{
    uint8_t id;
    uint8_t lostFlag;
    uint8_t communicationErrorFlag;
    uint8_t masterWriteBuf[8];
    uint8_t masterReadBuf[8];
}LIN_USER_t;




extern LIN_USER_t linUser;
extern uint8_t linError;


/*===========================================================================
������Lin_User_Rec_Process
���ܣ�LIN �û����Ľ���
���룺None
���أ�None
============================================================================*/
void Lin_User_Master_Write_Process(void);
void Lin_User_Slave_Response_Undate(void);
#endif
