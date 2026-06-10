/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    Algorithm.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains algorithms that used to generate PWM
                and get fixed LIN signal.
  *Others:      the intensity adjust parameters may combined to one. Using a 32 bit
                integer instead of two parameters.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

#include "Algorithm.h"
#include "FloatLib.h"
#include "ModuleLed.h"
#include "ModuleTemperature.h"
#include "SystemStatus.h"
#include "SystemControl.h"
#include "ModuleFlash.h"
#include "SystemLinComm.h"
#include "string.h"

#include "AlgoRatioK_localfit.h"
/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
static const fix16_t fix16Const75 = F16(7.5);
static const fix16_t fix16Const10000 = 10000*0x00010000;
static const fix16_t fix16Const1M = F16(0.0001);

// guangpu Best Ra target blend, DID 0x000B payload is value * 10000, little endian.
static fix16_t __algoWhiteRgbParamToFix16(uint16_t value)
{
    return fix16_div(fix16_from_int((int32_t)value), fix16Const10000);
}

static fix16_t __algoGetWhitePointX(void)
{
    return __algoWhiteRgbParamToFix16(savedConfig.whitex);
}

static fix16_t __algoGetWhitePointY(void)
{
    return __algoWhiteRgbParamToFix16(savedConfig.whitey);
}

static uint8_t __algoGetSonderfunktion2WhiteRatioQ10000(uint16_t targetU,
                                                        uint16_t targetV,
                                                        uint16_t *whiteRatioQ10000)
{
    return AlgoRatioK_GetWhiteRatioQ10000(targetU, targetV, whiteRatioQ10000);
}
/*This LED normal intensity may too high.*/
/*we use this parameter to adjust it to same range as other chip did.*/
static const fix16_t intensityAdjustConst = F16(1.0);
static const int32_t s32_plane_a[] = { 370, -387, -249 };
static const int32_t s32_plane_b[] = { 484, 487, -326 };
static const int32_t s32_plane_d[] = { -37502, 110603, 463879 };
static const int32_t s32_vectorNor[] = { 100691, -189723, 573 };
static const int32_t s32_pointK1[] = { 258, 395, 168290 };
static const int32_t s32_R_cornerPoint_large[] =  {135502234, 136970240, 165054 };
static const int32_t s32_G_cornerPoint_large[] = { 15781069, 150339584, 262144 };
static const int32_t s32_B_cornerPoint_large[] = { 51406438, 23121101, 77672 };
static const int32_t s32_R_cornerPoint_real[] = {517, 523, 630};
static const int32_t s32_G_cornerPoint_real[] = {60, 574, 1000};
static const int32_t s32_B_cornerPoint_real[] = {196, 88, 296};
static const int32_t s32_LineDirectionvec_GBm1n1p1[] = {138, -493};
static const int32_t s32_LineDirectionvec_BRm1n1p1[] = {304, 412};
static const int32_t s32_LineDirectionvec_GRm1n1p1[] = {509, -57};

static const fix16_t RC_UPART = F16(1.73439025052479);
static const fix16_t RC_VPART = F16(0.485686451774817);
static const fix16_t RC_CPART = F16(-382.951473174450);
static const fix16_t GC_UPART = F16(-1.70362297797533);
static const fix16_t GC_VPART = F16(1.25839799984915);
static const fix16_t GC_CPART = F16(223.089762394268);
static const fix16_t BC_UPART = F16(-0.0307672725494539);
static const fix16_t BC_VPART = F16(-0.275517909281090);
static const fix16_t BC_CPART = F16(159.861710780182);
static const fix16_t Color_Scale = F16(0.01);
static const fix16_t RxDivRy = F16(2.22588516746412);
static const fix16_t GxDivGy = F16(0.236181342632955);
static const fix16_t BxDivBy = F16(5.00255102040816);
static const fix16_t inv_Ry = F16(3.22555023923445);
static const fix16_t inv_Gy = F16(1.38849171752398);
static const fix16_t inv_By = F16(33.3486394557823);

fix16_t WB_L = F16(0), WB_M = F16(0), WB_N = F16(0);


#define WHITE_BALANCE_U_MAX 60
#define WHITE_BALANCE_U_MIN 20
#define WHITE_BALANCE_V_MAX 114
#define WHITE_BALANCE_V_MIN 74

#define BITSHIFT_PYRAMID (18u)
#define PLANECOUNT (3u)
#define SINT32_MAX (0x7fffffff)
#define CORNERPOINTU_IDX (0u)
#define CORNERPOINTV_IDX (1u)
#define CORNERPOINTL_IDX (2u)
#define DMAX_TEMP_SHIFT_RECORD_AMOUNT 8
typedef enum InputState {
    Cx00_INSIDE_GAMUT = 0,
    Cx02_BESIDE_EDGE_GB = 2,                 // == 0
    Cx03_BESIDE_EDGE_BR,      //1 minus
    Cx04_BESIDE_EDGE_GR,     //2 minus
    Cx05_BESIDE_CORNER_B,
    Cx06_BESIDE_CORNER_G,
    Cx07_BESIDE_CORNER_R
}InputState;

/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
/*@brief CIE color multiplied by 10000.*/
typedef struct CIExyY
{
 int16_t x,y;
 int32_t Y;
}SCIExyY;

/*@brief CIE color converted to fix16 floating.*/
typedef struct CIEColor
{
 fix16_t x,y;
 fix16_t Y;
}SCIEColor;
/*Original R, G, B colors CIE coordinates.*/
static SCIExyY redxyY, greenxyY, bluexyY, whitexyY;
/*After temperature compensation's  R, G, B colors CIE coordinates.*/
static SCIEColor currentRedCIE, currentGreenCIE, currentBlueCIE, currentWhiteCIE;
volatile SCIEColor targetColor;
volatile SCIEColor splitWhiteCIE, splitPointCIE;
volatile bool_t splitTargetColorValid;
volatile fix16_t splitLambda, splitOneMinusLambda, splitRayT;
volatile int16_t splitEdgeIndex;
static int32_t CtApSwcLedAlgorithm_AUDIspecBreachOnEdge(const uint8_t u8_inputState, int32_t* s32_intended_u, int32_t* s32_intended_v)
{
	int32_t s32_s, s32_u, s32_v;
    const int32_t* p_dirVecArray, * p_underPointArray_large, * p_upperPointArray_large, * p_linePoint_real;
    int32_t s32_intensity;
    int8_t u8_plane_idx;
    s32_u = *s32_intended_u;
    s32_v = *s32_intended_v;
    /*LineDirectionvec(1)*(normPoint(1) - LinePoint(1)) + LineDirectionvec(2)*(normPoint(2) - LinePoint(2));*/
    if (u8_inputState == Cx02_BESIDE_EDGE_GB)
    {
        p_upperPointArray_large = &s32_B_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_G_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_GBm1n1p1[0];
        p_linePoint_real = s32_B_cornerPoint_real;
        u8_plane_idx = 0u;
    }
    else if (u8_inputState == Cx03_BESIDE_EDGE_BR)
    {
        p_upperPointArray_large = &s32_R_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_B_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_BRm1n1p1[0];
        p_linePoint_real = s32_R_cornerPoint_real;
        u8_plane_idx = 1u;
    }
    else if (u8_inputState == Cx04_BESIDE_EDGE_GR)
    {
        p_upperPointArray_large = &s32_R_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_G_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_GRm1n1p1[0];
        p_linePoint_real = s32_R_cornerPoint_real;
        u8_plane_idx = 2u;
    }
    s32_s = p_dirVecArray[0] * (s32_u - p_linePoint_real[CORNERPOINTU_IDX]) + p_dirVecArray[1] * (s32_v - p_linePoint_real[CORNERPOINTV_IDX]);
    /*x = LinePoint(1) + s * LineDirectionvec(1);*/
    s32_u = p_upperPointArray_large[0] + s32_s * p_dirVecArray[0];
    if (s32_u >= p_upperPointArray_large[0])
    {
        /*upper corner*/
        *s32_intended_u = p_upperPointArray_large[0] >> BITSHIFT_PYRAMID;
        *s32_intended_v = p_upperPointArray_large[1] >> BITSHIFT_PYRAMID;
        s32_intensity = p_upperPointArray_large[2];
    }
    else if (s32_u <= p_underPointArray_large[0])
    {
        *s32_intended_u = p_underPointArray_large[0] >> BITSHIFT_PYRAMID;
        *s32_intended_v = p_underPointArray_large[1] >> BITSHIFT_PYRAMID;
        s32_intensity = p_underPointArray_large[2];
    }
    else
    {
        *s32_intended_u = s32_u >> BITSHIFT_PYRAMID;
        s32_v = p_upperPointArray_large[1] + s32_s * p_dirVecArray[1];
        *s32_intended_v = s32_v >> BITSHIFT_PYRAMID;
        s32_intensity = s32_plane_a[u8_plane_idx] * *s32_intended_u + s32_plane_b[u8_plane_idx] * *s32_intended_v + s32_plane_d[u8_plane_idx];
    }
    return s32_intensity;
}
static void CtApSwcLedAlgorithm_intensityPyramid(SColorParams* pst_LEDColor)
{
    uint8_t inputState = Cx00_INSIDE_GAMUT;
    int32_t s32_u = (int32_t)pst_LEDColor->colorU, s32_v = (int32_t)pst_LEDColor->colorV;
    int32_t s32_selectedIntensity = SINT32_MAX, s32_min_intensity_rotated = SINT32_MAX;
    int32_t s32_intensity, s32_vectorRef2Intensity[PLANECOUNT], s32_intensity_Rotated;
    for (uint8_t u8_plane_idx = 0; u8_plane_idx < PLANECOUNT; u8_plane_idx++)
    {
        s32_intensity = s32_plane_a[u8_plane_idx] * pst_LEDColor->colorU + s32_plane_b[u8_plane_idx] * pst_LEDColor->colorV + s32_plane_d[u8_plane_idx];
        s32_vectorRef2Intensity[0] = (int32_t)pst_LEDColor->colorU - s32_pointK1[0];
        s32_vectorRef2Intensity[1] = (int32_t)pst_LEDColor->colorV - s32_pointK1[1];
        s32_vectorRef2Intensity[2] = s32_intensity - s32_pointK1[2];
        s32_intensity_Rotated = s32_vectorRef2Intensity[0] * s32_vectorNor[0] + s32_vectorRef2Intensity[1] * s32_vectorNor[1] + s32_vectorRef2Intensity[2] * s32_vectorNor[2];
        /*if s32_intensity_Rotated == 0, Cx02_ONEDGE_GAMUT
        if 1 s32_intensity_Rotated < 0, Cx03_NOT_INGAMUT_BESIDE_EDGE
        if 2 s32_intensity_Rotated < 0, Cx04_NOT_INGAMUT_BESIDE_CORNER
        if 3 s32_intensity_Rotated < 0, mistake*/
        if (s32_intensity_Rotated < 0)
        {
            inputState += u8_plane_idx + 2;
        }
        if (s32_intensity_Rotated < s32_min_intensity_rotated)
        {
            s32_min_intensity_rotated = s32_intensity_Rotated;
            s32_selectedIntensity = s32_intensity;
        }
    }
    if (inputState == Cx00_INSIDE_GAMUT)
    {
       // s32_selectedIntensity = s32_selectedIntensity;
    }
    else if (inputState <= Cx04_BESIDE_EDGE_GR && inputState >= Cx02_BESIDE_EDGE_GB)
    {
        /*Beside edge*/
        s32_selectedIntensity = CtApSwcLedAlgorithm_AUDIspecBreachOnEdge(inputState, &s32_u, &s32_v);
    }
    else
    {
        /*Beside corner*/
        if (inputState == Cx05_BESIDE_CORNER_B)
        {
            s32_u = s32_B_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_B_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_B_cornerPoint_large[CORNERPOINTL_IDX];
        }
        else if (inputState == Cx06_BESIDE_CORNER_G)
        {
            s32_u = s32_G_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_G_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_G_cornerPoint_large[CORNERPOINTL_IDX];
        }
        else
        {
            s32_u = s32_R_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_R_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_R_cornerPoint_large[CORNERPOINTL_IDX];
        }
    }
    pst_LEDColor->intensity = (int32_t)(((int32_t)pst_LEDColor->intensity * (int32_t)s32_selectedIntensity) >> BITSHIFT_PYRAMID);
    pst_LEDColor->colorU = (int16_t)s32_u;
    pst_LEDColor->colorV = (int16_t)s32_v;
    return;
}
/**
*@details   Update red led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  red x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  red y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  red Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateRedCIE(int16_t x, int16_t y, int32_t Y)
{
  redxyY.x = x;
  redxyY.y = y;
  redxyY.Y = (Y/20);
}
/**
*@details   Update green led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  green x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  green y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  green Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateGreenCIE(int16_t x, int16_t y, int32_t Y)
{
  greenxyY.x = x;
  greenxyY.y = y;
  greenxyY.Y = (Y/20);
}
/**
*@details   Update blue led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  blue x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  blue y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  blue Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateBlueCIE(int16_t x, int16_t y, int32_t Y)
{
  bluexyY.x = x;
  bluexyY.y = y;
  bluexyY.Y = (Y/20);
}

void algoUpdateWhiteCIE(int32_t Y)
{
  whitexyY.Y = (Y/20);
}

#define WHITE_BALANCE_U_INIT 40
#define WHITE_BALANCE_V_INIT 94

void WitheBalanceTempInit()
{
		static CIE1976WhiteBalanceInfo Corolinit_temp;

	  uint16_t middleResult = 0u;
	  static uint16_t currentU, currentV;

//	  if(savedConfig.Utemp > WHITE_BALANCE_U_MAX )
//		  WhiteBalanceCalaU = WHITE_BALANCE_U_MAX;
//	  if(savedConfig.Utemp < WHITE_BALANCE_U_MIN )
//		  WhiteBalanceCalaU = WHITE_BALANCE_U_MIN;
//	  if(savedConfig.Vtemp > WHITE_BALANCE_V_MAX )
//		  WhiteBalanceCalaV = WHITE_BALANCE_V_MAX;
//	  if(savedConfig.Vtemp < WHITE_BALANCE_V_MIN )
//		  WhiteBalanceCalaV = WHITE_BALANCE_V_MIN;

	  Corolinit_temp.calibration_whitePointU = (int32_t)200;
	  Corolinit_temp.calibration_whitePointV = (int32_t)470;

	  middleResult = savedConfig.Utemp * 25;
	  if(middleResult % 10u >= 5u)
	  {
		  currentU = middleResult / 10 + 1;
	  }
	  else
	  {
		  currentU = middleResult / 10;
	  }

	  middleResult = WHITE_BALANCE_V_INIT*25;

	  middleResult = savedConfig.Vtemp * 25;
	  if(middleResult % 10u >= 5u)
	  {
		  currentV = middleResult / 10 + 1;
	  }
	  else
	  {
		  currentV = middleResult / 10;
	  }

	 if((Corolinit_temp.target_whitePointU != currentU) ||
			 (Corolinit_temp.target_whitePointV != currentV))
	{
	    	  Corolinit_temp.target_whitePointU = currentU;
	    	  Corolinit_temp.target_whitePointV = currentV;
	    	  whiteBalanceInit(&Corolinit_temp);

	 }
//	  Corolinit_temp.target_whitePointU = (int32_t)WhiteBalanceCalaU*WHITE_BALANCE_UV_RATION;
//	  Corolinit_temp.target_whitePointV = (int32_t)WhiteBalanceCalaV*WHITE_BALANCE_UV_RATION;
//	  whiteBalanceInit(&Corolinit_temp);
}
void whiteBalanceInit(const CIE1976WhiteBalanceInfo* WB_info)
{
       fix16_t Div_midResult;
       WB_L = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), RC_UPART);
       WB_L = fix16_add(WB_L, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), RC_VPART));
       WB_L = fix16_add(WB_L, RC_CPART);
       WB_L = fix16_mul(WB_L, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), RC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), RC_VPART));
       Div_midResult = fix16_add(Div_midResult, RC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_L = fix16_div(Div_midResult, WB_L);//original / target

       WB_M = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), GC_UPART);
       WB_M = fix16_add(WB_M, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), GC_VPART));
       WB_M = fix16_add(WB_M, GC_CPART);
       WB_M = fix16_mul(WB_M, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), GC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), GC_VPART));
       Div_midResult = fix16_add(Div_midResult, GC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_M = fix16_div(Div_midResult, WB_M);//original / target

       WB_N = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), BC_UPART);
       WB_N = fix16_add(WB_N, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), BC_VPART));
       WB_N = fix16_add(WB_N, BC_CPART);
       WB_N = fix16_mul(WB_N, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), BC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), BC_VPART));
       Div_midResult = fix16_add(Div_midResult, BC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_N = fix16_div(Div_midResult, WB_N);//original / target
}

void whiteBalance(const SColorParams *inputColor, SCIEColor *balancedColor)
{
       fix16_t f16_input_U, f16_input_V;
       //fix16_t Den = inputColor->colorV * DEN_CPART;
       fix16_t FluxR, FluxG, FluxB;
       fix16_t balanced_x, balanced_y, balanced_den;
       //FluxR = L * (RC_UPART * u_input + RC_VPART * v_input + RC_CPART) / Den;
       //FluxG = M * (GC_UPART * u_input + GC_VPART * v_input + GC_CPART) / Den;
       //FluxB = N * (BC_UPART * u_input + BC_VPART * v_input + BC_CPART) / Den;
       f16_input_U = fix16_from_int((int32_t)inputColor->colorU);
       f16_input_V = fix16_from_int((int32_t)inputColor->colorV);
       FluxR = fix16_mul(f16_input_U, RC_UPART);
       FluxR = fix16_add(FluxR, fix16_mul(RC_VPART, f16_input_V));
       FluxR = fix16_add(FluxR, RC_CPART);                                           //real fluxR mul Den * 1000
       FluxR = fix16_mul(WB_L, FluxR);

       FluxG = fix16_mul(f16_input_U, GC_UPART);
       FluxG = fix16_add(FluxG, fix16_mul(GC_VPART, f16_input_V));
       FluxG = fix16_add(FluxG, GC_CPART);                                          //real fluxG mul Den * 1000
       FluxG = fix16_mul(WB_M, FluxG);

       FluxB = fix16_mul(f16_input_U, BC_UPART);
       FluxB = fix16_add(FluxB, fix16_mul(BC_VPART, f16_input_V));
       FluxB = fix16_add(FluxB, BC_CPART);                                           //real fluxR mul Den * 1000
       FluxB = fix16_mul(WB_N, FluxB);

       balanced_x = fix16_mul(FluxR, RxDivRy);
       balanced_x = fix16_add(balanced_x, fix16_mul(FluxG, GxDivGy));
       balanced_x = fix16_add(balanced_x, fix16_mul(FluxB, BxDivBy));

       balanced_y = fix16_add(FluxR, FluxG);
       balanced_y = fix16_add(balanced_y, FluxB);

       balanced_den = fix16_mul(FluxR, inv_Ry);
       balanced_den = fix16_add(balanced_den, fix16_mul(FluxG, inv_Gy));
       balanced_den = fix16_add(balanced_den, fix16_mul(FluxB, inv_By));

       balanced_x = fix16_div(balanced_x, balanced_den);
       balanced_y = fix16_div(balanced_y, balanced_den);

       balancedColor->x = balanced_x;
       balancedColor->y = balanced_y;
// X_out_sym = (FluxR*RxDivRy + FluxG*GxDivGy + FluxB*BxDivBy) / (FluxR/Ry + FluxG/Gy + FluxB/By);
// Y_out_sym = (FluxR+FluxG+FluxB)/(FluxR/Ry + FluxG/Gy + FluxB/By);
}
/**@brief structure for temperature compensation.
 * @note more details about this structure, please refer to development document.*/
typedef struct LedTempShift{
 int16_t  Temperature;  /**<Tempetature.*/

 int16_t  whiteYShift;
 int16_t  redYShift;    /**<Red Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  greenYShift;  /**<Green Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  blueYshift;   /**<Blue Luminous shift of current temperature in percentage(multiplied by 1000).*/

 int16_t  redxShift;    /**<Red x coordinate shift at current temperature.*/
 int16_t  redyShift;    /**<Red y coordinate shift at current temperature.*/

 int16_t  greenxShift;  /**<green x coordinate shift at current temperature.*/
 int16_t  greenyShift;  /**<green y coordinate shift at current temperature.*/

 int16_t  bluexShift;   /**<blue x coordinate shift at current temperature.*/
 int16_t  blueyShift;   /**<blue y coordinate shift at current temperature.*/
}SLedTempShift;



static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
/*0*/{-40 ,1068 ,1383 ,1068 ,914  ,-60 , 63 ,-203 , 120  , 30 ,-40 },
/*1*/{-20 ,1045 ,1263 ,1045 ,934  ,-41 , 45 ,-139  , 82  , 21 ,-29 },
/*2*/{-10 ,1033 ,1202 ,1033 ,944  ,-32 , 35 ,-107  , 62  , 16 ,-23 },
/*3*/{0   ,1014 ,1130 ,1014 ,933  ,-25 , 30 ,-72  , 38   , 10 ,-20 },
/*4*/{25  ,1000 ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0   , 0  , 0  },
/*5*/{50  ,970  ,900  ,970  ,1016 , 26 ,-24 , 85  ,-51  ,-13 , 10 },
/*6*/{85  ,922  ,680  ,922  ,1035 , 56 ,-53 , 196 ,-121  ,-31 , 28 },
/*7*/{120 ,884  ,417  ,884  ,1074 , 87 ,-85 , 308 ,-189 ,-47 , 49 }
};

/**
*@details   Referring to current led attribute calculate PWM duty cycle.
*
*@note more details about this function, please refer to development document.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[in]  MxyzY      Target color in CIE coordinate.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    None.
*/

static void __algoLedFixCIE(int16_t ledTemp)
{
	int16_t fixLevel = -1, i;
	  int16_t line_p, redp, greenp, bluep, whitep;
	  /*Temporary use*/
	  //SCIExyY redTempxyY, greenTempxyY, blueTempxyY;
	  int16_t tempShift;

	  for( i = 0 ; i < (DMAX_TEMP_SHIFT_RECORD_AMOUNT-1) ; i++)
	  {
	    if ((ledTemp >= TtoCIE[i].Temperature) &&
	        (ledTemp < TtoCIE[i+1].Temperature))
	    {
	      fixLevel = i;
	      break;
	    }
	  }

	  if (fixLevel == -1)
	    i = 0;

	  tempShift = (TtoCIE[i+1].Temperature-TtoCIE[i].Temperature);

      //fix white color
      line_p = ((TtoCIE[i+1].whiteYShift - TtoCIE[i].whiteYShift)<<4)/tempShift;
	  whitep = TtoCIE[i].whiteYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentWhiteCIE.Y = fix16_div(fix16_from_int(whitep*whitexyY.Y/1000), fix16Const10000);
	  currentWhiteCIE.Y = fix16_mul(currentWhiteCIE.Y, fix16Const75);



	//fix red color
	  line_p = ((TtoCIE[i+1].redYShift - TtoCIE[i].redYShift)<<4)/tempShift;
	  redp = TtoCIE[i].redYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentRedCIE.Y = fix16_div(fix16_from_int(redp*redxyY.Y/1000), fix16Const10000);
	  currentRedCIE.Y = fix16_mul(currentRedCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].redxShift - TtoCIE[i].redxShift)<<4)/tempShift;
	  redp = TtoCIE[i].redxShift + ((line_p * (ledTemp-TtoCIE[i].Temperature))>>4);
	  currentRedCIE.x = fix16_div(fix16_from_int(redxyY.x+redp), fix16Const10000);

	  line_p = ((TtoCIE[i+1].redyShift - TtoCIE[i].redyShift)<<4)/tempShift;
	  redp = TtoCIE[i].redyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentRedCIE.y = fix16_div(fix16_from_int(redxyY.y+redp), fix16Const10000);
	//fix red color

	//fix green color
	  line_p = ((TtoCIE[i+1].greenYShift - TtoCIE[i].greenYShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentGreenCIE.Y = fix16_div(fix16_from_int(greenp*greenxyY.Y/1000), fix16Const10000);
	  currentGreenCIE.Y = fix16_mul(currentGreenCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].greenxShift - TtoCIE[i].greenxShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenxShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentGreenCIE.x = fix16_div(fix16_from_int(greenxyY.x+greenp), fix16Const10000);

	  line_p = ((TtoCIE[i+1].greenyShift - TtoCIE[i].greenyShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentGreenCIE.y = fix16_div(fix16_from_int(greenxyY.y+greenp), fix16Const10000);
	//fix green color

	//fix blue color
	  line_p = ((TtoCIE[i+1].blueYshift - TtoCIE[i].blueYshift)<<4)/tempShift;
	  bluep = TtoCIE[i].blueYshift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.Y = fix16_div(fix16_from_int(bluep*bluexyY.Y/1000), fix16Const10000);
	  currentBlueCIE.Y = fix16_mul(currentBlueCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].bluexShift - TtoCIE[i].bluexShift)<<4)/tempShift;
	  bluep = TtoCIE[i].bluexShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.x = fix16_div(fix16_from_int(bluexyY.x+bluep), fix16Const10000);

	  line_p = ((TtoCIE[i+1].blueyShift - TtoCIE[i].blueyShift)<<4)/tempShift;
	  bluep = TtoCIE[i].blueyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.y = fix16_div(fix16_from_int(bluexyY.y+bluep), fix16Const10000);


	//fix blue color
}

static bool_t __algoIsNearZero(fix16_t value)
{
    if ((value <= fix16Const1M) && (value >= -fix16Const1M))
    {
        return btrue;
    }
    return bfalse;
}

static fix16_t __algoCross2D(fix16_t ax, fix16_t ay, fix16_t bx, fix16_t by)
{
    return fix16_sub(fix16_mul(ax, by), fix16_mul(ay, bx));
}

static bool_t __algoSplitTargetColorByWhite(const SCIEColor *targetColor,
                                            volatile SCIEColor *whiteColor,
                                            volatile SCIEColor *pointColor)
{
    SCIEColor vertices[3];
    fix16_t dirX, dirY;
    fix16_t bestT, bestPX, bestPY;
    bool_t foundPoint;
    int16_t bestEdge;
    int16_t i;

    splitLambda = 0;
    splitOneMinusLambda = fix16_one;
    splitRayT = 0;
    splitEdgeIndex = -1;

    whiteColor->x = __algoGetWhitePointX();
    whiteColor->y = __algoGetWhitePointY();
    whiteColor->Y = 0;

    pointColor->x = targetColor->x;
    pointColor->y = targetColor->y;
    pointColor->Y = 0;

    if (targetColor->Y <= 0)
    {
        return btrue;
    }

    dirX = fix16_sub(targetColor->x, __algoGetWhitePointX());
    dirY = fix16_sub(targetColor->y, __algoGetWhitePointY());

    if ((__algoIsNearZero(dirX) == btrue) && (__algoIsNearZero(dirY) == btrue))
    {
        whiteColor->Y = targetColor->Y;
        splitLambda = 0;
        splitOneMinusLambda = fix16_one;
        splitRayT = fix16_one;
        splitEdgeIndex = -1;
        return btrue;
    }

    if (targetColor->y <= fix16Const1M)
    {
        return bfalse;
    }

    vertices[0].x = currentRedCIE.x;
    vertices[0].y = currentRedCIE.y;
    vertices[0].Y = 0;
    vertices[1].x = currentGreenCIE.x;
    vertices[1].y = currentGreenCIE.y;
    vertices[1].Y = 0;
    vertices[2].x = currentBlueCIE.x;
    vertices[2].y = currentBlueCIE.y;
    vertices[2].Y = 0;

    foundPoint = bfalse;
    bestT = 0;
    bestPX = 0;
    bestPY = 0;
    bestEdge = -1;

    for (i = 0; i < 3; i++)
    {
        SCIEColor startPoint, endPoint;
        fix16_t edgeX, edgeY;
        fix16_t relX, relY;
        fix16_t denominator;
        fix16_t rayT;
        fix16_t edgeU;

        startPoint = vertices[i];
        endPoint = vertices[(i + 1) % 3];

        edgeX = fix16_sub(endPoint.x, startPoint.x);
        edgeY = fix16_sub(endPoint.y, startPoint.y);
        relX = fix16_sub(startPoint.x, __algoGetWhitePointX());
        relY = fix16_sub(startPoint.y, __algoGetWhitePointY());

        denominator = __algoCross2D(dirX, dirY, edgeX, edgeY);
        if (__algoIsNearZero(denominator) == btrue)
        {
            continue;
        }

        rayT = fix16_div(__algoCross2D(relX, relY, edgeX, edgeY), denominator);
        edgeU = fix16_div(__algoCross2D(relX, relY, dirX, dirY), denominator);

        if ((rayT >= fix16_sub(fix16_one, fix16Const1M)) &&
            (edgeU >= -fix16Const1M) &&
            (edgeU <= fix16_add(fix16_one, fix16Const1M)))
        {
            if ((foundPoint == bfalse) || (rayT < bestT))
            {
                bestT = rayT;
                bestPX = fix16_add(__algoGetWhitePointX(), fix16_mul(rayT, dirX));
                bestPY = fix16_add(__algoGetWhitePointY(), fix16_mul(rayT, dirY));
                bestEdge = i;
                foundPoint = btrue;
            }
        }
    }

    if (foundPoint == bfalse)
    {
        return bfalse;
    }

    pointColor->x = bestPX;
    pointColor->y = bestPY;
    splitRayT = bestT;
    splitEdgeIndex = bestEdge;

    if (pointColor->y <= fix16Const1M)
    {
        return bfalse;
    }

    {
        fix16_t lambda;
        fix16_t oneMinusLambda;
        fix16_t whiteRatio;
        fix16_t pointRatio;

        lambda = fix16_div(fix16_one, bestT);
        oneMinusLambda = fix16_sub(fix16_one, lambda);

        splitLambda = lambda;
        splitOneMinusLambda = oneMinusLambda;

        whiteRatio = fix16_mul(oneMinusLambda, __algoGetWhitePointY());
        pointRatio = fix16_mul(lambda, pointColor->y);

        whiteColor->Y = fix16_mul(targetColor->Y, fix16_div(whiteRatio, targetColor->y));
        pointColor->Y = fix16_mul(targetColor->Y, fix16_div(pointRatio, targetColor->y));

        if (whiteColor->Y < 0)
        {
            whiteColor->Y = 0;
        }
        if (pointColor->Y < 0)
        {
            pointColor->Y = 0;
        }
    }

    return btrue;
}
static fix16_t __algoGetWhiteMixY(SColorParams * const inputColor,
                                  const SCIEColor *whiteColor)
{
    fix16_t mix_Y;

    /*
     * Same Y processing as RGB:
     * mix_Y = whiteColor->Y * tempAdjust / 6400
     */
    mix_Y = fix16_mul(whiteColor->Y,
                      fix16_from_int((int32_t)inputColor->tempAdjust / 2));

    mix_Y = fix16_mul(mix_Y, F16(0.02));
    mix_Y = fix16_mul(mix_Y, F16(0.015625));

    return mix_Y;
}
static fix16_t __algoConvertWhiteYToDutyRatio(fix16_t mixWhiteY)
{
    fix16_t duty_w_aa;

    if (mixWhiteY <= 0)
    {
        return 0;
    }

    if (currentWhiteCIE.Y <= 0)
    {
        return 0;
    }

    /*
     * White actual Y = currentWhiteCIE.Y * duty_w_aa
     * So:
     * duty_w_aa = target white Y / current white full-duty Y
     */
    duty_w_aa = fix16_div(mixWhiteY, currentWhiteCIE.Y);

    return duty_w_aa;
}
static void __algoGetWhiteDutyRatio(SColorParams * const inputColor,
                                    const SCIEColor *whiteColor,
                                    SPWMParams *outputPWM)
{
    fix16_t mixWhiteY;
    fix16_t duty_w_aa;
    int32_t duty_W;

    outputPWM->PWM_W = 0;

    mixWhiteY = __algoGetWhiteMixY(inputColor, whiteColor);

    duty_w_aa = __algoConvertWhiteYToDutyRatio(mixWhiteY);

    /* Clamp duty ratio to 0 ~ 1. */
    duty_w_aa = fix16_min(duty_w_aa, fix16_one);
    duty_w_aa = fix16_max(duty_w_aa, 0);

    /* Convert ratio to PWM count. */
    duty_w_aa = fix16_mul(duty_w_aa, F16(PWM_PERIOD));
    duty_W = fix16_to_int(duty_w_aa);
    // Debug_0100[5] = (uint16_t)duty_W;

    /* Same factor scaling as RGB. */
    duty_W = ((duty_W * (inputColor->factor >> 7)) >> 8);

    /* Same brightness_factor scaling as RGB. */
    duty_W = (int32_t)duty_W *
             (int32_t)(savedConfig.brightness_factor & 0x7f) /
             (int32_t)100;

    if (duty_W > 32767)
    {
        duty_W = 32767;
    }
    else if (duty_W < 0)
    {
        duty_W = 0;
    }
    else
    {
        ;
    }

    outputPWM->PWM_W = (uint16_t)duty_W;
}
static void __algoGetDutyRatio(SColorParams * const inputColor, SCIEColor *MxyzY, SPWMParams *outputPWM)
{
	  fix16_t mix_Y;

	  fix16_t a,b,c,d,e,f;
	  fix16_t Rm;
	  fix16_t Gm;
	  fix16_t Bm;

	  fix16_t R_all;
	  fix16_t G_all;
	  fix16_t B_all;

	  //float tempBRESS;

	  int32_t duty_R ,duty_G ,duty_B;

	  a = fix16_sub(MxyzY->x, currentBlueCIE.x);
	  d = fix16_sub(MxyzY->y, currentBlueCIE.y);
	  Bm = fix16_div(currentBlueCIE.Y, currentBlueCIE.y);

	  b = fix16_sub(currentGreenCIE.y, MxyzY->y);
	  c = fix16_sub(currentGreenCIE.x, MxyzY->x);
	  Gm = fix16_div(currentGreenCIE.Y, currentGreenCIE.y);

	  e = fix16_sub(currentRedCIE.x, MxyzY->x);
	  f = fix16_sub(currentRedCIE.y, MxyzY->y);
	  Rm = fix16_div(currentRedCIE.Y, currentRedCIE.y);



	  R_all = fix16_mul(a, b);
	  B_all = fix16_mul(e, b);
	  G_all = fix16_mul(d, e);
	  R_all = fix16_sub(R_all, fix16_mul(c, d));
	  B_all = fix16_sub(B_all, fix16_mul(c, f));
	  G_all = fix16_sub(G_all, fix16_mul(f, a));

	  fix16_t duty_r_aa ,duty_g_aa ,duty_b_aa;

	  duty_r_aa = Bm;
	  duty_g_aa = Rm;
	  duty_b_aa = Rm;

	  duty_r_aa = fix16_mul(duty_r_aa, Gm);
	  duty_g_aa = fix16_mul(duty_g_aa, Bm);
	  duty_b_aa = fix16_mul(duty_b_aa, Gm);
	  duty_r_aa = fix16_mul(duty_r_aa, R_all);
	  duty_g_aa = fix16_mul(duty_g_aa, G_all);
	  duty_b_aa = fix16_mul(duty_b_aa, B_all);

	  /*temperature protection use this(tempAdjust) parameter to control LED intensity.*/
	  /*Minimum is 0, maximum is 6400, so we divide 6400.*/
	  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int((int32_t)inputColor->tempAdjust/2));
	  /*0.01 = 1/100, mix_Y =  mix_Y /100, for more calculating speed, we convert divide to multiply*/
	  mix_Y = fix16_mul(mix_Y, F16(0.02));
	  /*0.015625 = 1/64, mix_Y =  mix_Y /64, for more calculating speed, we convert divide to multiply*/
	  mix_Y = fix16_mul(mix_Y, F16(0.015625));
//	  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int((int32_t)savedConfig.brightness_factor));
//	  mix_Y = fix16_mul(mix_Y, F16(0.01));

	  fix16_t temp = fix16_add(fix16_mul(currentRedCIE.Y, duty_r_aa),
	                           fix16_mul(currentGreenCIE.Y, duty_g_aa));

	  temp = fix16_add(temp, fix16_mul(currentBlueCIE.Y, duty_b_aa));
	  temp = fix16_div(mix_Y, temp);
#if 0
	  duty_r_aa = fix16_mul(duty_r_aa, temp);
	  duty_r_aa = fix16_mul(duty_r_aa, F16(32767));
	  //duty_r_aa = fix16_mul(duty_r_aa, fix16ConstPWM);
	  duty_g_aa = fix16_mul(duty_g_aa, temp);
	  duty_g_aa = fix16_mul(duty_g_aa, F16(32767));
	  //duty_g_aa = fix16_mul(duty_g_aa, fix16ConstPWM);
	  duty_b_aa = fix16_mul(duty_b_aa, temp);
	  duty_b_aa = fix16_mul(duty_b_aa, F16(32767));
	  //duty_b_aa = fix16_mul(duty_b_aa, fix16ConstPWM);
#endif

	  duty_r_aa = fix16_mul(duty_r_aa, temp);
	   duty_g_aa = fix16_mul(duty_g_aa, temp);
	   duty_b_aa = fix16_mul(duty_b_aa, temp);

	     /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	   duty_r_aa = fix16_min(duty_r_aa, fix16_one);
	   duty_g_aa = fix16_min(duty_g_aa, fix16_one);
	   duty_b_aa = fix16_min(duty_b_aa, fix16_one);
	   duty_r_aa = fix16_max(duty_r_aa, 0);
	   duty_g_aa = fix16_max(duty_g_aa, 0);
	   duty_b_aa = fix16_max(duty_b_aa, 0);

	   duty_r_aa = fix16_mul(duty_r_aa, F16(PWM_PERIOD));
	   duty_g_aa = fix16_mul(duty_g_aa, F16(PWM_PERIOD));
	   duty_b_aa = fix16_mul(duty_b_aa, F16(PWM_PERIOD));


	  duty_R = fix16_to_int(duty_r_aa);
	  duty_G = fix16_to_int(duty_g_aa);
	  duty_B = fix16_to_int(duty_b_aa);

	//   Debug_0100[3] = (uint16_t)duty_R;
	//   Debug_0100[4] = (uint16_t)duty_G;

	  /*factor is a parameter our customer using to adjust intensity.*/
	  duty_R = ((duty_R*(inputColor->factor>>7))>>8);
	  duty_G = ((duty_G*(inputColor->factor>>7))>>8);
	  duty_B = ((duty_B*(inputColor->factor>>7))>>8);


	  duty_R = (int32_t)duty_R *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  duty_G = (int32_t)duty_G *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  duty_B = (int32_t)duty_B *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_R > 32767)
		  duty_R = 32767;
	  else if (duty_R < 0)
	    duty_R = 0;
	  else
	  {;}
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_G > 32767)
		  duty_G = 32767;
	  else if (duty_G < 0)
	    duty_G = 0;
	  else
	  {;}
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_B > 32767)
		  duty_B = 32767;
	  else if (duty_B < 0)
	    duty_B = 0;
	  else
	  {;}

	//  duty_R = duty_R<<1;
	//  duty_G = duty_G<<1;
	//  duty_B = duty_B<<1;

	  outputPWM->PWM_R = (uint16_t)duty_R;
	  outputPWM->PWM_G = (uint16_t)duty_G;
	  outputPWM->PWM_B = (uint16_t)duty_B;
}

int32_t tempv,tempu,templ,tempDenominator ,tempz,tempred,tempgreen,tempbule,tempmax;
fix16_t tempRED,tempGREEN,tempBULE,tempINY;
/*get target CIE color by input LIN and other signals.*/
static void __algoGetTargetColorFromCIE1976( SColorParams *inputColor, SCIEColor *targetColor)
{

	 CtApSwcLedAlgorithm_intensityPyramid(inputColor);
	 whiteBalance(inputColor,targetColor);
	  templ = (uint32_t)inputColor->intensity;
	  targetColor->Y = fix16_mul(fix16_from_int(templ), fix16Const1M);
	  targetColor->Y = fix16_mul(intensityAdjustConst, targetColor->Y);

}


/*
 * Lookup helper for BCM_RGB_Sonderfunktion == 2.
 *
 * The lookup strategy is compile-time selected:
 *   - RatioK
 *   - Minimum-angle
 *
 * The split calculation stays in Algorithm.c because it uses private
 * helpers and state from this translation unit.
 */

static void __algoGetSonderfunktion2LookupPwmOutput(SColorParams * const inputColor,
                                     SCIEColor * const targetColorPtr,
                                     SPWMParams *outputPWM,
                                     uint16_t ratioKTargetU,
                                     uint16_t ratioKTargetV)
{

    uint16_t whiteRatioQ10000;
    uint8_t ratioValid;

    SCIEColor boundaryWhiteCIE;
    SCIEColor boundaryPointCIE;
    SCIEColor dutyColor;

    fix16_t whiteRatio;
    fix16_t maxWhiteRatio;
    fix16_t ratioFactor;
    fix16_t lambda;
    fix16_t rgbRatio;
    fix16_t dirX;
    fix16_t dirY;

    ratioValid = __algoGetSonderfunktion2WhiteRatioQ10000(ratioKTargetU,
                                                          ratioKTargetV,
                                                          &whiteRatioQ10000);

    if (ratioValid == 0u)
    {
        splitTargetColorValid = bfalse;
    }
    else
    {
        /*
         * Call original split function only to get the valid maximum white ratio
         * on current W->T ray. The final split result is overwritten below.
         */
        splitTargetColorValid = __algoSplitTargetColorByWhite(targetColorPtr,
                                                              &boundaryWhiteCIE,
                                                              &boundaryPointCIE);
        if ((splitTargetColorValid == btrue) && (targetColorPtr->Y > 0))
        {
            whiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);

            maxWhiteRatio = fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y);

            if (whiteRatio > maxWhiteRatio)
            {
                whiteRatio = maxWhiteRatio;
            }

            if (whiteRatio < 0)
            {
                whiteRatio = 0;
            }

            if (__algoGetWhitePointY() <= fix16Const1M)
            {
                splitTargetColorValid = bfalse;
            }
            else
            {
                ratioFactor = fix16_div(fix16_mul(whiteRatio, targetColorPtr->y),
                                        __algoGetWhitePointY());

                lambda = fix16_sub(fix16_one, ratioFactor);

                if (lambda <= fix16Const1M)
                {
                    splitTargetColorValid = bfalse;
                }
                else
                {
                    splitRayT = fix16_div(fix16_one, lambda);

                    dirX = fix16_sub(targetColorPtr->x, __algoGetWhitePointX());
                    dirY = fix16_sub(targetColorPtr->y, __algoGetWhitePointY());

                    splitWhiteCIE.x = __algoGetWhitePointX();
                    splitWhiteCIE.y = __algoGetWhitePointY();
                    splitWhiteCIE.Y = fix16_mul(targetColorPtr->Y, whiteRatio);

                    splitPointCIE.x = fix16_add(__algoGetWhitePointX(), fix16_mul(splitRayT, dirX));
                    splitPointCIE.y = fix16_add(__algoGetWhitePointY(), fix16_mul(splitRayT, dirY));

                    rgbRatio = fix16_sub(fix16_one, whiteRatio);

                    if (rgbRatio < 0)
                    {
                        rgbRatio = 0;
                    }

                    splitPointCIE.Y = fix16_mul(targetColorPtr->Y, rgbRatio);

                    splitLambda = lambda;
                    splitOneMinusLambda = ratioFactor;
                    splitEdgeIndex = -3;
                    splitTargetColorValid = btrue;
                }
            }
        }
    }

    if (splitTargetColorValid == bfalse)
    {
        splitWhiteCIE.x = __algoGetWhitePointX();
        splitWhiteCIE.y = __algoGetWhitePointY();
        splitWhiteCIE.Y = 0;
        dutyColor = *targetColorPtr;
    }
    else
    {
        dutyColor = splitPointCIE;
    }

    __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
    __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);
}



/**
*@details   This is a function generates PWM by calling internal functions.
*           The whole process is this: 1. get target color by LIN signals.
*                                      2. adjust current LED attributes by temperature.
*                                      3. generate PWM.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    always true.
*/
static void __algoGetSonderfunktion2PwmOutput(SColorParams * const inputColor,
                                              SCIEColor * const targetColorPtr,
                                              SPWMParams *outputPWM,
                                              uint16_t ratioKTargetU,
                                              uint16_t ratioKTargetV)
{
    __algoGetSonderfunktion2LookupPwmOutput(inputColor,
                                            targetColorPtr,
                                            outputPWM,
                                            ratioKTargetU,
                                            ratioKTargetV);
}
bool_t algoGetPWMOutput(SColorParams * const inputColor, SPWMParams *outputPWM)
{
//	SCIEColor targetColor;
	SCIEColor dutyColor;
    uint16_t ratioKTargetU;
    uint16_t ratioKTargetV;

    ratioKTargetU = (uint16_t)((uint16_t)inputColor->colorU);
    ratioKTargetV = (uint16_t)((uint16_t)inputColor->colorV);
    // Debug_0100[0] = (uint16_t)inputColor->tempAdjust;
    // Debug_0100[1] = inputColor->factor;
    // Debug_0100[2] = (uint16_t)savedConfig.brightness_factor;
    /*Turn off lights by default.*/
    outputPWM->PWM_R = 0;
    outputPWM->PWM_G = 0;
    outputPWM->PWM_B = 0;
    outputPWM->PWM_W = 0;`r`n    /*get target CIE color by input LIN and other signals.*/
    __algoGetTargetColorFromCIE1976(inputColor, &targetColor);
    /*Compensate LED by temperature.*/
	__algoLedFixCIE(inputColor->ledTemprature/DTEMP_MULTIPLIED_RATIO);


    if (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion == 0)
    {
        splitTargetColorValid = __algoSplitTargetColorByWhite(&targetColor, &splitWhiteCIE, &splitPointCIE);
        if (splitTargetColorValid == bfalse)
        {
            splitWhiteCIE.x = __algoGetWhitePointX();
            splitWhiteCIE.y = __algoGetWhitePointY();
            splitWhiteCIE.Y = 0;
            dutyColor = targetColor;
        }
        else
        {
            dutyColor = splitPointCIE;
        }
        /*Referring to target CIE-Color(targetColor), */
        /*some intensity control signals(inputColor)*/
        /*and current LED attributes, generate PWM*/
        __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
        /* White PWM from split white color. */
        __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);
    }
    else if (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion == 2)
    {
        __algoGetSonderfunktion2PwmOutput(inputColor,
                                          (SCIEColor *)&targetColor,
                                          outputPWM,
                                          ratioKTargetU,
                                          ratioKTargetV);
    }
    else
    {
        __algoGetDutyRatio(inputColor, &targetColor, outputPWM);
    }
    /* Debug_0100[0]=tempAdjust, [1]=factor, [2]=brightness_factor,
       [3]=raw PWM_R before factor/brightness,
       [4]=raw PWM_G before factor/brightness,
       [5]=raw PWM_W before factor/brightness. */

  return btrue;
}





