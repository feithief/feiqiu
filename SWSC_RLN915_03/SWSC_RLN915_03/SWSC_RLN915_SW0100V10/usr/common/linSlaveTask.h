#ifndef _LINS_TASK_H
#define _LINS_TASK_H

#include <appConfig.h>
#include <lin_device.h>


/**
* @brief A structure to represent LIN frame information.
*/

#define N_AS            (1000)
#define N_CR            (1000)

#define LINS_FUNCTION_ID_SNPD_FORCE_SWITCH_ON           (0xAA00U)
#define LINS_FUNCTION_ID_SNPD_INIT                      (0xAA01U)
#define LINS_FUNCTION_ID_SNPD_ASSIGN_NAD                (0xAA02U)
#define LINS_FUNCTION_ID_SNPD_STORAGE                   (0xAA03U)
#define LINS_FUNCTION_ID_SNPD_EXIT                      (0xAA04U)

#define LINS_SUPPLIER_ID                                (0x0084U)    /* LJM added on 202211 04*/
#define LINS_FUNCTION_ID                                (0x0003U)     
#define LINS_VARIANT                                    (0x01U)         
#define LINS_SERIAL_NO                                  (0x01234567U)
#define LINS_FW_VERSION                                 (0x0123U)
#define LINS_HW_VERSION                                 (0x0001U)

#define LIN_SID_APP_DIAG_LED_GET_CONFIG                 (0xBAU)   /* single pdu receive, multi pdu response */
#define LIN_SID_APP_DIAG_LED_SET_CONFIG                 (0xBBU)   /* multi pdu receive, single pdu response */
#define LIN_SID_APP_DIAG_SINGLE_PDU_RECV_MULTI_RSP      (0x22U)   
#define LIN_SID_APP_DIAG_MULTI_PDU_RECV_SINGLE_RSP      (0x2EU)
#define LIN_SID_APP_DIAG_MULTI_PDU_RECV_MULTI_RSP       (0x2FU)


#define LIN_SID_APP_DIAG_BOOT_MODE                      (0x20U)
#define BOOT_MODE_HANDSHAKE                             (0x00U)
#define BOOT_MODE_MAGIC_KEY_HANDSHAKE                   (0x5A5A55AAU)

/*  *****************LIN configuration default value **************************/
#if LIN_STACK_TYPE == LIN_STACK_TYPE_SAEJ2602
    #define INIT_LINS_NAD_ADDR              (0x6FU) /* NAD range is 0x60-0x6D,defalut*/
    #if (INIT_LINS_NAD_ADDR>0x6FU)
        #error INIT_LINS_NAD_ADDR must be within 0x60U-0x6FU!!!
    #endif

    #define MESSAGE_ID_BLOCK_COUNT           (0x08U)       /* MESSAGE_ID_BLOCK_COUNT = 2^n n= 2,3,4..  */
    #if (MESSAGE_ID_BLOCK_COUNT != 4U && MESSAGE_ID_BLOCK_COUNT != 8U && MESSAGE_ID_BLOCK_COUNT != 16U)
        #error Invalid MESSAGE_ID_BLOCK_COUNT count !!!
    #elif ((INIT_LINS_NAD_ADDR == 0x6CU || INIT_LINS_NAD_ADDR == 0x6DU) && MESSAGE_ID_BLOCK_COUNT != 4U)
        #error MESSAGE_ID_BLOCK_COUNT must be 4 when NAD is 0x6C or 0x6D!!!
    #endif

    /* it's only a reference, the real FID number is based on NAD according to SAEJ2602  */
    #define DEFAULT_LINS_FID0             (0x00U)
    #define DEFAULT_LINS_FID1             (0x01U)
    #define DEFAULT_LINS_FID2             (0x02U)
    #define DEFAULT_LINS_FID3             (0x03U)
    #define DEFAULT_LINS_FID4             (0x04U)
    #define DEFAULT_LINS_FID5             (0x05U)
    #define DEFAULT_LINS_FID6             (0x06U)
    #define DEFAULT_LINS_FID7             (0x07U)

#else

    #define INIT_LINS_NAD_ADDR              (0x01U)
    #define DEFAULT_LINS_FID_SIZE           (0x04U)       /* count number defined by user */

    /*  FID number is set by user  */
    #define DEFAULT_LINS_FID0               (0x20U)
    #define DEFAULT_LINS_FID1               (0x21U)
    #define DEFAULT_LINS_FID2               (0x22U)
    #define DEFAULT_LINS_FID3               (0x23U)

    void LINS_EventTriggered_ISR(void);
#endif


void LINS_TaskHandler(void);
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount);
void LINS_SendWakeUpSignal_ISR(void);
uint8_t LINS_GetPIDFromFID(uint8_t frameId);
void LINS_SetDeviceUnconfigured(void);




void SystemLINInit(void);   //LJM added
extern LIN_Device_Frame_t l_LinData_Receive;
extern LIN_Device_Frame_t l_LinData_Response;

/* 注意这种写法，斜杠后必须立即回车，不能有空格再回车，不然报错 */
/* Signal Read Access for Signal BCM_RGB_Slave_Adresse */
#define        l_u16_rd_CtrlList()    ((((uint16_t)l_LinData_Receive.data[0]) & 0xffu) | \
                                    ((uint16_t)((((uint16_t)l_LinData_Receive.data[1]) & (0xff)) << 8u)))

/* Signal Read Access for Signal BCM_RGB_Intensitaet */
#define        l_u8_rd_Brightness()     ((l_LinData_Receive.data[2]) & 0xffu)

/* Signal Read Access for Signal BCM_RGB_Rot */
#define        l_u8_rd_RGB_R()          ((l_LinData_Receive.data[3]) & 0xffu)

/* Signal Read Access for Signal BCM_RGB_Gruen */
#define        l_u8_rd_RGB_G()          ((l_LinData_Receive.data[4]) & 0xffu)

/* Signal Read Access for Signal BCM_RGB_Blau */
#define        l_u8_rd_RGB_B()          ((l_LinData_Receive.data[5]) & 0xffu)

/* Signal Read Access for Signal BCM_RGB_RampTime */
#define        l_u8_rd_RampTime()       ((l_LinData_Receive.data[6]) & 0xffu)

/* Signal Read Access for Signal BCM_RGB_VehicleTypeInfo */
#define        l_u8_rd_VehicleTypeInfo()       ((l_LinData_Receive.data[7]) & 0xffu)


/* Signal Write Access for Signal RGB_Slave_ResponseError */
#define        l_bool_wr_response_error(x)  \
               do       \
               {\
                 l_LinData_Response.data[0] &= (0xfeu);\
                 l_LinData_Response.data[0] |= ((l_u8)(x));\
               }while(0)

/* Signal Write Access for Signal RGB_Slave_HWVerNum */
#define        l_u8_wr_HWVerNum2(x)        \
               do       \
               {\
                 l_LinData_Response.data[1] &= (0u); \
                 l_LinData_Response.data[1] |= ((l_u8)(x)); \
               }while(0)


/* Signal Write Access for Signal RGB_Slave_SWVerNum */
#define        l_u8_wr_SWVerNum2(x)      \
               do       \
               {\
                 l_LinData_Response.data[2] &= (0u); \
                 l_LinData_Response.data[2] |= ((l_u8)(x)); \
               }while(0)

                 
#endif