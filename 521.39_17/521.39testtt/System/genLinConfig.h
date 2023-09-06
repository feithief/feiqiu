/******************************************************************************
*                                                                             *
*                                                                             *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*  ihr GmbH                                                                   *
*  Airport Boulevard B210                                                     *
*  77836 Rheinmuenster - Germany                                              *
*  http://www.ihr.de                                                          *
*  Phone +49(0) 7229-18475-0                                                  *
*  Fax   +49(0) 7229-18475-11                                                 *
*                                                                             *
******************************************************************************/
/* Date: 2019/04/01 */
/* LDF: mqb_bcm_lin5_kmatrix.ldf */
#ifndef        GEN_LIN_CONFIG_H
#define        GEN_LIN_CONFIG_H
 
#include "LinDrvImp_FrameConfig.h"
/* LIN Attributes */
#define        LIN_BAUDRATE 19200u    /* for CPU speed of 12 MHz */

/* Node Attributes */

/* Generated Node: AM_RGB_Slave_01 */

#define        LIN_SLAVE  
#define        LIN_PROTOCOL_VERSION_2_0
#define        LIN_SUPPLIER_ID    ((l_u16)0) 
#define        LIN_SUPPLIER_ID_HI ((l_u8)0) 
#define        LIN_SUPPLIER_ID_LO ((l_u8)0) 
#define        LIN_FUNCTION_ID    ((l_u16)0) 
#define        LIN_FUNCTION_ID_HI ((l_u8)0) 
#define        LIN_FUNCTION_ID_LO ((l_u8)0) 
#define        LIN_VARIANT_ID     ((l_u8)0)

/* Access Macros for Node Information */
#define        l_get_current_NAD()        (g_lin_prod_id.NAD)
#define        l_set_current_NAD(x)       (g_lin_prod_id.NAD = (x))
#define        l_get_initial_NAD()        (g_lin_prod_id.Initial_NAD)
#define        l_get_SupplierID_Low()     (g_lin_prod_id.Supplier_lo)
#define        l_get_SupplierID_High()    (g_lin_prod_id.Supplier_hi)
#define        l_get_FunctionID_Low()     (g_lin_prod_id.Function_lo)
#define        l_get_FunctionID_High()    (g_lin_prod_id.Function_hi)
#define        l_get_Variant()            (g_lin_prod_id.Variant)


/* User Defined Attributes */
#include "SystemType.h"



/* User Defined Diagnostic Attributes */

#define        LIN_DIAG_ENABLE
#define        LIN_COOKED_API
#define        LIN_TASK_CYCLE_MS       2u
#define        LIN_FRAME_TIMEOUT       3u
#define        LIN_AUTOBAUD_TIMEOUT    15u

/* User Defined Node Attributes */

#define        LIN_ENABLE_ASSIGN_NAD
#define        LIN_ENABLE_ASSIGN_FRAME_ID
#define        LIN_ENABLE_RBI_RD_MSG_ID_PID


/* User Defined MCU Attributes */

/* E521.36 */
#define        BRGVAL           ((l_u16)(2468u))
#define        LIN_UART1
#define        AUTOBAUD_ENABLED
#define        MAX_BDREG        ((l_u16)(2741u))
#define        MIN_BDREG        ((l_u16)(2241u))


/* Generate the Signal Attributes */

#include "lin_type.h"


/* Frame Attributes */

#define        LIN_NUMBER_OF_FRAMES    ((l_u8)2)

#define        LIN_FRAME_CTRL_INIT {\
  {{(l_u8)21,(l_u8)64,(l_u8)85,(l_u8)128,(l_u8)8}},\
  {{(l_u8)1,(l_u8)64,(l_u8)193,(l_u8)129,(l_u8)8}}\
}

extern unsigned char SlaveNodeNAD;
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT01 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT02 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT03 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT04 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT05 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT06 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT07 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT08 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT09 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT10 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT11 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT12 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT13 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT14 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT15 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT16 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT17 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT18 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT19 [LIN_NUMBER_OF_FRAMES];
extern const t_Lin_Frame_Ctrl const_LIN_FRAME_CTRL_INIT20 [LIN_NUMBER_OF_FRAMES];

/* Notification Flag API, PID and Message ID Access for whole frames */

/* Use only with LIN 2.0 */
#define        l_get_msg_id_low_frame(x)     (g_lin_frame_ctrl[(x)].frame.msg_id.lo)
#define        l_get_msg_id_high_frame(x)    (g_lin_frame_ctrl[(x)].frame.msg_id.hi)
/* Use only with LIN 2.X */
#define        l_get_pid_frame(x)            (g_lin_frame_ctrl[(x)].frame.pid)
#define        l_set_pid_frame(x,y)          (g_lin_frame_ctrl[(x)].frame.pid = (y))
#define        l_flg_tst_frm_RGBe_Slave()          (g_lin_frame_ctrl[0].frame.frame_type.update_flag == 1u)
#define        l_flg_clr_frm_RGBe_Slave()          (g_lin_frame_ctrl[0].frame.frame_type.update_flag = 0u)
#define        LIN_FRAME_RGBe_Slave()              (g_lin_frame_ctrl[0].frame.pid)
#define        LIN_CHANGE_PID_FRAME_RGBe_Slave(x)  (g_lin_frame_ctrl[0].frame.pid = (x))
#define        LIN_FRAME_LOW_RGBe_Slave_MSG_ID()   (g_lin_frame_ctrl[0].frame.msg_id.lo)
#define        LIN_FRAME_HIGH_RGBe_Slave_MSG_ID()  (g_lin_frame_ctrl[0].frame.msg_id.hi)
#define        l_flg_tst_frm_RGBs_Slave_01()          (g_lin_frame_ctrl[1].frame.frame_type.update_flag == 1u)
#define        l_flg_clr_frm_RGBs_Slave_01()          (g_lin_frame_ctrl[1].frame.frame_type.update_flag = 0u)
#define        LIN_FRAME_RGBs_Slave_01()              (g_lin_frame_ctrl[1].frame.pid)
#define        LIN_CHANGE_PID_FRAME_RGBs_Slave_01(x)  (g_lin_frame_ctrl[1].frame.pid = (x))
#define        LIN_FRAME_LOW_RGBs_Slave_01_MSG_ID()   (g_lin_frame_ctrl[1].frame.msg_id.lo)
#define        LIN_FRAME_HIGH_RGBs_Slave_01_MSG_ID()  (g_lin_frame_ctrl[1].frame.msg_id.hi)

/* Signal Attributes */

/* Signal to Frame Mapping and Signal Update Flags */

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif /* end #ifdef USE_PACKED */
{
  struct
  {
    l_u16 BCM_RGB_Slave_Adresse_f  :1;
    l_u16 BCM_RGB_Adressierungsart_f  :1;
    l_u16 BCM_RGB_Rot_f  :1;
    l_u16 BCM_RGB_Gruen_f  :1;
    l_u16 BCM_RGB_Blau_f  :1;
    l_u16 BCM_RGB_Intensitaet_f  :1;
    l_u16 BCM_RGB_Normierung_Intensitaet_f  :1;
    l_u16 BCM_RGB_Dimmrampe_f  :1;
    l_u16 BCM_RGB_Sonderfunktion_f  :1;
    l_u16 BCM_RGB_Bewertung_Dimmzeit_f  :1;
    l_u16 BCM_RGB_Gueltigkeit_f  :1;
    l_u16 BCM_RGB_Dimmzeit_Zeitbasis_f  :1;
    l_u16 res0  :4;
  }flags;
  l_u16 reg[1];
}t_l_flags_RGBe_Slave;

extern volatile t_l_flags_RGBe_Slave l_flags_RGBe_Slave;

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif /* end #ifdef USE_PACKED */
{
  struct
  {
    l_u16 RGB_Slave_01_ResponseError_f  :1;
    l_u16 RGB_Slave_01_LedRotStg_f  :1;
    l_u16 RGB_Slave_01_LedGruenStg_f  :1;
    l_u16 RGB_Slave_01_LedBlauStg_f  :1;
    l_u16 RGB_Slave_01_LedRotOl_f  :1;
    l_u16 RGB_Slave_01_LedGruenOl_f  :1;
    l_u16 RGB_Slave_01_LedBlauOl_f  :1;
    l_u16 RGB_Slave_01_TempState_f  :1;
    l_u16 RGB_Slave_01_IntErrorRam_f  :1;
    l_u16 RGB_Slave_01_IntErrorRom_f  :1;
    l_u16 RGB_Slave_01_IntErrorNvm_f  :1;
    l_u16 res0  :5;
  }flags;
  l_u16 reg[1];
}t_l_flags_RGBs_Slave_01;

extern volatile t_l_flags_RGBs_Slave_01 l_flags_RGBs_Slave_01;

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif /* end #ifdef USE_PACKED */
{
  l_u8    frame_data[8];
  l_u8    *dataptr;
}t_l_FrmData;

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif /* end #ifdef USE_PACKED */
{
  l_u8    dataBytes[16];
  struct
  {
  t_l_FrmData    l_frm_RGBe_Slave;
  t_l_FrmData    l_frm_RGBs_Slave_01;
  }frames;
  t_l_FrmData    Frame[LIN_NUMBER_OF_FRAMES];
}t_l_Lin_Data;
extern t_l_Lin_Data    l_LinData;

extern const t_l_Lin_Data const_LinData;

/* Signal Access Macros */

/* Signal Read Access for Signal BCM_RGB_Slave_Adresse */
#define        l_u16_rd_BCM_RGBa_Slave_Adresse()    (((masterSendBuffer[0]) & 255u) | \
                                    ((l_u16)(((masterSendBuffer[1]) & (127u)) << 8u)))
//#define        l_flg_tst_BCM_RGBa_Slave_Adresse()    (l_flags_RGBe_Slave.flags.BCM_RGB_Slave_Adresse_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Slave_Adresse()    (l_flags_RGBe_Slave.flags.BCM_RGB_Slave_Adresse_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Adressierungsart */
#define        l_bool_rd_BCM_RGBa_Adressierungsart()   ((masterSendBuffer[1] >> 7u) & 1u)
//#define        l_flg_tst_BCM_RGBa_Adressierungsart()    (l_flags_RGBe_Slave.flags.BCM_RGB_Adressierungsart_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Adressierungsart()    (l_flags_RGBe_Slave.flags.BCM_RGB_Adressierungsart_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Rot */
#define        l_u8_rd_BCM_RGBa_Rot()     ((masterSendBuffer[2]) & 255u)
//#define        l_flg_tst_BCM_RGBa_Rot()    (l_flags_RGBe_Slave.flags.BCM_RGB_Rot_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Rot()    (l_flags_RGBe_Slave.flags.BCM_RGB_Rot_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Gruen */
#define        l_u8_rd_BCM_RGBa_Gruen()     ((masterSendBuffer[3]) & 255u)
//#define        l_flg_tst_BCM_RGBa_Gruen()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gruen_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Gruen()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gruen_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Blau */
#define        l_u8_rd_BCM_RGBa_Blau()     ((masterSendBuffer[4]) & 255u)
//#define        l_flg_tst_BCM_RGBa_Blau()    (l_flags_RGBe_Slave.flags.BCM_RGB_Blau_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Blau()    (l_flags_RGBe_Slave.flags.BCM_RGB_Blau_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Intensitaet */
#define        l_u8_rd_BCM_RGBa_Intensitaet()     ((masterSendBuffer[5]) & 127u)
//#define        l_flg_tst_BCM_RGBa_Intensitaet()    (l_flags_RGBe_Slave.flags.BCM_RGB_Intensitaet_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Intensitaet()    (l_flags_RGBe_Slave.flags.BCM_RGB_Intensitaet_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Normierung_Intensitaet */
#define        l_bool_rd_BCM_RGBa_Normierung_Intensitaet()   ((masterSendBuffer[5] >> 7u) & 1u)
//#define        l_flg_tst_BCM_RGBa_Normierung_Intensitaet()    (l_flags_RGBe_Slave.flags.BCM_RGB_Normierung_Intensitaet_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Normierung_Intensitaet()    (l_flags_RGBe_Slave.flags.BCM_RGB_Normierung_Intensitaet_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Dimmrampe */
#define        l_u8_rd_BCM_RGBa_Dimmrampe()     ((masterSendBuffer[6]) & 15u)
//#define        l_flg_tst_BCM_RGBa_Dimmrampe()    (l_flags_RGBe_Slave.flags.BCM_RGB_Dimmrampe_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Dimmrampe()    (l_flags_RGBe_Slave.flags.BCM_RGB_Dimmrampe_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Sonderfunktion */
#define        l_u8_rd_BCM_RGBa_Sonderfunktion()     ((masterSendBuffer[6] >> 5u) & 7u)
//#define        l_flg_tst_BCM_RGBa_Sonderfunktion()    (l_flags_RGBe_Slave.flags.BCM_RGB_Sonderfunktion_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Sonderfunktion()    (l_flags_RGBe_Slave.flags.BCM_RGB_Sonderfunktion_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Bewertung_Dimmzeit */
#define        l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit()   ((masterSendBuffer[7]) & 1u)
//#define        l_flg_tst_BCM_RGBa_Bewertung_Dimmzeit()    (l_flags_RGBe_Slave.flags.BCM_RGB_Bewertung_Dimmzeit_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Bewertung_Dimmzeit()    (l_flags_RGBe_Slave.flags.BCM_RGB_Bewertung_Dimmzeit_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Gueltigkeit */
#define        l_bool_rd_BCM_RGBa_Gueltigkeit()   ((masterSendBuffer[7] >> 1u) & 1u)
//#define        l_flg_tst_BCM_RGBa_Gueltigkeit()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gueltigkeit_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Gueltigkeit()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gueltigkeit_f = 0u)

/* Signal Read Access for Signal BCM_RGB_Dimmzeit_Zeitbasis */
#define        l_bool_rd_BCM_RGBa_Dimmzeit_Zeitbasis()   ((masterSendBuffer[7] >> 2u) & 1u)
//#define        l_flg_tst_BCM_RGBa_Dimmzeit_Zeitbasis()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gueltigkeit_f == 1u)
//#define        l_flg_clr_BCM_RGBa_Dimmzeit_Zeitbasis()    (l_flags_RGBe_Slave.flags.BCM_RGB_Gueltigkeit_f = 0u)



/* Signal Write Access for Signal RGB_Slave_01_ResponseError */
#define        l_bool_wr_RGB_Slave_01_ResponseError(x)    do{ masterReadBuffer[0] &= (127u); \
                                                masterReadBuffer[0] |= ((l_u8)((x) << 7u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedRotStg */
#define        l_u8_wr_RGB_Slave_01_LedRotStg(x)    do{ masterReadBuffer[1] &= (252u); \
                                                masterReadBuffer[1] |= ((l_u8)(x)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedGruenStg */
#define        l_u8_wr_RGB_Slave_01_LedGruenStg(x)    do{ masterReadBuffer[1] &= (243u); \
                                                masterReadBuffer[1] |= ((l_u8)((x) << 2u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedBlauStg */
#define        l_u8_wr_RGB_Slave_01_LedBlauStg(x)    do{ masterReadBuffer[1] &= (207u); \
                                                masterReadBuffer[1] |= ((l_u8)((x) << 4u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedRotOl */
#define        l_u8_wr_RGB_Slave_01_LedRotOl(x)    do{ masterReadBuffer[1] &= (63u); \
                                                masterReadBuffer[1] |= ((l_u8)((x) << 6u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedGruenOl */
#define        l_u8_wr_RGB_Slave_01_LedGruenOl(x)    do{ masterReadBuffer[2] &= (252u); \
                                                masterReadBuffer[2] |= ((l_u8)(x)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_LedBlauOl */
#define        l_u8_wr_RGB_Slave_01_LedBlauOl(x)    do{ masterReadBuffer[2] &= (243u); \
                                                masterReadBuffer[2] |= ((l_u8)((x) << 2u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_TempState */
#define        l_u8_wr_RGB_Slave_01_TempState(x)    do{ masterReadBuffer[2] &= (143u); \
                                                masterReadBuffer[2] |= ((l_u8)((x) << 4u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_IntErrorRam */
#define        l_u8_wr_RGB_Slave_01_IntErrorRam(x)    do{ masterReadBuffer[3] &= (252u); \
                                                masterReadBuffer[3] |= ((l_u8)(x)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_IntErrorRom */
#define        l_u8_wr_RGB_Slave_01_IntErrorRom(x)    do{ masterReadBuffer[3] &= (243u); \
                                                masterReadBuffer[3] |= ((l_u8)((x) << 2u)); \
                                                }while(0)

/* Signal Write Access for Signal RGB_Slave_01_IntErrorNvm */
#define        l_u8_wr_RGB_Slave_01_IntErrorNvm(x)    do{ masterReadBuffer[3] &= (207u); \
                                                masterReadBuffer[3] |= ((l_u8)((x) << 4u)); \
                                                }while(0)

/* Lin Driver Access to Update Signal Flags for Frame RGBe_Slave */
#define        l_Update_flags_frame0()    (l_flags_RGBe_Slave.reg[0]=(l_u16)0xFFFFu)

/* Lin Driver Access to Update Signal Flags for Frame RGBs_Slave_01 */
#define        l_Update_flags_frame1()    (l_flags_RGBs_Slave_01.reg[0]=(l_u16)0xFFFFu)

#define        l_Update_flags_frame(x)    (l_Update_Frame_Flags((x))) 

void l_Update_Frame_Flags(l_u8 number);

/* Prototype for Byte Array Signal Access Function  */
void l_get_byte_array(l_u8 start[], l_u8  count, l_u8 destination[]);
void l_set_byte_array(l_u8 start[], l_u8  count, const l_u8 source[]);

/* Macros for Response Error Flag / Frame handling */

#define        l_Set_Response_Error_Flag()   do{ l_bool_wr_RGB_Slave_01_ResponseError(1u); } while (0)

#define        l_Reset_Response_Error_Flag() do{ l_bool_wr_RGB_Slave_01_ResponseError(0u); } while (0)

#define        LIN_Response_Error_Frame_PID  (g_lin_frame_ctrl[1].frame.pid)


/* File Footer */
#endif /* end #ifndef GEN_LIN_CONFIG_H */

/* genLinConfig.h file for E521.36 */
