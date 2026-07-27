/*********************************************************************************
*Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
*FileName:    SystemLinDiag.c
*Author:      Leo Song
*Version:     0.0.1
*Date:        2017.10.24
*Description: This file manage LIN slave receive and response functions.
*Others:      None.
*History:
1.Date:         2017.10.24
Author:       Leo song
Modification: Initial version.
V01: systemLinCommSignalUpdate will return LIN signal update status.
modified by Leo, Song. 2018.1.12
**********************************************************************************/
#include "SystemLinComm.h"
#include "ModulePower.h"
#include "ModuleLed.h"
#include "SystemControl.h"
#include "SystemStatus.h"
#include "SystemDerating.h"
#include "ModuleFlash.h"
#include <string.h>
#include "genLinConfig.h"
//this is a temp version of Lin Stack
#include "CommonInterruptHandler.h"
#include "io_e52139b_IAR.h"
#include "vic_InterruptHandler.h"
//#include "lin_driver_api.h"
//#include "lin_app.h"
#include "LinDrvImp_Config.h"

//void systemLinCommChangeNad(uint8_t newNad);

void SystemLinCommInit(void)
{
    //systemLinCommChangeNad(savedConfig.singleAddr);
  uint16_t cnt;
  for (cnt = 0; cnt<8; cnt++)
  {
    masterReadBuffer[cnt] = 0xff;
  }
  
  l_bool_wr_RGB_Slave_01_ResponseError(0);
  l_u8_wr_RGB_Slave_01_IntErrorRom(init);
  l_u8_wr_RGB_Slave_01_IntErrorNvm(init);
  l_u8_wr_RGB_Slave_01_IntErrorRam(init);
  l_u8_wr_RGB_Slave_01_TempState(init);

  l_u8_wr_RGB_Slave_01_LedRotStg(init);
  l_u8_wr_RGB_Slave_01_LedRotOl(init);
  l_u8_wr_RGB_Slave_01_LedGruenStg(init);
  l_u8_wr_RGB_Slave_01_LedGruenOl(init);
  l_u8_wr_RGB_Slave_01_LedBlauStg(init);
  l_u8_wr_RGB_Slave_01_LedBlauOl(init);
  l_u8_wr_RGB_Slave_01_IntErrorRom(init); 
}



static SLinSignals oldLinData;
extern SLinSignals currentLinBusMsg;
/**
*@details   system LIN communication module prepare to enter sleep.
*
*@retval    None.
*/
bool_t systemLinCommSignalUpdate(void)
{
  //if (l_flg_tst_BCM_RGBa_Slave_Adresse())  //delete 
  
    //l_flg_clr_BCM_RGBa_Slave_Adresse(); //delete 
    sysLin.BCM_RGB_Slave_Adresse    =  l_u16_rd_BCM_RGBa_Slave_Adresse() ;
    sysLin.BCM_RGB_Adressierungsart =   l_bool_rd_BCM_RGBa_Adressierungsart();


    if((sysLin.BCM_RGB_Adressierungsart == Einzeladresse  && sysLin.BCM_RGB_Slave_Adresse == savedConfig.singleAddr)||
       (sysLin.BCM_RGB_Adressierungsart == Gruppenadresse && (sysLin.BCM_RGB_Slave_Adresse & savedConfig.groupAddr) == savedConfig.groupAddr))
    {
      if ((oldLinData.BCM_RGB_Rot != l_u8_rd_BCM_RGBa_Rot()) ||
         (oldLinData.BCM_RGB_Gruen != l_u8_rd_BCM_RGBa_Gruen()) ||
         (oldLinData.BCM_RGB_Blau != l_u8_rd_BCM_RGBa_Blau()) ||
         (oldLinData.BCM_RGB_Intensitaet != l_u8_rd_BCM_RGBa_Intensitaet()) ||
         (oldLinData.BCM_RGB_Normierung_Intensitaet != l_bool_rd_BCM_RGBa_Normierung_Intensitaet()) ||
         (oldLinData.BCM_RGB_Dimmrampe !=  l_u8_rd_BCM_RGBa_Dimmrampe()) ||
         (oldLinData.BCM_RGB_Sonderfunktion !=  l_u8_rd_BCM_RGBa_Sonderfunktion()) ||
         (oldLinData.BCM_RGB_Bewertung_Dimmzeit !=  l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit()) ||
         (oldLinData.BCM_RGB_Dimmzeit_Zeitbasis !=  l_bool_rd_BCM_RGBa_Dimmzeit_Zeitbasis()) ||
         (oldLinData.BCM_RGB_Gueltigkeit != l_bool_rd_BCM_RGBa_Gueltigkeit()))
      {
        /*If platform has been written a wrong value, module will not function.*/
        if(savedConfig.platform == EPlatformTypeMQB)
        {
          sysLin.BCM_RGB_Bewertung_Dimmzeit = l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit();
          sysLin.BCM_RGB_Gueltigkeit        = l_bool_rd_BCM_RGBa_Gueltigkeit();
        }
        else if(savedConfig.platform == EPlatformTypeMLBevo)
        {
          sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_absolut;
          sysLin.BCM_RGB_Gueltigkeit        = 1;
        }
        else if(savedConfig.platform == EPlatformTypeMLBevoVW)
        {
          sysLin.BCM_RGB_Bewertung_Dimmzeit = l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit();//1;//
          sysLin.BCM_RGB_Gueltigkeit        = 1;
        }
  
        if(sysLin.BCM_RGB_Gueltigkeit & 0x01 == 1)
        {
          if((savedConfig.ledSupplier & 0xff00u) == 0x0000u)//!= 0x0200)//
          { //!= 0x0200;
            if ((l_u8_rd_BCM_RGBa_Rot() != 0) ||
                (l_u8_rd_BCM_RGBa_Gruen() != 0) ||
                (l_u8_rd_BCM_RGBa_Blau() != 0))
            {
              sysLin.BCM_RGB_Rot   = l_u8_rd_BCM_RGBa_Rot();
              sysLin.BCM_RGB_Gruen = l_u8_rd_BCM_RGBa_Gruen();
              sysLin.BCM_RGB_Blau  = l_u8_rd_BCM_RGBa_Blau();
            }
            sysLin.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGBa_Sonderfunktion() <=Stellgliedtest?l_u8_rd_BCM_RGBa_Sonderfunktion():keine_Sonderfunktion;
          }
          else if((savedConfig.ledSupplier & 0xff00u ) == 0x0200u)
          {
            sysLin.BCM_RGB_Rot   =  183;
            sysLin.BCM_RGB_Gruen =  255;
            sysLin.BCM_RGB_Blau  =  57;
            if (l_u8_rd_BCM_RGBa_Sonderfunktion()  != Showroommodus)
              sysLin.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGBa_Sonderfunktion()<=Stellgliedtest?l_u8_rd_BCM_RGBa_Sonderfunktion():keine_Sonderfunktion;
          }
  
          if( l_u8_rd_BCM_RGBa_Intensitaet()  <= 125)
          {
            sysLin.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGBa_Intensitaet() ;
            sysLin.BCM_RGB_Intensitaet = sysLin.BCM_RGB_Intensitaet>100?100:sysLin.BCM_RGB_Intensitaet ;
          }
  
          sysLin.BCM_RGB_Normierung_Intensitaet = l_bool_rd_BCM_RGBa_Normierung_Intensitaet();
          sysLin.BCM_RGB_Dimmrampe =  l_u8_rd_BCM_RGBa_Dimmrampe();
          sysLin.BCM_RGB_Dimmzeit_Zeitbasis = l_bool_rd_BCM_RGBa_Dimmzeit_Zeitbasis();
  
          if (sysLin.BCM_RGB_Sonderfunktion == Showroommodus)
          {
            sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_relativ;
            sysLin.BCM_RGB_Dimmrampe = 1;
          }
  
          oldLinData.BCM_RGB_Rot = l_u8_rd_BCM_RGBa_Rot();
          oldLinData.BCM_RGB_Gruen = l_u8_rd_BCM_RGBa_Gruen();
          oldLinData.BCM_RGB_Blau = l_u8_rd_BCM_RGBa_Blau();
          oldLinData.BCM_RGB_Intensitaet =  l_u8_rd_BCM_RGBa_Intensitaet();
          oldLinData.BCM_RGB_Normierung_Intensitaet = l_bool_rd_BCM_RGBa_Normierung_Intensitaet();
          oldLinData.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGBa_Dimmrampe();
          oldLinData.BCM_RGB_Sonderfunktion =  l_u8_rd_BCM_RGBa_Sonderfunktion() ;
          oldLinData.BCM_RGB_Bewertung_Dimmzeit =  l_bool_rd_BCM_RGBa_Bewertung_Dimmzeit();
          oldLinData.BCM_RGB_Gueltigkeit = l_bool_rd_BCM_RGBa_Gueltigkeit();
          oldLinData.BCM_RGB_Dimmzeit_Zeitbasis =  l_bool_rd_BCM_RGBa_Dimmzeit_Zeitbasis();
  
          sysStatus.newLinSignal = btrue;
        }
  
        return btrue;
      }
    }
  
  else
    ;

  return bfalse;
}


/**
*@details   system LIN communication module prepare to enter sleep.
*
*@retval    None.
*/
void systemLinCommSleep(void)
{
  oldLinData.BCM_RGB_Rot = 0;
  oldLinData.BCM_RGB_Gruen = 0;
  oldLinData.BCM_RGB_Blau = 0;
  oldLinData.BCM_RGB_Intensitaet = 0;
  oldLinData.BCM_RGB_Normierung_Intensitaet = 0;
  oldLinData.BCM_RGB_Dimmrampe = 0;
  oldLinData.BCM_RGB_Sonderfunktion = 0;
  oldLinData.BCM_RGB_Bewertung_Dimmzeit = 0;
  oldLinData.BCM_RGB_Gueltigkeit = 0;
}

/**
*@details   update LIN slave response signal.
*
*@retval    None.
*/
void systemLinCommRespUpdate(void)
{
  //l_bool_wr_RGB_Slave_01_ResponseError(
  l_u8_wr_RGB_Slave_01_IntErrorNvm(kein_Fehler);
  l_u8_wr_RGB_Slave_01_IntErrorRam(kein_Fehler);
  l_u8_wr_RGB_Slave_01_IntErrorRom(kein_Fehler);

  l_u8_wr_RGB_Slave_01_TempState(systemDeratingGetTempstate());

  if (sysStatus.errFlag & ESystemErrorLedError)
    l_u8_wr_RGB_Slave_01_IntErrorRom(Fehler);
  else
    l_u8_wr_RGB_Slave_01_IntErrorRom(kein_Fehler);

  if (moduleLedGetStatus(ELedChannelRed) == ELedStatusInit)
  {
    l_u8_wr_RGB_Slave_01_LedRotStg(init);
    l_u8_wr_RGB_Slave_01_LedRotOl(init);
  }
  else
  {
    l_u8_wr_RGB_Slave_01_LedRotStg(kein_Fehler);
    l_u8_wr_RGB_Slave_01_LedRotOl(kein_Fehler);
  }

  if (moduleLedGetStatus(ELedChannelRed) & ELedStatusOpen)
    l_u8_wr_RGB_Slave_01_LedRotOl(Fehler);
  else if (moduleLedGetStatus(ELedChannelRed) & ELedStatusShort)
    l_u8_wr_RGB_Slave_01_LedRotStg(Fehler);


  if (moduleLedGetStatus(ELedChannelGreen) == ELedStatusInit)
  {
    l_u8_wr_RGB_Slave_01_LedGruenStg(init);
    l_u8_wr_RGB_Slave_01_LedGruenOl(init);
  }
  else
  {
    l_u8_wr_RGB_Slave_01_LedGruenStg(kein_Fehler);
    l_u8_wr_RGB_Slave_01_LedGruenOl(kein_Fehler);
  }

  if (moduleLedGetStatus(ELedChannelGreen) & ELedStatusOpen)
    l_u8_wr_RGB_Slave_01_LedGruenOl(Fehler);
  else if (moduleLedGetStatus(ELedChannelGreen) & ELedStatusShort)
    l_u8_wr_RGB_Slave_01_LedGruenStg(Fehler);

  if (moduleLedGetStatus(ELedChannelBlue) == ELedStatusInit)
  {
    l_u8_wr_RGB_Slave_01_LedBlauStg(init);
    l_u8_wr_RGB_Slave_01_LedBlauOl(init);
  }
  else
  {
    l_u8_wr_RGB_Slave_01_LedBlauStg(kein_Fehler);
    l_u8_wr_RGB_Slave_01_LedBlauOl(kein_Fehler);
  }

  if (moduleLedGetStatus(ELedChannelBlue) & ELedStatusOpen)
    l_u8_wr_RGB_Slave_01_LedBlauOl(Fehler);
  else if (moduleLedGetStatus(ELedChannelBlue) & ELedStatusShort)
    l_u8_wr_RGB_Slave_01_LedBlauStg(Fehler);
}

