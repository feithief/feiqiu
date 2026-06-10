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


#define ALGO_RATIO_K_XY_SCALE            (2000000u)
#define ALGO_RATIO_K_VERTEX_K_X_QSCALE   (304324u)
#define ALGO_RATIO_K_VERTEX_K_Y_QSCALE   (56418u)
#define ALGO_RATIO_K_VERTEX_I_X_QSCALE   (1405626u)
#define ALGO_RATIO_K_VERTEX_I_Y_QSCALE   (592863u)
#define ALGO_RATIO_K_VERTEX_J_X_QSCALE   (272809u)
#define ALGO_RATIO_K_VERTEX_J_Y_QSCALE   (1416363u)

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
        0u,  /* idx=  3, ratio=-0.000000 */
        0u,  /* idx=  4, ratio=-0.000000 */
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
        0u,  /* idx= 17, ratio=-0.000000 */
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
        0u,  /* idx= 33, ratio=-0.000000 */
        0u,  /* idx= 34, ratio=-0.000000 */
        0u,  /* idx= 35, ratio=0.000000 */
        0u,  /* idx= 36, ratio=0.000000 */
        0u,  /* idx= 37, ratio=0.000000 */
        0u,  /* idx= 38, ratio=0.000000 */
        0u,  /* idx= 39, ratio=0.000000 */
        0u,  /* idx= 40, ratio=0.000000 */
        0u,  /* idx= 41, ratio=0.000000 */
        0u,  /* idx= 42, ratio=0.000000 */
        0u,  /* idx= 43, ratio=0.000000 */
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
     3849u,  /* idx= 88, ratio=0.384941 */
     2410u,  /* idx= 89, ratio=0.240970 */
     1258u,  /* idx= 90, ratio=0.125792 */
      349u,  /* idx= 91, ratio=0.034925 */
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
        0u,  /* idx=128, ratio=0.000000 */
        0u,  /* idx=129, ratio=0.000000 */
     3825u,  /* idx=130, ratio=0.382536 */
     6110u,  /* idx=131, ratio=0.611048 */
     4911u,  /* idx=132, ratio=0.491134 */
     3830u,  /* idx=133, ratio=0.383039 */
     2856u,  /* idx=134, ratio=0.285649 */
     2092u,  /* idx=135, ratio=0.209239 */
     1550u,  /* idx=136, ratio=0.154994 */
     1166u,  /* idx=137, ratio=0.116610 */
      888u,  /* idx=138, ratio=0.088840 */
      679u,  /* idx=139, ratio=0.067860 */
      419u,  /* idx=140, ratio=0.041919 */
      204u,  /* idx=141, ratio=0.020381 */
       25u,  /* idx=142, ratio=0.002507 */
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
        0u,  /* idx=169, ratio=0.000000 */
        0u,  /* idx=170, ratio=0.000000 */
     3479u,  /* idx=171, ratio=0.347940 */
     5661u,  /* idx=172, ratio=0.566094 */
     7157u,  /* idx=173, ratio=0.715664 */
     6550u,  /* idx=174, ratio=0.655009 */
     5557u,  /* idx=175, ratio=0.555702 */
     4629u,  /* idx=176, ratio=0.462916 */
     3815u,  /* idx=177, ratio=0.381514 */
     3197u,  /* idx=178, ratio=0.319677 */
     2737u,  /* idx=179, ratio=0.273725 */
     2392u,  /* idx=180, ratio=0.239194 */
     2031u,  /* idx=181, ratio=0.203098 */
     1737u,  /* idx=182, ratio=0.173683 */
     1483u,  /* idx=183, ratio=0.148274 */
     1243u,  /* idx=184, ratio=0.124265 */
     1025u,  /* idx=185, ratio=0.102466 */
      792u,  /* idx=186, ratio=0.079162 */
      468u,  /* idx=187, ratio=0.046756 */
      169u,  /* idx=188, ratio=0.016939 */
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
        0u,  /* idx=209, ratio=0.000000 */
        0u,  /* idx=210, ratio=0.000000 */
     3191u,  /* idx=211, ratio=0.319083 */
     5273u,  /* idx=212, ratio=0.527300 */
     6739u,  /* idx=213, ratio=0.673879 */
     7827u,  /* idx=214, ratio=0.782662 */
     7681u,  /* idx=215, ratio=0.768111 */
     6755u,  /* idx=216, ratio=0.675534 */
     5940u,  /* idx=217, ratio=0.594013 */
     5131u,  /* idx=218, ratio=0.513068 */
     4490u,  /* idx=219, ratio=0.448987 */
     4005u,  /* idx=220, ratio=0.400495 */
     3559u,  /* idx=221, ratio=0.355910 */
     3200u,  /* idx=222, ratio=0.319968 */
     2867u,  /* idx=223, ratio=0.286734 */
     2552u,  /* idx=224, ratio=0.255228 */
     2210u,  /* idx=225, ratio=0.220964 */
     1803u,  /* idx=226, ratio=0.180329 */
     1434u,  /* idx=227, ratio=0.143396 */
     1098u,  /* idx=228, ratio=0.109786 */
      788u,  /* idx=229, ratio=0.078774 */
      505u,  /* idx=230, ratio=0.050549 */
      245u,  /* idx=231, ratio=0.024491 */
        0u,  /* idx=232, ratio=0.000000 */
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
     2946u,  /* idx=250, ratio=0.294646 */
     4935u,  /* idx=251, ratio=0.493483 */
     6367u,  /* idx=252, ratio=0.636705 */
     7448u,  /* idx=253, ratio=0.744784 */
     8292u,  /* idx=254, ratio=0.829241 */
     8502u,  /* idx=255, ratio=0.850189 */
     7620u,  /* idx=256, ratio=0.761994 */
     6910u,  /* idx=257, ratio=0.691035 */
     6154u,  /* idx=258, ratio=0.615411 */
     5532u,  /* idx=259, ratio=0.553157 */
     5002u,  /* idx=260, ratio=0.500206 */
     4579u,  /* idx=261, ratio=0.457945 */
     4162u,  /* idx=262, ratio=0.416231 */
     3700u,  /* idx=263, ratio=0.370033 */
     3211u,  /* idx=264, ratio=0.321117 */
     2762u,  /* idx=265, ratio=0.276190 */
     2352u,  /* idx=266, ratio=0.235165 */
     1977u,  /* idx=267, ratio=0.197658 */
     1634u,  /* idx=268, ratio=0.163373 */
     1316u,  /* idx=269, ratio=0.131617 */
     1027u,  /* idx=270, ratio=0.102670 */
      749u,  /* idx=271, ratio=0.074941 */
      488u,  /* idx=272, ratio=0.048751 */
      240u,  /* idx=273, ratio=0.023978 */
        0u,  /* idx=274, ratio=0.000000 */
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
        0u,  /* idx=286, ratio=0.000000 */
        0u,  /* idx=287, ratio=0.000000 */
     2737u,  /* idx=288, ratio=0.273686 */
     4637u,  /* idx=289, ratio=0.463741 */
     6034u,  /* idx=290, ratio=0.603418 */
     7104u,  /* idx=291, ratio=0.710403 */
     7950u,  /* idx=292, ratio=0.794971 */
     8635u,  /* idx=293, ratio=0.863500 */
     9148u,  /* idx=294, ratio=0.914778 */
     8311u,  /* idx=295, ratio=0.831105 */
     7617u,  /* idx=296, ratio=0.761695 */
     6964u,  /* idx=297, ratio=0.696375 */
     6343u,  /* idx=298, ratio=0.634265 */
     5840u,  /* idx=299, ratio=0.584022 */
     5256u,  /* idx=300, ratio=0.525578 */
     4676u,  /* idx=301, ratio=0.467641 */
     4148u,  /* idx=302, ratio=0.414804 */
     3660u,  /* idx=303, ratio=0.365987 */
     3215u,  /* idx=304, ratio=0.321539 */
     2807u,  /* idx=305, ratio=0.280659 */
     2435u,  /* idx=306, ratio=0.243532 */
     2090u,  /* idx=307, ratio=0.209011 */
     1764u,  /* idx=308, ratio=0.176392 */
     1460u,  /* idx=309, ratio=0.146004 */
     1172u,  /* idx=310, ratio=0.117203 */
      893u,  /* idx=311, ratio=0.089306 */
      627u,  /* idx=312, ratio=0.062741 */
      368u,  /* idx=313, ratio=0.036836 */
      121u,  /* idx=314, ratio=0.012099 */
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
     2555u,  /* idx=325, ratio=0.255510 */
     4374u,  /* idx=326, ratio=0.437381 */
     5734u,  /* idx=327, ratio=0.573438 */
     6791u,  /* idx=328, ratio=0.679056 */
     7634u,  /* idx=329, ratio=0.763422 */
     8324u,  /* idx=330, ratio=0.832364 */
     8898u,  /* idx=331, ratio=0.889757 */
     9372u,  /* idx=332, ratio=0.937192 */
     8700u,  /* idx=333, ratio=0.869959 */
     8117u,  /* idx=334, ratio=0.811727 */
     7608u,  /* idx=335, ratio=0.760802 */
     6854u,  /* idx=336, ratio=0.685361 */
     6196u,  /* idx=337, ratio=0.619632 */
     5583u,  /* idx=338, ratio=0.558259 */
     5018u,  /* idx=339, ratio=0.501821 */
     4501u,  /* idx=340, ratio=0.450078 */
     4026u,  /* idx=341, ratio=0.402580 */
     3590u,  /* idx=342, ratio=0.359001 */
     3191u,  /* idx=343, ratio=0.319119 */
     2818u,  /* idx=344, ratio=0.281792 */
     2468u,  /* idx=345, ratio=0.246827 */
     2135u,  /* idx=346, ratio=0.213539 */
     1818u,  /* idx=347, ratio=0.181763 */
     1514u,  /* idx=348, ratio=0.151372 */
     1223u,  /* idx=349, ratio=0.122269 */
      937u,  /* idx=350, ratio=0.093729 */
      670u,  /* idx=351, ratio=0.066991 */
      414u,  /* idx=352, ratio=0.041408 */
      168u,  /* idx=353, ratio=0.016823 */
        0u,  /* idx=354, ratio=0.000000 */
        0u,  /* idx=355, ratio=0.000000 */
        0u,  /* idx=356, ratio=0.000000 */
        0u,  /* idx=357, ratio=0.000000 */
        0u,  /* idx=358, ratio=0.000000 */
        0u,  /* idx=359, ratio=0.000000 */
        0u,  /* idx=360, ratio=0.000000 */
     2396u,  /* idx=361, ratio=0.239598 */
     4139u,  /* idx=362, ratio=0.413857 */
     5463u,  /* idx=363, ratio=0.546297 */
     6504u,  /* idx=364, ratio=0.650359 */
     7343u,  /* idx=365, ratio=0.734281 */
     8034u,  /* idx=366, ratio=0.803395 */
     8613u,  /* idx=367, ratio=0.861301 */
     9105u,  /* idx=368, ratio=0.910522 */
     9518u,  /* idx=369, ratio=0.951771 */
     8897u,  /* idx=370, ratio=0.889702 */
     8352u,  /* idx=371, ratio=0.835233 */
     7776u,  /* idx=372, ratio=0.777553 */
     7050u,  /* idx=373, ratio=0.704975 */
     6413u,  /* idx=374, ratio=0.641343 */
     5827u,  /* idx=375, ratio=0.582710 */
     5285u,  /* idx=376, ratio=0.528464 */
     4786u,  /* idx=377, ratio=0.478574 */
     4328u,  /* idx=378, ratio=0.432795 */
     3900u,  /* idx=379, ratio=0.390044 */
     3501u,  /* idx=380, ratio=0.350129 */
     3124u,  /* idx=381, ratio=0.312400 */
     2767u,  /* idx=382, ratio=0.276718 */
     2424u,  /* idx=383, ratio=0.242400 */
     2099u,  /* idx=384, ratio=0.209938 */
     1780u,  /* idx=385, ratio=0.177995 */
     1483u,  /* idx=386, ratio=0.148337 */
     1198u,  /* idx=387, ratio=0.119769 */
      925u,  /* idx=388, ratio=0.092488 */
      672u,  /* idx=389, ratio=0.067202 */
      438u,  /* idx=390, ratio=0.043844 */
      228u,  /* idx=391, ratio=0.022830 */
       44u,  /* idx=392, ratio=0.004434 */
        0u,  /* idx=393, ratio=0.000000 */
        0u,  /* idx=394, ratio=0.000000 */
        0u,  /* idx=395, ratio=0.000000 */
     2256u,  /* idx=396, ratio=0.225551 */
     3927u,  /* idx=397, ratio=0.392733 */
     5216u,  /* idx=398, ratio=0.521608 */
     6240u,  /* idx=399, ratio=0.623989 */
     7073u,  /* idx=400, ratio=0.707284 */
     7764u,  /* idx=401, ratio=0.776375 */
     8346u,  /* idx=402, ratio=0.834609 */
     8844u,  /* idx=403, ratio=0.884360 */
     9274u,  /* idx=404, ratio=0.927356 */
     9638u,  /* idx=405, ratio=0.963765 */
     9062u,  /* idx=406, ratio=0.906153 */
     8550u,  /* idx=407, ratio=0.855040 */
     7810u,  /* idx=408, ratio=0.781012 */
     7171u,  /* idx=409, ratio=0.717076 */
     6565u,  /* idx=410, ratio=0.656451 */
     6008u,  /* idx=411, ratio=0.600770 */
     5490u,  /* idx=412, ratio=0.549006 */
     5009u,  /* idx=413, ratio=0.500915 */
     4564u,  /* idx=414, ratio=0.456383 */
     4139u,  /* idx=415, ratio=0.413905 */
     3743u,  /* idx=416, ratio=0.374339 */
     3360u,  /* idx=417, ratio=0.336022 */
     2994u,  /* idx=418, ratio=0.299364 */
     2646u,  /* idx=419, ratio=0.264619 */
     2314u,  /* idx=420, ratio=0.231399 */
     2003u,  /* idx=421, ratio=0.200270 */
     1705u,  /* idx=422, ratio=0.170494 */
     1425u,  /* idx=423, ratio=0.142495 */
     1167u,  /* idx=424, ratio=0.116665 */
      929u,  /* idx=425, ratio=0.092941 */
      722u,  /* idx=426, ratio=0.072188 */
      421u,  /* idx=427, ratio=0.042057 */
        0u,  /* idx=428, ratio=0.000000 */
        0u,  /* idx=429, ratio=0.000000 */
     2131u,  /* idx=430, ratio=0.213060 */
     3737u,  /* idx=431, ratio=0.373662 */
     4991u,  /* idx=432, ratio=0.499055 */
     5997u,  /* idx=433, ratio=0.599674 */
     6822u,  /* idx=434, ratio=0.682201 */
     7511u,  /* idx=435, ratio=0.751113 */
     8095u,  /* idx=436, ratio=0.809522 */
     8597u,  /* idx=437, ratio=0.859660 */
     9032u,  /* idx=438, ratio=0.903167 */
     9413u,  /* idx=439, ratio=0.941277 */
     9738u,  /* idx=440, ratio=0.973806 */
     9201u,  /* idx=441, ratio=0.920072 */
     8550u,  /* idx=442, ratio=0.855024 */
     7843u,  /* idx=443, ratio=0.784270 */
     7237u,  /* idx=444, ratio=0.723680 */
     6668u,  /* idx=445, ratio=0.666778 */
     6139u,  /* idx=446, ratio=0.613861 */
     5645u,  /* idx=447, ratio=0.564455 */
     5176u,  /* idx=448, ratio=0.517640 */
     4738u,  /* idx=449, ratio=0.473800 */
     4314u,  /* idx=450, ratio=0.431422 */
     3915u,  /* idx=451, ratio=0.391511 */
     3532u,  /* idx=452, ratio=0.353200 */
     3169u,  /* idx=453, ratio=0.316934 */
     2826u,  /* idx=454, ratio=0.282630 */
     2500u,  /* idx=455, ratio=0.250006 */
     2194u,  /* idx=456, ratio=0.219448 */
     1860u,  /* idx=457, ratio=0.186004 */
     1355u,  /* idx=458, ratio=0.135531 */
      879u,  /* idx=459, ratio=0.087853 */
      427u,  /* idx=460, ratio=0.042743 */
        0u,  /* idx=461, ratio=0.000000 */
        0u,  /* idx=462, ratio=0.000000 */
     2019u,  /* idx=463, ratio=0.201880 */
     3564u,  /* idx=464, ratio=0.356357 */
     4784u,  /* idx=465, ratio=0.478371 */
     5772u,  /* idx=466, ratio=0.577183 */
     6588u,  /* idx=467, ratio=0.658836 */
     7274u,  /* idx=468, ratio=0.727443 */
     7859u,  /* idx=469, ratio=0.785899 */
     8363u,  /* idx=470, ratio=0.836301 */
     8802u,  /* idx=471, ratio=0.880208 */
     9188u,  /* idx=472, ratio=0.918798 */
     9530u,  /* idx=473, ratio=0.952982 */
     9823u,  /* idx=474, ratio=0.982334 */
     9187u,  /* idx=475, ratio=0.918735 */
     8426u,  /* idx=476, ratio=0.842633 */
     7833u,  /* idx=477, ratio=0.783295 */
     7263u,  /* idx=478, ratio=0.726299 */
     6728u,  /* idx=479, ratio=0.672844 */
     6222u,  /* idx=480, ratio=0.622158 */
     5742u,  /* idx=481, ratio=0.574200 */
     5286u,  /* idx=482, ratio=0.528574 */
     4851u,  /* idx=483, ratio=0.485057 */
     4435u,  /* idx=484, ratio=0.443495 */
     4042u,  /* idx=485, ratio=0.404177 */
     3640u,  /* idx=486, ratio=0.363965 */
     3021u,  /* idx=487, ratio=0.302072 */
     2440u,  /* idx=488, ratio=0.243985 */
     1894u,  /* idx=489, ratio=0.189365 */
     1379u,  /* idx=490, ratio=0.137909 */
      894u,  /* idx=491, ratio=0.089351 */
      435u,  /* idx=492, ratio=0.043452 */
        0u,  /* idx=493, ratio=0.000000 */
        0u,  /* idx=494, ratio=0.000000 */
     1918u,  /* idx=495, ratio=0.191815 */
     3406u,  /* idx=496, ratio=0.340583 */
     4593u,  /* idx=497, ratio=0.459333 */
     5563u,  /* idx=498, ratio=0.556318 */
     6370u,  /* idx=499, ratio=0.637019 */
     7052u,  /* idx=500, ratio=0.705219 */
     7636u,  /* idx=501, ratio=0.763615 */
     8142u,  /* idx=502, ratio=0.814179 */
     8584u,  /* idx=503, ratio=0.858387 */
     8974u,  /* idx=504, ratio=0.897367 */
     9320u,  /* idx=505, ratio=0.931995 */
     9630u,  /* idx=506, ratio=0.962961 */
     9691u,  /* idx=507, ratio=0.969052 */
     8952u,  /* idx=508, ratio=0.895215 */
     8352u,  /* idx=509, ratio=0.835221 */
     7794u,  /* idx=510, ratio=0.779404 */
     7258u,  /* idx=511, ratio=0.725785 */
     6748u,  /* idx=512, ratio=0.674786 */
     6262u,  /* idx=513, ratio=0.626188 */
     5794u,  /* idx=514, ratio=0.579408 */
     5116u,  /* idx=515, ratio=0.511639 */
     4391u,  /* idx=516, ratio=0.439060 */
     3713u,  /* idx=517, ratio=0.371333 */
     3080u,  /* idx=518, ratio=0.307989 */
     2486u,  /* idx=519, ratio=0.248615 */
     1928u,  /* idx=520, ratio=0.192849 */
     1404u,  /* idx=521, ratio=0.140372 */
      909u,  /* idx=522, ratio=0.090901 */
      442u,  /* idx=523, ratio=0.044185 */
        0u,  /* idx=524, ratio=0.000000 */
        0u,  /* idx=525, ratio=0.000000 */
     1827u,  /* idx=526, ratio=0.182706 */
     3261u,  /* idx=527, ratio=0.326147 */
     4418u,  /* idx=528, ratio=0.441753 */
     5369u,  /* idx=529, ratio=0.536909 */
     6166u,  /* idx=530, ratio=0.616600 */
     6843u,  /* idx=531, ratio=0.684313 */
     7426u,  /* idx=532, ratio=0.742560 */
     7932u,  /* idx=533, ratio=0.793197 */
     8376u,  /* idx=534, ratio=0.837622 */
     8769u,  /* idx=535, ratio=0.876914 */
     9119u,  /* idx=536, ratio=0.911913 */
     9433u,  /* idx=537, ratio=0.943286 */
     9716u,  /* idx=538, ratio=0.971569 */
     9399u,  /* idx=539, ratio=0.939912 */
     8788u,  /* idx=540, ratio=0.878764 */
     8259u,  /* idx=541, ratio=0.825878 */
     7730u,  /* idx=542, ratio=0.773030 */
     6897u,  /* idx=543, ratio=0.689721 */
     6032u,  /* idx=544, ratio=0.603183 */
     5230u,  /* idx=545, ratio=0.522985 */
     4485u,  /* idx=546, ratio=0.448452 */
     3790u,  /* idx=547, ratio=0.379006 */
     3141u,  /* idx=548, ratio=0.314143 */
     2534u,  /* idx=549, ratio=0.253424 */
     1965u,  /* idx=550, ratio=0.196464 */
     1429u,  /* idx=551, ratio=0.142924 */
      925u,  /* idx=552, ratio=0.092505 */
      449u,  /* idx=553, ratio=0.044942 */
        0u,  /* idx=554, ratio=0.000000 */
        0u,  /* idx=555, ratio=0.000000 */
     1744u,  /* idx=556, ratio=0.174423 */
     3129u,  /* idx=557, ratio=0.312885 */
     4255u,  /* idx=558, ratio=0.425469 */
     5188u,  /* idx=559, ratio=0.518808 */
     5974u,  /* idx=560, ratio=0.597450 */
     6646u,  /* idx=561, ratio=0.664611 */
     7226u,  /* idx=562, ratio=0.722635 */
     7733u,  /* idx=563, ratio=0.773269 */
     8178u,  /* idx=564, ratio=0.817838 */
     8574u,  /* idx=565, ratio=0.857372 */
     8927u,  /* idx=566, ratio=0.892677 */
     9244u,  /* idx=567, ratio=0.924399 */
     9531u,  /* idx=568, ratio=0.953056 */
     9734u,  /* idx=569, ratio=0.973392 */
     9087u,  /* idx=570, ratio=0.908682 */
     8034u,  /* idx=571, ratio=0.803393 */
     7066u,  /* idx=572, ratio=0.706629 */
     6174u,  /* idx=573, ratio=0.617395 */
     5348u,  /* idx=574, ratio=0.534845 */
     4583u,  /* idx=575, ratio=0.458255 */
     3870u,  /* idx=576, ratio=0.387003 */
     3205u,  /* idx=577, ratio=0.320548 */
     2584u,  /* idx=578, ratio=0.258422 */
     2002u,  /* idx=579, ratio=0.200216 */
     1456u,  /* idx=580, ratio=0.145570 */
      942u,  /* idx=581, ratio=0.094167 */
      457u,  /* idx=582, ratio=0.045727 */
        0u,  /* idx=583, ratio=0.000000 */
        0u,  /* idx=584, ratio=0.000000 */
     1669u,  /* idx=585, ratio=0.166858 */
     3007u,  /* idx=586, ratio=0.300660 */
     4103u,  /* idx=587, ratio=0.410342 */
     5019u,  /* idx=588, ratio=0.501888 */
     5795u,  /* idx=589, ratio=0.579453 */
     6460u,  /* idx=590, ratio=0.646012 */
     7038u,  /* idx=591, ratio=0.703752 */
     7543u,  /* idx=592, ratio=0.754317 */
     7990u,  /* idx=593, ratio=0.798967 */
     8387u,  /* idx=594, ratio=0.838682 */
     8742u,  /* idx=595, ratio=0.874237 */
     9063u,  /* idx=596, ratio=0.906253 */
     9352u,  /* idx=597, ratio=0.935234 */
     9336u,  /* idx=598, ratio=0.933585 */
     8245u,  /* idx=599, ratio=0.824457 */
     7244u,  /* idx=600, ratio=0.724387 */
     6323u,  /* idx=601, ratio=0.632293 */
     5473u,  /* idx=602, ratio=0.547256 */
     4685u,  /* idx=603, ratio=0.468497 */
     3953u,  /* idx=604, ratio=0.395344 */
     3272u,  /* idx=605, ratio=0.327219 */
     2636u,  /* idx=606, ratio=0.263622 */
     2041u,  /* idx=607, ratio=0.204115 */
     1483u,  /* idx=608, ratio=0.148317 */
      959u,  /* idx=609, ratio=0.095890 */
      465u,  /* idx=610, ratio=0.046539 */
        0u,  /* idx=611, ratio=0.000000 */
        0u,  /* idx=612, ratio=0.000000 */
     1599u,  /* idx=613, ratio=0.159922 */
     2894u,  /* idx=614, ratio=0.289353 */
     3963u,  /* idx=615, ratio=0.396254 */
     4860u,  /* idx=616, ratio=0.486037 */
     5625u,  /* idx=617, ratio=0.562509 */
     6284u,  /* idx=618, ratio=0.628425 */
     6858u,  /* idx=619, ratio=0.685830 */
     7363u,  /* idx=620, ratio=0.736273 */
     7809u,  /* idx=621, ratio=0.780947 */
     8208u,  /* idx=622, ratio=0.820789 */
     8565u,  /* idx=623, ratio=0.856543 */
     8888u,  /* idx=624, ratio=0.888807 */
     9181u,  /* idx=625, ratio=0.918067 */
     8467u,  /* idx=626, ratio=0.846656 */
     7431u,  /* idx=627, ratio=0.743061 */
     6479u,  /* idx=628, ratio=0.647927 */
     5603u,  /* idx=629, ratio=0.560257 */
     4792u,  /* idx=630, ratio=0.479206 */
     4041u,  /* idx=631, ratio=0.404052 */
     3342u,  /* idx=632, ratio=0.334174 */
     2690u,  /* idx=633, ratio=0.269035 */
     2082u,  /* idx=634, ratio=0.208169 */
     1512u,  /* idx=635, ratio=0.151169 */
      977u,  /* idx=636, ratio=0.097677 */
      474u,  /* idx=637, ratio=0.047380 */
        0u,  /* idx=638, ratio=0.000000 */
        0u,  /* idx=639, ratio=0.000000 */
     1535u,  /* idx=640, ratio=0.153540 */
     2789u,  /* idx=641, ratio=0.278867 */
     3831u,  /* idx=642, ratio=0.383102 */
     4712u,  /* idx=643, ratio=0.471157 */
     5465u,  /* idx=644, ratio=0.546527 */
     6118u,  /* idx=645, ratio=0.611770 */
     6688u,  /* idx=646, ratio=0.668799 */
     7191u,  /* idx=647, ratio=0.719072 */
     7637u,  /* idx=648, ratio=0.763722 */
     8036u,  /* idx=649, ratio=0.803644 */
     8396u,  /* idx=650, ratio=0.839551 */
     8720u,  /* idx=651, ratio=0.872019 */
     8701u,  /* idx=652, ratio=0.870083 */
     7627u,  /* idx=653, ratio=0.762724 */
     6644u,  /* idx=654, ratio=0.664354 */
     5739u,  /* idx=655, ratio=0.573890 */
     4904u,  /* idx=656, ratio=0.490417 */
     4132u,  /* idx=657, ratio=0.413153 */
     3414u,  /* idx=658, ratio=0.341431 */
     2747u,  /* idx=659, ratio=0.274675 */
     2124u,  /* idx=660, ratio=0.212387 */
     1541u,  /* idx=661, ratio=0.154133 */
      995u,  /* idx=662, ratio=0.099532 */
      483u,  /* idx=663, ratio=0.048253 */
        0u,  /* idx=664, ratio=0.000000 */
        0u,  /* idx=665, ratio=0.000000 */
     1476u,  /* idx=666, ratio=0.147648 */
     2691u,  /* idx=667, ratio=0.269114 */
     3708u,  /* idx=668, ratio=0.370794 */
     4572u,  /* idx=669, ratio=0.457160 */
     5314u,  /* idx=670, ratio=0.531429 */
     5960u,  /* idx=671, ratio=0.595976 */
     6526u,  /* idx=672, ratio=0.652592 */
     7027u,  /* idx=673, ratio=0.702656 */
     7472u,  /* idx=674, ratio=0.747241 */
     7872u,  /* idx=675, ratio=0.787201 */
     8232u,  /* idx=676, ratio=0.823220 */
     8559u,  /* idx=677, ratio=0.855853 */
     7835u,  /* idx=678, ratio=0.783455 */
     6816u,  /* idx=679, ratio=0.681635 */
     5882u,  /* idx=680, ratio=0.588204 */
     5022u,  /* idx=681, ratio=0.502165 */
     4227u,  /* idx=682, ratio=0.422673 */
     3490u,  /* idx=683, ratio=0.349010 */
     2806u,  /* idx=684, ratio=0.280557 */
     2168u,  /* idx=685, ratio=0.216780 */
     1572u,  /* idx=686, ratio=0.157215 */
     1015u,  /* idx=687, ratio=0.101459 */
      492u,  /* idx=688, ratio=0.049158 */
        0u,  /* idx=689, ratio=0.000000 */
        0u,  /* idx=690, ratio=0.000000 */
     1422u,  /* idx=691, ratio=0.142191 */
     2600u,  /* idx=692, ratio=0.260020 */
     3593u,  /* idx=693, ratio=0.359253 */
     4440u,  /* idx=694, ratio=0.443972 */
     5171u,  /* idx=695, ratio=0.517142 */
     5810u,  /* idx=696, ratio=0.580976 */
     6372u,  /* idx=697, ratio=0.637153 */
     6870u,  /* idx=698, ratio=0.686972 */
     7315u,  /* idx=699, ratio=0.731456 */
     7714u,  /* idx=700, ratio=0.771417 */
     8075u,  /* idx=701, ratio=0.807512 */
     8053u,  /* idx=702, ratio=0.805344 */
     6998u,  /* idx=703, ratio=0.699840 */
     6032u,  /* idx=704, ratio=0.603250 */
     5145u,  /* idx=705, ratio=0.514489 */
     4326u,  /* idx=706, ratio=0.432643 */
     3569u,  /* idx=707, ratio=0.356934 */
     2867u,  /* idx=708, ratio=0.286696 */
     2214u,  /* idx=709, ratio=0.221358 */
     1604u,  /* idx=710, ratio=0.160424 */
     1035u,  /* idx=711, ratio=0.103462 */
      501u,  /* idx=712, ratio=0.050098 */
        0u,  /* idx=713, ratio=0.000000 */
        0u,  /* idx=714, ratio=0.000000 */
     1371u,  /* idx=715, ratio=0.137123 */
     2515u,  /* idx=716, ratio=0.251520 */
     3484u,  /* idx=717, ratio=0.348409 */
     4315u,  /* idx=718, ratio=0.431522 */
     5036u,  /* idx=719, ratio=0.503604 */
     5667u,  /* idx=720, ratio=0.566713 */
     6224u,  /* idx=721, ratio=0.622427 */
     6720u,  /* idx=722, ratio=0.671974 */
     7163u,  /* idx=723, ratio=0.716324 */
     7563u,  /* idx=724, ratio=0.756253 */
     7924u,  /* idx=725, ratio=0.792393 */
     7190u,  /* idx=726, ratio=0.719044 */
     6191u,  /* idx=727, ratio=0.619085 */
     5274u,  /* idx=728, ratio=0.527433 */
     4431u,  /* idx=729, ratio=0.443094 */
     3652u,  /* idx=730, ratio=0.365225 */
     2931u,  /* idx=731, ratio=0.293110 */
     2261u,  /* idx=732, ratio=0.226134 */
     1638u,  /* idx=733, ratio=0.163765 */
     1055u,  /* idx=734, ratio=0.105546 */
      511u,  /* idx=735, ratio=0.051074 */
        0u,  /* idx=736, ratio=0.000000 */
        0u,  /* idx=737, ratio=0.000000 */
     1324u,  /* idx=738, ratio=0.132404 */
     2436u,  /* idx=739, ratio=0.243559 */
     3382u,  /* idx=740, ratio=0.338200 */
     4198u,  /* idx=741, ratio=0.419752 */
     4908u,  /* idx=742, ratio=0.490756 */
     5531u,  /* idx=743, ratio=0.553134 */
     6084u,  /* idx=744, ratio=0.608367 */
     6576u,  /* idx=745, ratio=0.657617 */
     7018u,  /* idx=746, ratio=0.701805 */
     7417u,  /* idx=747, ratio=0.741675 */
     7393u,  /* idx=748, ratio=0.739331 */
     6358u,  /* idx=749, ratio=0.635775 */
     5410u,  /* idx=750, ratio=0.541046 */
     4541u,  /* idx=751, ratio=0.454062 */
     3739u,  /* idx=752, ratio=0.373911 */
     2998u,  /* idx=753, ratio=0.299817 */
     2311u,  /* idx=754, ratio=0.231120 */
     1672u,  /* idx=755, ratio=0.167250 */
     1077u,  /* idx=756, ratio=0.107715 */
      521u,  /* idx=757, ratio=0.052090 */
        0u,  /* idx=758, ratio=0.000000 */
        0u,  /* idx=759, ratio=0.000000 */
     1280u,  /* idx=760, ratio=0.127999 */
     2361u,  /* idx=761, ratio=0.236086 */
     3286u,  /* idx=762, ratio=0.328572 */
     4086u,  /* idx=763, ratio=0.408607 */
     4785u,  /* idx=764, ratio=0.478548 */
     5402u,  /* idx=765, ratio=0.540190 */
     5949u,  /* idx=766, ratio=0.594928 */
     6439u,  /* idx=767, ratio=0.643860 */
     6879u,  /* idx=768, ratio=0.687863 */
     7276u,  /* idx=769, ratio=0.727647 */
     6534u,  /* idx=770, ratio=0.653389 */
     5554u,  /* idx=771, ratio=0.555380 */
     4656u,  /* idx=772, ratio=0.465588 */
     3830u,  /* idx=773, ratio=0.383020 */
     3068u,  /* idx=774, ratio=0.306839 */
     2363u,  /* idx=775, ratio=0.236331 */
     1709u,  /* idx=776, ratio=0.170885 */
     1100u,  /* idx=777, ratio=0.109976 */
      531u,  /* idx=778, ratio=0.053146 */
        0u,  /* idx=779, ratio=0.000000 */
        0u,  /* idx=780, ratio=0.000000 */
     1239u,  /* idx=781, ratio=0.123878 */
     2291u,  /* idx=782, ratio=0.229058 */
     3195u,  /* idx=783, ratio=0.319477 */
     3980u,  /* idx=784, ratio=0.398039 */
     4669u,  /* idx=785, ratio=0.466932 */
     5278u,  /* idx=786, ratio=0.527838 */
     5821u,  /* idx=787, ratio=0.582069 */
     6307u,  /* idx=788, ratio=0.630667 */
     6745u,  /* idx=789, ratio=0.674465 */
     6720u,  /* idx=790, ratio=0.672007 */
     5705u,  /* idx=791, ratio=0.570494 */
     4777u,  /* idx=792, ratio=0.477713 */
     3926u,  /* idx=793, ratio=0.392584 */
     3142u,  /* idx=794, ratio=0.314197 */
     2418u,  /* idx=795, ratio=0.241782 */
     1747u,  /* idx=796, ratio=0.174683 */
     1123u,  /* idx=797, ratio=0.112333 */
      542u,  /* idx=798, ratio=0.054246 */
        0u,  /* idx=799, ratio=0.000000 */
        0u,  /* idx=800, ratio=0.000000 */
     1200u,  /* idx=801, ratio=0.120013 */
     2224u,  /* idx=802, ratio=0.222436 */
     3109u,  /* idx=803, ratio=0.310872 */
     3880u,  /* idx=804, ratio=0.388003 */
     4559u,  /* idx=805, ratio=0.455867 */
     5160u,  /* idx=806, ratio=0.516038 */
     5698u,  /* idx=807, ratio=0.569755 */
     6180u,  /* idx=808, ratio=0.618004 */
     6616u,  /* idx=809, ratio=0.661578 */
     5865u,  /* idx=810, ratio=0.586454 */
     4905u,  /* idx=811, ratio=0.490487 */
     4026u,  /* idx=812, ratio=0.402638 */
     3219u,  /* idx=813, ratio=0.321917 */
     2475u,  /* idx=814, ratio=0.247491 */
     1787u,  /* idx=815, ratio=0.178652 */
     1148u,  /* idx=816, ratio=0.114793 */
      554u,  /* idx=817, ratio=0.055393 */
        0u,  /* idx=818, ratio=-0.000000 */
        0u,  /* idx=819, ratio=0.000000 */
     1164u,  /* idx=820, ratio=0.116383 */
     2162u,  /* idx=821, ratio=0.216187 */
     3027u,  /* idx=822, ratio=0.302719 */
     3785u,  /* idx=823, ratio=0.378461 */
     4453u,  /* idx=824, ratio=0.445314 */
     5048u,  /* idx=825, ratio=0.504754 */
     5580u,  /* idx=826, ratio=0.557951 */
     6058u,  /* idx=827, ratio=0.605839 */
     6033u,  /* idx=828, ratio=0.603333 */
     5040u,  /* idx=829, ratio=0.503963 */
     4132u,  /* idx=830, ratio=0.413220 */
     3300u,  /* idx=831, ratio=0.330026 */
     2535u,  /* idx=832, ratio=0.253477 */
     1828u,  /* idx=833, ratio=0.182807 */
     1174u,  /* idx=834, ratio=0.117364 */
      566u,  /* idx=835, ratio=0.056589 */
        0u,  /* idx=836, ratio=0.000000 */
        0u,  /* idx=837, ratio=0.000000 */
     1130u,  /* idx=838, ratio=0.112966 */
     2103u,  /* idx=839, ratio=0.210279 */
     2950u,  /* idx=840, ratio=0.294982 */
     3694u,  /* idx=841, ratio=0.369377 */
     4352u,  /* idx=842, ratio=0.435238 */
     4940u,  /* idx=843, ratio=0.493953 */
     5466u,  /* idx=844, ratio=0.546626 */
     5941u,  /* idx=845, ratio=0.594144 */
     5182u,  /* idx=846, ratio=0.518201 */
     4244u,  /* idx=847, ratio=0.424373 */
     3386u,  /* idx=848, ratio=0.338554 */
     2598u,  /* idx=849, ratio=0.259758 */
     1872u,  /* idx=850, ratio=0.187159 */
     1201u,  /* idx=851, ratio=0.120053 */
      578u,  /* idx=852, ratio=0.057838 */
        0u,  /* idx=853, ratio=0.000000 */
        0u,  /* idx=854, ratio=0.000000 */
     1097u,  /* idx=855, ratio=0.109743 */
     2047u,  /* idx=856, ratio=0.204685 */
     2876u,  /* idx=857, ratio=0.287631 */
     3607u,  /* idx=858, ratio=0.360719 */
     4256u,  /* idx=859, ratio=0.425608 */
     4836u,  /* idx=860, ratio=0.483605 */
     5358u,  /* idx=861, ratio=0.535752 */
     5333u,  /* idx=862, ratio=0.533266 */
     4361u,  /* idx=863, ratio=0.436146 */
     3475u,  /* idx=864, ratio=0.347534 */
     2664u,  /* idx=865, ratio=0.266359 */
     1917u,  /* idx=866, ratio=0.191724 */
     1229u,  /* idx=867, ratio=0.122867 */
      591u,  /* idx=868, ratio=0.059144 */
        0u,  /* idx=869, ratio=-0.000000 */
        0u,  /* idx=870, ratio=0.000000 */
     1067u,  /* idx=871, ratio=0.106700 */
     1994u,  /* idx=872, ratio=0.199381 */
     2806u,  /* idx=873, ratio=0.280637 */
     3525u,  /* idx=874, ratio=0.352458 */
     4164u,  /* idx=875, ratio=0.416396 */
     4737u,  /* idx=876, ratio=0.473682 */
     5253u,  /* idx=877, ratio=0.525302 */
     4486u,  /* idx=878, ratio=0.448590 */
     3570u,  /* idx=879, ratio=0.357003 */
     2733u,  /* idx=880, ratio=0.273305 */
     1965u,  /* idx=881, ratio=0.196516 */
     1258u,  /* idx=882, ratio=0.125817 */
      605u,  /* idx=883, ratio=0.060509 */
        0u,  /* idx=884, ratio=0.000000 */
        0u,  /* idx=885, ratio=0.000000 */
     1038u,  /* idx=886, ratio=0.103820 */
     1943u,  /* idx=887, ratio=0.194346 */
     2740u,  /* idx=888, ratio=0.273976 */
     3446u,  /* idx=889, ratio=0.344566 */
     4076u,  /* idx=890, ratio=0.407573 */
     4642u,  /* idx=891, ratio=0.464157 */
     4618u,  /* idx=892, ratio=0.461765 */
     3670u,  /* idx=893, ratio=0.367003 */
     2806u,  /* idx=894, ratio=0.280622 */
     2016u,  /* idx=895, ratio=0.201555 */
     1289u,  /* idx=896, ratio=0.128912 */
      619u,  /* idx=897, ratio=0.061940 */
        0u,  /* idx=898, ratio=0.000000 */
        0u,  /* idx=899, ratio=0.000000 */
     1011u,  /* idx=900, ratio=0.101092 */
     1896u,  /* idx=901, ratio=0.189558 */
     2676u,  /* idx=902, ratio=0.267623 */
     3370u,  /* idx=903, ratio=0.337020 */
     3991u,  /* idx=904, ratio=0.399117 */
     4550u,  /* idx=905, ratio=0.455008 */
     3776u,  /* idx=906, ratio=0.377580 */
     2883u,  /* idx=907, ratio=0.288341 */
     2069u,  /* idx=908, ratio=0.206859 */
     1322u,  /* idx=909, ratio=0.132163 */
      634u,  /* idx=910, ratio=0.063439 */
        0u,  /* idx=911, ratio=0.000000 */
        0u,  /* idx=912, ratio=0.000000 */
      985u,  /* idx=913, ratio=0.098504 */
     1850u,  /* idx=914, ratio=0.185000 */
     2616u,  /* idx=915, ratio=0.261558 */
     3298u,  /* idx=916, ratio=0.329798 */
     3910u,  /* idx=917, ratio=0.391005 */
     3888u,  /* idx=918, ratio=0.388784 */
     2965u,  /* idx=919, ratio=0.296498 */
     2124u,  /* idx=920, ratio=0.212449 */
     1356u,  /* idx=921, ratio=0.135582 */
      650u,  /* idx=922, ratio=0.065013 */
        0u,  /* idx=923, ratio=0.000000 */
        0u,  /* idx=924, ratio=0.000000 */
      960u,  /* idx=925, ratio=0.096045 */
     1807u,  /* idx=926, ratio=0.180657 */
     2558u,  /* idx=927, ratio=0.255762 */
     3229u,  /* idx=928, ratio=0.322878 */
     3832u,  /* idx=929, ratio=0.383215 */
     3051u,  /* idx=930, ratio=0.305129 */
     2183u,  /* idx=931, ratio=0.218350 */
     1392u,  /* idx=932, ratio=0.139182 */
      667u,  /* idx=933, ratio=0.066667 */
        0u,  /* idx=934, ratio=0.000000 */
        0u,  /* idx=935, ratio=0.000000 */
      937u,  /* idx=936, ratio=0.093706 */
     1765u,  /* idx=937, ratio=0.176513 */
     2502u,  /* idx=938, ratio=0.250218 */
     3162u,  /* idx=939, ratio=0.316243 */
     3143u,  /* idx=940, ratio=0.314278 */
     2246u,  /* idx=941, ratio=0.224588 */
     1430u,  /* idx=942, ratio=0.142980 */
      684u,  /* idx=943, ratio=0.068408 */
        0u,  /* idx=944, ratio=-0.000000 */
        0u,  /* idx=945, ratio=0.000000 */
      915u,  /* idx=946, ratio=0.091478 */
     1726u,  /* idx=947, ratio=0.172554 */
     2449u,  /* idx=948, ratio=0.244908 */
     3099u,  /* idx=949, ratio=0.309876 */
     2312u,  /* idx=950, ratio=0.231193 */
     1470u,  /* idx=951, ratio=0.146990 */
      702u,  /* idx=952, ratio=0.070241 */
        0u,  /* idx=953, ratio=-0.000000 */
        0u,  /* idx=954, ratio=0.000000 */
      894u,  /* idx=955, ratio=0.089353 */
     1688u,  /* idx=956, ratio=0.168770 */
     2398u,  /* idx=957, ratio=0.239820 */
     2382u,  /* idx=958, ratio=0.238198 */
     1512u,  /* idx=959, ratio=0.151231 */
      722u,  /* idx=960, ratio=0.072176 */
        0u,  /* idx=961, ratio=-0.000000 */
        0u,  /* idx=962, ratio=0.000000 */
      873u,  /* idx=963, ratio=0.087325 */
     1651u,  /* idx=964, ratio=0.165147 */
     2349u,  /* idx=965, ratio=0.234938 */
     1557u,  /* idx=966, ratio=0.155725 */
      742u,  /* idx=967, ratio=0.074220 */
        0u,  /* idx=968, ratio=0.000000 */
        0u,  /* idx=969, ratio=0.000000 */
      854u,  /* idx=970, ratio=0.085387 */
     1617u,  /* idx=971, ratio=0.161677 */
     1605u,  /* idx=972, ratio=0.160494 */
      764u,  /* idx=973, ratio=0.076384 */
        0u,  /* idx=974, ratio=0.000000 */
        0u,  /* idx=975, ratio=0.000000 */
      835u,  /* idx=976, ratio=0.083533 */
     1584u,  /* idx=977, ratio=0.158350 */
      787u,  /* idx=978, ratio=0.078677 */
        0u,  /* idx=979, ratio=0.000000 */
        0u,  /* idx=980, ratio=0.000000 */
      798u,  /* idx=981, ratio=0.079775 */
      811u,  /* idx=982, ratio=0.081113 */
        0u,  /* idx=983, ratio=0.000000 */
        0u,  /* idx=984, ratio=0.000000 */
      609u,  /* idx=985, ratio=0.060861 */
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
