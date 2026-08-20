/**
 *****************************************************************************
 * @brief   pal lin tl header file.
 *
 * @file    pal_lin_tl.h
 * @author  AE/FAE team
 * @date    2024.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __PAL_LIN_TL_H__
#define __PAL_LIN_TL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define POSITIVE 1
#define NEGATIVE 0

#define LIN_BROADCAST_NAD                     0x7F   /* NAD */
#define LIN_FUNCTION_NAD                      0x7E   /* function nad */
#define LIN_RES_NEGATIVE                      0x7F   /* negative response */

/**
 * @brief lin negative response
 */
#define RES_NEGATIVE                        0x7F      /* negative response */
/* -- Negative Response Code (NRC) definition [ ISO14229-1-2020 p.390 ]   -- */
#define UDS_NRC_NULL        0x00
#define UDS_NRC_GR          0x10    /* generalReject                         */
#define UDS_NRC_SNS         0x11    /* serviceNotSupported                   */
#define UDS_NRC_SFNS        0x12    /* SubFunctionNotSupported               */
#define UDS_NRC_IMLOIF      0x13    /* incorrectMessageLengthOrInvalidFormat */
#define UDS_NRC_RTL         0x14    /* responseTooLong                       */
#define UDS_NRC_BRR         0x21    /* busyRepeatRequest                     */
#define UDS_NRC_CNC         0x22    /* conditionsNotCorrect                  */
#define UDS_NRC_RSE         0x24    /* requestSequenceError                  */
#define UDS_NRC_NRFSC       0x25    /* noResponseFromSubnetComponent         */
#define UDS_NRC_FPEORA      0x26    /* FailurePreventsExecutionOfReq.Action  */
#define UDS_NRC_ROOR        0x31    /* requestOutOfRange                     */
#define UDS_NRC_SAD         0x33    /* securityAccessDenied                  */
#define UDS_NRC_AR          0x34    /* authenticationRequired                */
#define UDS_NRC_IK          0x35    /* invalidKey                            */
#define UDS_NRC_ENOA        0x36    /* exceedNumberOfAttempts                */
#define UDS_NRC_RTDNE       0x37    /* requiredTimeDelayNotExpired           */
#define UDS_NRC_SDTR        0x38    /* secureDataTransmissionRequired        */
#define UDS_NRC_SDTNA       0x39    /* secureDataTransmissionNotAllowed      */
#define UDS_NRC_SDTF        0x3A    /* secureDataVerificationFailed          */
#define UDS_NRC_CVFITP      0x50    /* Cert. vrf. fld. inv. Time Period      */
#define UDS_NRC_CVFISI      0x51    /* Cert. vrf. fld. inv. Signature        */
#define UDS_NRC_CVFICOT     0x52    /* Cert. vrf. fld. inv. Chain of Trust   */
#define UDS_NRC_CVFIT       0x53    /* Cert. verif. failed. Inv. Type        */
#define UDS_NRC_CVFIF       0x54    /* Cert. verif. failed. Inv. Format      */
#define UDS_NRC_CVFICO      0x55    /* Cert. verif. failed. Inv. Content     */
#define UDS_NRC_CVFISC      0x56    /* Cert. verif. failed. Inv. Scope       */
#define UDS_NRC_CVFICE      0x57    /* Cert. verif. failed. Inv. Cert.(rvk)  */
#define UDS_NRC_OVF         0x58    /* Ownership verification failed         */
#define UDS_NRC_CCF         0x59    /* Challenge calculation failed          */
#define UDS_NRC_SARF        0x5A    /* Setting Access Rights failed          */
#define UDS_NRC_SKCDF       0x5B    /* Session key cr/tn-deri/vtion failed   */
#define UDS_NRC_CDUF        0x5C    /* Configuration data usage failed       */
#define UDS_NRC_DAF         0x5D    /* DeAuthentication failed               */
#define UDS_NRC_UDNA        0x70    /* uploadDownloadNotAccepted             */
#define UDS_NRC_TDS         0x71    /* transferDataSuspended                 */
#define UDS_NRC_GPF         0x72    /* generalProgrammingFailure             */
#define UDS_NRC_WBSC        0x73    /* wrongBlockSequenceCounter             */
#define UDS_NRC_RCRRP       0x78    /* req.Received-Resp.Pending             */
#define UDS_NRC_SFNSIAS     0x7E    /* SubFunc. NotSupp. In Actv.Session     */
#define UDS_NRC_SNSIAS      0x7F    /* serviceNotSupportedInActiveSession    */
#define UDS_NRC_RPMTH       0x81    /* rpmTooHigh                            */
#define UDS_NRC_RPMTL       0x82    /* rpmTooLow                             */
#define UDS_NRC_EIR         0x83    /* engineIsRunning                       */
#define UDS_NRC_EINR        0x84    /* engineIsNotRunning                    */
#define UDS_NRC_ERTTL       0x85    /* engineRunTimeTooLow                   */
#define UDS_NRC_TEMPTH      0x86    /* temperatureTooHigh                    */
#define UDS_NRC_TEMPTL      0x87    /* temperatureTooLow                     */
#define UDS_NRC_VSTH        0x88    /* vehicleSpeedTooHigh                   */
#define UDS_NRC_VSTL        0x89    /* vehicleSpeedTooLow                    */
#define UDS_NRC_TPTH        0x8A    /* throttle/PedalTooHigh                 */
#define UDS_NRC_TPTL        0x8B    /* throttle/PedalTooLow                  */
#define UDS_NRC_TRNIN       0x8C    /* transmissionRangeNotInNeutral         */
#define UDS_NRC_TRNIG       0x8D    /* transmissionRangeNotInGear            */
#define UDS_NRC_BSNC        0x8F    /* brakeSwitch(es)NotClosed              */
#define UDS_NRC_SLNIP       0x90    /* shifterLeverNotInPark                 */
#define UDS_NRC_TCCL        0x91    /* torqueConverterClutchLocked           */
#define UDS_NRC_VTH         0x92    /* voltageTooHigh                        */
#define UDS_NRC_VTL         0x93    /* voltageTooLow                         */
#define UDS_NRC_RTNA        0x94    /* ResourceTemporarilyNotAvailable       */

/**
  * @brief  lin event type enumeration
  */
typedef enum
{
    LIN_EVENT_PID_OK,               /**< LIN_EVENT_PID_OK */
    LIN_EVENT_TX_COMPLETED,         /**< LIN_EVENT_TX_COMPLETED */
    LIN_EVENT_RX_COMPLETED,         /**< LIN_EVENT_RX_COMPLETED */
    LIN_EVENT_PID_ERR,              /**< LIN_EVENT_PID_ERR */
    LIN_EVENT_CHECKSUM_ERR,         /**< LIN_EVENT_CHECKSUM_ERR */

    LIN_EVENT_SYNC_VALUE_ERR,       /**< LIN_EVENT_SYNC_VALUE_ERR */
    LIN_EVENT_RX_PTY_CHK_ERR,       /**< LIN_EVENT_RX_PTY_CHK_ERR */
    LIN_EVENT_RX_TIMEOUT,           /**< LIN_EVENT_RX_TIMEOUT */
    LIN_EVENT_TX_RX_CONF,           /**< LIN_EVENT_TX_RX_CONF */
    LIN_EVENT_TX_PID_DONE,          /**< LIN_EVENT_TX_PID_DONE */
    LIN_EVENT_RX_BYTE,              /**< LIN_EVENT_RX_BYTE */
} lin_event_type_e;

/**
  * @brief  lin bus state enumeration
  */
typedef enum
{
    LIN_BUS_IDLE           = 0,
    LIN_BUS_SEND          = 1,
    LIN_BUS_RECV,
    LIN_BUS_ERROR,
} lin_bus_state_e;

/**
  * @brief  lin transport layer data
  */
typedef uint8_t lin_tl_data[8];

/**
  * @brief  lin packet struct
  */
typedef struct
{
    uint8_t id;
    uint8_t len;
    uint8_t buff[8];
} lin_packet_t;

/**
  * @brief  lin  transport layer queue struct
  */
typedef struct
{
    bool            ready;
    uint8_t         header;           /* the first element of queue */
    uint8_t         tail;             /* the last element of queue */
    uint16_t        frame_index;      /* the length of data */
    uint8_t         frame_byte;       /* the byte cnt of a frame */
    uint8_t         pid;              /* the pid of a frame */
    lin_tl_data     *tl_data;         /* the ptr of lin data */
} lin_tl_queue_t;

/**
  * @brief  lin  recv context struct
  */
typedef struct
{
    uint8_t         nad;
    uint8_t         pci;
    uint8_t         sid;
    uint16_t        remain_length;
    uint16_t        total_length;
    uint8_t         frame_index;
} lin_recv_context_t;

/*-------------Function port--------------------------*/
void lin_tl_init(void);
bool lin_uds_send(uint8_t nad, uint8_t *data, uint16_t length);
bool lin_uds_negative_response(uint8_t nad, uint8_t sid, uint8_t error_code);
bool lin_uds_receive(uint8_t nad, uint8_t *data, uint16_t *length);
bool lin_tl_uncd_frame_get(lin_bus_e bus, uint8_t *pid, uint8_t *buffer);
#ifdef __cplusplus
}
#endif
#endif
