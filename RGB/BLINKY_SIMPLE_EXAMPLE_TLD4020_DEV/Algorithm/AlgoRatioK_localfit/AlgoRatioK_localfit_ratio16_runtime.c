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
#define ALGO_RATIO_K_VERTEX_K_V_Q1000    (85u)
#define ALGO_RATIO_K_VERTEX_I_U_Q1000    (547u)
#define ALGO_RATIO_K_VERTEX_I_V_Q1000    (518u)
#define ALGO_RATIO_K_VERTEX_J_U_Q1000    (49u)
#define ALGO_RATIO_K_VERTEX_J_V_Q1000    (568u)

#define ALGO_RATIO_K_VERTEX_K_U_FLOAT    (0.19972226f)
#define ALGO_RATIO_K_VERTEX_K_V_FLOAT    (0.08507725f)
#define ALGO_RATIO_K_VERTEX_I_U_FLOAT    (0.547029138f)
#define ALGO_RATIO_K_VERTEX_I_V_FLOAT    (0.517643452f)
#define ALGO_RATIO_K_VERTEX_J_U_FLOAT    (0.04930155f)
#define ALGO_RATIO_K_VERTEX_J_V_FLOAT    (0.567755f)

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
        0u,  /* idx=  5, ratio=-0.000000 */
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
        0u,  /* idx= 16, ratio=-0.000000 */
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
        0u,  /* idx= 39, ratio=-0.000000 */
        0u,  /* idx= 40, ratio=0.000000 */
        0u,  /* idx= 41, ratio=0.000000 */
        0u,  /* idx= 42, ratio=-0.000000 */
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
     3805u,  /* idx= 88, ratio=0.380482 */
     2399u,  /* idx= 89, ratio=0.239936 */
     1263u,  /* idx= 90, ratio=0.126300 */
      360u,  /* idx= 91, ratio=0.035988 */
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
     3790u,  /* idx=130, ratio=0.378961 */
     6064u,  /* idx=131, ratio=0.606441 */
     4893u,  /* idx=132, ratio=0.489319 */
     3833u,  /* idx=133, ratio=0.383307 */
     2866u,  /* idx=134, ratio=0.286644 */
     2107u,  /* idx=135, ratio=0.210743 */
     1564u,  /* idx=136, ratio=0.156387 */
     1183u,  /* idx=137, ratio=0.118349 */
      904u,  /* idx=138, ratio=0.090438 */
      701u,  /* idx=139, ratio=0.070144 */
      460u,  /* idx=140, ratio=0.045972 */
      238u,  /* idx=141, ratio=0.023820 */
       57u,  /* idx=142, ratio=0.005741 */
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
     3450u,  /* idx=171, ratio=0.345032 */
     5622u,  /* idx=172, ratio=0.562206 */
     7115u,  /* idx=173, ratio=0.711482 */
     6538u,  /* idx=174, ratio=0.653822 */
     5559u,  /* idx=175, ratio=0.555949 */
     4645u,  /* idx=176, ratio=0.464470 */
     3836u,  /* idx=177, ratio=0.383591 */
     3217u,  /* idx=178, ratio=0.321660 */
     2756u,  /* idx=179, ratio=0.275603 */
     2410u,  /* idx=180, ratio=0.240969 */
     2074u,  /* idx=181, ratio=0.207389 */
     1769u,  /* idx=182, ratio=0.176921 */
     1513u,  /* idx=183, ratio=0.151345 */
     1280u,  /* idx=184, ratio=0.127965 */
     1060u,  /* idx=185, ratio=0.106000 */
      825u,  /* idx=186, ratio=0.082547 */
      500u,  /* idx=187, ratio=0.050012 */
      208u,  /* idx=188, ratio=0.020824 */
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
     3167u,  /* idx=211, ratio=0.316679 */
     5240u,  /* idx=212, ratio=0.523985 */
     6702u,  /* idx=213, ratio=0.670235 */
     7789u,  /* idx=214, ratio=0.778941 */
     7683u,  /* idx=215, ratio=0.768284 */
     6758u,  /* idx=216, ratio=0.675823 */
     5966u,  /* idx=217, ratio=0.596610 */
     5156u,  /* idx=218, ratio=0.515614 */
     4510u,  /* idx=219, ratio=0.450953 */
     4024u,  /* idx=220, ratio=0.402388 */
     3600u,  /* idx=221, ratio=0.359987 */
     3230u,  /* idx=222, ratio=0.322999 */
     2901u,  /* idx=223, ratio=0.290056 */
     2588u,  /* idx=224, ratio=0.258825 */
     2244u,  /* idx=225, ratio=0.224434 */
     1841u,  /* idx=226, ratio=0.184101 */
     1471u,  /* idx=227, ratio=0.147064 */
     1138u,  /* idx=228, ratio=0.113770 */
      827u,  /* idx=229, ratio=0.082660 */
      548u,  /* idx=230, ratio=0.054769 */
      286u,  /* idx=231, ratio=0.028621 */
       40u,  /* idx=232, ratio=0.004042 */
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
     2926u,  /* idx=250, ratio=0.292632 */
     4906u,  /* idx=251, ratio=0.490630 */
     6335u,  /* idx=252, ratio=0.633509 */
     7415u,  /* idx=253, ratio=0.741474 */
     8259u,  /* idx=254, ratio=0.825928 */
     8515u,  /* idx=255, ratio=0.851465 */
     7634u,  /* idx=256, ratio=0.763419 */
     6925u,  /* idx=257, ratio=0.692513 */
     6183u,  /* idx=258, ratio=0.618305 */
     5551u,  /* idx=259, ratio=0.555066 */
     5043u,  /* idx=260, ratio=0.504256 */
     4614u,  /* idx=261, ratio=0.461427 */
     4196u,  /* idx=262, ratio=0.419599 */
     3737u,  /* idx=263, ratio=0.373720 */
     3247u,  /* idx=264, ratio=0.324728 */
     2797u,  /* idx=265, ratio=0.279720 */
     2390u,  /* idx=266, ratio=0.239047 */
     2019u,  /* idx=267, ratio=0.201908 */
     1675u,  /* idx=268, ratio=0.167549 */
     1362u,  /* idx=269, ratio=0.136186 */
     1067u,  /* idx=270, ratio=0.106694 */
      794u,  /* idx=271, ratio=0.079380 */
      531u,  /* idx=272, ratio=0.053127 */
      283u,  /* idx=273, ratio=0.028292 */
       43u,  /* idx=274, ratio=0.004255 */
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
     2720u,  /* idx=288, ratio=0.271980 */
     4613u,  /* idx=289, ratio=0.461268 */
     6006u,  /* idx=290, ratio=0.600599 */
     7074u,  /* idx=291, ratio=0.707445 */
     7920u,  /* idx=292, ratio=0.791981 */
     8605u,  /* idx=293, ratio=0.860533 */
     9168u,  /* idx=294, ratio=0.916768 */
     8325u,  /* idx=295, ratio=0.832538 */
     7627u,  /* idx=296, ratio=0.762737 */
     6974u,  /* idx=297, ratio=0.697439 */
     6373u,  /* idx=298, ratio=0.637327 */
     5882u,  /* idx=299, ratio=0.588171 */
     5293u,  /* idx=300, ratio=0.529294 */
     4713u,  /* idx=301, ratio=0.471324 */
     4184u,  /* idx=302, ratio=0.418441 */
     3700u,  /* idx=303, ratio=0.370001 */
     3255u,  /* idx=304, ratio=0.325504 */
     2850u,  /* idx=305, ratio=0.285027 */
     2479u,  /* idx=306, ratio=0.247852 */
     2133u,  /* idx=307, ratio=0.213279 */
     1811u,  /* idx=308, ratio=0.181112 */
     1507u,  /* idx=309, ratio=0.150686 */
     1218u,  /* idx=310, ratio=0.121845 */
      939u,  /* idx=311, ratio=0.093911 */
      673u,  /* idx=312, ratio=0.067309 */
      420u,  /* idx=313, ratio=0.041967 */
      166u,  /* idx=314, ratio=0.016593 */
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
     2541u,  /* idx=325, ratio=0.254050 */
     4352u,  /* idx=326, ratio=0.435221 */
     5709u,  /* idx=327, ratio=0.570939 */
     6764u,  /* idx=328, ratio=0.676403 */
     7607u,  /* idx=329, ratio=0.760714 */
     8297u,  /* idx=330, ratio=0.829657 */
     8871u,  /* idx=331, ratio=0.887082 */
     9357u,  /* idx=332, ratio=0.935653 */
     8726u,  /* idx=333, ratio=0.872618 */
     8143u,  /* idx=334, ratio=0.814334 */
     7633u,  /* idx=335, ratio=0.763348 */
     6900u,  /* idx=336, ratio=0.690038 */
     6236u,  /* idx=337, ratio=0.623622 */
     5623u,  /* idx=338, ratio=0.562266 */
     5058u,  /* idx=339, ratio=0.505830 */
     4545u,  /* idx=340, ratio=0.454497 */
     4070u,  /* idx=341, ratio=0.406997 */
     3634u,  /* idx=342, ratio=0.363405 */
     3235u,  /* idx=343, ratio=0.323500 */
     2861u,  /* idx=344, ratio=0.286145 */
     2511u,  /* idx=345, ratio=0.251148 */
     2178u,  /* idx=346, ratio=0.217824 */
     1866u,  /* idx=347, ratio=0.186580 */
     1562u,  /* idx=348, ratio=0.156174 */
     1271u,  /* idx=349, ratio=0.127055 */
      992u,  /* idx=350, ratio=0.099152 */
      717u,  /* idx=351, ratio=0.071740 */
      464u,  /* idx=352, ratio=0.046367 */
      222u,  /* idx=353, ratio=0.022229 */
        0u,  /* idx=354, ratio=0.000000 */
        0u,  /* idx=355, ratio=0.000000 */
        0u,  /* idx=356, ratio=0.000000 */
        0u,  /* idx=357, ratio=0.000000 */
        0u,  /* idx=358, ratio=0.000000 */
        0u,  /* idx=359, ratio=0.000000 */
        0u,  /* idx=360, ratio=0.000000 */
     2383u,  /* idx=361, ratio=0.238338 */
     4120u,  /* idx=362, ratio=0.411959 */
     5441u,  /* idx=363, ratio=0.544071 */
     6480u,  /* idx=364, ratio=0.647970 */
     7318u,  /* idx=365, ratio=0.731823 */
     8009u,  /* idx=366, ratio=0.800919 */
     8588u,  /* idx=367, ratio=0.858840 */
     9081u,  /* idx=368, ratio=0.908094 */
     9505u,  /* idx=369, ratio=0.950490 */
     8927u,  /* idx=370, ratio=0.892654 */
     8381u,  /* idx=371, ratio=0.838109 */
     7841u,  /* idx=372, ratio=0.784095 */
     7098u,  /* idx=373, ratio=0.709833 */
     6456u,  /* idx=374, ratio=0.645612 */
     5867u,  /* idx=375, ratio=0.586663 */
     5328u,  /* idx=376, ratio=0.532836 */
     4830u,  /* idx=377, ratio=0.482977 */
     4368u,  /* idx=378, ratio=0.436765 */
     3945u,  /* idx=379, ratio=0.394468 */
     3545u,  /* idx=380, ratio=0.354549 */
     3173u,  /* idx=381, ratio=0.317341 */
     2817u,  /* idx=382, ratio=0.281672 */
     2474u,  /* idx=383, ratio=0.247366 */
     2149u,  /* idx=384, ratio=0.214908 */
     1836u,  /* idx=385, ratio=0.183639 */
     1536u,  /* idx=386, ratio=0.153578 */
     1249u,  /* idx=387, ratio=0.124926 */
      977u,  /* idx=388, ratio=0.097739 */
      725u,  /* idx=389, ratio=0.072494 */
      490u,  /* idx=390, ratio=0.048969 */
      277u,  /* idx=391, ratio=0.027745 */
       90u,  /* idx=392, ratio=0.009046 */
        0u,  /* idx=393, ratio=0.000000 */
        0u,  /* idx=394, ratio=0.000000 */
        0u,  /* idx=395, ratio=0.000000 */
     2245u,  /* idx=396, ratio=0.224457 */
     3911u,  /* idx=397, ratio=0.391057 */
     5196u,  /* idx=398, ratio=0.519618 */
     6218u,  /* idx=399, ratio=0.621832 */
     7050u,  /* idx=400, ratio=0.705045 */
     7741u,  /* idx=401, ratio=0.774106 */
     8323u,  /* idx=402, ratio=0.832341 */
     8821u,  /* idx=403, ratio=0.882111 */
     9251u,  /* idx=404, ratio=0.925137 */
     9627u,  /* idx=405, ratio=0.962703 */
     9094u,  /* idx=406, ratio=0.909357 */
     8582u,  /* idx=407, ratio=0.858152 */
     7865u,  /* idx=408, ratio=0.786540 */
     7216u,  /* idx=409, ratio=0.721573 */
     6607u,  /* idx=410, ratio=0.660740 */
     6051u,  /* idx=411, ratio=0.605142 */
     5534u,  /* idx=412, ratio=0.553444 */
     5054u,  /* idx=413, ratio=0.505403 */
     4609u,  /* idx=414, ratio=0.460904 */
     4185u,  /* idx=415, ratio=0.418450 */
     3789u,  /* idx=416, ratio=0.378893 */
     3406u,  /* idx=417, ratio=0.340579 */
     3045u,  /* idx=418, ratio=0.304536 */
     2697u,  /* idx=419, ratio=0.269730 */
     2368u,  /* idx=420, ratio=0.236819 */
     2053u,  /* idx=421, ratio=0.205349 */
     1757u,  /* idx=422, ratio=0.175702 */
     1477u,  /* idx=423, ratio=0.147711 */
     1218u,  /* idx=424, ratio=0.121811 */
      979u,  /* idx=425, ratio=0.097943 */
      769u,  /* idx=426, ratio=0.076877 */
      419u,  /* idx=427, ratio=0.041876 */
        0u,  /* idx=428, ratio=0.000000 */
        0u,  /* idx=429, ratio=0.000000 */
     2121u,  /* idx=430, ratio=0.212103 */
     3722u,  /* idx=431, ratio=0.372174 */
     4973u,  /* idx=432, ratio=0.497268 */
     5977u,  /* idx=433, ratio=0.597720 */
     6802u,  /* idx=434, ratio=0.680158 */
     7490u,  /* idx=435, ratio=0.749030 */
     8074u,  /* idx=436, ratio=0.807428 */
     8576u,  /* idx=437, ratio=0.857574 */
     9011u,  /* idx=438, ratio=0.901102 */
     9392u,  /* idx=439, ratio=0.939240 */
     9729u,  /* idx=440, ratio=0.972931 */
     9235u,  /* idx=441, ratio=0.923496 */
     8613u,  /* idx=442, ratio=0.861342 */
     7893u,  /* idx=443, ratio=0.789300 */
     7280u,  /* idx=444, ratio=0.728047 */
     6713u,  /* idx=445, ratio=0.671280 */
     6181u,  /* idx=446, ratio=0.618104 */
     5687u,  /* idx=447, ratio=0.568749 */
     5224u,  /* idx=448, ratio=0.522421 */
     4781u,  /* idx=449, ratio=0.478135 */
     4363u,  /* idx=450, ratio=0.436300 */
     3964u,  /* idx=451, ratio=0.396385 */
     3586u,  /* idx=452, ratio=0.358627 */
     3223u,  /* idx=453, ratio=0.322284 */
     2880u,  /* idx=454, ratio=0.288016 */
     2554u,  /* idx=455, ratio=0.255408 */
     2247u,  /* idx=456, ratio=0.224724 */
     1852u,  /* idx=457, ratio=0.185189 */
     1349u,  /* idx=458, ratio=0.134941 */
      875u,  /* idx=459, ratio=0.087472 */
      426u,  /* idx=460, ratio=0.042559 */
        0u,  /* idx=461, ratio=0.000000 */
        0u,  /* idx=462, ratio=0.000000 */
     2010u,  /* idx=463, ratio=0.201038 */
     3550u,  /* idx=464, ratio=0.355031 */
     4768u,  /* idx=465, ratio=0.476762 */
     5754u,  /* idx=466, ratio=0.575409 */
     6570u,  /* idx=467, ratio=0.656968 */
     7255u,  /* idx=468, ratio=0.725527 */
     7840u,  /* idx=469, ratio=0.783963 */
     8344u,  /* idx=470, ratio=0.834366 */
     8783u,  /* idx=471, ratio=0.878284 */
     9169u,  /* idx=472, ratio=0.916893 */
     9511u,  /* idx=473, ratio=0.951102 */
     9816u,  /* idx=474, ratio=0.981622 */
     9242u,  /* idx=475, ratio=0.924228 */
     8481u,  /* idx=476, ratio=0.848068 */
     7880u,  /* idx=477, ratio=0.788012 */
     7309u,  /* idx=478, ratio=0.730926 */
     6773u,  /* idx=479, ratio=0.677267 */
     6266u,  /* idx=480, ratio=0.626648 */
     5787u,  /* idx=481, ratio=0.578718 */
     5335u,  /* idx=482, ratio=0.533541 */
     4899u,  /* idx=483, ratio=0.489880 */
     4487u,  /* idx=484, ratio=0.448700 */
     4093u,  /* idx=485, ratio=0.409344 */
     3623u,  /* idx=486, ratio=0.362343 */
     3007u,  /* idx=487, ratio=0.300734 */
     2429u,  /* idx=488, ratio=0.242912 */
     1885u,  /* idx=489, ratio=0.188537 */
     1373u,  /* idx=490, ratio=0.137309 */
      890u,  /* idx=491, ratio=0.088964 */
      433u,  /* idx=492, ratio=0.043265 */
        0u,  /* idx=493, ratio=0.000000 */
        0u,  /* idx=494, ratio=0.000000 */
     1911u,  /* idx=495, ratio=0.191071 */
     3394u,  /* idx=496, ratio=0.339397 */
     4579u,  /* idx=497, ratio=0.457880 */
     5547u,  /* idx=498, ratio=0.554703 */
     6353u,  /* idx=499, ratio=0.635308 */
     7035u,  /* idx=500, ratio=0.703454 */
     7618u,  /* idx=501, ratio=0.761824 */
     8124u,  /* idx=502, ratio=0.812380 */
     8566u,  /* idx=503, ratio=0.856593 */
     8956u,  /* idx=504, ratio=0.895586 */
     9302u,  /* idx=505, ratio=0.930231 */
     9612u,  /* idx=506, ratio=0.961219 */
     9740u,  /* idx=507, ratio=0.973989 */
     9014u,  /* idx=508, ratio=0.901400 */
     8403u,  /* idx=509, ratio=0.840322 */
     7839u,  /* idx=510, ratio=0.783866 */
     7303u,  /* idx=511, ratio=0.730261 */
     6794u,  /* idx=512, ratio=0.679447 */
     6308u,  /* idx=513, ratio=0.630758 */
     5842u,  /* idx=514, ratio=0.584229 */
     5093u,  /* idx=515, ratio=0.509326 */
     4371u,  /* idx=516, ratio=0.437091 */
     3697u,  /* idx=517, ratio=0.369681 */
     3066u,  /* idx=518, ratio=0.306628 */
     2475u,  /* idx=519, ratio=0.247523 */
     1920u,  /* idx=520, ratio=0.192007 */
     1398u,  /* idx=521, ratio=0.139762 */
      905u,  /* idx=522, ratio=0.090508 */
      440u,  /* idx=523, ratio=0.043995 */
        0u,  /* idx=524, ratio=0.000000 */
        0u,  /* idx=525, ratio=0.000000 */
     1820u,  /* idx=526, ratio=0.182045 */
     3251u,  /* idx=527, ratio=0.325083 */
     4404u,  /* idx=528, ratio=0.440437 */
     5354u,  /* idx=529, ratio=0.535435 */
     6150u,  /* idx=530, ratio=0.615030 */
     6827u,  /* idx=531, ratio=0.682685 */
     7409u,  /* idx=532, ratio=0.740901 */
     7915u,  /* idx=533, ratio=0.791523 */
     8359u,  /* idx=534, ratio=0.835947 */
     8752u,  /* idx=535, ratio=0.875245 */
     9103u,  /* idx=536, ratio=0.910257 */
     9416u,  /* idx=537, ratio=0.941646 */
     9699u,  /* idx=538, ratio=0.969949 */
     9453u,  /* idx=539, ratio=0.945288 */
     8838u,  /* idx=540, ratio=0.883766 */
     8304u,  /* idx=541, ratio=0.830381 */
     7774u,  /* idx=542, ratio=0.777402 */
     6866u,  /* idx=543, ratio=0.686553 */
     6004u,  /* idx=544, ratio=0.600438 */
     5206u,  /* idx=545, ratio=0.520624 */
     4464u,  /* idx=546, ratio=0.446444 */
     3773u,  /* idx=547, ratio=0.377322 */
     3128u,  /* idx=548, ratio=0.312757 */
     2523u,  /* idx=549, ratio=0.252313 */
     1956u,  /* idx=550, ratio=0.195607 */
     1423u,  /* idx=551, ratio=0.142304 */
      921u,  /* idx=552, ratio=0.092107 */
      447u,  /* idx=553, ratio=0.044750 */
        0u,  /* idx=554, ratio=0.000000 */
        0u,  /* idx=555, ratio=0.000000 */
     1738u,  /* idx=556, ratio=0.173833 */
     3119u,  /* idx=557, ratio=0.311926 */
     4243u,  /* idx=558, ratio=0.424274 */
     5175u,  /* idx=559, ratio=0.517462 */
     5960u,  /* idx=560, ratio=0.596006 */
     6631u,  /* idx=561, ratio=0.663107 */
     7211u,  /* idx=562, ratio=0.721096 */
     7717u,  /* idx=563, ratio=0.771710 */
     8163u,  /* idx=564, ratio=0.816273 */
     8558u,  /* idx=565, ratio=0.855809 */
     8911u,  /* idx=566, ratio=0.891122 */
     9229u,  /* idx=567, ratio=0.922855 */
     9515u,  /* idx=568, ratio=0.951526 */
     9776u,  /* idx=569, ratio=0.977558 */
     9044u,  /* idx=570, ratio=0.904428 */
     7997u,  /* idx=571, ratio=0.799671 */
     7034u,  /* idx=572, ratio=0.703388 */
     6146u,  /* idx=573, ratio=0.614589 */
     5324u,  /* idx=574, ratio=0.532435 */
     4562u,  /* idx=575, ratio=0.456207 */
     3853u,  /* idx=576, ratio=0.385285 */
     3191u,  /* idx=577, ratio=0.319136 */
     2573u,  /* idx=578, ratio=0.257291 */
     1993u,  /* idx=579, ratio=0.199346 */
     1449u,  /* idx=580, ratio=0.144941 */
      938u,  /* idx=581, ratio=0.093762 */
      455u,  /* idx=582, ratio=0.045531 */
        0u,  /* idx=583, ratio=0.000000 */
        0u,  /* idx=584, ratio=0.000000 */
     1663u,  /* idx=585, ratio=0.166331 */
     2998u,  /* idx=586, ratio=0.299794 */
     4093u,  /* idx=587, ratio=0.409255 */
     5007u,  /* idx=588, ratio=0.500655 */
     5781u,  /* idx=589, ratio=0.578124 */
     6446u,  /* idx=590, ratio=0.644621 */
     7023u,  /* idx=591, ratio=0.702322 */
     7529u,  /* idx=592, ratio=0.752865 */
     7975u,  /* idx=593, ratio=0.797504 */
     8372u,  /* idx=594, ratio=0.837217 */
     8728u,  /* idx=595, ratio=0.872775 */
     9048u,  /* idx=596, ratio=0.904799 */
     9338u,  /* idx=597, ratio=0.933791 */
     9292u,  /* idx=598, ratio=0.929217 */
     8206u,  /* idx=599, ratio=0.820642 */
     7211u,  /* idx=600, ratio=0.721068 */
     6294u,  /* idx=601, ratio=0.629423 */
     5448u,  /* idx=602, ratio=0.544794 */
     4664u,  /* idx=603, ratio=0.466406 */
     3936u,  /* idx=604, ratio=0.393593 */
     3258u,  /* idx=605, ratio=0.325780 */
     2625u,  /* idx=606, ratio=0.262470 */
     2032u,  /* idx=607, ratio=0.203229 */
     1477u,  /* idx=608, ratio=0.147677 */
      955u,  /* idx=609, ratio=0.095479 */
      463u,  /* idx=610, ratio=0.046340 */
        0u,  /* idx=611, ratio=-0.000000 */
        0u,  /* idx=612, ratio=-0.000000 */
     1594u,  /* idx=613, ratio=0.159449 */
     2886u,  /* idx=614, ratio=0.288569 */
     3953u,  /* idx=615, ratio=0.395263 */
     4849u,  /* idx=616, ratio=0.484906 */
     5613u,  /* idx=617, ratio=0.561284 */
     6271u,  /* idx=618, ratio=0.627137 */
     6845u,  /* idx=619, ratio=0.684502 */
     7349u,  /* idx=620, ratio=0.734919 */
     7796u,  /* idx=621, ratio=0.779579 */
     8194u,  /* idx=622, ratio=0.819415 */
     8552u,  /* idx=623, ratio=0.855169 */
     8874u,  /* idx=624, ratio=0.887436 */
     9167u,  /* idx=625, ratio=0.916705 */
     8427u,  /* idx=626, ratio=0.842741 */
     7397u,  /* idx=627, ratio=0.739661 */
     6450u,  /* idx=628, ratio=0.644990 */
     5577u,  /* idx=629, ratio=0.557740 */
     4771u,  /* idx=630, ratio=0.477071 */
     4023u,  /* idx=631, ratio=0.402266 */
     3327u,  /* idx=632, ratio=0.332707 */
     2679u,  /* idx=633, ratio=0.267862 */
     2073u,  /* idx=634, ratio=0.207267 */
     1505u,  /* idx=635, ratio=0.150518 */
      973u,  /* idx=636, ratio=0.097259 */
      472u,  /* idx=637, ratio=0.047179 */
        0u,  /* idx=638, ratio=0.000000 */
        0u,  /* idx=639, ratio=0.000000 */
     1531u,  /* idx=640, ratio=0.153114 */
     2782u,  /* idx=641, ratio=0.278155 */
     3822u,  /* idx=642, ratio=0.382197 */
     4701u,  /* idx=643, ratio=0.470118 */
     5454u,  /* idx=644, ratio=0.545397 */
     6106u,  /* idx=645, ratio=0.610577 */
     6676u,  /* idx=646, ratio=0.667563 */
     7178u,  /* idx=647, ratio=0.717808 */
     7624u,  /* idx=648, ratio=0.762442 */
     8024u,  /* idx=649, ratio=0.802355 */
     8383u,  /* idx=650, ratio=0.838258 */
     8707u,  /* idx=651, ratio=0.870727 */
     8661u,  /* idx=652, ratio=0.866064 */
     7592u,  /* idx=653, ratio=0.759237 */
     6613u,  /* idx=654, ratio=0.661347 */
     5713u,  /* idx=655, ratio=0.571316 */
     4882u,  /* idx=656, ratio=0.488236 */
     4113u,  /* idx=657, ratio=0.411330 */
     3399u,  /* idx=658, ratio=0.339936 */
     2735u,  /* idx=659, ratio=0.273481 */
     2115u,  /* idx=660, ratio=0.211469 */
     1535u,  /* idx=661, ratio=0.153471 */
      991u,  /* idx=662, ratio=0.099107 */
      480u,  /* idx=663, ratio=0.048048 */
        0u,  /* idx=664, ratio=-0.000000 */
        0u,  /* idx=665, ratio=0.000000 */
     1473u,  /* idx=666, ratio=0.147263 */
     2685u,  /* idx=667, ratio=0.268467 */
     3700u,  /* idx=668, ratio=0.369966 */
     4562u,  /* idx=669, ratio=0.456205 */
     5304u,  /* idx=670, ratio=0.530385 */
     5949u,  /* idx=671, ratio=0.594869 */
     6514u,  /* idx=672, ratio=0.651442 */
     7015u,  /* idx=673, ratio=0.701476 */
     7460u,  /* idx=674, ratio=0.746042 */
     7860u,  /* idx=675, ratio=0.785991 */
     8220u,  /* idx=676, ratio=0.822004 */
     8546u,  /* idx=677, ratio=0.854636 */
     7799u,  /* idx=678, ratio=0.779879 */
     6786u,  /* idx=679, ratio=0.678555 */
     5856u,  /* idx=680, ratio=0.585570 */
     4999u,  /* idx=681, ratio=0.499935 */
     4208u,  /* idx=682, ratio=0.420812 */
     3475u,  /* idx=683, ratio=0.347485 */
     2793u,  /* idx=684, ratio=0.279339 */
     2158u,  /* idx=685, ratio=0.215845 */
     1565u,  /* idx=686, ratio=0.156541 */
     1010u,  /* idx=687, ratio=0.101027 */
      489u,  /* idx=688, ratio=0.048950 */
        0u,  /* idx=689, ratio=-0.000000 */
        0u,  /* idx=690, ratio=0.000000 */
     1418u,  /* idx=691, ratio=0.141843 */
     2594u,  /* idx=692, ratio=0.259430 */
     3585u,  /* idx=693, ratio=0.358494 */
     4431u,  /* idx=694, ratio=0.443092 */
     5162u,  /* idx=695, ratio=0.516176 */
     5799u,  /* idx=696, ratio=0.579949 */
     6361u,  /* idx=697, ratio=0.636081 */
     6859u,  /* idx=698, ratio=0.685870 */
     7303u,  /* idx=699, ratio=0.730333 */
     7703u,  /* idx=700, ratio=0.770280 */
     8064u,  /* idx=701, ratio=0.806368 */
     8017u,  /* idx=702, ratio=0.801673 */
     6967u,  /* idx=703, ratio=0.696683 */
     6006u,  /* idx=704, ratio=0.600554 */
     5122u,  /* idx=705, ratio=0.512210 */
     4307u,  /* idx=706, ratio=0.430742 */
     3554u,  /* idx=707, ratio=0.355377 */
     2855u,  /* idx=708, ratio=0.285455 */
     2204u,  /* idx=709, ratio=0.220406 */
     1597u,  /* idx=710, ratio=0.159738 */
     1030u,  /* idx=711, ratio=0.103023 */
      499u,  /* idx=712, ratio=0.049886 */
        0u,  /* idx=713, ratio=0.000000 */
        0u,  /* idx=714, ratio=0.000000 */
     1368u,  /* idx=715, ratio=0.136807 */
     2510u,  /* idx=716, ratio=0.250982 */
     3477u,  /* idx=717, ratio=0.347712 */
     4307u,  /* idx=718, ratio=0.430711 */
     5027u,  /* idx=719, ratio=0.502710 */
     5658u,  /* idx=720, ratio=0.565759 */
     6214u,  /* idx=721, ratio=0.621429 */
     6709u,  /* idx=722, ratio=0.670944 */
     7153u,  /* idx=723, ratio=0.715271 */
     7552u,  /* idx=724, ratio=0.755186 */
     7913u,  /* idx=725, ratio=0.791315 */
     7158u,  /* idx=726, ratio=0.715806 */
     6163u,  /* idx=727, ratio=0.616324 */
     5251u,  /* idx=728, ratio=0.525102 */
     4412u,  /* idx=729, ratio=0.441151 */
     3636u,  /* idx=730, ratio=0.363636 */
     2918u,  /* idx=731, ratio=0.291844 */
     2252u,  /* idx=732, ratio=0.225163 */
     1631u,  /* idx=733, ratio=0.163067 */
     1051u,  /* idx=734, ratio=0.105099 */
      509u,  /* idx=735, ratio=0.050859 */
        0u,  /* idx=736, ratio=0.000000 */
        0u,  /* idx=737, ratio=0.000000 */
     1321u,  /* idx=738, ratio=0.132117 */
     2431u,  /* idx=739, ratio=0.243067 */
     3376u,  /* idx=740, ratio=0.337560 */
     4190u,  /* idx=741, ratio=0.419004 */
     4899u,  /* idx=742, ratio=0.489928 */
     5522u,  /* idx=743, ratio=0.552246 */
     6074u,  /* idx=744, ratio=0.607436 */
     6567u,  /* idx=745, ratio=0.656653 */
     7008u,  /* idx=746, ratio=0.700819 */
     7407u,  /* idx=747, ratio=0.740672 */
     7360u,  /* idx=748, ratio=0.736008 */
     6329u,  /* idx=749, ratio=0.632945 */
     5387u,  /* idx=750, ratio=0.538659 */
     4521u,  /* idx=751, ratio=0.452076 */
     3723u,  /* idx=752, ratio=0.372288 */
     2985u,  /* idx=753, ratio=0.298525 */
     2301u,  /* idx=754, ratio=0.230131 */
     1665u,  /* idx=755, ratio=0.166538 */
     1073u,  /* idx=756, ratio=0.107260 */
      519u,  /* idx=757, ratio=0.051871 */
        0u,  /* idx=758, ratio=0.000000 */
        0u,  /* idx=759, ratio=0.000000 */
     1277u,  /* idx=760, ratio=0.127738 */
     2356u,  /* idx=761, ratio=0.235636 */
     3280u,  /* idx=762, ratio=0.327984 */
     4079u,  /* idx=763, ratio=0.407916 */
     4778u,  /* idx=764, ratio=0.477780 */
     5394u,  /* idx=765, ratio=0.539364 */
     5941u,  /* idx=766, ratio=0.594059 */
     6430u,  /* idx=767, ratio=0.642959 */
     6869u,  /* idx=768, ratio=0.686938 */
     7267u,  /* idx=769, ratio=0.726705 */
     6505u,  /* idx=770, ratio=0.650487 */
     5529u,  /* idx=771, ratio=0.552936 */
     4636u,  /* idx=772, ratio=0.463556 */
     3814u,  /* idx=773, ratio=0.381362 */
     3055u,  /* idx=774, ratio=0.305520 */
     2353u,  /* idx=775, ratio=0.235322 */
     1702u,  /* idx=776, ratio=0.170161 */
     1095u,  /* idx=777, ratio=0.109512 */
      529u,  /* idx=778, ratio=0.052923 */
        0u,  /* idx=779, ratio=0.000000 */
        0u,  /* idx=780, ratio=0.000000 */
     1236u,  /* idx=781, ratio=0.123640 */
     2286u,  /* idx=782, ratio=0.228646 */
     3189u,  /* idx=783, ratio=0.318936 */
     3974u,  /* idx=784, ratio=0.397400 */
     4662u,  /* idx=785, ratio=0.466220 */
     5271u,  /* idx=786, ratio=0.527070 */
     5813u,  /* idx=787, ratio=0.581259 */
     6298u,  /* idx=788, ratio=0.629824 */
     6736u,  /* idx=789, ratio=0.673597 */
     6690u,  /* idx=790, ratio=0.669029 */
     5680u,  /* idx=791, ratio=0.567990 */
     4756u,  /* idx=792, ratio=0.475634 */
     3909u,  /* idx=793, ratio=0.390889 */
     3129u,  /* idx=794, ratio=0.312850 */
     2408u,  /* idx=795, ratio=0.240753 */
     1739u,  /* idx=796, ratio=0.173944 */
     1119u,  /* idx=797, ratio=0.111861 */
      540u,  /* idx=798, ratio=0.054020 */
        0u,  /* idx=799, ratio=0.000000 */
        0u,  /* idx=800, ratio=0.000000 */
     1198u,  /* idx=801, ratio=0.119796 */
     2221u,  /* idx=802, ratio=0.222058 */
     3104u,  /* idx=803, ratio=0.310374 */
     3874u,  /* idx=804, ratio=0.387413 */
     4552u,  /* idx=805, ratio=0.455206 */
     5153u,  /* idx=806, ratio=0.515323 */
     5690u,  /* idx=807, ratio=0.568999 */
     6172u,  /* idx=808, ratio=0.617215 */
     6608u,  /* idx=809, ratio=0.660765 */
     5839u,  /* idx=810, ratio=0.583886 */
     4884u,  /* idx=811, ratio=0.488358 */
     4009u,  /* idx=812, ratio=0.400904 */
     3205u,  /* idx=813, ratio=0.320541 */
     2464u,  /* idx=814, ratio=0.246441 */
     1779u,  /* idx=815, ratio=0.177899 */
     1143u,  /* idx=816, ratio=0.114312 */
      552u,  /* idx=817, ratio=0.055162 */
        0u,  /* idx=818, ratio=0.000000 */
        0u,  /* idx=819, ratio=0.000000 */
     1162u,  /* idx=820, ratio=0.116185 */
     2158u,  /* idx=821, ratio=0.215840 */
     3023u,  /* idx=822, ratio=0.302259 */
     3779u,  /* idx=823, ratio=0.377915 */
     4447u,  /* idx=824, ratio=0.444700 */
     5041u,  /* idx=825, ratio=0.504089 */
     5572u,  /* idx=826, ratio=0.557245 */
     6051u,  /* idx=827, ratio=0.605101 */
     6007u,  /* idx=828, ratio=0.600698 */
     5018u,  /* idx=829, ratio=0.501782 */
     4114u,  /* idx=830, ratio=0.411446 */
     3286u,  /* idx=831, ratio=0.328620 */
     2524u,  /* idx=832, ratio=0.252404 */
     1820u,  /* idx=833, ratio=0.182039 */
     1169u,  /* idx=834, ratio=0.116874 */
      564u,  /* idx=835, ratio=0.056354 */
        0u,  /* idx=836, ratio=0.000000 */
        0u,  /* idx=837, ratio=0.000000 */
     1128u,  /* idx=838, ratio=0.112784 */
     2100u,  /* idx=839, ratio=0.209960 */
     2946u,  /* idx=840, ratio=0.294558 */
     3689u,  /* idx=841, ratio=0.368872 */
     4347u,  /* idx=842, ratio=0.434669 */
     4933u,  /* idx=843, ratio=0.493334 */
     5460u,  /* idx=844, ratio=0.545967 */
     5935u,  /* idx=845, ratio=0.593453 */
     5160u,  /* idx=846, ratio=0.515965 */
     4226u,  /* idx=847, ratio=0.422557 */
     3371u,  /* idx=848, ratio=0.337116 */
     2587u,  /* idx=849, ratio=0.258663 */
     1864u,  /* idx=850, ratio=0.186375 */
     1196u,  /* idx=851, ratio=0.119553 */
      576u,  /* idx=852, ratio=0.057599 */
        0u,  /* idx=853, ratio=0.000000 */
        0u,  /* idx=854, ratio=0.000000 */
     1096u,  /* idx=855, ratio=0.109578 */
     2044u,  /* idx=856, ratio=0.204392 */
     2872u,  /* idx=857, ratio=0.287240 */
     3603u,  /* idx=858, ratio=0.360251 */
     4251u,  /* idx=859, ratio=0.425080 */
     4830u,  /* idx=860, ratio=0.483028 */
     5351u,  /* idx=861, ratio=0.535137 */
     5310u,  /* idx=862, ratio=0.530972 */
     4343u,  /* idx=863, ratio=0.434285 */
     3461u,  /* idx=864, ratio=0.346063 */
     2652u,  /* idx=865, ratio=0.265240 */
     1909u,  /* idx=866, ratio=0.190923 */
     1224u,  /* idx=867, ratio=0.122358 */
      589u,  /* idx=868, ratio=0.058900 */
        0u,  /* idx=869, ratio=-0.000000 */
        0u,  /* idx=870, ratio=0.000000 */
     1065u,  /* idx=871, ratio=0.106548 */
     1991u,  /* idx=872, ratio=0.199112 */
     2803u,  /* idx=873, ratio=0.280276 */
     3520u,  /* idx=874, ratio=0.352024 */
     4159u,  /* idx=875, ratio=0.415905 */
     4731u,  /* idx=876, ratio=0.473144 */
     5247u,  /* idx=877, ratio=0.524727 */
     4467u,  /* idx=878, ratio=0.446683 */
     3555u,  /* idx=879, ratio=0.355498 */
     2722u,  /* idx=880, ratio=0.272161 */
     1957u,  /* idx=881, ratio=0.195699 */
     1253u,  /* idx=882, ratio=0.125297 */
      603u,  /* idx=883, ratio=0.060261 */
        0u,  /* idx=884, ratio=0.000000 */
        0u,  /* idx=885, ratio=0.000000 */
     1037u,  /* idx=886, ratio=0.103681 */
     1941u,  /* idx=887, ratio=0.194098 */
     2736u,  /* idx=888, ratio=0.273643 */
     3442u,  /* idx=889, ratio=0.344165 */
     4071u,  /* idx=890, ratio=0.407117 */
     4637u,  /* idx=891, ratio=0.463657 */
     4598u,  /* idx=892, ratio=0.459810 */
     3655u,  /* idx=893, ratio=0.365462 */
     2795u,  /* idx=894, ratio=0.279452 */
     2007u,  /* idx=895, ratio=0.200720 */
     1284u,  /* idx=896, ratio=0.128381 */
      617u,  /* idx=897, ratio=0.061686 */
        0u,  /* idx=898, ratio=0.000000 */
        0u,  /* idx=899, ratio=0.000000 */
     1010u,  /* idx=900, ratio=0.100965 */
     1893u,  /* idx=901, ratio=0.189330 */
     2673u,  /* idx=902, ratio=0.267316 */
     3366u,  /* idx=903, ratio=0.336649 */
     3987u,  /* idx=904, ratio=0.398694 */
     4545u,  /* idx=905, ratio=0.454542 */
     3760u,  /* idx=906, ratio=0.376001 */
     2871u,  /* idx=907, ratio=0.287144 */
     2060u,  /* idx=908, ratio=0.206005 */
     1316u,  /* idx=909, ratio=0.131621 */
      632u,  /* idx=910, ratio=0.063181 */
        0u,  /* idx=911, ratio=0.000000 */
        0u,  /* idx=912, ratio=0.000000 */
      984u,  /* idx=913, ratio=0.098387 */
     1848u,  /* idx=914, ratio=0.184791 */
     2613u,  /* idx=915, ratio=0.261274 */
     3295u,  /* idx=916, ratio=0.329454 */
     3906u,  /* idx=917, ratio=0.390611 */
     3872u,  /* idx=918, ratio=0.387165 */
     2953u,  /* idx=919, ratio=0.295272 */
     2116u,  /* idx=920, ratio=0.211577 */
     1350u,  /* idx=921, ratio=0.135028 */
      647u,  /* idx=922, ratio=0.064749 */
        0u,  /* idx=923, ratio=0.000000 */
        0u,  /* idx=924, ratio=0.000000 */
      959u,  /* idx=925, ratio=0.095938 */
     1805u,  /* idx=926, ratio=0.180464 */
     2555u,  /* idx=927, ratio=0.255500 */
     3226u,  /* idx=928, ratio=0.322560 */
     3829u,  /* idx=929, ratio=0.382850 */
     3039u,  /* idx=930, ratio=0.303874 */
     2175u,  /* idx=931, ratio=0.217457 */
     1386u,  /* idx=932, ratio=0.138617 */
      664u,  /* idx=933, ratio=0.066398 */
        0u,  /* idx=934, ratio=0.000000 */
        0u,  /* idx=935, ratio=0.000000 */
      936u,  /* idx=936, ratio=0.093608 */
     1763u,  /* idx=937, ratio=0.176336 */
     2500u,  /* idx=938, ratio=0.249976 */
     3159u,  /* idx=939, ratio=0.315949 */
     3130u,  /* idx=940, ratio=0.312991 */
     2237u,  /* idx=941, ratio=0.223675 */
     1424u,  /* idx=942, ratio=0.142402 */
      681u,  /* idx=943, ratio=0.068133 */
        0u,  /* idx=944, ratio=0.000000 */
        0u,  /* idx=945, ratio=-0.000000 */
      914u,  /* idx=946, ratio=0.091388 */
     1724u,  /* idx=947, ratio=0.172391 */
     2447u,  /* idx=948, ratio=0.244685 */
     3096u,  /* idx=949, ratio=0.309603 */
     2303u,  /* idx=950, ratio=0.230258 */
     1464u,  /* idx=951, ratio=0.146399 */
      700u,  /* idx=952, ratio=0.069961 */
        0u,  /* idx=953, ratio=0.000000 */
        0u,  /* idx=954, ratio=-0.000000 */
      893u,  /* idx=955, ratio=0.089271 */
     1686u,  /* idx=956, ratio=0.168620 */
     2396u,  /* idx=957, ratio=0.239614 */
     2372u,  /* idx=958, ratio=0.237240 */
     1506u,  /* idx=959, ratio=0.150627 */
      719u,  /* idx=960, ratio=0.071889 */
        0u,  /* idx=961, ratio=0.000000 */
        0u,  /* idx=962, ratio=0.000000 */
      872u,  /* idx=963, ratio=0.087250 */
     1650u,  /* idx=964, ratio=0.165010 */
     2347u,  /* idx=965, ratio=0.234749 */
     1551u,  /* idx=966, ratio=0.155106 */
      739u,  /* idx=967, ratio=0.073927 */
        0u,  /* idx=968, ratio=0.000000 */
        0u,  /* idx=969, ratio=0.000000 */
      853u,  /* idx=970, ratio=0.085318 */
     1616u,  /* idx=971, ratio=0.161551 */
     1599u,  /* idx=972, ratio=0.159860 */
      761u,  /* idx=973, ratio=0.076084 */
        0u,  /* idx=974, ratio=-0.000000 */
        0u,  /* idx=975, ratio=0.000000 */
      835u,  /* idx=976, ratio=0.083470 */
     1582u,  /* idx=977, ratio=0.158235 */
      784u,  /* idx=978, ratio=0.078370 */
        0u,  /* idx=979, ratio=0.000000 */
        0u,  /* idx=980, ratio=0.000000 */
      817u,  /* idx=981, ratio=0.081701 */
      808u,  /* idx=982, ratio=0.080799 */
        0u,  /* idx=983, ratio=0.000000 */
        0u,  /* idx=984, ratio=0.000000 */
      647u,  /* idx=985, ratio=0.064666 */
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

static int32_t AlgoRatioK_ClampS32(int32_t value,
                                   int32_t minValue,
                                   int32_t maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

static int32_t AlgoRatioK_RoundShiftRightS64(int64_t value,
                                             uint8_t shift)
{
    int64_t offset;

    if (shift == 0u)
    {
        return (int32_t)value;
    }

    offset = (int64_t)1 << (shift - 1u);
    if (value >= 0)
    {
        value += offset;
    }
    else
    {
        value -= offset;
    }

    return (int32_t)(value >> shift);
}

static void AlgoRatioK_ProjectToSegmentQ1000(int32_t pointU,
                                             int32_t pointV,
                                             int32_t startU,
                                             int32_t startV,
                                             int32_t endU,
                                             int32_t endV,
                                             int32_t *projectedU,
                                             int32_t *projectedV)
{
    int32_t edgeU;
    int32_t edgeV;
    int64_t edgeLen2;
    int64_t dot;
    int64_t tQ16;

    if ((projectedU == (void *)0) || (projectedV == (void *)0))
    {
        return;
    }

    edgeU = endU - startU;
    edgeV = endV - startV;
    edgeLen2 = ((int64_t)edgeU * (int64_t)edgeU) + ((int64_t)edgeV * (int64_t)edgeV);

    if (edgeLen2 <= 0)
    {
        *projectedU = startU;
        *projectedV = startV;
        return;
    }

    dot = ((int64_t)(pointU - startU) * (int64_t)edgeU) +
          ((int64_t)(pointV - startV) * (int64_t)edgeV);
    tQ16 = ((dot << 16) + (edgeLen2 / 2)) / edgeLen2;

    if (tQ16 < 0)
    {
        tQ16 = 0;
    }
    else if (tQ16 > 65536)
    {
        tQ16 = 65536;
    }

    *projectedU = startU + AlgoRatioK_RoundShiftRightS64((int64_t)edgeU * tQ16, 16u);
    *projectedV = startV + AlgoRatioK_RoundShiftRightS64((int64_t)edgeV * tQ16, 16u);
}

static void AlgoRatioK_ProjectToNearestBoundaryQ1000(uint16_t targetU,
                                                     uint16_t targetV,
                                                     uint16_t *projectedU,
                                                     uint16_t *projectedV)
{
    const int32_t vertexU[3] = {
        (int32_t)ALGO_RATIO_K_VERTEX_K_U_Q1000,
        (int32_t)ALGO_RATIO_K_VERTEX_I_U_Q1000,
        (int32_t)ALGO_RATIO_K_VERTEX_J_U_Q1000
    };
    const int32_t vertexV[3] = {
        (int32_t)ALGO_RATIO_K_VERTEX_K_V_Q1000,
        (int32_t)ALGO_RATIO_K_VERTEX_I_V_Q1000,
        (int32_t)ALGO_RATIO_K_VERTEX_J_V_Q1000
    };
    int32_t pointU;
    int32_t pointV;
    int32_t candidateU;
    int32_t candidateV;
    int32_t bestU;
    int32_t bestV;
    uint32_t bestDistance2;
    uint8_t edgeIndex;

    if ((projectedU == (void *)0) || (projectedV == (void *)0))
    {
        return;
    }

    pointU = (int32_t)targetU;
    pointV = (int32_t)targetV;
    bestU = vertexU[0];
    bestV = vertexV[0];
    bestDistance2 = ALGO_RATIO_K_DISTANCE_MAX;

    for (edgeIndex = 0u; edgeIndex < 3u; edgeIndex++)
    {
        uint8_t nextIndex;
        uint32_t distance2;

        nextIndex = (uint8_t)((edgeIndex + 1u) % 3u);
        AlgoRatioK_ProjectToSegmentQ1000(pointU,
                                         pointV,
                                         vertexU[edgeIndex],
                                         vertexV[edgeIndex],
                                         vertexU[nextIndex],
                                         vertexV[nextIndex],
                                         &candidateU,
                                         &candidateV);
        distance2 = AlgoRatioK_CalcDistance2(targetU,
                                             targetV,
                                             (uint16_t)AlgoRatioK_ClampS32(candidateU, 0, 1023),
                                             (uint16_t)AlgoRatioK_ClampS32(candidateV, 0, 1023));
        if (distance2 < bestDistance2)
        {
            bestDistance2 = distance2;
            bestU = candidateU;
            bestV = candidateV;
        }
    }

    *projectedU = (uint16_t)AlgoRatioK_ClampS32(bestU, 0, 1023);
    *projectedV = (uint16_t)AlgoRatioK_ClampS32(bestV, 0, 1023);
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

static uint16_t AlgoRatioK_RoundFloatToU16(float value)
{
    if (value <= 0.0f)
    {
        return 0u;
    }

    if (value >= 1023.0f)
    {
        return 1023u;
    }

    return (uint16_t)(value + 0.5f);
}

static void AlgoRatioK_GetPointQ1000ByGridIJ(uint16_t gridI,
                                             uint16_t gridJ,
                                             uint16_t *pointU,
                                             uint16_t *pointV)
{
    uint16_t gridK;
    float pointUFloat;
    float pointVFloat;

    if ((pointU == (void *)0) || (pointV == (void *)0))
    {
        return;
    }

    gridK = (uint16_t)(ALGO_RATIO_K_GRID_ORDER - gridI - gridJ);
    pointUFloat = ((((float)gridK * ALGO_RATIO_K_VERTEX_K_U_FLOAT) +
                    ((float)gridI * ALGO_RATIO_K_VERTEX_I_U_FLOAT) +
                    ((float)gridJ * ALGO_RATIO_K_VERTEX_J_U_FLOAT)) * 1000.0f) /
                  (float)ALGO_RATIO_K_GRID_ORDER;
    pointVFloat = ((((float)gridK * ALGO_RATIO_K_VERTEX_K_V_FLOAT) +
                    ((float)gridI * ALGO_RATIO_K_VERTEX_I_V_FLOAT) +
                    ((float)gridJ * ALGO_RATIO_K_VERTEX_J_V_FLOAT)) * 1000.0f) /
                  (float)ALGO_RATIO_K_GRID_ORDER;

    *pointU = AlgoRatioK_RoundFloatToU16(pointUFloat);
    *pointV = AlgoRatioK_RoundFloatToU16(pointVFloat);
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
    const int32_t vertexKU = (int32_t)ALGO_RATIO_K_VERTEX_K_U_Q1000;
    const int32_t vertexKV = (int32_t)ALGO_RATIO_K_VERTEX_K_V_Q1000;
    const int32_t vertexIU = (int32_t)ALGO_RATIO_K_VERTEX_I_U_Q1000;
    const int32_t vertexIV = (int32_t)ALGO_RATIO_K_VERTEX_I_V_Q1000;
    const int32_t vertexJU = (int32_t)ALGO_RATIO_K_VERTEX_J_U_Q1000;
    const int32_t vertexJV = (int32_t)ALGO_RATIO_K_VERTEX_J_V_Q1000;
    int32_t axisIU;
    int32_t axisIV;
    int32_t axisJU;
    int32_t axisJV;
    int32_t relU;
    int32_t relV;
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

    axisIU = vertexIU - vertexKU;
    axisIV = vertexIV - vertexKV;
    axisJU = vertexJU - vertexKU;
    axisJV = vertexJV - vertexKV;
    relU = (int32_t)targetU - (int32_t)vertexKU;
    relV = (int32_t)targetV - (int32_t)vertexKV;

    denominator = AlgoRatioK_Cross2DS64(axisIU, axisIV, axisJU, axisJV);
    if (denominator == 0)
    {
        return 0u;
    }

    numeratorI = AlgoRatioK_Cross2DS64(relU, relV, axisJU, axisJV);
    numeratorJ = AlgoRatioK_Cross2DS64(axisIU, axisIV, relU, relV);

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
    uint16_t lookupTargetU;
    uint16_t lookupTargetV;

    if (whiteRatioQ10000 == (void *)0)
    {
        return 0u;
    }

    lookupTargetU = targetU;
    lookupTargetV = targetV;
    AlgoRatioK_ResetNearestPoints(nearest);

    status = AlgoRatioK_FindNearestPointsFast(lookupTargetU,
                                              lookupTargetV,
                                              nearest,
                                              &nearestCount,
                                              whiteRatioQ10000);
    if (status == 2u)
    {
        return 1u;
    }

    if (status == 0u)
    {
        AlgoRatioK_ProjectToNearestBoundaryQ1000(targetU,
                                                 targetV,
                                                 &lookupTargetU,
                                                 &lookupTargetV);
        AlgoRatioK_ResetNearestPoints(nearest);
        status = AlgoRatioK_FindNearestPointsFast(lookupTargetU,
                                                  lookupTargetV,
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
    }

    if (AlgoRatioK_LocalPlaneFitQ10000(nearest,
                                       nearestCount,
                                       lookupTargetU,
                                       lookupTargetV,
                                       whiteRatioQ10000) != 0u)
    {
        return 1u;
    }

    return AlgoRatioK_WeightedAverageQ10000(nearest,
                                            nearestCount,
                                            whiteRatioQ10000);
}
