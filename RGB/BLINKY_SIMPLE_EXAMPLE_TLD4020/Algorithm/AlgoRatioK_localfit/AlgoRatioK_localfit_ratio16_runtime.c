/*********************************************************************************
 * FileName:    AlgoRatioK_localfit.c
 * Description: Auto-generated runtime localfit white-ratio lookup module.
 *              The LUT stores only Best_SingleWhiteRatio in Q10000 uint16 form.
 *              Target-point coordinates are regenerated online from the same
 *              triangle-grid logic as s01_make_lut_xy_grid_final.m.
 *
 * Data layout:
 *   g_algoRatioKRatioQ10000Lut[idx] = round(Best_SingleWhiteRatio * 10000)
 *
 * Runtime flow:
 *   1. Generate nearest ALGO_RATIO_K_AMOUNT grid points online.
 *   2. Each point carries its LUT index among ALGO_RATIO_K_LUT_COUNT.
 *   3. Use lutIndex to read ratioQ10000 from the 16-bit ratio table.
 *   4. Fit ratio = a * (U - targetU) + b * (V - targetV) + c.
 *   5. Return c, or fall back to inverse-distance weighted average.
 **********************************************************************************/

#include "AlgoRatioK_localfit.h"
#include "FloatLib.h"

#define ALGO_RATIO_K_LUT_COUNT           (990u)
#define ALGO_RATIO_K_GRID_ORDER          (43u)
#define ALGO_RATIO_K_NEIGHBOR_RING       (4u)

#define ALGO_RATIO_K_VERTEX_K_U_Q1000    (200u)
#define ALGO_RATIO_K_VERTEX_K_V_Q1000    (84u)
#define ALGO_RATIO_K_VERTEX_I_U_Q1000    (546u)
#define ALGO_RATIO_K_VERTEX_I_V_Q1000    (518u)
#define ALGO_RATIO_K_VERTEX_J_U_Q1000    (49u)
#define ALGO_RATIO_K_VERTEX_J_V_Q1000    (568u)

#define ALGO_RATIO_K_XY_SCALE            (1000000u)
#define ALGO_RATIO_K_VERTEX_K_X_QSCALE   (152100u)
#define ALGO_RATIO_K_VERTEX_K_Y_QSCALE   (28318u)
#define ALGO_RATIO_K_VERTEX_I_X_QSCALE   (702970u)
#define ALGO_RATIO_K_VERTEX_I_Y_QSCALE   (296225u)
#define ALGO_RATIO_K_VERTEX_J_X_QSCALE   (136662u)
#define ALGO_RATIO_K_VERTEX_J_Y_QSCALE   (707606u)

#define ALGO_RATIO_K_RATIO_Q10000_MAX    (10000u)
#define ALGO_RATIO_K_DISTANCE_MAX        (0xFFFFFFFFu)

#ifndef ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16
#define ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16 (4)
#endif

#define ALGO_RATIO_K_WEIGHT_Q_SHIFT      (28u)
#define ALGO_RATIO_K_RATIO_NORMALIZER    (10000u)

#if (ALGO_RATIO_K_AMOUNT == 0u)
#error "ALGO_RATIO_K_AMOUNT must be greater than 0."
#endif

#if (ALGO_RATIO_K_LUT_COUNT != (((ALGO_RATIO_K_GRID_ORDER + 1u) * (ALGO_RATIO_K_GRID_ORDER + 2u)) / 2u))
#error "ALGO_RATIO_K_LUT_COUNT does not match triangular grid count."
#endif

static const uint16_t g_algoRatioKRatioQ10000Lut[ALGO_RATIO_K_LUT_COUNT] =
{
        0u,  /* idx=  0, ratio=0.000000 */
        0u,  /* idx=  1, ratio=0.000000 */
        0u,  /* idx=  2, ratio=0.000000 */
        0u,  /* idx=  3, ratio=0.000000 */
        0u,  /* idx=  4, ratio=0.000000 */
        0u,  /* idx=  5, ratio=0.000000 */
        0u,  /* idx=  6, ratio=0.000000 */
        0u,  /* idx=  7, ratio=0.000000 */
        0u,  /* idx=  8, ratio=0.000000 */
        0u,  /* idx=  9, ratio=0.000000 */
        0u,  /* idx= 10, ratio=0.000000 */
        0u,  /* idx= 11, ratio=0.000000 */
        0u,  /* idx= 12, ratio=0.000000 */
        0u,  /* idx= 13, ratio=0.000000 */
        0u,  /* idx= 14, ratio=0.000000 */
        0u,  /* idx= 15, ratio=0.000000 */
        0u,  /* idx= 16, ratio=0.000000 */
        0u,  /* idx= 17, ratio=0.000000 */
        0u,  /* idx= 18, ratio=0.000000 */
        0u,  /* idx= 19, ratio=0.000000 */
        0u,  /* idx= 20, ratio=0.000000 */
        0u,  /* idx= 21, ratio=0.000000 */
        0u,  /* idx= 22, ratio=0.000000 */
        0u,  /* idx= 23, ratio=0.000000 */
        0u,  /* idx= 24, ratio=0.000000 */
        0u,  /* idx= 25, ratio=0.000000 */
        0u,  /* idx= 26, ratio=0.000000 */
        0u,  /* idx= 27, ratio=0.000000 */
        0u,  /* idx= 28, ratio=0.000000 */
        0u,  /* idx= 29, ratio=0.000000 */
        0u,  /* idx= 30, ratio=0.000000 */
        0u,  /* idx= 31, ratio=0.000000 */
        0u,  /* idx= 32, ratio=0.000000 */
        0u,  /* idx= 33, ratio=0.000000 */
        0u,  /* idx= 34, ratio=0.000000 */
        0u,  /* idx= 35, ratio=0.000000 */
        0u,  /* idx= 36, ratio=0.000000 */
        0u,  /* idx= 37, ratio=0.000000 */
        0u,  /* idx= 38, ratio=0.000000 */
        0u,  /* idx= 39, ratio=0.000000 */
        0u,  /* idx= 40, ratio=0.000000 */
        0u,  /* idx= 41, ratio=0.000000 */
        0u,  /* idx= 42, ratio=0.000000 */
        0u,  /* idx= 43, ratio=-0.000000 */
        0u,  /* idx= 44, ratio=0.000000 */
        0u,  /* idx= 45, ratio=0.000000 */
        0u,  /* idx= 46, ratio=0.000000 */
        0u,  /* idx= 47, ratio=0.000000 */
        0u,  /* idx= 48, ratio=0.000000 */
        0u,  /* idx= 49, ratio=0.000000 */
        0u,  /* idx= 50, ratio=0.000000 */
        0u,  /* idx= 51, ratio=0.000000 */
        0u,  /* idx= 52, ratio=0.000000 */
        0u,  /* idx= 53, ratio=0.000000 */
        0u,  /* idx= 54, ratio=0.000000 */
        0u,  /* idx= 55, ratio=0.000000 */
        0u,  /* idx= 56, ratio=0.000000 */
        0u,  /* idx= 57, ratio=0.000000 */
        0u,  /* idx= 58, ratio=0.000000 */
        0u,  /* idx= 59, ratio=0.000000 */
        0u,  /* idx= 60, ratio=0.000000 */
        0u,  /* idx= 61, ratio=0.000000 */
        0u,  /* idx= 62, ratio=0.000000 */
        0u,  /* idx= 63, ratio=0.000000 */
        0u,  /* idx= 64, ratio=0.000000 */
        0u,  /* idx= 65, ratio=0.000000 */
        0u,  /* idx= 66, ratio=0.000000 */
        0u,  /* idx= 67, ratio=0.000000 */
        0u,  /* idx= 68, ratio=0.000000 */
        0u,  /* idx= 69, ratio=0.000000 */
        0u,  /* idx= 70, ratio=0.000000 */
        0u,  /* idx= 71, ratio=0.000000 */
        0u,  /* idx= 72, ratio=0.000000 */
        0u,  /* idx= 73, ratio=0.000000 */
        0u,  /* idx= 74, ratio=0.000000 */
        0u,  /* idx= 75, ratio=0.000000 */
        0u,  /* idx= 76, ratio=0.000000 */
        0u,  /* idx= 77, ratio=0.000000 */
        0u,  /* idx= 78, ratio=0.000000 */
        0u,  /* idx= 79, ratio=0.000000 */
        0u,  /* idx= 80, ratio=0.000000 */
        0u,  /* idx= 81, ratio=0.000000 */
        0u,  /* idx= 82, ratio=0.000000 */
        0u,  /* idx= 83, ratio=0.000000 */
        0u,  /* idx= 84, ratio=0.000000 */
        0u,  /* idx= 85, ratio=0.000000 */
        0u,  /* idx= 86, ratio=0.000000 */
        0u,  /* idx= 87, ratio=0.000000 */
     3850u,  /* idx= 88, ratio=0.385033 */
     2421u,  /* idx= 89, ratio=0.242055 */
     1271u,  /* idx= 90, ratio=0.127067 */
      360u,  /* idx= 91, ratio=0.036007 */
        0u,  /* idx= 92, ratio=0.000000 */
        0u,  /* idx= 93, ratio=0.000000 */
        0u,  /* idx= 94, ratio=0.000000 */
        0u,  /* idx= 95, ratio=0.000000 */
        0u,  /* idx= 96, ratio=0.000000 */
        0u,  /* idx= 97, ratio=0.000000 */
        0u,  /* idx= 98, ratio=0.000000 */
        0u,  /* idx= 99, ratio=0.000000 */
        0u,  /* idx=100, ratio=0.000000 */
        0u,  /* idx=101, ratio=0.000000 */
        0u,  /* idx=102, ratio=0.000000 */
        0u,  /* idx=103, ratio=0.000000 */
        0u,  /* idx=104, ratio=0.000000 */
        0u,  /* idx=105, ratio=0.000000 */
        0u,  /* idx=106, ratio=0.000000 */
        0u,  /* idx=107, ratio=0.000000 */
        0u,  /* idx=108, ratio=0.000000 */
        0u,  /* idx=109, ratio=0.000000 */
        0u,  /* idx=110, ratio=0.000000 */
        0u,  /* idx=111, ratio=0.000000 */
        0u,  /* idx=112, ratio=0.000000 */
        0u,  /* idx=113, ratio=0.000000 */
        0u,  /* idx=114, ratio=0.000000 */
        0u,  /* idx=115, ratio=0.000000 */
        0u,  /* idx=116, ratio=0.000000 */
        0u,  /* idx=117, ratio=0.000000 */
        0u,  /* idx=118, ratio=0.000000 */
        0u,  /* idx=119, ratio=0.000000 */
        0u,  /* idx=120, ratio=0.000000 */
        0u,  /* idx=121, ratio=0.000000 */
        0u,  /* idx=122, ratio=0.000000 */
        0u,  /* idx=123, ratio=0.000000 */
        0u,  /* idx=124, ratio=0.000000 */
        0u,  /* idx=125, ratio=0.000000 */
        0u,  /* idx=126, ratio=0.000000 */
        0u,  /* idx=127, ratio=0.000000 */
        0u,  /* idx=128, ratio=-0.000000 */
        0u,  /* idx=129, ratio=0.000000 */
     3815u,  /* idx=130, ratio=0.381537 */
     6097u,  /* idx=131, ratio=0.609714 */
     4915u,  /* idx=132, ratio=0.491464 */
     3834u,  /* idx=133, ratio=0.383436 */
     2866u,  /* idx=134, ratio=0.286573 */
     2101u,  /* idx=135, ratio=0.210056 */
     1557u,  /* idx=136, ratio=0.155717 */
     1173u,  /* idx=137, ratio=0.117255 */
      899u,  /* idx=138, ratio=0.089853 */
      688u,  /* idx=139, ratio=0.068790 */
      432u,  /* idx=140, ratio=0.043160 */
      215u,  /* idx=141, ratio=0.021535 */
       36u,  /* idx=142, ratio=0.003585 */
        0u,  /* idx=143, ratio=0.000000 */
        0u,  /* idx=144, ratio=0.000000 */
        0u,  /* idx=145, ratio=0.000000 */
        0u,  /* idx=146, ratio=0.000000 */
        0u,  /* idx=147, ratio=0.000000 */
        0u,  /* idx=148, ratio=0.000000 */
        0u,  /* idx=149, ratio=0.000000 */
        0u,  /* idx=150, ratio=0.000000 */
        0u,  /* idx=151, ratio=0.000000 */
        0u,  /* idx=152, ratio=0.000000 */
        0u,  /* idx=153, ratio=0.000000 */
        0u,  /* idx=154, ratio=0.000000 */
        0u,  /* idx=155, ratio=0.000000 */
        0u,  /* idx=156, ratio=0.000000 */
        0u,  /* idx=157, ratio=0.000000 */
        0u,  /* idx=158, ratio=0.000000 */
        0u,  /* idx=159, ratio=0.000000 */
        0u,  /* idx=160, ratio=0.000000 */
        0u,  /* idx=161, ratio=0.000000 */
        0u,  /* idx=162, ratio=0.000000 */
        0u,  /* idx=163, ratio=0.000000 */
        0u,  /* idx=164, ratio=0.000000 */
        0u,  /* idx=165, ratio=0.000000 */
        0u,  /* idx=166, ratio=0.000000 */
        0u,  /* idx=167, ratio=0.000000 */
        0u,  /* idx=168, ratio=0.000000 */
        0u,  /* idx=169, ratio=-0.000000 */
        0u,  /* idx=170, ratio=0.000000 */
     3471u,  /* idx=171, ratio=0.347104 */
     5649u,  /* idx=172, ratio=0.564935 */
     7144u,  /* idx=173, ratio=0.714375 */
     6554u,  /* idx=174, ratio=0.655368 */
     5564u,  /* idx=175, ratio=0.556386 */
     4635u,  /* idx=176, ratio=0.463544 */
     3826u,  /* idx=177, ratio=0.382617 */
     3207u,  /* idx=178, ratio=0.320687 */
     2747u,  /* idx=179, ratio=0.274655 */
     2401u,  /* idx=180, ratio=0.240056 */
     2048u,  /* idx=181, ratio=0.204768 */
     1748u,  /* idx=182, ratio=0.174838 */
     1494u,  /* idx=183, ratio=0.149358 */
     1257u,  /* idx=184, ratio=0.125677 */
     1034u,  /* idx=185, ratio=0.103426 */
      804u,  /* idx=186, ratio=0.080441 */
      480u,  /* idx=187, ratio=0.047980 */
      185u,  /* idx=188, ratio=0.018490 */
        0u,  /* idx=189, ratio=0.000000 */
        0u,  /* idx=190, ratio=0.000000 */
        0u,  /* idx=191, ratio=0.000000 */
        0u,  /* idx=192, ratio=0.000000 */
        0u,  /* idx=193, ratio=0.000000 */
        0u,  /* idx=194, ratio=0.000000 */
        0u,  /* idx=195, ratio=0.000000 */
        0u,  /* idx=196, ratio=0.000000 */
        0u,  /* idx=197, ratio=0.000000 */
        0u,  /* idx=198, ratio=0.000000 */
        0u,  /* idx=199, ratio=0.000000 */
        0u,  /* idx=200, ratio=0.000000 */
        0u,  /* idx=201, ratio=0.000000 */
        0u,  /* idx=202, ratio=0.000000 */
        0u,  /* idx=203, ratio=0.000000 */
        0u,  /* idx=204, ratio=0.000000 */
        0u,  /* idx=205, ratio=0.000000 */
        0u,  /* idx=206, ratio=0.000000 */
        0u,  /* idx=207, ratio=0.000000 */
        0u,  /* idx=208, ratio=0.000000 */
        0u,  /* idx=209, ratio=-0.000000 */
        0u,  /* idx=210, ratio=0.000000 */
     3184u,  /* idx=211, ratio=0.318371 */
     5263u,  /* idx=212, ratio=0.526284 */
     6727u,  /* idx=213, ratio=0.672725 */
     7814u,  /* idx=214, ratio=0.781445 */
     7685u,  /* idx=215, ratio=0.768510 */
     6765u,  /* idx=216, ratio=0.676455 */
     5949u,  /* idx=217, ratio=0.594878 */
     5139u,  /* idx=218, ratio=0.513860 */
     4497u,  /* idx=219, ratio=0.449713 */
     4016u,  /* idx=220, ratio=0.401641 */
     3574u,  /* idx=221, ratio=0.357436 */
     3210u,  /* idx=222, ratio=0.320975 */
     2881u,  /* idx=223, ratio=0.288102 */
     2565u,  /* idx=224, ratio=0.256528 */
     2226u,  /* idx=225, ratio=0.222607 */
     1819u,  /* idx=226, ratio=0.181919 */
     1449u,  /* idx=227, ratio=0.144937 */
     1113u,  /* idx=228, ratio=0.111282 */
      802u,  /* idx=229, ratio=0.080229 */
      520u,  /* idx=230, ratio=0.051964 */
      259u,  /* idx=231, ratio=0.025870 */
       13u,  /* idx=232, ratio=0.001346 */
        0u,  /* idx=233, ratio=0.000000 */
        0u,  /* idx=234, ratio=0.000000 */
        0u,  /* idx=235, ratio=0.000000 */
        0u,  /* idx=236, ratio=0.000000 */
        0u,  /* idx=237, ratio=0.000000 */
        0u,  /* idx=238, ratio=0.000000 */
        0u,  /* idx=239, ratio=0.000000 */
        0u,  /* idx=240, ratio=0.000000 */
        0u,  /* idx=241, ratio=0.000000 */
        0u,  /* idx=242, ratio=0.000000 */
        0u,  /* idx=243, ratio=0.000000 */
        0u,  /* idx=244, ratio=0.000000 */
        0u,  /* idx=245, ratio=0.000000 */
        0u,  /* idx=246, ratio=0.000000 */
        0u,  /* idx=247, ratio=0.000000 */
        0u,  /* idx=248, ratio=0.000000 */
        0u,  /* idx=249, ratio=0.000000 */
     2940u,  /* idx=250, ratio=0.294032 */
     4926u,  /* idx=251, ratio=0.492583 */
     6357u,  /* idx=252, ratio=0.635664 */
     7437u,  /* idx=253, ratio=0.743671 */
     8281u,  /* idx=254, ratio=0.828093 */
     8506u,  /* idx=255, ratio=0.850621 */
     7631u,  /* idx=256, ratio=0.763105 */
     6921u,  /* idx=257, ratio=0.692090 */
     6164u,  /* idx=258, ratio=0.616395 */
     5541u,  /* idx=259, ratio=0.554076 */
     5015u,  /* idx=260, ratio=0.501505 */
     4592u,  /* idx=261, ratio=0.459175 */
     4174u,  /* idx=262, ratio=0.417398 */
     3715u,  /* idx=263, ratio=0.371548 */
     3226u,  /* idx=264, ratio=0.322577 */
     2776u,  /* idx=265, ratio=0.277598 */
     2365u,  /* idx=266, ratio=0.236525 */
     1990u,  /* idx=267, ratio=0.198973 */
     1651u,  /* idx=268, ratio=0.165103 */
     1333u,  /* idx=269, ratio=0.133317 */
     1039u,  /* idx=270, ratio=0.103863 */
      766u,  /* idx=271, ratio=0.076590 */
      504u,  /* idx=272, ratio=0.050378 */
      256u,  /* idx=273, ratio=0.025587 */
       16u,  /* idx=274, ratio=0.001593 */
        0u,  /* idx=275, ratio=0.000000 */
        0u,  /* idx=276, ratio=0.000000 */
        0u,  /* idx=277, ratio=0.000000 */
        0u,  /* idx=278, ratio=0.000000 */
        0u,  /* idx=279, ratio=0.000000 */
        0u,  /* idx=280, ratio=0.000000 */
        0u,  /* idx=281, ratio=0.000000 */
        0u,  /* idx=282, ratio=0.000000 */
        0u,  /* idx=283, ratio=0.000000 */
        0u,  /* idx=284, ratio=0.000000 */
        0u,  /* idx=285, ratio=0.000000 */
        0u,  /* idx=286, ratio=-0.000000 */
        0u,  /* idx=287, ratio=0.000000 */
     2732u,  /* idx=288, ratio=0.273150 */
     4629u,  /* idx=289, ratio=0.462938 */
     6025u,  /* idx=290, ratio=0.602473 */
     7094u,  /* idx=291, ratio=0.709380 */
     7939u,  /* idx=292, ratio=0.793907 */
     8624u,  /* idx=293, ratio=0.862414 */
     9157u,  /* idx=294, ratio=0.915709 */
     8319u,  /* idx=295, ratio=0.831915 */
     7620u,  /* idx=296, ratio=0.762038 */
     6967u,  /* idx=297, ratio=0.696688 */
     6353u,  /* idx=298, ratio=0.635337 */
     5850u,  /* idx=299, ratio=0.585036 */
     5269u,  /* idx=300, ratio=0.526918 */
     4689u,  /* idx=301, ratio=0.468920 */
     4160u,  /* idx=302, ratio=0.416024 */
     3676u,  /* idx=303, ratio=0.367583 */
     3231u,  /* idx=304, ratio=0.323095 */
     2822u,  /* idx=305, ratio=0.282177 */
     2450u,  /* idx=306, ratio=0.245016 */
     2105u,  /* idx=307, ratio=0.210461 */
     1783u,  /* idx=308, ratio=0.178319 */
     1479u,  /* idx=309, ratio=0.147919 */
     1186u,  /* idx=310, ratio=0.118569 */
      912u,  /* idx=311, ratio=0.091208 */
      641u,  /* idx=312, ratio=0.064063 */
      381u,  /* idx=313, ratio=0.038138 */
      134u,  /* idx=314, ratio=0.013384 */
        0u,  /* idx=315, ratio=0.000000 */
        0u,  /* idx=316, ratio=0.000000 */
        0u,  /* idx=317, ratio=0.000000 */
        0u,  /* idx=318, ratio=0.000000 */
        0u,  /* idx=319, ratio=0.000000 */
        0u,  /* idx=320, ratio=0.000000 */
        0u,  /* idx=321, ratio=0.000000 */
        0u,  /* idx=322, ratio=0.000000 */
        0u,  /* idx=323, ratio=0.000000 */
        0u,  /* idx=324, ratio=0.000000 */
     2550u,  /* idx=325, ratio=0.255037 */
     4367u,  /* idx=326, ratio=0.436659 */
     5726u,  /* idx=327, ratio=0.572576 */
     6781u,  /* idx=328, ratio=0.678112 */
     7624u,  /* idx=329, ratio=0.762431 */
     8313u,  /* idx=330, ratio=0.831345 */
     8887u,  /* idx=331, ratio=0.888724 */
     9372u,  /* idx=332, ratio=0.937239 */
     8713u,  /* idx=333, ratio=0.871331 */
     8130u,  /* idx=334, ratio=0.813039 */
     7621u,  /* idx=335, ratio=0.762057 */
     6869u,  /* idx=336, ratio=0.686900 */
     6211u,  /* idx=337, ratio=0.621114 */
     5597u,  /* idx=338, ratio=0.559687 */
     5032u,  /* idx=339, ratio=0.503197 */
     4518u,  /* idx=340, ratio=0.451826 */
     4043u,  /* idx=341, ratio=0.404300 */
     3607u,  /* idx=342, ratio=0.360694 */
     3203u,  /* idx=343, ratio=0.320307 */
     2834u,  /* idx=344, ratio=0.283438 */
     2485u,  /* idx=345, ratio=0.248453 */
     2151u,  /* idx=346, ratio=0.215148 */
     1834u,  /* idx=347, ratio=0.183358 */
     1530u,  /* idx=348, ratio=0.152955 */
     1238u,  /* idx=349, ratio=0.123842 */
      953u,  /* idx=350, ratio=0.095296 */
      686u,  /* idx=351, ratio=0.068553 */
      427u,  /* idx=352, ratio=0.042721 */
      187u,  /* idx=353, ratio=0.018740 */
        0u,  /* idx=354, ratio=0.000000 */
        0u,  /* idx=355, ratio=0.000000 */
        0u,  /* idx=356, ratio=0.000000 */
        0u,  /* idx=357, ratio=0.000000 */
        0u,  /* idx=358, ratio=0.000000 */
        0u,  /* idx=359, ratio=-0.000000 */
        0u,  /* idx=360, ratio=0.000000 */
     2392u,  /* idx=361, ratio=0.239177 */
     4132u,  /* idx=362, ratio=0.413203 */
     5455u,  /* idx=363, ratio=0.545506 */
     6495u,  /* idx=364, ratio=0.649485 */
     7334u,  /* idx=365, ratio=0.733356 */
     8024u,  /* idx=366, ratio=0.802437 */
     8603u,  /* idx=367, ratio=0.860324 */
     9095u,  /* idx=368, ratio=0.909534 */
     9519u,  /* idx=369, ratio=0.951882 */
     8912u,  /* idx=370, ratio=0.891155 */
     8366u,  /* idx=371, ratio=0.836624 */
     7795u,  /* idx=372, ratio=0.779508 */
     7066u,  /* idx=373, ratio=0.706575 */
     6429u,  /* idx=374, ratio=0.642898 */
     5842u,  /* idx=375, ratio=0.584223 */
     5299u,  /* idx=376, ratio=0.529936 */
     4800u,  /* idx=377, ratio=0.480006 */
     4342u,  /* idx=378, ratio=0.434189 */
     3914u,  /* idx=379, ratio=0.391402 */
     3515u,  /* idx=380, ratio=0.351452 */
     3142u,  /* idx=381, ratio=0.314222 */
     2785u,  /* idx=382, ratio=0.278538 */
     2442u,  /* idx=383, ratio=0.244221 */
     2111u,  /* idx=384, ratio=0.211130 */
     1798u,  /* idx=385, ratio=0.179828 */
     1499u,  /* idx=386, ratio=0.149940 */
     1215u,  /* idx=387, ratio=0.121492 */
      946u,  /* idx=388, ratio=0.094574 */
      694u,  /* idx=389, ratio=0.069384 */
      459u,  /* idx=390, ratio=0.045905 */
      249u,  /* idx=391, ratio=0.024890 */
       65u,  /* idx=392, ratio=0.006495 */
        0u,  /* idx=393, ratio=0.000000 */
        0u,  /* idx=394, ratio=0.000000 */
        0u,  /* idx=395, ratio=0.000000 */
     2252u,  /* idx=396, ratio=0.225175 */
     3921u,  /* idx=397, ratio=0.392138 */
     5209u,  /* idx=398, ratio=0.520880 */
     6232u,  /* idx=399, ratio=0.623176 */
     7064u,  /* idx=400, ratio=0.706416 */
     7755u,  /* idx=401, ratio=0.775472 */
     8337u,  /* idx=402, ratio=0.833684 */
     8834u,  /* idx=403, ratio=0.883420 */
     9264u,  /* idx=404, ratio=0.926407 */
     9639u,  /* idx=405, ratio=0.963930 */
     9077u,  /* idx=406, ratio=0.907677 */
     8565u,  /* idx=407, ratio=0.856501 */
     7827u,  /* idx=408, ratio=0.782712 */
     7184u,  /* idx=409, ratio=0.718425 */
     6581u,  /* idx=410, ratio=0.658088 */
     6024u,  /* idx=411, ratio=0.602378 */
     5506u,  /* idx=412, ratio=0.550587 */
     5025u,  /* idx=413, ratio=0.502470 */
     4579u,  /* idx=414, ratio=0.457912 */
     4159u,  /* idx=415, ratio=0.415913 */
     3758u,  /* idx=416, ratio=0.375821 */
     3375u,  /* idx=417, ratio=0.337482 */
     3014u,  /* idx=418, ratio=0.301430 */
     2665u,  /* idx=419, ratio=0.266472 */
     2333u,  /* idx=420, ratio=0.233325 */
     2018u,  /* idx=421, ratio=0.201820 */
     1723u,  /* idx=422, ratio=0.172311 */
     1444u,  /* idx=423, ratio=0.144380 */
     1186u,  /* idx=424, ratio=0.118622 */
      950u,  /* idx=425, ratio=0.094971 */
      743u,  /* idx=426, ratio=0.074285 */
      421u,  /* idx=427, ratio=0.042142 */
        0u,  /* idx=428, ratio=-0.000000 */
        0u,  /* idx=429, ratio=0.000000 */
     2127u,  /* idx=430, ratio=0.212721 */
     3731u,  /* idx=431, ratio=0.373117 */
     4984u,  /* idx=432, ratio=0.498381 */
     5989u,  /* idx=433, ratio=0.598916 */
     6814u,  /* idx=434, ratio=0.681386 */
     7503u,  /* idx=435, ratio=0.750260 */
     8086u,  /* idx=436, ratio=0.808644 */
     8588u,  /* idx=437, ratio=0.858764 */
     9023u,  /* idx=438, ratio=0.902259 */
     9404u,  /* idx=439, ratio=0.940362 */
     9740u,  /* idx=440, ratio=0.974016 */
     9217u,  /* idx=441, ratio=0.921657 */
     8570u,  /* idx=442, ratio=0.857038 */
     7860u,  /* idx=443, ratio=0.786043 */
     7251u,  /* idx=444, ratio=0.725134 */
     6682u,  /* idx=445, ratio=0.668187 */
     6152u,  /* idx=446, ratio=0.615223 */
     5658u,  /* idx=447, ratio=0.565768 */
     5194u,  /* idx=448, ratio=0.519360 */
     4750u,  /* idx=449, ratio=0.475010 */
     4331u,  /* idx=450, ratio=0.433130 */
     3931u,  /* idx=451, ratio=0.393069 */
     3552u,  /* idx=452, ratio=0.355250 */
     3189u,  /* idx=453, ratio=0.318934 */
     2845u,  /* idx=454, ratio=0.284542 */
     2520u,  /* idx=455, ratio=0.252004 */
     2214u,  /* idx=456, ratio=0.221406 */
     1864u,  /* idx=457, ratio=0.186376 */
     1358u,  /* idx=458, ratio=0.135803 */
      880u,  /* idx=459, ratio=0.088030 */
      428u,  /* idx=460, ratio=0.042830 */
        0u,  /* idx=461, ratio=-0.000000 */
        0u,  /* idx=462, ratio=0.000000 */
     2016u,  /* idx=463, ratio=0.201572 */
     3559u,  /* idx=464, ratio=0.355856 */
     4777u,  /* idx=465, ratio=0.477746 */
     5765u,  /* idx=466, ratio=0.576474 */
     6581u,  /* idx=467, ratio=0.658069 */
     7266u,  /* idx=468, ratio=0.726636 */
     7851u,  /* idx=469, ratio=0.785064 */
     8354u,  /* idx=470, ratio=0.835447 */
     8793u,  /* idx=471, ratio=0.879339 */
     9179u,  /* idx=472, ratio=0.917919 */
     9521u,  /* idx=473, ratio=0.952096 */
     9826u,  /* idx=474, ratio=0.982584 */
     9204u,  /* idx=475, ratio=0.920402 */
     8441u,  /* idx=476, ratio=0.844065 */
     7849u,  /* idx=477, ratio=0.784936 */
     7279u,  /* idx=478, ratio=0.727914 */
     6744u,  /* idx=479, ratio=0.674431 */
     6237u,  /* idx=480, ratio=0.623714 */
     5762u,  /* idx=481, ratio=0.576178 */
     5304u,  /* idx=482, ratio=0.530388 */
     4868u,  /* idx=483, ratio=0.486827 */
     4452u,  /* idx=484, ratio=0.445225 */
     4059u,  /* idx=485, ratio=0.405856 */
     3647u,  /* idx=486, ratio=0.364684 */
     3027u,  /* idx=487, ratio=0.302671 */
     2445u,  /* idx=488, ratio=0.244471 */
     1897u,  /* idx=489, ratio=0.189743 */
     1382u,  /* idx=490, ratio=0.138186 */
      895u,  /* idx=491, ratio=0.089531 */
      435u,  /* idx=492, ratio=0.043540 */
        0u,  /* idx=493, ratio=-0.000000 */
        0u,  /* idx=494, ratio=0.000000 */
     1915u,  /* idx=495, ratio=0.191534 */
     3401u,  /* idx=496, ratio=0.340122 */
     4588u,  /* idx=497, ratio=0.458751 */
     5557u,  /* idx=498, ratio=0.555653 */
     6363u,  /* idx=499, ratio=0.636295 */
     7045u,  /* idx=500, ratio=0.704454 */
     7628u,  /* idx=501, ratio=0.762820 */
     8134u,  /* idx=502, ratio=0.813362 */
     8576u,  /* idx=503, ratio=0.857554 */
     8965u,  /* idx=504, ratio=0.896522 */
     9311u,  /* idx=505, ratio=0.931142 */
     9621u,  /* idx=506, ratio=0.962101 */
     9707u,  /* idx=507, ratio=0.970679 */
     8971u,  /* idx=508, ratio=0.897141 */
     8369u,  /* idx=509, ratio=0.836918 */
     7811u,  /* idx=510, ratio=0.781063 */
     7274u,  /* idx=511, ratio=0.727397 */
     6766u,  /* idx=512, ratio=0.676552 */
     6278u,  /* idx=513, ratio=0.627780 */
     5811u,  /* idx=514, ratio=0.581099 */
     5126u,  /* idx=515, ratio=0.512638 */
     4399u,  /* idx=516, ratio=0.439921 */
     3721u,  /* idx=517, ratio=0.372066 */
     3086u,  /* idx=518, ratio=0.308599 */
     2491u,  /* idx=519, ratio=0.249109 */
     1932u,  /* idx=520, ratio=0.193234 */
     1407u,  /* idx=521, ratio=0.140653 */
      911u,  /* idx=522, ratio=0.091083 */
      443u,  /* idx=523, ratio=0.044274 */
        0u,  /* idx=524, ratio=-0.000000 */
        0u,  /* idx=525, ratio=0.000000 */
     1824u,  /* idx=526, ratio=0.182448 */
     3257u,  /* idx=527, ratio=0.325719 */
     4412u,  /* idx=528, ratio=0.441209 */
     5363u,  /* idx=529, ratio=0.536283 */
     6159u,  /* idx=530, ratio=0.615916 */
     6836u,  /* idx=531, ratio=0.683587 */
     7418u,  /* idx=532, ratio=0.741802 */
     7924u,  /* idx=533, ratio=0.792415 */
     8368u,  /* idx=534, ratio=0.836823 */
     8761u,  /* idx=535, ratio=0.876101 */
     9111u,  /* idx=536, ratio=0.911089 */
     9425u,  /* idx=537, ratio=0.942455 */
     9707u,  /* idx=538, ratio=0.970733 */
     9416u,  /* idx=539, ratio=0.941603 */
     8803u,  /* idx=540, ratio=0.880286 */
     8273u,  /* idx=541, ratio=0.827313 */
     7746u,  /* idx=542, ratio=0.774571 */
     6910u,  /* idx=543, ratio=0.691050 */
     6044u,  /* idx=544, ratio=0.604353 */
     5240u,  /* idx=545, ratio=0.524004 */
     4493u,  /* idx=546, ratio=0.449331 */
     3798u,  /* idx=547, ratio=0.379752 */
     3148u,  /* idx=548, ratio=0.314764 */
     2539u,  /* idx=549, ratio=0.253927 */
     1969u,  /* idx=550, ratio=0.196855 */
     1432u,  /* idx=551, ratio=0.143210 */
      927u,  /* idx=552, ratio=0.092691 */
      450u,  /* idx=553, ratio=0.045033 */
        0u,  /* idx=554, ratio=-0.000000 */
        0u,  /* idx=555, ratio=0.000000 */
     1742u,  /* idx=556, ratio=0.174185 */
     3125u,  /* idx=557, ratio=0.312487 */
     4250u,  /* idx=558, ratio=0.424959 */
     5182u,  /* idx=559, ratio=0.518219 */
     5968u,  /* idx=560, ratio=0.596802 */
     6639u,  /* idx=561, ratio=0.663920 */
     7219u,  /* idx=562, ratio=0.721911 */
     7725u,  /* idx=563, ratio=0.772520 */
     8171u,  /* idx=564, ratio=0.817070 */
     8566u,  /* idx=565, ratio=0.856589 */
     8919u,  /* idx=566, ratio=0.891883 */
     9236u,  /* idx=567, ratio=0.923595 */
     9522u,  /* idx=568, ratio=0.952245 */
     9749u,  /* idx=569, ratio=0.974879 */
     9104u,  /* idx=570, ratio=0.910404 */
     8049u,  /* idx=571, ratio=0.804927 */
     7080u,  /* idx=572, ratio=0.707987 */
     6186u,  /* idx=573, ratio=0.618589 */
     5359u,  /* idx=574, ratio=0.535886 */
     4592u,  /* idx=575, ratio=0.459152 */
     3878u,  /* idx=576, ratio=0.387763 */
     3212u,  /* idx=577, ratio=0.321181 */
     2589u,  /* idx=578, ratio=0.258935 */
     2006u,  /* idx=579, ratio=0.200615 */
     1459u,  /* idx=580, ratio=0.145861 */
      944u,  /* idx=581, ratio=0.094356 */
      458u,  /* idx=582, ratio=0.045819 */
        0u,  /* idx=583, ratio=-0.000000 */
        0u,  /* idx=584, ratio=0.000000 */
     1666u,  /* idx=585, ratio=0.166639 */
     3003u,  /* idx=586, ratio=0.300289 */
     4099u,  /* idx=587, ratio=0.409864 */
     5013u,  /* idx=588, ratio=0.501332 */
     5788u,  /* idx=589, ratio=0.578838 */
     6454u,  /* idx=590, ratio=0.645353 */
     7031u,  /* idx=591, ratio=0.703059 */
     7536u,  /* idx=592, ratio=0.753599 */
     7982u,  /* idx=593, ratio=0.798228 */
     8379u,  /* idx=594, ratio=0.837927 */
     8735u,  /* idx=595, ratio=0.873469 */
     9055u,  /* idx=596, ratio=0.905475 */
     9344u,  /* idx=597, ratio=0.934448 */
     9353u,  /* idx=598, ratio=0.935348 */
     8260u,  /* idx=599, ratio=0.826027 */
     7258u,  /* idx=600, ratio=0.725776 */
     6335u,  /* idx=601, ratio=0.633513 */
     5483u,  /* idx=602, ratio=0.548319 */
     4694u,  /* idx=603, ratio=0.469411 */
     3961u,  /* idx=604, ratio=0.396119 */
     3279u,  /* idx=605, ratio=0.327864 */
     2641u,  /* idx=606, ratio=0.264144 */
     2045u,  /* idx=607, ratio=0.204521 */
     1486u,  /* idx=608, ratio=0.148613 */
      961u,  /* idx=609, ratio=0.096082 */
      466u,  /* idx=610, ratio=0.046632 */
        0u,  /* idx=611, ratio=0.000000 */
        0u,  /* idx=612, ratio=0.000000 */
     1597u,  /* idx=613, ratio=0.159718 */
     2890u,  /* idx=614, ratio=0.289006 */
     3958u,  /* idx=615, ratio=0.395804 */
     4855u,  /* idx=616, ratio=0.485510 */
     5619u,  /* idx=617, ratio=0.561924 */
     6278u,  /* idx=618, ratio=0.627796 */
     6852u,  /* idx=619, ratio=0.685167 */
     7356u,  /* idx=620, ratio=0.735583 */
     7802u,  /* idx=621, ratio=0.780236 */
     8201u,  /* idx=622, ratio=0.820061 */
     8558u,  /* idx=623, ratio=0.855800 */
     8881u,  /* idx=624, ratio=0.888053 */
     9173u,  /* idx=625, ratio=0.917305 */
     8483u,  /* idx=626, ratio=0.848263 */
     7445u,  /* idx=627, ratio=0.744482 */
     6492u,  /* idx=628, ratio=0.649174 */
     5613u,  /* idx=629, ratio=0.561342 */
     4801u,  /* idx=630, ratio=0.480140 */
     4048u,  /* idx=631, ratio=0.404844 */
     3348u,  /* idx=632, ratio=0.334832 */
     2696u,  /* idx=633, ratio=0.269567 */
     2086u,  /* idx=634, ratio=0.208582 */
     1515u,  /* idx=635, ratio=0.151470 */
      979u,  /* idx=636, ratio=0.097873 */
      475u,  /* idx=637, ratio=0.047475 */
        0u,  /* idx=638, ratio=-0.000000 */
        0u,  /* idx=639, ratio=0.000000 */
     1534u,  /* idx=640, ratio=0.153350 */
     2785u,  /* idx=641, ratio=0.278541 */
     3827u,  /* idx=642, ratio=0.382677 */
     4707u,  /* idx=643, ratio=0.470657 */
     5460u,  /* idx=644, ratio=0.545971 */
     6112u,  /* idx=645, ratio=0.611170 */
     6682u,  /* idx=646, ratio=0.668163 */
     7184u,  /* idx=647, ratio=0.718408 */
     7630u,  /* idx=648, ratio=0.763037 */
     8029u,  /* idx=649, ratio=0.802940 */
     8388u,  /* idx=650, ratio=0.838832 */
     8713u,  /* idx=651, ratio=0.871288 */
     8717u,  /* idx=652, ratio=0.871729 */
     7642u,  /* idx=653, ratio=0.764177 */
     6656u,  /* idx=654, ratio=0.665629 */
     5750u,  /* idx=655, ratio=0.574999 */
     4914u,  /* idx=656, ratio=0.491370 */
     4140u,  /* idx=657, ratio=0.413961 */
     3421u,  /* idx=658, ratio=0.342102 */
     2752u,  /* idx=659, ratio=0.275218 */
     2128u,  /* idx=660, ratio=0.212808 */
     1544u,  /* idx=661, ratio=0.154440 */
      997u,  /* idx=662, ratio=0.099731 */
      483u,  /* idx=663, ratio=0.048350 */
        0u,  /* idx=664, ratio=0.000000 */
        0u,  /* idx=665, ratio=0.000000 */
     1475u,  /* idx=666, ratio=0.147470 */
     2688u,  /* idx=667, ratio=0.268808 */
     3704u,  /* idx=668, ratio=0.370393 */
     4567u,  /* idx=669, ratio=0.456686 */
     5309u,  /* idx=670, ratio=0.530898 */
     5954u,  /* idx=671, ratio=0.595401 */
     6520u,  /* idx=672, ratio=0.651982 */
     7020u,  /* idx=673, ratio=0.702017 */
     7466u,  /* idx=674, ratio=0.746579 */
     7865u,  /* idx=675, ratio=0.786521 */
     8225u,  /* idx=676, ratio=0.822524 */
     8551u,  /* idx=677, ratio=0.855145 */
     7849u,  /* idx=678, ratio=0.784943 */
     6829u,  /* idx=679, ratio=0.682940 */
     5893u,  /* idx=680, ratio=0.589338 */
     5031u,  /* idx=681, ratio=0.503139 */
     4235u,  /* idx=682, ratio=0.423498 */
     3497u,  /* idx=683, ratio=0.349695 */
     2811u,  /* idx=684, ratio=0.281110 */
     2172u,  /* idx=685, ratio=0.217209 */
     1575u,  /* idx=686, ratio=0.157528 */
     1017u,  /* idx=687, ratio=0.101662 */
      493u,  /* idx=688, ratio=0.049257 */
        0u,  /* idx=689, ratio=0.000000 */
        0u,  /* idx=690, ratio=0.000000 */
     1420u,  /* idx=691, ratio=0.142025 */
     2597u,  /* idx=692, ratio=0.259731 */
     3589u,  /* idx=693, ratio=0.358873 */
     4435u,  /* idx=694, ratio=0.443520 */
     5166u,  /* idx=695, ratio=0.516635 */
     5804u,  /* idx=696, ratio=0.580425 */
     6366u,  /* idx=697, ratio=0.636567 */
     6864u,  /* idx=698, ratio=0.686357 */
     7308u,  /* idx=699, ratio=0.730817 */
     7708u,  /* idx=700, ratio=0.770759 */
     8068u,  /* idx=701, ratio=0.806838 */
     8069u,  /* idx=702, ratio=0.806869 */
     7012u,  /* idx=703, ratio=0.701176 */
     6044u,  /* idx=704, ratio=0.604410 */
     5155u,  /* idx=705, ratio=0.515485 */
     4335u,  /* idx=706, ratio=0.433485 */
     3576u,  /* idx=707, ratio=0.357632 */
     2873u,  /* idx=708, ratio=0.287260 */
     2218u,  /* idx=709, ratio=0.221796 */
     1607u,  /* idx=710, ratio=0.160742 */
     1037u,  /* idx=711, ratio=0.103669 */
      502u,  /* idx=712, ratio=0.050198 */
        0u,  /* idx=713, ratio=0.000000 */
        0u,  /* idx=714, ratio=0.000000 */
     1370u,  /* idx=715, ratio=0.136967 */
     2512u,  /* idx=716, ratio=0.251248 */
     3480u,  /* idx=717, ratio=0.348047 */
     4311u,  /* idx=718, ratio=0.431092 */
     5031u,  /* idx=719, ratio=0.503119 */
     5662u,  /* idx=720, ratio=0.566185 */
     6219u,  /* idx=721, ratio=0.621863 */
     6714u,  /* idx=722, ratio=0.671381 */
     7157u,  /* idx=723, ratio=0.715707 */
     7556u,  /* idx=724, ratio=0.755616 */
     7917u,  /* idx=725, ratio=0.791739 */
     7204u,  /* idx=726, ratio=0.720412 */
     6203u,  /* idx=727, ratio=0.620272 */
     5285u,  /* idx=728, ratio=0.528452 */
     4440u,  /* idx=729, ratio=0.443955 */
     3659u,  /* idx=730, ratio=0.365939 */
     2937u,  /* idx=731, ratio=0.293686 */
     2266u,  /* idx=732, ratio=0.226580 */
     1641u,  /* idx=733, ratio=0.164090 */
     1058u,  /* idx=734, ratio=0.105756 */
      512u,  /* idx=735, ratio=0.051176 */
        0u,  /* idx=736, ratio=0.000000 */
        0u,  /* idx=737, ratio=0.000000 */
     1323u,  /* idx=738, ratio=0.132257 */
     2433u,  /* idx=739, ratio=0.243301 */
     3379u,  /* idx=740, ratio=0.337856 */
     4193u,  /* idx=741, ratio=0.419342 */
     4903u,  /* idx=742, ratio=0.490292 */
     5526u,  /* idx=743, ratio=0.552626 */
     6078u,  /* idx=744, ratio=0.607824 */
     6570u,  /* idx=745, ratio=0.657044 */
     7012u,  /* idx=746, ratio=0.701208 */
     7411u,  /* idx=747, ratio=0.741057 */
     7407u,  /* idx=748, ratio=0.740734 */
     6370u,  /* idx=749, ratio=0.636990 */
     5421u,  /* idx=750, ratio=0.542088 */
     4549u,  /* idx=751, ratio=0.454942 */
     3746u,  /* idx=752, ratio=0.374640 */
     3004u,  /* idx=753, ratio=0.300405 */
     2316u,  /* idx=754, ratio=0.231575 */
     1676u,  /* idx=755, ratio=0.167581 */
     1079u,  /* idx=756, ratio=0.107929 */
      522u,  /* idx=757, ratio=0.052194 */
        0u,  /* idx=758, ratio=0.000000 */
        0u,  /* idx=759, ratio=0.000000 */
     1279u,  /* idx=760, ratio=0.127860 */
     2358u,  /* idx=761, ratio=0.235841 */
     3282u,  /* idx=762, ratio=0.328245 */
     4082u,  /* idx=763, ratio=0.408215 */
     4781u,  /* idx=764, ratio=0.478102 */
     5397u,  /* idx=765, ratio=0.539701 */
     5944u,  /* idx=766, ratio=0.594404 */
     6433u,  /* idx=767, ratio=0.643307 */
     6873u,  /* idx=768, ratio=0.687286 */
     7270u,  /* idx=769, ratio=0.727049 */
     6546u,  /* idx=770, ratio=0.654635 */
     5564u,  /* idx=771, ratio=0.556447 */
     4665u,  /* idx=772, ratio=0.466488 */
     3838u,  /* idx=773, ratio=0.383765 */
     3074u,  /* idx=774, ratio=0.307439 */
     2368u,  /* idx=775, ratio=0.236796 */
     1712u,  /* idx=776, ratio=0.171223 */
     1102u,  /* idx=777, ratio=0.110194 */
      533u,  /* idx=778, ratio=0.053252 */
        0u,  /* idx=779, ratio=0.000000 */
        0u,  /* idx=780, ratio=0.000000 */
     1237u,  /* idx=781, ratio=0.123747 */
     2288u,  /* idx=782, ratio=0.228826 */
     3192u,  /* idx=783, ratio=0.319165 */
     3977u,  /* idx=784, ratio=0.397663 */
     4665u,  /* idx=785, ratio=0.466504 */
     5274u,  /* idx=786, ratio=0.527368 */
     5816u,  /* idx=787, ratio=0.581564 */
     6301u,  /* idx=788, ratio=0.630132 */
     6739u,  /* idx=789, ratio=0.673905 */
     6733u,  /* idx=790, ratio=0.673284 */
     5716u,  /* idx=791, ratio=0.571587 */
     4786u,  /* idx=792, ratio=0.478634 */
     3933u,  /* idx=793, ratio=0.393346 */
     3148u,  /* idx=794, ratio=0.314811 */
     2423u,  /* idx=795, ratio=0.242257 */
     1750u,  /* idx=796, ratio=0.175027 */
     1126u,  /* idx=797, ratio=0.112556 */
      544u,  /* idx=798, ratio=0.054354 */
        0u,  /* idx=799, ratio=0.000000 */
        0u,  /* idx=800, ratio=0.000000 */
     1199u,  /* idx=801, ratio=0.119889 */
     2222u,  /* idx=802, ratio=0.222215 */
     3106u,  /* idx=803, ratio=0.310574 */
     3876u,  /* idx=804, ratio=0.387643 */
     4555u,  /* idx=805, ratio=0.455456 */
     5156u,  /* idx=806, ratio=0.515585 */
     5693u,  /* idx=807, ratio=0.569267 */
     6175u,  /* idx=808, ratio=0.617486 */
     6610u,  /* idx=809, ratio=0.661036 */
     5876u,  /* idx=810, ratio=0.587574 */
     4914u,  /* idx=811, ratio=0.491431 */
     4034u,  /* idx=812, ratio=0.403417 */
     3225u,  /* idx=813, ratio=0.322544 */
     2480u,  /* idx=814, ratio=0.247976 */
     1790u,  /* idx=815, ratio=0.179004 */
     1150u,  /* idx=816, ratio=0.115021 */
      555u,  /* idx=817, ratio=0.055503 */
        0u,  /* idx=818, ratio=0.000000 */
        0u,  /* idx=819, ratio=0.000000 */
     1163u,  /* idx=820, ratio=0.116265 */
     2160u,  /* idx=821, ratio=0.215976 */
     3024u,  /* idx=822, ratio=0.302434 */
     3781u,  /* idx=823, ratio=0.378116 */
     4449u,  /* idx=824, ratio=0.444918 */
     5043u,  /* idx=825, ratio=0.504318 */
     5575u,  /* idx=826, ratio=0.557480 */
     6053u,  /* idx=827, ratio=0.605338 */
     6045u,  /* idx=828, ratio=0.604481 */
     5049u,  /* idx=829, ratio=0.504930 */
     4140u,  /* idx=830, ratio=0.414018 */
     3307u,  /* idx=831, ratio=0.330667 */
     2540u,  /* idx=832, ratio=0.253972 */
     1832u,  /* idx=833, ratio=0.183166 */
     1176u,  /* idx=834, ratio=0.117596 */
      567u,  /* idx=835, ratio=0.056702 */
        0u,  /* idx=836, ratio=0.000000 */
        0u,  /* idx=837, ratio=0.000000 */
     1129u,  /* idx=838, ratio=0.112854 */
     2101u,  /* idx=839, ratio=0.210078 */
     2947u,  /* idx=840, ratio=0.294709 */
     3690u,  /* idx=841, ratio=0.369045 */
     4349u,  /* idx=842, ratio=0.434857 */
     4935u,  /* idx=843, ratio=0.493532 */
     5462u,  /* idx=844, ratio=0.546170 */
     5937u,  /* idx=845, ratio=0.593659 */
     5192u,  /* idx=846, ratio=0.519191 */
     4252u,  /* idx=847, ratio=0.425191 */
     3392u,  /* idx=848, ratio=0.339210 */
     2603u,  /* idx=849, ratio=0.260265 */
     1875u,  /* idx=850, ratio=0.187526 */
     1203u,  /* idx=851, ratio=0.120289 */
      580u,  /* idx=852, ratio=0.057953 */
        0u,  /* idx=853, ratio=0.000000 */
        0u,  /* idx=854, ratio=-0.000000 */
     1096u,  /* idx=855, ratio=0.109637 */
     2045u,  /* idx=856, ratio=0.204493 */
     2874u,  /* idx=857, ratio=0.287369 */
     3604u,  /* idx=858, ratio=0.360400 */
     4252u,  /* idx=859, ratio=0.425241 */
     4832u,  /* idx=860, ratio=0.483198 */
     5353u,  /* idx=861, ratio=0.535311 */
     5343u,  /* idx=862, ratio=0.534282 */
     4370u,  /* idx=863, ratio=0.436983 */
     3482u,  /* idx=864, ratio=0.348206 */
     2669u,  /* idx=865, ratio=0.266878 */
     1921u,  /* idx=866, ratio=0.192099 */
     1231u,  /* idx=867, ratio=0.123109 */
      593u,  /* idx=868, ratio=0.059261 */
        0u,  /* idx=869, ratio=0.000000 */
        0u,  /* idx=870, ratio=0.000000 */
     1066u,  /* idx=871, ratio=0.106598 */
     1992u,  /* idx=872, ratio=0.199197 */
     2804u,  /* idx=873, ratio=0.280386 */
     3522u,  /* idx=874, ratio=0.352150 */
     4160u,  /* idx=875, ratio=0.416042 */
     4733u,  /* idx=876, ratio=0.473288 */
     5249u,  /* idx=877, ratio=0.524875 */
     4494u,  /* idx=878, ratio=0.449449 */
     3577u,  /* idx=879, ratio=0.357692 */
     2738u,  /* idx=880, ratio=0.273835 */
     1969u,  /* idx=881, ratio=0.196900 */
     1261u,  /* idx=882, ratio=0.126064 */
      606u,  /* idx=883, ratio=0.060629 */
        0u,  /* idx=884, ratio=0.000000 */
        0u,  /* idx=885, ratio=0.000000 */
     1037u,  /* idx=886, ratio=0.103723 */
     1942u,  /* idx=887, ratio=0.194169 */
     2737u,  /* idx=888, ratio=0.273734 */
     3443u,  /* idx=889, ratio=0.344270 */
     4072u,  /* idx=890, ratio=0.407231 */
     4638u,  /* idx=891, ratio=0.463776 */
     4626u,  /* idx=892, ratio=0.462646 */
     3677u,  /* idx=893, ratio=0.367709 */
     2812u,  /* idx=894, ratio=0.281165 */
     2019u,  /* idx=895, ratio=0.201948 */
     1292u,  /* idx=896, ratio=0.129165 */
      621u,  /* idx=897, ratio=0.062062 */
        0u,  /* idx=898, ratio=0.000000 */
        0u,  /* idx=899, ratio=0.000000 */
     1010u,  /* idx=900, ratio=0.100999 */
     1894u,  /* idx=901, ratio=0.189389 */
     2674u,  /* idx=902, ratio=0.267391 */
     3367u,  /* idx=903, ratio=0.336735 */
     3988u,  /* idx=904, ratio=0.398787 */
     4546u,  /* idx=905, ratio=0.454639 */
     3783u,  /* idx=906, ratio=0.378304 */
     2889u,  /* idx=907, ratio=0.288899 */
     2073u,  /* idx=908, ratio=0.207261 */
     1324u,  /* idx=909, ratio=0.132421 */
      636u,  /* idx=910, ratio=0.063564 */
        0u,  /* idx=911, ratio=0.000000 */
        0u,  /* idx=912, ratio=0.000000 */
      984u,  /* idx=913, ratio=0.098415 */
     1848u,  /* idx=914, ratio=0.184838 */
     2613u,  /* idx=915, ratio=0.261334 */
     3295u,  /* idx=916, ratio=0.329522 */
     3907u,  /* idx=917, ratio=0.390685 */
     3895u,  /* idx=918, ratio=0.389527 */
     2971u,  /* idx=919, ratio=0.297069 */
     2129u,  /* idx=920, ratio=0.212861 */
     1358u,  /* idx=921, ratio=0.135846 */
      651u,  /* idx=922, ratio=0.065141 */
        0u,  /* idx=923, ratio=-0.000000 */
        0u,  /* idx=924, ratio=0.000000 */
      960u,  /* idx=925, ratio=0.095960 */
     1805u,  /* idx=926, ratio=0.180500 */
     2555u,  /* idx=927, ratio=0.255546 */
     3226u,  /* idx=928, ratio=0.322612 */
     3829u,  /* idx=929, ratio=0.382906 */
     3057u,  /* idx=930, ratio=0.305715 */
     2188u,  /* idx=931, ratio=0.218772 */
     1395u,  /* idx=932, ratio=0.139454 */
      668u,  /* idx=933, ratio=0.066798 */
        0u,  /* idx=934, ratio=-0.000000 */
        0u,  /* idx=935, ratio=0.000000 */
      936u,  /* idx=936, ratio=0.093624 */
     1764u,  /* idx=937, ratio=0.176362 */
     2500u,  /* idx=938, ratio=0.250009 */
     3160u,  /* idx=939, ratio=0.315986 */
     3149u,  /* idx=940, ratio=0.314880 */
     2250u,  /* idx=941, ratio=0.225021 */
     1433u,  /* idx=942, ratio=0.143257 */
      685u,  /* idx=943, ratio=0.068542 */
        0u,  /* idx=944, ratio=0.000000 */
        0u,  /* idx=945, ratio=0.000000 */
      914u,  /* idx=946, ratio=0.091399 */
     1724u,  /* idx=947, ratio=0.172409 */
     2447u,  /* idx=948, ratio=0.244707 */
     3096u,  /* idx=949, ratio=0.309626 */
     2316u,  /* idx=950, ratio=0.231638 */
     1473u,  /* idx=951, ratio=0.147275 */
      704u,  /* idx=952, ratio=0.070379 */
        0u,  /* idx=953, ratio=0.000000 */
        0u,  /* idx=954, ratio=0.000000 */
      893u,  /* idx=955, ratio=0.089277 */
     1686u,  /* idx=956, ratio=0.168630 */
     2396u,  /* idx=957, ratio=0.239625 */
     2387u,  /* idx=958, ratio=0.238655 */
     1515u,  /* idx=959, ratio=0.151524 */
      723u,  /* idx=960, ratio=0.072317 */
        0u,  /* idx=961, ratio=0.000000 */
        0u,  /* idx=962, ratio=0.000000 */
      873u,  /* idx=963, ratio=0.087252 */
     1650u,  /* idx=964, ratio=0.165012 */
     2347u,  /* idx=965, ratio=0.234750 */
     1560u,  /* idx=966, ratio=0.156025 */
      744u,  /* idx=967, ratio=0.074365 */
        0u,  /* idx=968, ratio=0.000000 */
        0u,  /* idx=969, ratio=0.000000 */
      853u,  /* idx=970, ratio=0.085316 */
     1615u,  /* idx=971, ratio=0.161547 */
     1608u,  /* idx=972, ratio=0.160802 */
      765u,  /* idx=973, ratio=0.076532 */
        0u,  /* idx=974, ratio=0.000000 */
        0u,  /* idx=975, ratio=0.000000 */
      835u,  /* idx=976, ratio=0.083465 */
     1582u,  /* idx=977, ratio=0.158224 */
      788u,  /* idx=978, ratio=0.078829 */
        0u,  /* idx=979, ratio=0.000000 */
        0u,  /* idx=980, ratio=-0.000000 */
      809u,  /* idx=981, ratio=0.080931 */
      813u,  /* idx=982, ratio=0.081269 */
        0u,  /* idx=983, ratio=0.000000 */
        0u,  /* idx=984, ratio=0.000000 */
      619u,  /* idx=985, ratio=0.061890 */
        0u,  /* idx=986, ratio=0.000000 */
        0u,  /* idx=987, ratio=0.000000 */
        0u,  /* idx=988, ratio=0.000000 */
        0u,  /* idx=989, ratio=0.000000 */
};

typedef struct
{
    uint16_t lutIndex;
    uint16_t u;
    uint16_t v;
    uint32_t distance2;
} AlgoRatioK_NearestPointType;

static uint16_t AlgoRatioK_GetRatioQ10000ByIndex(uint16_t lutIndex)
{
    if (lutIndex >= ALGO_RATIO_K_LUT_COUNT)
    {
        return 0u;
    }

    return g_algoRatioKRatioQ10000Lut[lutIndex];
}
static uint32_t AlgoRatioK_CalcDistance2(uint16_t u1,
                                         uint16_t v1,
                                         uint16_t u2,
                                         uint16_t v2)
{
    int32_t du;
    int32_t dv;

    du = (int32_t)u1 - (int32_t)u2;
    dv = (int32_t)v1 - (int32_t)v2;

    return (uint32_t)((du * du) + (dv * dv));
}

static int32_t AlgoRatioK_AbsS32(int32_t value)
{
    if (value < 0)
    {
        return -value;
    }

    return value;
}

static int64_t AlgoRatioK_AbsS64(int64_t value)
{
    if (value < 0)
    {
        return -value;
    }

    return value;
}

static int64_t AlgoRatioK_Cross2DS64(int32_t ax,
                                     int32_t ay,
                                     int32_t bx,
                                     int32_t by)
{
    return ((int64_t)ax * (int64_t)by) - ((int64_t)ay * (int64_t)bx);
}

static fix16_t AlgoRatioK_AbsFix16(fix16_t value)
{
    if (value < 0)
    {
        return -value;
    }

    return value;
}

static uint32_t AlgoRatioK_ReciprocalWeightQ(uint32_t distance2)
{
    uint64_t numerator;

    if (distance2 == 0u)
    {
        return 0u;
    }

    numerator = (uint64_t)1u << ALGO_RATIO_K_WEIGHT_Q_SHIFT;
    return (uint32_t)((numerator + (uint64_t)(distance2 / 2u)) / (uint64_t)distance2);
}

static fix16_t AlgoRatioK_DivS64ToFix16(int64_t numerator,
                                        int64_t denominator)
{
    uint64_t absNumerator;
    uint64_t absDenominator;
    uint64_t quotient;
    uint8_t negative;

    if (denominator == 0)
    {
        return 0;
    }

    negative = 0u;
    if ((numerator < 0) != (denominator < 0))
    {
        negative = 1u;
    }

    absNumerator = (uint64_t)AlgoRatioK_AbsS64(numerator);
    absDenominator = (uint64_t)AlgoRatioK_AbsS64(denominator);
    quotient = ((absNumerator << 16) + (absDenominator / 2u)) / absDenominator;

    if (quotient > 0x7FFFFFFFu)
    {
        quotient = 0x7FFFFFFFu;
    }

    if (negative != 0u)
    {
        return (fix16_t)(-(int32_t)quotient);
    }

    return (fix16_t)quotient;
}

static fix16_t AlgoRatioK_ScaledIntToFix16(int32_t value,
                                           int32_t scale)
{
    return AlgoRatioK_DivS64ToFix16((int64_t)value, (int64_t)scale);
}

static uint16_t AlgoRatioK_ClampRatioQ10000FromFix16(fix16_t value)
{
    fix16_t ratioFix16;
    int32_t ratio;

    if (value <= 0)
    {
        return 0u;
    }

    if (value >= fix16_one)
    {
        return ALGO_RATIO_K_RATIO_Q10000_MAX;
    }

    ratioFix16 = fix16_mul(value, fix16_from_int(ALGO_RATIO_K_RATIO_Q10000_MAX));
    ratio = fix16_to_int(ratioFix16);
    if (ratio < 0)
    {
        return 0u;
    }
    if (ratio > (int32_t)ALGO_RATIO_K_RATIO_Q10000_MAX)
    {
        return ALGO_RATIO_K_RATIO_Q10000_MAX;
    }

    return (uint16_t)ratio;
}

static void AlgoRatioK_InsertNearestPoint(AlgoRatioK_NearestPointType *nearest,
                                          uint8_t nearestCount,
                                          AlgoRatioK_NearestPointType newPoint)
{
    uint8_t insertIndex;
    uint8_t shiftIndex;

    insertIndex = 0u;
    while ((insertIndex < nearestCount) && (insertIndex < ALGO_RATIO_K_AMOUNT))
    {
        if (newPoint.distance2 < nearest[insertIndex].distance2)
        {
            break;
        }
        insertIndex++;
    }

    if (insertIndex >= ALGO_RATIO_K_AMOUNT)
    {
        return;
    }

    if (nearestCount < ALGO_RATIO_K_AMOUNT)
    {
        shiftIndex = nearestCount;
    }
    else
    {
        shiftIndex = (uint8_t)(ALGO_RATIO_K_AMOUNT - 1u);
    }

    while (shiftIndex > insertIndex)
    {
        nearest[shiftIndex] = nearest[shiftIndex - 1u];
        shiftIndex--;
    }

    nearest[insertIndex] = newPoint;
}

static uint8_t AlgoRatioK_Solve3x3Fix16(fix16_t A[3][3],
                                        fix16_t B[3],
                                        fix16_t X[3])
{
    uint8_t pivotIndex;
    uint8_t rowIndex;
    uint8_t colIndex;
    uint8_t bestRow;
    fix16_t bestAbs;
    fix16_t pivot;
    fix16_t factor;
    fix16_t temp;

    if ((A == (void *)0) || (B == (void *)0) || (X == (void *)0))
    {
        return 0u;
    }

    for (pivotIndex = 0u; pivotIndex < 3u; pivotIndex++)
    {
        bestRow = pivotIndex;
        bestAbs = AlgoRatioK_AbsFix16(A[pivotIndex][pivotIndex]);

        for (rowIndex = (uint8_t)(pivotIndex + 1u); rowIndex < 3u; rowIndex++)
        {
            temp = AlgoRatioK_AbsFix16(A[rowIndex][pivotIndex]);
            if (temp > bestAbs)
            {
                bestAbs = temp;
                bestRow = rowIndex;
            }
        }

        if (bestAbs <= ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16)
        {
            return 0u;
        }

        if (bestRow != pivotIndex)
        {
            for (colIndex = pivotIndex; colIndex < 3u; colIndex++)
            {
                temp = A[pivotIndex][colIndex];
                A[pivotIndex][colIndex] = A[bestRow][colIndex];
                A[bestRow][colIndex] = temp;
            }

            temp = B[pivotIndex];
            B[pivotIndex] = B[bestRow];
            B[bestRow] = temp;
        }

        pivot = A[pivotIndex][pivotIndex];

        for (rowIndex = (uint8_t)(pivotIndex + 1u); rowIndex < 3u; rowIndex++)
        {
            if (AlgoRatioK_AbsFix16(A[rowIndex][pivotIndex]) <= ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16)
            {
                continue;
            }

            factor = fix16_div(A[rowIndex][pivotIndex], pivot);
            for (colIndex = pivotIndex; colIndex < 3u; colIndex++)
            {
                A[rowIndex][colIndex] = fix16_sub(A[rowIndex][colIndex],
                                                  fix16_mul(factor, A[pivotIndex][colIndex]));
            }
            B[rowIndex] = fix16_sub(B[rowIndex], fix16_mul(factor, B[pivotIndex]));
        }
    }

    for (rowIndex = 3u; rowIndex > 0u; rowIndex--)
    {
        pivotIndex = (uint8_t)(rowIndex - 1u);
        temp = B[pivotIndex];
        for (colIndex = (uint8_t)(pivotIndex + 1u); colIndex < 3u; colIndex++)
        {
            temp = fix16_sub(temp, fix16_mul(A[pivotIndex][colIndex], X[colIndex]));
        }

        pivot = A[pivotIndex][pivotIndex];
        if (AlgoRatioK_AbsFix16(pivot) <= ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16)
        {
            return 0u;
        }

        X[pivotIndex] = fix16_div(temp, pivot);
    }

    return 1u;
}
static uint8_t AlgoRatioK_LocalPlaneFitQ10000(const AlgoRatioK_NearestPointType *nearest,
                                              uint8_t count,
                                              uint16_t targetU,
                                              uint16_t targetV,
                                              uint16_t *whiteRatioQ10000)
{
    uint32_t maxWeightQ;
    int32_t coordNormalizer;
    int32_t pointX;
    int32_t pointY;
    uint32_t weightQ;
    uint16_t ratioQ10000;
    fix16_t weightFix16;
    fix16_t xFix16;
    fix16_t yFix16;
    fix16_t zFix16;
    fix16_t sxx;
    fix16_t sxy;
    fix16_t sx;
    fix16_t syy;
    fix16_t sy;
    fix16_t s;
    fix16_t sxz;
    fix16_t syz;
    fix16_t sz;
    fix16_t matrixA[3][3];
    fix16_t vectorB[3];
    fix16_t coef[3];
    uint8_t i;

    if ((nearest == (void *)0) || (whiteRatioQ10000 == (void *)0) || (count < 3u))
    {
        return 0u;
    }

    maxWeightQ = 0u;
    coordNormalizer = 0;

    for (i = 0u; i < count; i++)
    {
        ratioQ10000 = AlgoRatioK_GetRatioQ10000ByIndex(nearest[i].lutIndex);
        if (nearest[i].distance2 == 0u)
        {
            *whiteRatioQ10000 = ratioQ10000;
            return 1u;
        }

        pointX = (int32_t)nearest[i].u - (int32_t)targetU;
        pointY = (int32_t)nearest[i].v - (int32_t)targetV;
        weightQ = AlgoRatioK_ReciprocalWeightQ(nearest[i].distance2);

        if (coordNormalizer < (int32_t)AlgoRatioK_AbsS64(pointX))
        {
            coordNormalizer = (int32_t)AlgoRatioK_AbsS64(pointX);
        }
        if (coordNormalizer < (int32_t)AlgoRatioK_AbsS64(pointY))
        {
            coordNormalizer = (int32_t)AlgoRatioK_AbsS64(pointY);
        }
        if (maxWeightQ < weightQ)
        {
            maxWeightQ = weightQ;
        }
    }

    if ((coordNormalizer <= 0) || (maxWeightQ == 0u))
    {
        return 0u;
    }

    sxx = 0;
    sxy = 0;
    sx = 0;
    syy = 0;
    sy = 0;
    s = 0;
    sxz = 0;
    syz = 0;
    sz = 0;

    for (i = 0u; i < count; i++)
    {
        ratioQ10000 = AlgoRatioK_GetRatioQ10000ByIndex(nearest[i].lutIndex);
        pointX = (int32_t)nearest[i].u - (int32_t)targetU;
        pointY = (int32_t)nearest[i].v - (int32_t)targetV;
        weightQ = AlgoRatioK_ReciprocalWeightQ(nearest[i].distance2);

        weightFix16 = AlgoRatioK_DivS64ToFix16((int64_t)weightQ, (int64_t)maxWeightQ);
        xFix16 = AlgoRatioK_ScaledIntToFix16(pointX, coordNormalizer);
        yFix16 = AlgoRatioK_ScaledIntToFix16(pointY, coordNormalizer);
        zFix16 = AlgoRatioK_ScaledIntToFix16((int32_t)ratioQ10000, ALGO_RATIO_K_RATIO_NORMALIZER);

        sxx = fix16_add(sxx, fix16_mul(weightFix16, fix16_mul(xFix16, xFix16)));
        sxy = fix16_add(sxy, fix16_mul(weightFix16, fix16_mul(xFix16, yFix16)));
        sx = fix16_add(sx, fix16_mul(weightFix16, xFix16));
        syy = fix16_add(syy, fix16_mul(weightFix16, fix16_mul(yFix16, yFix16)));
        sy = fix16_add(sy, fix16_mul(weightFix16, yFix16));
        s = fix16_add(s, weightFix16);
        sxz = fix16_add(sxz, fix16_mul(weightFix16, fix16_mul(xFix16, zFix16)));
        syz = fix16_add(syz, fix16_mul(weightFix16, fix16_mul(yFix16, zFix16)));
        sz = fix16_add(sz, fix16_mul(weightFix16, zFix16));
    }

    matrixA[0][0] = sxx;
    matrixA[0][1] = sxy;
    matrixA[0][2] = sx;
    matrixA[1][0] = sxy;
    matrixA[1][1] = syy;
    matrixA[1][2] = sy;
    matrixA[2][0] = sx;
    matrixA[2][1] = sy;
    matrixA[2][2] = s;

    vectorB[0] = sxz;
    vectorB[1] = syz;
    vectorB[2] = sz;

    coef[0] = 0;
    coef[1] = 0;
    coef[2] = 0;

    if (AlgoRatioK_Solve3x3Fix16(matrixA, vectorB, coef) == 0u)
    {
        return 0u;
    }

    *whiteRatioQ10000 = AlgoRatioK_ClampRatioQ10000FromFix16(coef[2]);
    return 1u;
}

static uint8_t AlgoRatioK_WeightedAverageQ10000(const AlgoRatioK_NearestPointType *nearest,
                                                uint8_t count,
                                                uint16_t *whiteRatioQ10000)
{
    int64_t weightedRatioSum;
    int64_t weightSum;
    uint32_t weightQ;
    uint16_t ratioQ10000;
    uint8_t i;

    if ((nearest == (void *)0) || (whiteRatioQ10000 == (void *)0) || (count == 0u))
    {
        return 0u;
    }

    weightedRatioSum = 0;
    weightSum = 0;

    for (i = 0u; i < count; i++)
    {
        ratioQ10000 = AlgoRatioK_GetRatioQ10000ByIndex(nearest[i].lutIndex);
        if (nearest[i].distance2 == 0u)
        {
            *whiteRatioQ10000 = ratioQ10000;
            return 1u;
        }

        weightQ = AlgoRatioK_ReciprocalWeightQ(nearest[i].distance2);
        weightedRatioSum += (int64_t)weightQ * (int64_t)ratioQ10000;
        weightSum += (int64_t)weightQ;
    }

    if (weightSum <= 0)
    {
        return 0u;
    }

    *whiteRatioQ10000 = AlgoRatioK_ClampRatioQ10000FromFix16(
        AlgoRatioK_DivS64ToFix16(weightedRatioSum, weightSum * (int64_t)ALGO_RATIO_K_RATIO_NORMALIZER));
    return 1u;
}

static uint16_t AlgoRatioK_GridIndexFromIJ(uint16_t gridI,
                                           uint16_t gridJ)
{
    uint32_t rowStart;

    rowStart = (uint32_t)gridI * (((uint32_t)2u * ALGO_RATIO_K_GRID_ORDER) - (uint32_t)gridI + 3u);
    rowStart /= 2u;
    return (uint16_t)(rowStart + (uint32_t)gridJ);
}

static uint16_t AlgoRatioK_RoundDividePositiveU64ToU16(uint64_t numerator,
                                                   uint64_t denominator)
{
    uint64_t value;

    if (denominator == 0u)
    {
        return 0u;
    }

    value = (numerator + (denominator / 2u)) / denominator;
    if (value > 1023u)
    {
        value = 1023u;
    }

    return (uint16_t)value;
}

static int32_t AlgoRatioK_RoundDividePositiveS64ToS32(int64_t numerator,
                                                      int64_t denominator)
{
    int64_t value;

    if (denominator <= 0)
    {
        return 0;
    }

    value = (numerator + (denominator / 2)) / denominator;
    if (value < 0)
    {
        return 0;
    }
    if (value > 2147483647LL)
    {
        return 2147483647;
    }

    return (int32_t)value;
}

static void AlgoRatioK_GetPointQ1000ByGridIJ(uint16_t gridI,
                                             uint16_t gridJ,
                                             uint16_t *pointU,
                                             uint16_t *pointV)
{
    uint16_t gridK;
    int64_t xNum;
    int64_t yNum;
    int64_t denNum;

    if ((pointU == (void *)0) || (pointV == (void *)0))
    {
        return;
    }

    gridK = (uint16_t)(ALGO_RATIO_K_GRID_ORDER - gridI - gridJ);

    xNum = ((int64_t)gridK * (int64_t)ALGO_RATIO_K_VERTEX_K_X_QSCALE) +
           ((int64_t)gridI * (int64_t)ALGO_RATIO_K_VERTEX_I_X_QSCALE) +
           ((int64_t)gridJ * (int64_t)ALGO_RATIO_K_VERTEX_J_X_QSCALE);
    yNum = ((int64_t)gridK * (int64_t)ALGO_RATIO_K_VERTEX_K_Y_QSCALE) +
           ((int64_t)gridI * (int64_t)ALGO_RATIO_K_VERTEX_I_Y_QSCALE) +
           ((int64_t)gridJ * (int64_t)ALGO_RATIO_K_VERTEX_J_Y_QSCALE);

    denNum = ((int64_t)3 * (int64_t)ALGO_RATIO_K_GRID_ORDER * (int64_t)ALGO_RATIO_K_XY_SCALE) -
             ((int64_t)2 * xNum) + ((int64_t)12 * yNum);

    if (denNum <= 0)
    {
        *pointU = 0u;
        *pointV = 0u;
        return;
    }

    *pointU = AlgoRatioK_RoundDividePositiveU64ToU16((uint64_t)((int64_t)4000 * xNum),
                                                     (uint64_t)denNum);
    *pointV = AlgoRatioK_RoundDividePositiveU64ToU16((uint64_t)((int64_t)9000 * yNum),
                                                     (uint64_t)denNum);
}

static void AlgoRatioK_ResetNearestPoints(AlgoRatioK_NearestPointType *nearest)
{
    uint8_t i;

    for (i = 0u; i < ALGO_RATIO_K_AMOUNT; i++)
    {
        nearest[i].lutIndex = 0u;
        nearest[i].u = 0u;
        nearest[i].v = 0u;
        nearest[i].distance2 = ALGO_RATIO_K_DISTANCE_MAX;
    }
}

static uint8_t AlgoRatioK_TryInsertPointByGridIJ(uint16_t gridI,
                                                 uint16_t gridJ,
                                                 uint16_t targetU,
                                                 uint16_t targetV,
                                                 AlgoRatioK_NearestPointType *nearest,
                                                 uint8_t *nearestCount,
                                                 uint16_t *directRatioQ10000)
{
    AlgoRatioK_NearestPointType point;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    point.lutIndex = AlgoRatioK_GridIndexFromIJ(gridI, gridJ);
    AlgoRatioK_GetPointQ1000ByGridIJ(gridI, gridJ, &point.u, &point.v);
    point.distance2 = AlgoRatioK_CalcDistance2(targetU, targetV, point.u, point.v);

    if (point.distance2 == 0u)
    {
        if (directRatioQ10000 != (void *)0)
        {
            *directRatioQ10000 = AlgoRatioK_GetRatioQ10000ByIndex(point.lutIndex);
        }
        return 2u;
    }

    if (*nearestCount < ALGO_RATIO_K_AMOUNT)
    {
        AlgoRatioK_InsertNearestPoint(nearest, *nearestCount, point);
        (*nearestCount)++;
    }
    else if (point.distance2 < nearest[ALGO_RATIO_K_AMOUNT - 1u].distance2)
    {
        AlgoRatioK_InsertNearestPoint(nearest, *nearestCount, point);
    }

    return 1u;
}

static uint8_t AlgoRatioK_FindNearestPointsFast(uint16_t targetU,
                                                uint16_t targetV,
                                                AlgoRatioK_NearestPointType *nearest,
                                                uint8_t *nearestCount,
                                                uint16_t *directRatioQ10000)
{
    const int32_t gridOrder = (int32_t)ALGO_RATIO_K_GRID_ORDER;
    const int32_t vertexKX = (int32_t)ALGO_RATIO_K_VERTEX_K_X_QSCALE;
    const int32_t vertexKY = (int32_t)ALGO_RATIO_K_VERTEX_K_Y_QSCALE;
    const int32_t vertexIX = (int32_t)ALGO_RATIO_K_VERTEX_I_X_QSCALE;
    const int32_t vertexIY = (int32_t)ALGO_RATIO_K_VERTEX_I_Y_QSCALE;
    const int32_t vertexJX = (int32_t)ALGO_RATIO_K_VERTEX_J_X_QSCALE;
    const int32_t vertexJY = (int32_t)ALGO_RATIO_K_VERTEX_J_Y_QSCALE;
    int32_t axisIX;
    int32_t axisIY;
    int32_t axisJX;
    int32_t axisJY;
    int32_t relX;
    int32_t relY;
    int32_t targetXQ;
    int32_t targetYQ;
    int32_t uvDenominator;
    int64_t denominator;
    int64_t numeratorI;
    int64_t numeratorJ;
    fix16_t gridIFix16;
    fix16_t gridJFix16;
    fix16_t gridKFix16;
    int32_t centerI;
    int32_t centerJ;
    int32_t centerK;
    fix16_t errorI;
    fix16_t errorJ;
    fix16_t errorK;
    int32_t roundedSum;
    int32_t deltaI;
    int32_t deltaJ;
    int32_t deltaK;
    int32_t candidateI;
    int32_t candidateJ;
    int32_t candidateK;
    uint8_t status;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    uvDenominator = ((int32_t)6 * (int32_t)targetU) - ((int32_t)16 * (int32_t)targetV) + 12000;
    if (uvDenominator <= 0)
    {
        return 0u;
    }

    targetXQ = AlgoRatioK_RoundDividePositiveS64ToS32((int64_t)9 * (int64_t)targetU * (int64_t)ALGO_RATIO_K_XY_SCALE,
                                                      (int64_t)uvDenominator);
    targetYQ = AlgoRatioK_RoundDividePositiveS64ToS32((int64_t)4 * (int64_t)targetV * (int64_t)ALGO_RATIO_K_XY_SCALE,
                                                      (int64_t)uvDenominator);

    axisIX = vertexIX - vertexKX;
    axisIY = vertexIY - vertexKY;
    axisJX = vertexJX - vertexKX;
    axisJY = vertexJY - vertexKY;
    relX = targetXQ - vertexKX;
    relY = targetYQ - vertexKY;

    denominator = AlgoRatioK_Cross2DS64(axisIX, axisIY, axisJX, axisJY);
    if (denominator == 0)
    {
        return 0u;
    }

    numeratorI = AlgoRatioK_Cross2DS64(relX, relY, axisJX, axisJY);
    numeratorJ = AlgoRatioK_Cross2DS64(axisIX, axisIY, relX, relY);

    gridIFix16 = AlgoRatioK_DivS64ToFix16(numeratorI * (int64_t)gridOrder, denominator);
    gridJFix16 = AlgoRatioK_DivS64ToFix16(numeratorJ * (int64_t)gridOrder, denominator);
    gridKFix16 = fix16_sub(fix16_from_int(gridOrder), fix16_add(gridIFix16, gridJFix16));

    centerI = fix16_to_int(gridIFix16);
    centerJ = fix16_to_int(gridJFix16);
    centerK = fix16_to_int(gridKFix16);

    errorI = AlgoRatioK_AbsFix16(fix16_sub(gridIFix16, fix16_from_int(centerI)));
    errorJ = AlgoRatioK_AbsFix16(fix16_sub(gridJFix16, fix16_from_int(centerJ)));
    errorK = AlgoRatioK_AbsFix16(fix16_sub(gridKFix16, fix16_from_int(centerK)));

    roundedSum = centerI + centerJ + centerK;
    if (roundedSum != gridOrder)
    {
        if ((errorI >= errorJ) && (errorI >= errorK))
        {
            centerI = gridOrder - centerJ - centerK;
        }
        else if (errorJ >= errorK)
        {
            centerJ = gridOrder - centerI - centerK;
        }
        else
        {
            centerK = gridOrder - centerI - centerJ;
        }
    }

    if ((centerI < 0) || (centerJ < 0) || (centerK < 0) ||
        (centerI > gridOrder) || (centerJ > gridOrder) || (centerK > gridOrder))
    {
        return 0u;
    }

    *nearestCount = 0u;

    for (deltaI = -(int32_t)ALGO_RATIO_K_NEIGHBOR_RING;
         deltaI <= (int32_t)ALGO_RATIO_K_NEIGHBOR_RING;
         deltaI++)
    {
        for (deltaJ = -(int32_t)ALGO_RATIO_K_NEIGHBOR_RING;
             deltaJ <= (int32_t)ALGO_RATIO_K_NEIGHBOR_RING;
             deltaJ++)
        {
            int32_t maxAbsDelta;

            deltaK = -deltaI - deltaJ;
            maxAbsDelta = AlgoRatioK_AbsS32(deltaI);
            if (maxAbsDelta < AlgoRatioK_AbsS32(deltaJ))
            {
                maxAbsDelta = AlgoRatioK_AbsS32(deltaJ);
            }
            if (maxAbsDelta < AlgoRatioK_AbsS32(deltaK))
            {
                maxAbsDelta = AlgoRatioK_AbsS32(deltaK);
            }
            if (maxAbsDelta > (int32_t)ALGO_RATIO_K_NEIGHBOR_RING)
            {
                continue;
            }

            candidateI = centerI + deltaI;
            candidateJ = centerJ + deltaJ;
            candidateK = gridOrder - candidateI - candidateJ;

            if ((candidateI < 0) || (candidateJ < 0) || (candidateK < 0) ||
                (candidateI > gridOrder) || (candidateJ > gridOrder) || (candidateK > gridOrder))
            {
                continue;
            }

            status = AlgoRatioK_TryInsertPointByGridIJ((uint16_t)candidateI,
                                                       (uint16_t)candidateJ,
                                                       targetU,
                                                       targetV,
                                                       nearest,
                                                       nearestCount,
                                                       directRatioQ10000);
            if (status == 2u)
            {
                return 2u;
            }
        }
    }

    if (*nearestCount < ALGO_RATIO_K_AMOUNT)
    {
        return 0u;
    }

    return 1u;
}

static uint8_t AlgoRatioK_FindNearestPointsFullScan(uint16_t targetU,
                                                    uint16_t targetV,
                                                    AlgoRatioK_NearestPointType *nearest,
                                                    uint8_t *nearestCount,
                                                    uint16_t *directRatioQ10000)
{
    uint16_t gridI;
    uint16_t gridJ;
    uint16_t gridJMax;
    uint8_t status;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    *nearestCount = 0u;

    for (gridI = 0u; gridI <= ALGO_RATIO_K_GRID_ORDER; gridI++)
    {
        gridJMax = (uint16_t)(ALGO_RATIO_K_GRID_ORDER - gridI);
        for (gridJ = 0u; gridJ <= gridJMax; gridJ++)
        {
            status = AlgoRatioK_TryInsertPointByGridIJ(gridI,
                                                       gridJ,
                                                       targetU,
                                                       targetV,
                                                       nearest,
                                                       nearestCount,
                                                       directRatioQ10000);
            if (status == 2u)
            {
                return 2u;
            }
        }
    }

    if (*nearestCount < ALGO_RATIO_K_AMOUNT)
    {
        return 0u;
    }

    return 1u;
}

uint8_t AlgoRatioK_GetWhiteRatioQ10000(uint16_t targetU,
                                       uint16_t targetV,
                                       uint16_t *whiteRatioQ10000)
{
    AlgoRatioK_NearestPointType nearest[ALGO_RATIO_K_AMOUNT];
    uint8_t nearestCount;
    uint8_t status;

    if (whiteRatioQ10000 == (void *)0)
    {
        return 0u;
    }

    AlgoRatioK_ResetNearestPoints(nearest);

    status = AlgoRatioK_FindNearestPointsFast(targetU,
                                              targetV,
                                              nearest,
                                              &nearestCount,
                                              whiteRatioQ10000);
    if (status == 2u)
    {
        return 1u;
    }
    if (status == 0u)
    {
        return 0u;
    }

    if (AlgoRatioK_LocalPlaneFitQ10000(nearest,
                                       nearestCount,
                                       targetU,
                                       targetV,
                                       whiteRatioQ10000) != 0u)
    {
        return 1u;
    }

    return AlgoRatioK_WeightedAverageQ10000(nearest,
                                            nearestCount,
                                            whiteRatioQ10000);
}
