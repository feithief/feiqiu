#include "Rte_Com.h"

#include "genLinConfig.h"
#include "lin_main.h"

bool Rte_IsUpdated_LightCommand(void)
{
    return (l_flg_tst_frm_RGBe_Slave_a() != 0U);
}

Std_ReturnType Rte_Read_LightCommand(Rte_LightCommandType *CommandPtr)
{
    if ((CommandPtr == NULL) || !Rte_IsUpdated_LightCommand())
    {
        return E_NOT_OK;
    }

    CommandPtr->slave_address = l_u16_rd_BCM_RGBa_Slave_Adresse();
    CommandPtr->group_addressing =
        (l_bool_rd_BCM_RGBa_Adressierungsart() != 0U);
    CommandPtr->red = l_u8_rd_BCM_RGBa_Rot();
    CommandPtr->green = l_u8_rd_BCM_RGBa_Gruen();
    CommandPtr->blue = l_u8_rd_BCM_RGBa_Blau();
    CommandPtr->brightness = l_u8_rd_BCM_RGBa_Intensitaet();
    CommandPtr->brightness_normalized =
        (l_bool_rd_BCM_RGBa_Normierung_Intensitaet() != 0U);
    CommandPtr->dimming_time = l_u8_rd_BCM_RGBa_Dimmrampe();
    CommandPtr->work_mode = l_u8_rd_BCM_RGBa_Sonderfunktion();
    CommandPtr->dimming_time_relative =
        (l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit() != 0U);
    CommandPtr->valid = (l_bool_rd_BCM_RGBa_Gueltigkeit() != 0U);
    CommandPtr->dimming_time_base_250ms =
        (l_bool_rd_BCM_RGBa_Dimmzeit_Zeitbasis() != 0U);
    l_flg_clr_frm_RGBe_Slave_a();
    return E_OK;
}

Std_ReturnType Rte_Write_LightStatus(const Rte_LightStatusType *StatusPtr)
{
    if (StatusPtr == NULL)
    {
        return E_NOT_OK;
    }

    l_bool_wr_RGB_Slave_08a_ResponseError(StatusPtr->ResponseError);
    l_u8_wr_RGB_Slave_08a_LedRotStg(StatusPtr->RedShort);
    l_u8_wr_RGB_Slave_08a_LedGruenStg(StatusPtr->GreenShort);
    l_u8_wr_RGB_Slave_08a_LedBlauStg(StatusPtr->BlueShort);
    l_u8_wr_RGB_Slave_08a_LedRotOl(StatusPtr->RedOpen);
    l_u8_wr_RGB_Slave_08a_LedGruenOl(StatusPtr->GreenOpen);
    l_u8_wr_RGB_Slave_08a_LedBlauOl(StatusPtr->BlueOpen);
    l_u8_wr_RGB_Slave_08a_TempState(StatusPtr->TemperatureState);
    l_u8_wr_RGB_Slave_08a_IntErrorRam(StatusPtr->RamError);
    l_u8_wr_RGB_Slave_08a_IntErrorRom(StatusPtr->RomError);
    l_u8_wr_RGB_Slave_08a_IntErrorNvm(StatusPtr->NvMError);
    return E_OK;
}
