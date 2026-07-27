/***************************************************************************//**
 * @file			LinProto_Signal.h
 *
 * @creator		sbai
 * @created		13.01.2015
 *
 * @brief  		Definition of the 'Signal' data types for the
 *            'LIN PROTO Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup PROTO PROTO Layer
 * @{
 *
 * $Id$
 *
 * $Revision$
 *
 * @misra{M3CM Rule-21.2. and 11.1. - PRQA Msg 4603 and 4604,
 * A real time operating system is not used in this project and the
 * signal.h library, either. Signal is also an item in the LIN protocol
 * layer specification.,
 * Could cause conflicts in functions with the parameter signal.,
 * None.}
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 ******************************************************************************/

#ifndef LINPROTO_SIGNAL_H_
#define LINPROTO_SIGNAL_H_

/* ****************************************************************************/
/* ********************** MODULE MISRA RULE EXCEPTIONS ***************************/
/* ****************************************************************************/
/* PRQA S 1535 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types are defined that are not used in this project scope. */
/* Rule: 1535 MISRA a Rule-2.3
   Circumstances: The typedef '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinProto_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief LIN Protocol Layer signal types.
 *
 * @see LIN 2.2 Specification - Chapter 2.2.1 SIGNAL TYPES
 ******************************************************************************/
enum LinProtoIf_eSignalType
{
  LinProtoIf_SignalType_Invalid                        = 0, /**< Invalid signal type. */
  LinProtoIf_SignalType_Scalar_MEM8                    = 1, /**< Scalar with 8-bit memory access. */
  LinProtoIf_SignalType_Scalar_MEM16                   = 2, /**< Scalar with 16-bit memory access.  */
  LinProtoIf_SignalType_Scalar_FNC_WITH_OWN_CTX        = 3, /**< Scalar build by a function with callback context data from 'Signal Definition'.  */
  LinProtoIf_SignalType_Scalar_FNC_WITH_DESCLST_CTX    = 4, /**< Scalar build by a function with callback context data from 'Frame Description List'.  */
  LinProtoIf_SignalType_ByteArray_MEM                  = 5, /**< Byte array with 8-Bit memory access. */
  LinProtoIf_SignalType_ByteArray_FNC_WITH_OWN_CTX     = 6, /**< Byte array build by a function with callback context data from 'Signal Definition'.  */
  LinProtoIf_SignalType_ByteArray_FNC_WITH_DESCLST_CTX = 7  /**< Byte array build by a function with callback context data from 'Frame Description List'.  */
};

typedef enum LinProtoIf_eSignalType LinProtoIf_eSignalType_t; /**< Typedef of LinProtoIf_eSignalType */

typedef Lin_uint8_t LinProtoIf_SignalLenght_t; /**< Data type for the length of a signal. */

typedef Lin_uint8_t LinProtoIf_SignalOffset_t; /**< Data type for the signal offset. */

typedef Lin_uint8_t * LinProtoIf_pScalarDataMem8_t; /**< Data type for 8-bit memory access to an scalar. */

typedef Lin_uint16_t * LinProtoIf_pScalarDataMem16_t; /**< Data type for 16-bit memory access to an scalar. */

typedef void * LinProtoIf_pSignalData_t; /**< Data type for a pointer to the signal data. */

typedef void * LinProtoIf_pCtxData_t; /**< Typedef of pointer to signal callback context data. */

/***************************************************************************//**
 * @brief LIN Protocol Layer signal definition struct.
 *
 * @see LIN 2.2 Specification - Chapter 2.2 SIGNAL MANAGEMENT
 ******************************************************************************/
struct LinProtoIf_sSignal
{
  LinProtoIf_eSignalType_t  Type;        /**< @copydoc LinProtoIf_eSignalType_t */
  LinProtoIf_SignalLenght_t Length;      /**< Length of the signal. */
  LinProtoIf_SignalOffset_t Offset;      /**< Offset of the signal. */
  LinProtoIf_pSignalData_t  PtrToData;   /**< @copydoc LinProtoIf_pSignalData_t */
  LinProtoIf_pCtxData_t     CtxData;     /**< @copydoc LinProtoIf_pCtxData_t */
};

typedef struct LinProtoIf_sSignal     LinProtoIf_sSignal_t;  /**< Typedef for LinProtoIf_sSignal. */
typedef        LinProtoIf_sSignal_t*  LinProtoIf_pSignal_t;  /**< Typedef of pointer to LinProtoIf_sSignal. */
typedef const  LinProtoIf_sSignal_t*  LinProtoIf_cpSignal_t; /**< Typedef of constant pointer to LinProtoIf_sSignal. */
typedef        LinProtoIf_sSignal_t** LinProtoIf_ppSignal_t; /**< Typedef of pointer-pointer to LinProtoIf_sSignal. */

/** @addtogroup LinProtoIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Callback function to build published scalar signal.
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param signal[in]                Pointer to 'Signal' associated to the
 *                                  received Frame ID.
 * @param data[out]                 Pointer to the buffer where write the data to.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return  LIN_TRUE if the user application or higher level has written the
 *          data to the buffer.
 *
 * This function type is called if the LIN frame in the
 * 'Frame Description List' is configured to be build by 'Signal List' and in the
 * 'Signal List' the signal is defined as a published scalar build by a function.
 *
 * @see LinProtoIf_eFrameKind, LinProtoIf_eSignalType
 *
 * @par "Published Scalar:"
 * @mscfile msc_proto_pubbuildsignalcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_pPublScalarDataFunction_t)  (LinProtoIf_pGenericEnvData_t genericProtoEnvData, LinProtoIf_pSignal_t           signal,
                                                             Lin_uint16_t*                data,                LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Callback function to build subscribed scalar signal.
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param signal[in]                Pointer to 'Signal' associated to the
 *                                  received Frame ID.
 * @param data[in]                  Pointer to the buffer where to read the data from.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return  LIN_TRUE if the user application or higher level has read the
 *          data from the buffer.
 *
 * This function type is called if the LIN frame in the
 * 'Frame Description List' is configured to be build by 'Signal List' and in the
 * 'Signal List' the signal is defined as a subscribed scalar build by a function.
 *
 * @see LinProtoIf_eFrameKind, LinProtoIf_eSignalType
 *
 * @par "Subscribed Scalar:"
 * @mscfile msc_proto_subscrbuildsignalcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_pSubscrScalarDataFunction_t) (LinProtoIf_pGenericEnvData_t genericProtoEnvData, LinProtoIf_pSignal_t           signal,
                                                              Lin_uint16_t                 data,                LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Callback function to build 'Byte Array' signal.
 *        Published and subscribed.
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param signal[in]                Pointer to 'Signal' associated to the
 *                                  received Frame ID.
 * @param msgBuf[in,out]            Pointer to the buffer.
 * @param msgBufLen[in]             Length of the buffer.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return  LIN_TRUE if the user application or higher level has read the
 *          data from or written to the buffer.
 *
 * This function type is called if the LIN frame in the
 * 'Frame Description List' is configured to be build by 'Signal List' and in the
 * 'Signal List' the signal is defined as a byte array build by a function.
 *
 * @see LinProtoIf_eFrameKind, LinProtoIf_eSignalType
 *
 * @par "Published Byte Array:"
 * @mscfile msc_proto_pubbuildbytearraycbcalled.dox
 * @n
 * @par "Subscribed Byte Array:"
 * @mscfile msc_proto_subscrbuildbytearraycbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_pByteArrayDataFunction_t)   (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_pSignal_t   signal,
                                                             LinProtoIf_pData_t             msgBuf,              LinProtoIf_BufLength_t msgBufLen,
                                                             LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/** @} LinProtoIfCbFunDefs */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINPROTO_SIGNAL_H_ */


/*! @} */ /* PROTO */
/*! @} */ /* ELMOS_LIN_DRIVER */
