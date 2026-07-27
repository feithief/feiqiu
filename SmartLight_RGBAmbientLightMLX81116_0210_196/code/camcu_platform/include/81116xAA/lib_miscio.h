/**
 * @file
 * @brief Project specific interrupts, IO's and another settings control
 * @internal
 *
 * @copyright (C) 2017-2018 Melexis N.V.
 * git 6d04ab96
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup CAMCU_library
 *
 * @details
 */

#include "compiler_abstraction.h"
#include "io.h"

/** UvVdda_ClearInt clears the UV_VDDA interrupt pending flag */
INLINE void UvVdda_ClearInt(void)
{
    IO_SET(MLX16, UV_VDDA_PEND, 1u);
}

/** UvVs_ClearInt clears the UV_VS interrupt pending flag */
INLINE void UvVs_ClearInt(void)
{
    IO_SET(MLX16, UV_VS_PEND, 1u);
}

/** Ovt_ClearInt clears the OVT (Overtemperature) interrupt pending flag */
INLINE void Ovt_ClearInt(void)
{
    IO_SET(MLX16, OVT_PEND, 1u);
}

/** OvVs_ClearInt clears the OV_VS interrupt pending flag */
INLINE void OvVs_ClearInt(void)
{
    IO_SET(MLX16, OV_VS_PEND, 1u);
}

/** OvtWarn_ClearInt clears the OVT_WARN (Overtemperature) interrupt pending flag */
INLINE void OvtWarn_ClearInt(void)
{
    IO_SET(MLX16, OVT_WARN_PEND, 1u);
}

/** UvVdd5_ClearInt clears the UV_VDD5 interrupt pending flag */
INLINE void UvVdd5_ClearInt(void)
{
    IO_SET(MLX16, UV_VDD5_PEND, 1u);
}

/** AaIn_ClearInt clears the AA_IN interrupt pending flag */
INLINE void AaIn_ClearInt(void)
{
#ifdef IO_MLX16__AA_IN_PEND_GET
    IO_SET(MLX16, AA_IN_PEND, 1u);
#endif
#ifdef IO_MLX16__AA_IN_FE_PEND_GET
    IO_SET(MLX16, AA_IN_FE_PEND, 1u);
#endif
}

/** UvVdda_EnableInt enables the UV_VDDA interrupt and clears its pending bit */
INLINE void UvVdda_EnableInt(void)
{
    UvVdda_ClearInt();
    IO_SET(MLX16, UV_VDDA_ITC, 1u);
}

/** UvVs_EnableInt enables the UV_VS interrupt and clears its pending bit */
INLINE void UvVs_EnableInt(void)
{
    UvVs_ClearInt();
    IO_SET(MLX16, UV_VS_ITC, 1u);
}

/** Ovt_EnableInt enables the OVT interrupt and clears its pending bit */
INLINE void Ovt_EnableInt(void)
{
    Ovt_ClearInt();
    IO_SET(MLX16, OVT_ITC, 1u);
}

/** OvVs_EnableInt enables the OV_VS interrupt and clears its pending bit */
INLINE void OvVs_EnableInt(void)
{
    OvVs_ClearInt();
    IO_SET(MLX16, OV_VS_ITC, 1u);
}

/** OvtWarn_EnableInt enables the OVT_WARN interrupt and clears its pending bit */
INLINE void OvtWarn_EnableInt(void)
{
    OvtWarn_ClearInt();
    IO_SET(MLX16, OVT_WARN_ITC, 1u);
}

/** UvVdd5_EnableInt enables the UV_VDD5 interrupt and clears its pending bit */
INLINE void UvVdd5_EnableInt(void)
{
    UvVdd5_ClearInt();
    IO_SET(MLX16, UV_VDD5_ITC, 1u);
}

/** AaIn_EnableInt enables the AA_IN interrupt and clears its pending bit */
INLINE void AaIn_EnableInt(void)
{
    AaIn_ClearInt();
#ifdef IO_MLX16__AA_IN_ITC_GET
    IO_SET(MLX16, AA_IN_ITC, 1u);
#endif
#ifdef IO_MLX16__AA_IN_FE_ITC_GET
    IO_SET(MLX16, AA_IN_FE_ITC, 1u);
#endif
}

/** UvVdda_DisableInt disables the UV_VDDA interrupt */
INLINE void UvVdda_DisableInt(void)
{
    IO_SET(MLX16, UV_VDDA_ITC, 0u);
}

/** UvVs_DisableInt disables the UV_VS interrupt */
INLINE void UvVs_DisableInt(void)
{
    IO_SET(MLX16, UV_VS_ITC, 0u);
}

/** Ovt_DisableInt disables the OVT interrupt */
INLINE void Ovt_DisableInt(void)
{
    IO_SET(MLX16, OVT_ITC, 0u);
}

/** OvVs_DisableInt disables the OV_VS interrupt */
INLINE void OvVs_DisableInt(void)
{
    IO_SET(MLX16, OV_VS_ITC, 0u);
}

/** OvtWarn_DisableInt disables the OVT_WARN interrupt */
INLINE void OvtWarn_DisableInt(void)
{
    IO_SET(MLX16, OVT_WARN_ITC, 0u);
}

/** UvVdd5_DisableInt disables the UV_VDD5 interrupt */
INLINE void UvVdd5_DisableInt(void)
{
    IO_SET(MLX16, UV_VDD5_ITC, 0u);
}

/** AaIn_DisableInt disables the AA_IN interrupt */
INLINE void AaIn_DisableInt(void)
{
#ifdef IO_MLX16__AA_IN_ITC_GET
    IO_SET(MLX16, AA_IN_ITC, 0u);
#endif
#ifdef IO_MLX16__AA_IN_FE_ITC_GET
    IO_SET(MLX16, AA_IN_FE_ITC, 0u);
#endif
}

/* Miscellanious IO-ports getters */

/** VDDA Under-voltage interrupt flag */
INLINE uint16_t UvVdda_GetInt(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VDDA_IT);
}

INLINE uint16_t UvVdda_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VDDA_SYNC);
}

/** VS Under-voltage interrupt flag */
INLINE uint16_t UvVs_GetInt(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VS_IT);
}

INLINE uint16_t UvVs_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VS_SYNC);
}

/** VS Over-voltage interrupt flag */
INLINE uint16_t OvVs_GetInt(void)
{
    return IO_GET(PORT_SUPP_IN, OV_VS_IT);
}

INLINE uint16_t OvVs_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, OV_VS_SYNC);
}

INLINE uint16_t Ovt_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, OVT_SYNC);
}

/** Overtemperature warning interrupt flag */
INLINE uint16_t OvtWarn_GetInt(void)
{
    return IO_GET(PORT_SUPP_IN, OVT_WARN_IT);
}

INLINE uint16_t OvtWarn_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, OVT_WARN_SYNC);
}

/** VDD5 Under-voltage warning interrupt flag */
INLINE uint16_t UvVdd5_GetInt(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VDD5_IT);
}

INLINE uint16_t UvVdd5_GetSync(void)
{
    return IO_GET(PORT_SUPP_IN, UV_VDD5_SYNC);
}

/** VDD5 Under-voltage warning interrupt flag */
INLINE uint16_t AinSup_GetInt(void)
{
    return IO_GET(PORT_MISC_IN, AIN_SUP);
}

/** getStopMode returns state after digital power up
 *
 * @retval 1: return from STOP_MODE
 * @retval 0: return from POR or SLEEP
 */
INLINE uint16_t getStopMode(void)
{
    return IO_GET(PORT_MISC_IN, STOP_MODE);
}

/** getRStat returns status of RSTAT flip flop in analog */
INLINE uint16_t getRStat(void)
{
    return IO_GET(PORT_MISC_IN, RSTAT);
}

/** getInternalWu indicates wake up from internal timer condition */
INLINE uint16_t getInternalWu(void)
{
    return IO_GET(PORT_MISC_IN, INTERNAL_WU);
}

/** getComWu indicates MeLiBu as wake up source */
INLINE uint16_t getComWu(void)
{
    return IO_GET(PORT_MISC_IN, LIN_WU);
}

/** getPhyStat1 The signal From MeLiBu Physical layer */
INLINE uint16_t getPhyStat1(void)
{
    return IO_GET(PORT_MISC_IN, PHYSTAT1);
}

/** getAaOutIn returns the AA value for analog */
INLINE uint16_t getAaOutIn(void)
{
    return IO_GET(PORT_MISC_IN, AAOUT_IN);
}


/* Miscellanious output signals' support */

/** setPrUvVs sets under voltage programming for VS;
 *
 * @param[in] newValue 3-bit value to program the under-voltage
 *
 * detection level (V) = (PRUV_VS+4) -> from 4 to 9V
 */
INLINE void setPrUvVs(uint16_t newValue)
{
    IO_SET(PORT_MISC_OUT, PRUV_VS, newValue);
}

/** setPrOvVs control for VS over voltage monitor
 *
 * @param[in] newValue 2-bit value
 */
INLINE void setPrOvVs(uint16_t newValue)
{
    IO_SET(PORT_MISC_OUT, PROV_VS, newValue);
}

/** setPrUvVdda under voltage programming for VDDA
 *
 * @param[in] newValue 1-bit value
 */
INLINE void setPrUvVdda(uint16_t newValue)
{
    IO_SET(PORT_MISC_OUT, PRUV_VDDA, newValue);
}

/** setSelTemp select the temperature diode input for the temp. sensor
 *
 * @param[in] newValue 4-bit value
 */
INLINE void setSelTemp(uint16_t newValue)
{
    IO_SET(PORT_MISC_OUT, SEL_TEMP, newValue);
}

/** setOvtDisable disables the overtemperature signal
 *
 * @param[in] newValue
 */
INLINE void setOvtDisable(uint16_t newValue)
{
    IO_SET(PORT_MISC2_OUT, OVT_DISABLE, newValue);
}

/** setSwitchOffUvVdd5 disables the under-voltage VDD5 signal
 *
 * @param[in] newValue
 */
INLINE void setSwitchOffUvVdd5(uint16_t newValue)
{
    IO_SET(PORT_MISC2_OUT, SWITCHOFFUV_VDD5, newValue);
}

/** Enables communication transmitter
 *
 * @param[in] newValue
 */
INLINE void setEnableTx(uint16_t newValue)
{
    IO_SET(PORT_MISC2_OUT, ENABLE_TX, newValue);
}

/**
 *
 * @param[in] newValue
 */
INLINE void setEnInAaIn(uint16_t newValue)
{
    IO_SET(PORT_MISC2_OUT, EN_IN_AAIN, newValue);
}



/**  Complex Timer channel selection.
 *
 *   For every Complex Timer we have 2 inputs Channel A and Channel B separately connectable to each MS_IO port. */
typedef enum CTimerCHinputSelect_e {
    CTIMER_IN_CH_PWM0     = 0U,
    CTIMER_IN_CH_PWM1     = 1U,
    CTIMER_IN_CH_PWM2     = 2U,
    CTIMER_IN_CH_PWM3     = 3U,
    CTIMER_IN_CH_PWM4     = 4U,
    CTIMER_IN_CH_PWM5     = 5U,
    CTIMER_IN_CH_PWM6     = 6U,
    CTIMER_IN_CH_PWM7     = 7U,
    CTIMER_IN_CH_PWM8     = 8U,
    CTIMER_IN_CH_PWM9     = 9U,
    CTIMER_IN_CH_PWM10    =10U,
    CTIMER_IN_CH_PWM11    =11U,
    CTIMER_IN_CH_URX      =12U,
    CTIMER_IN_AA_IN       =13U
} CTimerCHinputSelect_t;


/* EOF */
