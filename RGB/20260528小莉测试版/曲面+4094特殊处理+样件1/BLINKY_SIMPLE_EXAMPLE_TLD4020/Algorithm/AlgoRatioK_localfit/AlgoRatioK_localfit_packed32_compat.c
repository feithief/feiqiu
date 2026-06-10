/*********************************************************************************
 * FileName:    AlgoRatioK_localfit.c
 * Description: 32-bit packed white-ratio lookup module.
 *              Online algorithm changed from direct K-point ratio average to
 *              local weighted plane fit of whiteRatio = a*dU + b*dV + c.
 *              The public interface is kept unchanged.
 *
 * Packed LUT:
 *   bit[ 0: 9] = U,      10 bit, u' * 1000
 *   bit[10:19] = V,      10 bit, v' * 1000
 *   bit[20:31] = ratio,  12 bit, whiteRatio * 4095
 **********************************************************************************/

#include "AlgoRatioK_localfit.h"
#include "FloatLib.h"

#define ALGO_RATIO_K_LUT_COUNT           (990u)
#define ALGO_RATIO_K_GRID_ORDER          (43u)
#define ALGO_RATIO_K_NEIGHBOR_RING       (4u)

#define ALGO_RATIO_K_VERTEX_K_U          (201u)
#define ALGO_RATIO_K_VERTEX_K_V          (84u)
#define ALGO_RATIO_K_VERTEX_I_U          (546u)
#define ALGO_RATIO_K_VERTEX_I_V          (518u)
#define ALGO_RATIO_K_VERTEX_J_U          (49u)
#define ALGO_RATIO_K_VERTEX_J_V          (568u)

#define ALGO_RATIO_K_U_MASK              (0x000003FFu)
#define ALGO_RATIO_K_V_MASK              (0x000FFC00u)
#define ALGO_RATIO_K_RATIO_MASK          (0xFFF00000u)

#define ALGO_RATIO_K_V_SHIFT             (10u)
#define ALGO_RATIO_K_RATIO_SHIFT         (20u)

#define ALGO_RATIO_K_RATIO12_MAX         (4095u)
#define ALGO_RATIO_K_RATIO_Q10000_MAX    (10000u)
#define ALGO_RATIO_K_DISTANCE_MAX        (0xFFFFFFFFu)

#ifndef ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16
#define ALGO_RATIO_K_LOCAL_FIT_EPS_FIX16 (4)
#endif

#define ALGO_RATIO_K_WEIGHT_Q_SHIFT       (28u)
#define ALGO_RATIO_K_RATIO_NORMALIZER     (10000u)

#if (ALGO_RATIO_K_AMOUNT == 0u)
#error "ALGO_RATIO_K_AMOUNT must be greater than 0."
#endif

#define ALGO_RATIO_K_PACK(U_, V_, RATIO12_) \
    ((((uint32_t)(RATIO12_) & 0xFFFu) << ALGO_RATIO_K_RATIO_SHIFT) | \
     (((uint32_t)(V_)       & 0x3FFu) << ALGO_RATIO_K_V_SHIFT) | \
     (((uint32_t)(U_)       & 0x3FFu)))

static const uint32_t g_algoRatioKPackedLut[ALGO_RATIO_K_LUT_COUNT] =
{
    ALGO_RATIO_K_PACK( 201u,   84u,    0u),  /* idx=  0, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 188u,  123u,    0u),  /* idx=  1, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 177u,  158u,    0u),  /* idx=  2, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 168u,  189u,    0u),  /* idx=  3, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 159u,  217u,    0u),  /* idx=  4, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 151u,  242u,    0u),  /* idx=  5, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 144u,  265u,    0u),  /* idx=  6, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 137u,  286u,    0u),  /* idx=  7, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 131u,  305u,    0u),  /* idx=  8, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 125u,  323u,    0u),  /* idx=  9, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 120u,  340u,    0u),  /* idx= 10, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 116u,  355u,    0u),  /* idx= 11, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  369u,    0u),  /* idx= 12, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 107u,  382u,    0u),  /* idx= 13, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  394u,    0u),  /* idx= 14, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  405u,    0u),  /* idx= 15, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  416u,    0u),  /* idx= 16, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  426u,    0u),  /* idx= 17, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  436u,    0u),  /* idx= 18, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  445u,    0u),  /* idx= 19, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  453u,    0u),  /* idx= 20, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  82u,  461u,    0u),  /* idx= 21, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  80u,  468u,    0u),  /* idx= 22, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  476u,    0u),  /* idx= 23, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  482u,    0u),  /* idx= 24, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  489u,    0u),  /* idx= 25, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  495u,    0u),  /* idx= 26, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  501u,    0u),  /* idx= 27, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  507u,    0u),  /* idx= 28, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  512u,    0u),  /* idx= 29, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  517u,    0u),  /* idx= 30, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  522u,    0u),  /* idx= 31, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  62u,  527u,    0u),  /* idx= 32, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  531u,    0u),  /* idx= 33, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  535u,    0u),  /* idx= 34, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  57u,  540u,    0u),  /* idx= 35, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  56u,  544u,    0u),  /* idx= 36, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  55u,  547u,    0u),  /* idx= 37, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  54u,  551u,    0u),  /* idx= 38, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  53u,  555u,    0u),  /* idx= 39, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  52u,  558u,    0u),  /* idx= 40, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  51u,  561u,    0u),  /* idx= 41, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  50u,  565u,    0u),  /* idx= 42, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  49u,  568u,    0u),  /* idx= 43, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 214u,  101u,    0u),  /* idx= 44, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 201u,  138u,    0u),  /* idx= 45, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 190u,  172u,    0u),  /* idx= 46, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 179u,  202u,    0u),  /* idx= 47, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 170u,  229u,    0u),  /* idx= 48, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 162u,  253u,    0u),  /* idx= 49, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 154u,  275u,    0u),  /* idx= 50, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 147u,  296u,    0u),  /* idx= 51, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 141u,  314u,    0u),  /* idx= 52, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 135u,  332u,    0u),  /* idx= 53, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 129u,  347u,    0u),  /* idx= 54, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 124u,  362u,    0u),  /* idx= 55, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 120u,  376u,    0u),  /* idx= 56, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 115u,  389u,    0u),  /* idx= 57, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  400u,    0u),  /* idx= 58, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 107u,  412u,    0u),  /* idx= 59, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  422u,    0u),  /* idx= 60, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  432u,    0u),  /* idx= 61, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  441u,    0u),  /* idx= 62, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  450u,    0u),  /* idx= 63, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  458u,    0u),  /* idx= 64, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  466u,    0u),  /* idx= 65, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  473u,    0u),  /* idx= 66, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  480u,    0u),  /* idx= 67, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  487u,    0u),  /* idx= 68, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  493u,    0u),  /* idx= 69, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  499u,    0u),  /* idx= 70, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  505u,    0u),  /* idx= 71, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  510u,    0u),  /* idx= 72, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  515u,    0u),  /* idx= 73, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  520u,    0u),  /* idx= 74, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  525u,    0u),  /* idx= 75, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  530u,    0u),  /* idx= 76, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  534u,    0u),  /* idx= 77, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  64u,  539u,    0u),  /* idx= 78, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  62u,  543u,    0u),  /* idx= 79, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  61u,  547u,    0u),  /* idx= 80, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  550u,    0u),  /* idx= 81, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  554u,    0u),  /* idx= 82, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  57u,  557u,    0u),  /* idx= 83, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  56u,  561u,    0u),  /* idx= 84, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  55u,  564u,    0u),  /* idx= 85, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  54u,  567u,    0u),  /* idx= 86, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 227u,  117u,    0u),  /* idx= 87, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 214u,  153u, 1576u),  /* idx= 88, ratio=0.384846 */
    ALGO_RATIO_K_PACK( 202u,  185u,  987u),  /* idx= 89, ratio=0.240943 */
    ALGO_RATIO_K_PACK( 191u,  214u,  515u),  /* idx= 90, ratio=0.125782 */
    ALGO_RATIO_K_PACK( 181u,  240u,  143u),  /* idx= 91, ratio=0.034923 */
    ALGO_RATIO_K_PACK( 172u,  264u,    0u),  /* idx= 92, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 164u,  285u,    0u),  /* idx= 93, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 157u,  305u,    0u),  /* idx= 94, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 150u,  323u,    0u),  /* idx= 95, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 144u,  340u,    0u),  /* idx= 96, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 138u,  355u,    0u),  /* idx= 97, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 133u,  369u,    0u),  /* idx= 98, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 128u,  383u,    0u),  /* idx= 99, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 123u,  395u,    0u),  /* idx=100, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 119u,  407u,    0u),  /* idx=101, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 115u,  417u,    0u),  /* idx=102, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  428u,    0u),  /* idx=103, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  437u,    0u),  /* idx=104, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  446u,    0u),  /* idx=105, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  455u,    0u),  /* idx=106, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  463u,    0u),  /* idx=107, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  470u,    0u),  /* idx=108, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  478u,    0u),  /* idx=109, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  484u,    0u),  /* idx=110, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  491u,    0u),  /* idx=111, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  497u,    0u),  /* idx=112, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  503u,    0u),  /* idx=113, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  509u,    0u),  /* idx=114, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  514u,    0u),  /* idx=115, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  519u,    0u),  /* idx=116, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  524u,    0u),  /* idx=117, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  529u,    0u),  /* idx=118, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  533u,    0u),  /* idx=119, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  537u,    0u),  /* idx=120, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  542u,    0u),  /* idx=121, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  546u,    0u),  /* idx=122, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  549u,    0u),  /* idx=123, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  553u,    0u),  /* idx=124, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  557u,    0u),  /* idx=125, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  62u,  560u,    0u),  /* idx=126, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  61u,  563u,    0u),  /* idx=127, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  567u,    0u),  /* idx=128, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 240u,  133u,    0u),  /* idx=129, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 226u,  167u, 1566u),  /* idx=130, ratio=0.382449 */
    ALGO_RATIO_K_PACK( 213u,  198u, 2502u),  /* idx=131, ratio=0.610941 */
    ALGO_RATIO_K_PACK( 202u,  226u, 2011u),  /* idx=132, ratio=0.491101 */
    ALGO_RATIO_K_PACK( 192u,  251u, 1568u),  /* idx=133, ratio=0.383023 */
    ALGO_RATIO_K_PACK( 183u,  274u, 1170u),  /* idx=134, ratio=0.285642 */
    ALGO_RATIO_K_PACK( 174u,  295u,  857u),  /* idx=135, ratio=0.209236 */
    ALGO_RATIO_K_PACK( 167u,  314u,  635u),  /* idx=136, ratio=0.154993 */
    ALGO_RATIO_K_PACK( 159u,  332u,  478u),  /* idx=137, ratio=0.116610 */
    ALGO_RATIO_K_PACK( 153u,  348u,  364u),  /* idx=138, ratio=0.088841 */
    ALGO_RATIO_K_PACK( 147u,  363u,  278u),  /* idx=139, ratio=0.067861 */
    ALGO_RATIO_K_PACK( 141u,  377u,  170u),  /* idx=140, ratio=0.041532 */
    ALGO_RATIO_K_PACK( 136u,  390u,   83u),  /* idx=141, ratio=0.020381 */
    ALGO_RATIO_K_PACK( 131u,  402u,   10u),  /* idx=142, ratio=0.002507 */
    ALGO_RATIO_K_PACK( 127u,  413u,    0u),  /* idx=143, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 123u,  423u,    0u),  /* idx=144, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 119u,  433u,    0u),  /* idx=145, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 115u,  443u,    0u),  /* idx=146, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  451u,    0u),  /* idx=147, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  460u,    0u),  /* idx=148, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  467u,    0u),  /* idx=149, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  475u,    0u),  /* idx=150, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  482u,    0u),  /* idx=151, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  489u,    0u),  /* idx=152, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  495u,    0u),  /* idx=153, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  501u,    0u),  /* idx=154, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  507u,    0u),  /* idx=155, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  512u,    0u),  /* idx=156, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  518u,    0u),  /* idx=157, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  523u,    0u),  /* idx=158, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  527u,    0u),  /* idx=159, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  532u,    0u),  /* idx=160, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  536u,    0u),  /* idx=161, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  541u,    0u),  /* idx=162, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  545u,    0u),  /* idx=163, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  549u,    0u),  /* idx=164, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  552u,    0u),  /* idx=165, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  556u,    0u),  /* idx=166, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  559u,    0u),  /* idx=167, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  563u,    0u),  /* idx=168, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  566u,    0u),  /* idx=169, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 252u,  148u,    0u),  /* idx=170, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 237u,  181u, 1425u),  /* idx=171, ratio=0.347866 */
    ALGO_RATIO_K_PACK( 224u,  211u, 2318u),  /* idx=172, ratio=0.565999 */
    ALGO_RATIO_K_PACK( 213u,  238u, 2930u),  /* idx=173, ratio=0.715567 */
    ALGO_RATIO_K_PACK( 202u,  262u, 2682u),  /* idx=174, ratio=0.654984 */
    ALGO_RATIO_K_PACK( 193u,  284u, 2273u),  /* idx=175, ratio=0.555067 */
    ALGO_RATIO_K_PACK( 184u,  305u, 1896u),  /* idx=176, ratio=0.462912 */
    ALGO_RATIO_K_PACK( 176u,  323u, 1562u),  /* idx=177, ratio=0.381515 */
    ALGO_RATIO_K_PACK( 169u,  340u, 1309u),  /* idx=178, ratio=0.319680 */
    ALGO_RATIO_K_PACK( 162u,  356u, 1121u),  /* idx=179, ratio=0.273729 */
    ALGO_RATIO_K_PACK( 156u,  370u,  980u),  /* idx=180, ratio=0.239199 */
    ALGO_RATIO_K_PACK( 150u,  384u,  832u),  /* idx=181, ratio=0.203103 */
    ALGO_RATIO_K_PACK( 144u,  396u,  711u),  /* idx=182, ratio=0.173688 */
    ALGO_RATIO_K_PACK( 139u,  408u,  606u),  /* idx=183, ratio=0.147874 */
    ALGO_RATIO_K_PACK( 134u,  419u,  509u),  /* idx=184, ratio=0.124270 */
    ALGO_RATIO_K_PACK( 130u,  429u,  420u),  /* idx=185, ratio=0.102469 */
    ALGO_RATIO_K_PACK( 126u,  439u,  324u),  /* idx=186, ratio=0.079165 */
    ALGO_RATIO_K_PACK( 122u,  448u,  191u),  /* idx=187, ratio=0.046758 */
    ALGO_RATIO_K_PACK( 118u,  456u,   69u),  /* idx=188, ratio=0.016940 */
    ALGO_RATIO_K_PACK( 115u,  465u,    0u),  /* idx=189, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  472u,    0u),  /* idx=190, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  479u,    0u),  /* idx=191, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  486u,    0u),  /* idx=192, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  493u,    0u),  /* idx=193, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  499u,    0u),  /* idx=194, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  505u,    0u),  /* idx=195, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  511u,    0u),  /* idx=196, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  516u,    0u),  /* idx=197, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  521u,    0u),  /* idx=198, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  526u,    0u),  /* idx=199, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  531u,    0u),  /* idx=200, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  535u,    0u),  /* idx=201, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  540u,    0u),  /* idx=202, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  544u,    0u),  /* idx=203, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  548u,    0u),  /* idx=204, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  552u,    0u),  /* idx=205, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  555u,    0u),  /* idx=206, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  559u,    0u),  /* idx=207, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  562u,    0u),  /* idx=208, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  566u,    0u),  /* idx=209, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 264u,  163u,    0u),  /* idx=210, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 249u,  195u, 1306u),  /* idx=211, ratio=0.319020 */
    ALGO_RATIO_K_PACK( 235u,  224u, 2159u),  /* idx=212, ratio=0.527216 */
    ALGO_RATIO_K_PACK( 223u,  250u, 2759u),  /* idx=213, ratio=0.673791 */
    ALGO_RATIO_K_PACK( 212u,  273u, 3205u),  /* idx=214, ratio=0.782576 */
    ALGO_RATIO_K_PACK( 203u,  294u, 3145u),  /* idx=215, ratio=0.768098 */
    ALGO_RATIO_K_PACK( 194u,  314u, 2766u),  /* idx=216, ratio=0.675531 */
    ALGO_RATIO_K_PACK( 185u,  332u, 2432u),  /* idx=217, ratio=0.594016 */
    ALGO_RATIO_K_PACK( 178u,  348u, 2101u),  /* idx=218, ratio=0.513074 */
    ALGO_RATIO_K_PACK( 170u,  363u, 1839u),  /* idx=219, ratio=0.448995 */
    ALGO_RATIO_K_PACK( 164u,  378u, 1640u),  /* idx=220, ratio=0.400505 */
    ALGO_RATIO_K_PACK( 158u,  391u, 1457u),  /* idx=221, ratio=0.355919 */
    ALGO_RATIO_K_PACK( 152u,  403u, 1310u),  /* idx=222, ratio=0.319978 */
    ALGO_RATIO_K_PACK( 147u,  414u, 1174u),  /* idx=223, ratio=0.286744 */
    ALGO_RATIO_K_PACK( 142u,  425u, 1045u),  /* idx=224, ratio=0.255237 */
    ALGO_RATIO_K_PACK( 137u,  435u,  905u),  /* idx=225, ratio=0.220972 */
    ALGO_RATIO_K_PACK( 133u,  444u,  740u),  /* idx=226, ratio=0.180746 */
    ALGO_RATIO_K_PACK( 129u,  453u,  587u),  /* idx=227, ratio=0.143401 */
    ALGO_RATIO_K_PACK( 125u,  461u,  450u),  /* idx=228, ratio=0.109790 */
    ALGO_RATIO_K_PACK( 121u,  469u,  323u),  /* idx=229, ratio=0.078777 */
    ALGO_RATIO_K_PACK( 118u,  477u,  207u),  /* idx=230, ratio=0.050551 */
    ALGO_RATIO_K_PACK( 115u,  484u,  100u),  /* idx=231, ratio=0.024492 */
    ALGO_RATIO_K_PACK( 111u,  491u,    0u),  /* idx=232, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  497u,    0u),  /* idx=233, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 106u,  503u,    0u),  /* idx=234, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  509u,    0u),  /* idx=235, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  514u,    0u),  /* idx=236, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  520u,    0u),  /* idx=237, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  525u,    0u),  /* idx=238, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  529u,    0u),  /* idx=239, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  534u,    0u),  /* idx=240, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  538u,    0u),  /* idx=241, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  543u,    0u),  /* idx=242, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  547u,    0u),  /* idx=243, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  551u,    0u),  /* idx=244, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  554u,    0u),  /* idx=245, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  558u,    0u),  /* idx=246, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  562u,    0u),  /* idx=247, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  565u,    0u),  /* idx=248, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 275u,  177u,    0u),  /* idx=249, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 260u,  208u, 1206u),  /* idx=250, ratio=0.294591 */
    ALGO_RATIO_K_PACK( 246u,  236u, 2021u),  /* idx=251, ratio=0.493407 */
    ALGO_RATIO_K_PACK( 234u,  261u, 2607u),  /* idx=252, ratio=0.636625 */
    ALGO_RATIO_K_PACK( 222u,  284u, 3050u),  /* idx=253, ratio=0.744704 */
    ALGO_RATIO_K_PACK( 212u,  304u, 3395u),  /* idx=254, ratio=0.829164 */
    ALGO_RATIO_K_PACK( 203u,  323u, 3482u),  /* idx=255, ratio=0.850188 */
    ALGO_RATIO_K_PACK( 194u,  340u, 3120u),  /* idx=256, ratio=0.762000 */
    ALGO_RATIO_K_PACK( 186u,  356u, 2830u),  /* idx=257, ratio=0.691046 */
    ALGO_RATIO_K_PACK( 179u,  371u, 2520u),  /* idx=258, ratio=0.615425 */
    ALGO_RATIO_K_PACK( 172u,  385u, 2265u),  /* idx=259, ratio=0.553172 */
    ALGO_RATIO_K_PACK( 166u,  397u, 2048u),  /* idx=260, ratio=0.500221 */
    ALGO_RATIO_K_PACK( 160u,  409u, 1875u),  /* idx=261, ratio=0.457960 */
    ALGO_RATIO_K_PACK( 154u,  420u, 1705u),  /* idx=262, ratio=0.416247 */
    ALGO_RATIO_K_PACK( 149u,  431u, 1515u),  /* idx=263, ratio=0.370047 */
    ALGO_RATIO_K_PACK( 144u,  441u, 1315u),  /* idx=264, ratio=0.321129 */
    ALGO_RATIO_K_PACK( 140u,  450u, 1131u),  /* idx=265, ratio=0.276200 */
    ALGO_RATIO_K_PACK( 136u,  458u,  963u),  /* idx=266, ratio=0.235174 */
    ALGO_RATIO_K_PACK( 132u,  466u,  809u),  /* idx=267, ratio=0.197666 */
    ALGO_RATIO_K_PACK( 128u,  474u,  669u),  /* idx=268, ratio=0.163379 */
    ALGO_RATIO_K_PACK( 124u,  481u,  539u),  /* idx=269, ratio=0.131621 */
    ALGO_RATIO_K_PACK( 121u,  488u,  420u),  /* idx=270, ratio=0.102674 */
    ALGO_RATIO_K_PACK( 118u,  495u,  307u),  /* idx=271, ratio=0.074944 */
    ALGO_RATIO_K_PACK( 114u,  501u,  200u),  /* idx=272, ratio=0.048752 */
    ALGO_RATIO_K_PACK( 111u,  507u,   98u),  /* idx=273, ratio=0.023979 */
    ALGO_RATIO_K_PACK( 109u,  513u,    0u),  /* idx=274, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 106u,  518u,    0u),  /* idx=275, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  523u,    0u),  /* idx=276, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  528u,    0u),  /* idx=277, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  533u,    0u),  /* idx=278, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  537u,    0u),  /* idx=279, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  542u,    0u),  /* idx=280, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  546u,    0u),  /* idx=281, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  550u,    0u),  /* idx=282, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  554u,    0u),  /* idx=283, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  557u,    0u),  /* idx=284, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  561u,    0u),  /* idx=285, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  564u,    0u),  /* idx=286, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 286u,  191u,    0u),  /* idx=287, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 271u,  221u, 1121u),  /* idx=288, ratio=0.273637 */
    ALGO_RATIO_K_PACK( 256u,  248u, 1899u),  /* idx=289, ratio=0.463673 */
    ALGO_RATIO_K_PACK( 244u,  272u, 2471u),  /* idx=290, ratio=0.603344 */
    ALGO_RATIO_K_PACK( 232u,  294u, 2909u),  /* idx=291, ratio=0.710329 */
    ALGO_RATIO_K_PACK( 222u,  314u, 3255u),  /* idx=292, ratio=0.794899 */
    ALGO_RATIO_K_PACK( 212u,  332u, 3536u),  /* idx=293, ratio=0.863432 */
    ALGO_RATIO_K_PACK( 203u,  349u, 3746u),  /* idx=294, ratio=0.914789 */
    ALGO_RATIO_K_PACK( 195u,  364u, 3403u),  /* idx=295, ratio=0.831121 */
    ALGO_RATIO_K_PACK( 187u,  378u, 3117u),  /* idx=296, ratio=0.761285 */
    ALGO_RATIO_K_PACK( 180u,  392u, 2850u),  /* idx=297, ratio=0.695984 */
    ALGO_RATIO_K_PACK( 174u,  404u, 2597u),  /* idx=298, ratio=0.634286 */
    ALGO_RATIO_K_PACK( 168u,  416u, 2390u),  /* idx=299, ratio=0.583653 */
    ALGO_RATIO_K_PACK( 162u,  426u, 2152u),  /* idx=300, ratio=0.525597 */
    ALGO_RATIO_K_PACK( 157u,  437u, 1915u),  /* idx=301, ratio=0.467659 */
    ALGO_RATIO_K_PACK( 152u,  446u, 1699u),  /* idx=302, ratio=0.414820 */
    ALGO_RATIO_K_PACK( 147u,  455u, 1499u),  /* idx=303, ratio=0.366001 */
    ALGO_RATIO_K_PACK( 142u,  463u, 1317u),  /* idx=304, ratio=0.321551 */
    ALGO_RATIO_K_PACK( 138u,  471u, 1149u),  /* idx=305, ratio=0.280669 */
    ALGO_RATIO_K_PACK( 134u,  479u,  997u),  /* idx=306, ratio=0.243541 */
    ALGO_RATIO_K_PACK( 130u,  486u,  856u),  /* idx=307, ratio=0.209018 */
    ALGO_RATIO_K_PACK( 127u,  493u,  722u),  /* idx=308, ratio=0.176398 */
    ALGO_RATIO_K_PACK( 124u,  499u,  598u),  /* idx=309, ratio=0.146009 */
    ALGO_RATIO_K_PACK( 120u,  505u,  480u),  /* idx=310, ratio=0.117206 */
    ALGO_RATIO_K_PACK( 117u,  511u,  366u),  /* idx=311, ratio=0.089308 */
    ALGO_RATIO_K_PACK( 114u,  516u,  257u),  /* idx=312, ratio=0.062743 */
    ALGO_RATIO_K_PACK( 112u,  522u,  151u),  /* idx=313, ratio=0.036836 */
    ALGO_RATIO_K_PACK( 109u,  527u,   50u),  /* idx=314, ratio=0.012099 */
    ALGO_RATIO_K_PACK( 106u,  532u,    0u),  /* idx=315, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  536u,    0u),  /* idx=316, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  541u,    0u),  /* idx=317, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  545u,    0u),  /* idx=318, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  549u,    0u),  /* idx=319, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  553u,    0u),  /* idx=320, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  557u,    0u),  /* idx=321, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  560u,    0u),  /* idx=322, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  564u,    0u),  /* idx=323, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 297u,  205u,    0u),  /* idx=324, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 281u,  234u, 1046u),  /* idx=325, ratio=0.255466 */
    ALGO_RATIO_K_PACK( 267u,  259u, 1791u),  /* idx=326, ratio=0.437319 */
    ALGO_RATIO_K_PACK( 254u,  283u, 2348u),  /* idx=327, ratio=0.573370 */
    ALGO_RATIO_K_PACK( 242u,  304u, 2780u),  /* idx=328, ratio=0.678986 */
    ALGO_RATIO_K_PACK( 231u,  323u, 3126u),  /* idx=329, ratio=0.763354 */
    ALGO_RATIO_K_PACK( 221u,  341u, 3408u),  /* idx=330, ratio=0.832299 */
    ALGO_RATIO_K_PACK( 212u,  357u, 3643u),  /* idx=331, ratio=0.889696 */
    ALGO_RATIO_K_PACK( 203u,  372u, 3838u),  /* idx=332, ratio=0.937210 */
    ALGO_RATIO_K_PACK( 195u,  386u, 3563u),  /* idx=333, ratio=0.869982 */
    ALGO_RATIO_K_PACK( 188u,  399u, 3324u),  /* idx=334, ratio=0.811753 */
    ALGO_RATIO_K_PACK( 181u,  411u, 3116u),  /* idx=335, ratio=0.760830 */
    ALGO_RATIO_K_PACK( 175u,  422u, 2807u),  /* idx=336, ratio=0.685387 */
    ALGO_RATIO_K_PACK( 169u,  432u, 2537u),  /* idx=337, ratio=0.619656 */
    ALGO_RATIO_K_PACK( 164u,  442u, 2286u),  /* idx=338, ratio=0.558281 */
    ALGO_RATIO_K_PACK( 159u,  451u, 2055u),  /* idx=339, ratio=0.501840 */
    ALGO_RATIO_K_PACK( 154u,  460u, 1843u),  /* idx=340, ratio=0.450095 */
    ALGO_RATIO_K_PACK( 149u,  468u, 1649u),  /* idx=341, ratio=0.402595 */
    ALGO_RATIO_K_PACK( 145u,  476u, 1470u),  /* idx=342, ratio=0.359014 */
    ALGO_RATIO_K_PACK( 141u,  483u, 1307u),  /* idx=343, ratio=0.319129 */
    ALGO_RATIO_K_PACK( 137u,  490u, 1154u),  /* idx=344, ratio=0.281801 */
    ALGO_RATIO_K_PACK( 133u,  497u, 1011u),  /* idx=345, ratio=0.246834 */
    ALGO_RATIO_K_PACK( 129u,  503u,  874u),  /* idx=346, ratio=0.213544 */
    ALGO_RATIO_K_PACK( 126u,  509u,  744u),  /* idx=347, ratio=0.181767 */
    ALGO_RATIO_K_PACK( 123u,  515u,  620u),  /* idx=348, ratio=0.151375 */
    ALGO_RATIO_K_PACK( 120u,  520u,  501u),  /* idx=349, ratio=0.122271 */
    ALGO_RATIO_K_PACK( 117u,  525u,  384u),  /* idx=350, ratio=0.093730 */
    ALGO_RATIO_K_PACK( 114u,  530u,  274u),  /* idx=351, ratio=0.066991 */
    ALGO_RATIO_K_PACK( 112u,  535u,  170u),  /* idx=352, ratio=0.041431 */
    ALGO_RATIO_K_PACK( 109u,  540u,   69u),  /* idx=353, ratio=0.016832 */
    ALGO_RATIO_K_PACK( 107u,  544u,    0u),  /* idx=354, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  548u,    0u),  /* idx=355, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  552u,    0u),  /* idx=356, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  556u,    0u),  /* idx=357, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  559u,    0u),  /* idx=358, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  563u,    0u),  /* idx=359, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 308u,  218u,    0u),  /* idx=360, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 291u,  246u,  981u),  /* idx=361, ratio=0.239558 */
    ALGO_RATIO_K_PACK( 276u,  271u, 1695u),  /* idx=362, ratio=0.413800 */
    ALGO_RATIO_K_PACK( 263u,  293u, 2237u),  /* idx=363, ratio=0.546233 */
    ALGO_RATIO_K_PACK( 251u,  313u, 2663u),  /* idx=364, ratio=0.650293 */
    ALGO_RATIO_K_PACK( 240u,  332u, 3007u),  /* idx=365, ratio=0.734217 */
    ALGO_RATIO_K_PACK( 230u,  349u, 3290u),  /* idx=366, ratio=0.803333 */
    ALGO_RATIO_K_PACK( 220u,  365u, 3527u),  /* idx=367, ratio=0.861243 */
    ALGO_RATIO_K_PACK( 212u,  379u, 3728u),  /* idx=368, ratio=0.910467 */
    ALGO_RATIO_K_PACK( 203u,  393u, 3898u),  /* idx=369, ratio=0.951796 */
    ALGO_RATIO_K_PACK( 196u,  405u, 3643u),  /* idx=370, ratio=0.889730 */
    ALGO_RATIO_K_PACK( 189u,  417u, 3420u),  /* idx=371, ratio=0.835263 */
    ALGO_RATIO_K_PACK( 183u,  428u, 3184u),  /* idx=372, ratio=0.777584 */
    ALGO_RATIO_K_PACK( 176u,  438u, 2887u),  /* idx=373, ratio=0.705003 */
    ALGO_RATIO_K_PACK( 171u,  448u, 2626u),  /* idx=374, ratio=0.641368 */
    ALGO_RATIO_K_PACK( 165u,  457u, 2386u),  /* idx=375, ratio=0.582732 */
    ALGO_RATIO_K_PACK( 160u,  465u, 2164u),  /* idx=376, ratio=0.528483 */
    ALGO_RATIO_K_PACK( 156u,  473u, 1960u),  /* idx=377, ratio=0.478590 */
    ALGO_RATIO_K_PACK( 151u,  481u, 1772u),  /* idx=378, ratio=0.432809 */
    ALGO_RATIO_K_PACK( 147u,  488u, 1597u),  /* idx=379, ratio=0.390056 */
    ALGO_RATIO_K_PACK( 143u,  495u, 1434u),  /* idx=380, ratio=0.350139 */
    ALGO_RATIO_K_PACK( 139u,  501u, 1279u),  /* idx=381, ratio=0.312408 */
    ALGO_RATIO_K_PACK( 135u,  507u, 1133u),  /* idx=382, ratio=0.276724 */
    ALGO_RATIO_K_PACK( 132u,  513u,  993u),  /* idx=383, ratio=0.242404 */
    ALGO_RATIO_K_PACK( 129u,  519u,  860u),  /* idx=384, ratio=0.209940 */
    ALGO_RATIO_K_PACK( 125u,  524u,  729u),  /* idx=385, ratio=0.177997 */
    ALGO_RATIO_K_PACK( 122u,  529u,  608u),  /* idx=386, ratio=0.148419 */
    ALGO_RATIO_K_PACK( 119u,  534u,  491u),  /* idx=387, ratio=0.119830 */
    ALGO_RATIO_K_PACK( 117u,  538u,  379u),  /* idx=388, ratio=0.092532 */
    ALGO_RATIO_K_PACK( 114u,  543u,  275u),  /* idx=389, ratio=0.067232 */
    ALGO_RATIO_K_PACK( 112u,  547u,  181u),  /* idx=390, ratio=0.044081 */
    ALGO_RATIO_K_PACK( 109u,  551u,   94u),  /* idx=391, ratio=0.022840 */
    ALGO_RATIO_K_PACK( 107u,  555u,   19u),  /* idx=392, ratio=0.004534 */
    ALGO_RATIO_K_PACK( 105u,  559u,    0u),  /* idx=393, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  562u,    0u),  /* idx=394, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 318u,  231u,    0u),  /* idx=395, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 301u,  258u,  923u),  /* idx=396, ratio=0.225516 */
    ALGO_RATIO_K_PACK( 286u,  281u, 1608u),  /* idx=397, ratio=0.392681 */
    ALGO_RATIO_K_PACK( 272u,  303u, 2136u),  /* idx=398, ratio=0.521549 */
    ALGO_RATIO_K_PACK( 260u,  323u, 2555u),  /* idx=399, ratio=0.623927 */
    ALGO_RATIO_K_PACK( 249u,  341u, 2896u),  /* idx=400, ratio=0.707222 */
    ALGO_RATIO_K_PACK( 238u,  357u, 3179u),  /* idx=401, ratio=0.776315 */
    ALGO_RATIO_K_PACK( 229u,  373u, 3417u),  /* idx=402, ratio=0.834553 */
    ALGO_RATIO_K_PACK( 220u,  387u, 3621u),  /* idx=403, ratio=0.884307 */
    ALGO_RATIO_K_PACK( 211u,  400u, 3797u),  /* idx=404, ratio=0.927305 */
    ALGO_RATIO_K_PACK( 204u,  412u, 3947u),  /* idx=405, ratio=0.963796 */
    ALGO_RATIO_K_PACK( 197u,  423u, 3711u),  /* idx=406, ratio=0.906186 */
    ALGO_RATIO_K_PACK( 190u,  434u, 3502u),  /* idx=407, ratio=0.855075 */
    ALGO_RATIO_K_PACK( 184u,  444u, 3198u),  /* idx=408, ratio=0.781043 */
    ALGO_RATIO_K_PACK( 178u,  453u, 2937u),  /* idx=409, ratio=0.717103 */
    ALGO_RATIO_K_PACK( 172u,  462u, 2688u),  /* idx=410, ratio=0.656475 */
    ALGO_RATIO_K_PACK( 167u,  470u, 2460u),  /* idx=411, ratio=0.600791 */
    ALGO_RATIO_K_PACK( 162u,  478u, 2248u),  /* idx=412, ratio=0.549023 */
    ALGO_RATIO_K_PACK( 157u,  486u, 2051u),  /* idx=413, ratio=0.500929 */
    ALGO_RATIO_K_PACK( 153u,  492u, 1869u),  /* idx=414, ratio=0.456394 */
    ALGO_RATIO_K_PACK( 149u,  499u, 1697u),  /* idx=415, ratio=0.414417 */
    ALGO_RATIO_K_PACK( 145u,  505u, 1533u),  /* idx=416, ratio=0.374345 */
    ALGO_RATIO_K_PACK( 141u,  511u, 1376u),  /* idx=417, ratio=0.336026 */
    ALGO_RATIO_K_PACK( 137u,  517u, 1226u),  /* idx=418, ratio=0.299366 */
    ALGO_RATIO_K_PACK( 134u,  522u, 1084u),  /* idx=419, ratio=0.264762 */
    ALGO_RATIO_K_PACK( 131u,  528u,  948u),  /* idx=420, ratio=0.231513 */
    ALGO_RATIO_K_PACK( 128u,  533u,  819u),  /* idx=421, ratio=0.200003 */
    ALGO_RATIO_K_PACK( 125u,  537u,  698u),  /* idx=422, ratio=0.170568 */
    ALGO_RATIO_K_PACK( 122u,  542u,  584u),  /* idx=423, ratio=0.142554 */
    ALGO_RATIO_K_PACK( 119u,  546u,  478u),  /* idx=424, ratio=0.116711 */
    ALGO_RATIO_K_PACK( 117u,  550u,  381u),  /* idx=425, ratio=0.093099 */
    ALGO_RATIO_K_PACK( 114u,  554u,  296u),  /* idx=426, ratio=0.072215 */
    ALGO_RATIO_K_PACK( 112u,  558u,  172u),  /* idx=427, ratio=0.042073 */
    ALGO_RATIO_K_PACK( 109u,  562u,    0u),  /* idx=428, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 328u,  244u,    0u),  /* idx=429, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 311u,  269u,  872u),  /* idx=430, ratio=0.213028 */
    ALGO_RATIO_K_PACK( 295u,  292u, 1530u),  /* idx=431, ratio=0.373613 */
    ALGO_RATIO_K_PACK( 282u,  313u, 2043u),  /* idx=432, ratio=0.498999 */
    ALGO_RATIO_K_PACK( 269u,  332u, 2455u),  /* idx=433, ratio=0.599616 */
    ALGO_RATIO_K_PACK( 257u,  350u, 2793u),  /* idx=434, ratio=0.682142 */
    ALGO_RATIO_K_PACK( 247u,  366u, 3076u),  /* idx=435, ratio=0.751056 */
    ALGO_RATIO_K_PACK( 237u,  380u, 3315u),  /* idx=436, ratio=0.809467 */
    ALGO_RATIO_K_PACK( 228u,  394u, 3520u),  /* idx=437, ratio=0.859608 */
    ALGO_RATIO_K_PACK( 219u,  407u, 3698u),  /* idx=438, ratio=0.903118 */
    ALGO_RATIO_K_PACK( 211u,  419u, 3854u),  /* idx=439, ratio=0.941231 */
    ALGO_RATIO_K_PACK( 204u,  430u, 3988u),  /* idx=440, ratio=0.973841 */
    ALGO_RATIO_K_PACK( 197u,  440u, 3768u),  /* idx=441, ratio=0.920110 */
    ALGO_RATIO_K_PACK( 191u,  450u, 3501u),  /* idx=442, ratio=0.855058 */
    ALGO_RATIO_K_PACK( 184u,  459u, 3212u),  /* idx=443, ratio=0.784299 */
    ALGO_RATIO_K_PACK( 179u,  467u, 2964u),  /* idx=444, ratio=0.723705 */
    ALGO_RATIO_K_PACK( 173u,  475u, 2731u),  /* idx=445, ratio=0.666798 */
    ALGO_RATIO_K_PACK( 168u,  483u, 2514u),  /* idx=446, ratio=0.613877 */
    ALGO_RATIO_K_PACK( 164u,  490u, 2311u),  /* idx=447, ratio=0.564467 */
    ALGO_RATIO_K_PACK( 159u,  497u, 2120u),  /* idx=448, ratio=0.517648 */
    ALGO_RATIO_K_PACK( 155u,  503u, 1940u),  /* idx=449, ratio=0.473805 */
    ALGO_RATIO_K_PACK( 151u,  509u, 1767u),  /* idx=450, ratio=0.431424 */
    ALGO_RATIO_K_PACK( 147u,  515u, 1604u),  /* idx=451, ratio=0.391718 */
    ALGO_RATIO_K_PACK( 143u,  521u, 1447u),  /* idx=452, ratio=0.353366 */
    ALGO_RATIO_K_PACK( 140u,  526u, 1298u),  /* idx=453, ratio=0.317071 */
    ALGO_RATIO_K_PACK( 136u,  531u, 1158u),  /* idx=454, ratio=0.282744 */
    ALGO_RATIO_K_PACK( 133u,  536u, 1024u),  /* idx=455, ratio=0.250102 */
    ALGO_RATIO_K_PACK( 130u,  541u,  899u),  /* idx=456, ratio=0.219529 */
    ALGO_RATIO_K_PACK( 127u,  545u,  762u),  /* idx=457, ratio=0.186072 */
    ALGO_RATIO_K_PACK( 124u,  549u,  555u),  /* idx=458, ratio=0.135581 */
    ALGO_RATIO_K_PACK( 121u,  553u,  360u),  /* idx=459, ratio=0.087885 */
    ALGO_RATIO_K_PACK( 119u,  557u,  175u),  /* idx=460, ratio=0.042759 */
    ALGO_RATIO_K_PACK( 116u,  561u,    0u),  /* idx=461, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 337u,  256u,    0u),  /* idx=462, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 320u,  280u,  827u),  /* idx=463, ratio=0.201851 */
    ALGO_RATIO_K_PACK( 305u,  303u, 1459u),  /* idx=464, ratio=0.356312 */
    ALGO_RATIO_K_PACK( 290u,  323u, 1959u),  /* idx=465, ratio=0.478319 */
    ALGO_RATIO_K_PACK( 277u,  341u, 2363u),  /* idx=466, ratio=0.577127 */
    ALGO_RATIO_K_PACK( 266u,  358u, 2698u),  /* idx=467, ratio=0.658780 */
    ALGO_RATIO_K_PACK( 255u,  374u, 2979u),  /* idx=468, ratio=0.727388 */
    ALGO_RATIO_K_PACK( 245u,  388u, 3218u),  /* idx=469, ratio=0.785846 */
    ALGO_RATIO_K_PACK( 235u,  401u, 3424u),  /* idx=470, ratio=0.836251 */
    ALGO_RATIO_K_PACK( 227u,  414u, 3604u),  /* idx=471, ratio=0.880160 */
    ALGO_RATIO_K_PACK( 219u,  425u, 3762u),  /* idx=472, ratio=0.918753 */
    ALGO_RATIO_K_PACK( 211u,  436u, 3902u),  /* idx=473, ratio=0.952940 */
    ALGO_RATIO_K_PACK( 204u,  446u, 4023u),  /* idx=474, ratio=0.982373 */
    ALGO_RATIO_K_PACK( 197u,  455u, 3762u),  /* idx=475, ratio=0.918771 */
    ALGO_RATIO_K_PACK( 191u,  464u, 3451u),  /* idx=476, ratio=0.842660 */
    ALGO_RATIO_K_PACK( 185u,  472u, 3208u),  /* idx=477, ratio=0.783317 */
    ALGO_RATIO_K_PACK( 180u,  480u, 2974u),  /* idx=478, ratio=0.726315 */
    ALGO_RATIO_K_PACK( 175u,  488u, 2755u),  /* idx=479, ratio=0.672854 */
    ALGO_RATIO_K_PACK( 170u,  495u, 2548u),  /* idx=480, ratio=0.622163 */
    ALGO_RATIO_K_PACK( 165u,  501u, 2351u),  /* idx=481, ratio=0.574200 */
    ALGO_RATIO_K_PACK( 161u,  508u, 2166u),  /* idx=482, ratio=0.528846 */
    ALGO_RATIO_K_PACK( 156u,  514u, 1987u),  /* idx=483, ratio=0.485271 */
    ALGO_RATIO_K_PACK( 152u,  519u, 1817u),  /* idx=484, ratio=0.443672 */
    ALGO_RATIO_K_PACK( 149u,  525u, 1655u),  /* idx=485, ratio=0.404114 */
    ALGO_RATIO_K_PACK( 145u,  530u, 1491u),  /* idx=486, ratio=0.364095 */
    ALGO_RATIO_K_PACK( 141u,  535u, 1237u),  /* idx=487, ratio=0.302180 */
    ALGO_RATIO_K_PACK( 138u,  539u,  999u),  /* idx=488, ratio=0.244073 */
    ALGO_RATIO_K_PACK( 135u,  544u,  776u),  /* idx=489, ratio=0.189433 */
    ALGO_RATIO_K_PACK( 132u,  548u,  565u),  /* idx=490, ratio=0.137959 */
    ALGO_RATIO_K_PACK( 129u,  552u,  366u),  /* idx=491, ratio=0.089383 */
    ALGO_RATIO_K_PACK( 126u,  556u,  178u),  /* idx=492, ratio=0.043468 */
    ALGO_RATIO_K_PACK( 124u,  560u,    0u),  /* idx=493, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 347u,  268u,    0u),  /* idx=494, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 329u,  291u,  785u),  /* idx=495, ratio=0.191788 */
    ALGO_RATIO_K_PACK( 314u,  313u, 1395u),  /* idx=496, ratio=0.340542 */
    ALGO_RATIO_K_PACK( 299u,  332u, 1881u),  /* idx=497, ratio=0.459284 */
    ALGO_RATIO_K_PACK( 286u,  350u, 2278u),  /* idx=498, ratio=0.556265 */
    ALGO_RATIO_K_PACK( 274u,  366u, 2608u),  /* idx=499, ratio=0.636965 */
    ALGO_RATIO_K_PACK( 263u,  381u, 2888u),  /* idx=500, ratio=0.705167 */
    ALGO_RATIO_K_PACK( 252u,  395u, 3127u),  /* idx=501, ratio=0.763564 */
    ALGO_RATIO_K_PACK( 243u,  408u, 3334u),  /* idx=502, ratio=0.814130 */
    ALGO_RATIO_K_PACK( 234u,  420u, 3515u),  /* idx=503, ratio=0.858340 */
    ALGO_RATIO_K_PACK( 226u,  431u, 3675u),  /* idx=504, ratio=0.897323 */
    ALGO_RATIO_K_PACK( 218u,  442u, 3816u),  /* idx=505, ratio=0.931953 */
    ALGO_RATIO_K_PACK( 211u,  452u, 3943u),  /* idx=506, ratio=0.962922 */
    ALGO_RATIO_K_PACK( 204u,  461u, 3968u),  /* idx=507, ratio=0.969083 */
    ALGO_RATIO_K_PACK( 198u,  469u, 3666u),  /* idx=508, ratio=0.895232 */
    ALGO_RATIO_K_PACK( 192u,  477u, 3421u),  /* idx=509, ratio=0.835475 */
    ALGO_RATIO_K_PACK( 186u,  485u, 3192u),  /* idx=510, ratio=0.779404 */
    ALGO_RATIO_K_PACK( 181u,  492u, 2972u),  /* idx=511, ratio=0.725778 */
    ALGO_RATIO_K_PACK( 176u,  499u, 2763u),  /* idx=512, ratio=0.674813 */
    ALGO_RATIO_K_PACK( 171u,  506u, 2564u),  /* idx=513, ratio=0.626217 */
    ALGO_RATIO_K_PACK( 166u,  512u, 2373u),  /* idx=514, ratio=0.579436 */
    ALGO_RATIO_K_PACK( 162u,  518u, 2096u),  /* idx=515, ratio=0.511818 */
    ALGO_RATIO_K_PACK( 158u,  523u, 1799u),  /* idx=516, ratio=0.439214 */
    ALGO_RATIO_K_PACK( 154u,  528u, 1521u),  /* idx=517, ratio=0.371465 */
    ALGO_RATIO_K_PACK( 150u,  533u, 1262u),  /* idx=518, ratio=0.308099 */
    ALGO_RATIO_K_PACK( 147u,  538u, 1018u),  /* idx=519, ratio=0.248704 */
    ALGO_RATIO_K_PACK( 143u,  543u,  790u),  /* idx=520, ratio=0.192918 */
    ALGO_RATIO_K_PACK( 140u,  547u,  575u),  /* idx=521, ratio=0.140422 */
    ALGO_RATIO_K_PACK( 137u,  552u,  372u),  /* idx=522, ratio=0.090933 */
    ALGO_RATIO_K_PACK( 134u,  556u,  181u),  /* idx=523, ratio=0.044201 */
    ALGO_RATIO_K_PACK( 131u,  559u,    0u),  /* idx=524, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 356u,  279u,    0u),  /* idx=525, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 338u,  302u,  748u),  /* idx=526, ratio=0.182681 */
    ALGO_RATIO_K_PACK( 322u,  323u, 1335u),  /* idx=527, ratio=0.326108 */
    ALGO_RATIO_K_PACK( 308u,  342u, 1809u),  /* idx=528, ratio=0.441706 */
    ALGO_RATIO_K_PACK( 294u,  359u, 2198u),  /* idx=529, ratio=0.536859 */
    ALGO_RATIO_K_PACK( 282u,  375u, 2525u),  /* idx=530, ratio=0.616549 */
    ALGO_RATIO_K_PACK( 271u,  389u, 2802u),  /* idx=531, ratio=0.684263 */
    ALGO_RATIO_K_PACK( 260u,  403u, 3041u),  /* idx=532, ratio=0.742511 */
    ALGO_RATIO_K_PACK( 250u,  415u, 3248u),  /* idx=533, ratio=0.793149 */
    ALGO_RATIO_K_PACK( 241u,  427u, 3430u),  /* idx=534, ratio=0.837577 */
    ALGO_RATIO_K_PACK( 233u,  438u, 3591u),  /* idx=535, ratio=0.876870 */
    ALGO_RATIO_K_PACK( 225u,  448u, 3734u),  /* idx=536, ratio=0.911872 */
    ALGO_RATIO_K_PACK( 218u,  457u, 3863u),  /* idx=537, ratio=0.943247 */
    ALGO_RATIO_K_PACK( 211u,  466u, 3978u),  /* idx=538, ratio=0.971533 */
    ALGO_RATIO_K_PACK( 204u,  474u, 3849u),  /* idx=539, ratio=0.939868 */
    ALGO_RATIO_K_PACK( 198u,  482u, 3598u),  /* idx=540, ratio=0.878696 */
    ALGO_RATIO_K_PACK( 192u,  490u, 3382u),  /* idx=541, ratio=0.825891 */
    ALGO_RATIO_K_PACK( 187u,  497u, 3166u),  /* idx=542, ratio=0.773057 */
    ALGO_RATIO_K_PACK( 182u,  504u, 2825u),  /* idx=543, ratio=0.689958 */
    ALGO_RATIO_K_PACK( 177u,  510u, 2471u),  /* idx=544, ratio=0.603393 */
    ALGO_RATIO_K_PACK( 172u,  516u, 2142u),  /* idx=545, ratio=0.523167 */
    ALGO_RATIO_K_PACK( 168u,  522u, 1837u),  /* idx=546, ratio=0.448609 */
    ALGO_RATIO_K_PACK( 163u,  527u, 1553u),  /* idx=547, ratio=0.379140 */
    ALGO_RATIO_K_PACK( 159u,  532u, 1287u),  /* idx=548, ratio=0.314254 */
    ALGO_RATIO_K_PACK( 156u,  537u, 1038u),  /* idx=549, ratio=0.253514 */
    ALGO_RATIO_K_PACK( 152u,  542u,  805u),  /* idx=550, ratio=0.196534 */
    ALGO_RATIO_K_PACK( 148u,  546u,  585u),  /* idx=551, ratio=0.142975 */
    ALGO_RATIO_K_PACK( 145u,  551u,  379u),  /* idx=552, ratio=0.092538 */
    ALGO_RATIO_K_PACK( 142u,  555u,  184u),  /* idx=553, ratio=0.044959 */
    ALGO_RATIO_K_PACK( 139u,  559u,    0u),  /* idx=554, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 365u,  290u,    0u),  /* idx=555, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 347u,  312u,  714u),  /* idx=556, ratio=0.174399 */
    ALGO_RATIO_K_PACK( 331u,  332u, 1281u),  /* idx=557, ratio=0.312848 */
    ALGO_RATIO_K_PACK( 316u,  351u, 1742u),  /* idx=558, ratio=0.425425 */
    ALGO_RATIO_K_PACK( 302u,  367u, 2124u),  /* idx=559, ratio=0.518761 */
    ALGO_RATIO_K_PACK( 290u,  383u, 2446u),  /* idx=560, ratio=0.597401 */
    ALGO_RATIO_K_PACK( 278u,  397u, 2721u),  /* idx=561, ratio=0.664562 */
    ALGO_RATIO_K_PACK( 268u,  410u, 2959u),  /* idx=562, ratio=0.722587 */
    ALGO_RATIO_K_PACK( 258u,  422u, 3166u),  /* idx=563, ratio=0.773222 */
    ALGO_RATIO_K_PACK( 249u,  433u, 3349u),  /* idx=564, ratio=0.817794 */
    ALGO_RATIO_K_PACK( 240u,  444u, 3511u),  /* idx=565, ratio=0.857329 */
    ALGO_RATIO_K_PACK( 232u,  454u, 3655u),  /* idx=566, ratio=0.892637 */
    ALGO_RATIO_K_PACK( 224u,  463u, 3785u),  /* idx=567, ratio=0.924361 */
    ALGO_RATIO_K_PACK( 217u,  471u, 3903u),  /* idx=568, ratio=0.953020 */
    ALGO_RATIO_K_PACK( 211u,  480u, 3986u),  /* idx=569, ratio=0.973385 */
    ALGO_RATIO_K_PACK( 204u,  487u, 3722u),  /* idx=570, ratio=0.908988 */
    ALGO_RATIO_K_PACK( 198u,  495u, 3291u),  /* idx=571, ratio=0.803666 */
    ALGO_RATIO_K_PACK( 193u,  501u, 2895u),  /* idx=572, ratio=0.706871 */
    ALGO_RATIO_K_PACK( 188u,  508u, 2529u),  /* idx=573, ratio=0.617608 */
    ALGO_RATIO_K_PACK( 183u,  514u, 2191u),  /* idx=574, ratio=0.535030 */
    ALGO_RATIO_K_PACK( 178u,  520u, 1877u),  /* idx=575, ratio=0.458415 */
    ALGO_RATIO_K_PACK( 173u,  525u, 1585u),  /* idx=576, ratio=0.387138 */
    ALGO_RATIO_K_PACK( 169u,  531u, 1313u),  /* idx=577, ratio=0.320661 */
    ALGO_RATIO_K_PACK( 165u,  536u, 1059u),  /* idx=578, ratio=0.258514 */
    ALGO_RATIO_K_PACK( 161u,  541u,  820u),  /* idx=579, ratio=0.200287 */
    ALGO_RATIO_K_PACK( 157u,  545u,  596u),  /* idx=580, ratio=0.145622 */
    ALGO_RATIO_K_PACK( 153u,  550u,  386u),  /* idx=581, ratio=0.094201 */
    ALGO_RATIO_K_PACK( 150u,  554u,  187u),  /* idx=582, ratio=0.045743 */
    ALGO_RATIO_K_PACK( 147u,  558u,    0u),  /* idx=583, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 374u,  301u,    0u),  /* idx=584, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 356u,  323u,  683u),  /* idx=585, ratio=0.166836 */
    ALGO_RATIO_K_PACK( 339u,  342u, 1231u),  /* idx=586, ratio=0.300625 */
    ALGO_RATIO_K_PACK( 324u,  359u, 1680u),  /* idx=587, ratio=0.410300 */
    ALGO_RATIO_K_PACK( 310u,  376u, 2055u),  /* idx=588, ratio=0.501843 */
    ALGO_RATIO_K_PACK( 298u,  390u, 2373u),  /* idx=589, ratio=0.579406 */
    ALGO_RATIO_K_PACK( 286u,  404u, 2645u),  /* idx=590, ratio=0.645964 */
    ALGO_RATIO_K_PACK( 275u,  417u, 2882u),  /* idx=591, ratio=0.703705 */
    ALGO_RATIO_K_PACK( 265u,  428u, 3089u),  /* idx=592, ratio=0.754272 */
    ALGO_RATIO_K_PACK( 256u,  439u, 3272u),  /* idx=593, ratio=0.798923 */
    ALGO_RATIO_K_PACK( 247u,  450u, 3434u),  /* idx=594, ratio=0.838640 */
    ALGO_RATIO_K_PACK( 239u,  459u, 3580u),  /* idx=595, ratio=0.874197 */
    ALGO_RATIO_K_PACK( 231u,  468u, 3711u),  /* idx=596, ratio=0.906216 */
    ALGO_RATIO_K_PACK( 224u,  477u, 3830u),  /* idx=597, ratio=0.935199 */
    ALGO_RATIO_K_PACK( 217u,  485u, 3824u),  /* idx=598, ratio=0.933897 */
    ALGO_RATIO_K_PACK( 211u,  492u, 3377u),  /* idx=599, ratio=0.824735 */
    ALGO_RATIO_K_PACK( 204u,  499u, 2967u),  /* idx=600, ratio=0.724633 */
    ALGO_RATIO_K_PACK( 199u,  506u, 2590u),  /* idx=601, ratio=0.632508 */
    ALGO_RATIO_K_PACK( 193u,  512u, 2242u),  /* idx=602, ratio=0.547444 */
    ALGO_RATIO_K_PACK( 188u,  518u, 1919u),  /* idx=603, ratio=0.468659 */
    ALGO_RATIO_K_PACK( 183u,  524u, 1619u),  /* idx=604, ratio=0.395481 */
    ALGO_RATIO_K_PACK( 179u,  529u, 1340u),  /* idx=605, ratio=0.327334 */
    ALGO_RATIO_K_PACK( 174u,  535u, 1080u),  /* idx=606, ratio=0.263715 */
    ALGO_RATIO_K_PACK( 170u,  539u,  836u),  /* idx=607, ratio=0.204187 */
    ALGO_RATIO_K_PACK( 166u,  544u,  608u),  /* idx=608, ratio=0.148369 */
    ALGO_RATIO_K_PACK( 162u,  549u,  393u),  /* idx=609, ratio=0.095924 */
    ALGO_RATIO_K_PACK( 158u,  553u,  191u),  /* idx=610, ratio=0.046555 */
    ALGO_RATIO_K_PACK( 155u,  557u,    0u),  /* idx=611, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 382u,  312u,    0u),  /* idx=612, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 364u,  333u,  655u),  /* idx=613, ratio=0.159902 */
    ALGO_RATIO_K_PACK( 347u,  351u, 1185u),  /* idx=614, ratio=0.289321 */
    ALGO_RATIO_K_PACK( 332u,  368u, 1622u),  /* idx=615, ratio=0.396215 */
    ALGO_RATIO_K_PACK( 318u,  384u, 1990u),  /* idx=616, ratio=0.485994 */
    ALGO_RATIO_K_PACK( 305u,  398u, 2303u),  /* idx=617, ratio=0.562464 */
    ALGO_RATIO_K_PACK( 293u,  411u, 2573u),  /* idx=618, ratio=0.628379 */
    ALGO_RATIO_K_PACK( 282u,  424u, 2808u),  /* idx=619, ratio=0.685785 */
    ALGO_RATIO_K_PACK( 272u,  435u, 3015u),  /* idx=620, ratio=0.736229 */
    ALGO_RATIO_K_PACK( 262u,  446u, 3198u),  /* idx=621, ratio=0.780905 */
    ALGO_RATIO_K_PACK( 254u,  456u, 3361u),  /* idx=622, ratio=0.820748 */
    ALGO_RATIO_K_PACK( 245u,  465u, 3507u),  /* idx=623, ratio=0.856504 */
    ALGO_RATIO_K_PACK( 237u,  474u, 3640u),  /* idx=624, ratio=0.888769 */
    ALGO_RATIO_K_PACK( 230u,  482u, 3759u),  /* idx=625, ratio=0.918032 */
    ALGO_RATIO_K_PACK( 223u,  490u, 3468u),  /* idx=626, ratio=0.846938 */
    ALGO_RATIO_K_PACK( 217u,  497u, 3044u),  /* idx=627, ratio=0.743311 */
    ALGO_RATIO_K_PACK( 210u,  504u, 2654u),  /* idx=628, ratio=0.648146 */
    ALGO_RATIO_K_PACK( 205u,  510u, 2295u),  /* idx=629, ratio=0.560448 */
    ALGO_RATIO_K_PACK( 199u,  516u, 1963u),  /* idx=630, ratio=0.479371 */
    ALGO_RATIO_K_PACK( 194u,  522u, 1655u),  /* idx=631, ratio=0.404192 */
    ALGO_RATIO_K_PACK( 189u,  528u, 1369u),  /* idx=632, ratio=0.334290 */
    ALGO_RATIO_K_PACK( 184u,  533u, 1102u),  /* idx=633, ratio=0.269129 */
    ALGO_RATIO_K_PACK( 180u,  538u,  853u),  /* idx=634, ratio=0.208242 */
    ALGO_RATIO_K_PACK( 175u,  543u,  619u),  /* idx=635, ratio=0.151222 */
    ALGO_RATIO_K_PACK( 171u,  548u,  400u),  /* idx=636, ratio=0.097712 */
    ALGO_RATIO_K_PACK( 167u,  552u,  194u),  /* idx=637, ratio=0.047397 */
    ALGO_RATIO_K_PACK( 163u,  556u,    0u),  /* idx=638, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 390u,  323u,    0u),  /* idx=639, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 372u,  342u,  629u),  /* idx=640, ratio=0.153521 */
    ALGO_RATIO_K_PACK( 355u,  360u, 1142u),  /* idx=641, ratio=0.278836 */
    ALGO_RATIO_K_PACK( 340u,  377u, 1569u),  /* idx=642, ratio=0.383064 */
    ALGO_RATIO_K_PACK( 326u,  392u, 1929u),  /* idx=643, ratio=0.471115 */
    ALGO_RATIO_K_PACK( 313u,  405u, 2238u),  /* idx=644, ratio=0.546484 */
    ALGO_RATIO_K_PACK( 301u,  418u, 2505u),  /* idx=645, ratio=0.611726 */
    ALGO_RATIO_K_PACK( 289u,  430u, 2739u),  /* idx=646, ratio=0.668755 */
    ALGO_RATIO_K_PACK( 279u,  441u, 2944u),  /* idx=647, ratio=0.719028 */
    ALGO_RATIO_K_PACK( 269u,  452u, 3127u),  /* idx=648, ratio=0.763681 */
    ALGO_RATIO_K_PACK( 260u,  461u, 3291u),  /* idx=649, ratio=0.803604 */
    ALGO_RATIO_K_PACK( 252u,  470u, 3438u),  /* idx=650, ratio=0.839512 */
    ALGO_RATIO_K_PACK( 244u,  479u, 3571u),  /* idx=651, ratio=0.871982 */
    ALGO_RATIO_K_PACK( 236u,  487u, 3564u),  /* idx=652, ratio=0.870370 */
    ALGO_RATIO_K_PACK( 229u,  494u, 3124u),  /* idx=653, ratio=0.762977 */
    ALGO_RATIO_K_PACK( 223u,  501u, 2721u),  /* idx=654, ratio=0.664577 */
    ALGO_RATIO_K_PACK( 216u,  508u, 2351u),  /* idx=655, ratio=0.574084 */
    ALGO_RATIO_K_PACK( 210u,  515u, 2009u),  /* idx=656, ratio=0.490584 */
    ALGO_RATIO_K_PACK( 205u,  521u, 1692u),  /* idx=657, ratio=0.413295 */
    ALGO_RATIO_K_PACK( 199u,  526u, 1399u),  /* idx=658, ratio=0.341549 */
    ALGO_RATIO_K_PACK( 194u,  532u, 1125u),  /* idx=659, ratio=0.274771 */
    ALGO_RATIO_K_PACK( 189u,  537u,  870u),  /* idx=660, ratio=0.212461 */
    ALGO_RATIO_K_PACK( 185u,  542u,  631u),  /* idx=661, ratio=0.154187 */
    ALGO_RATIO_K_PACK( 180u,  547u,  408u),  /* idx=662, ratio=0.099567 */
    ALGO_RATIO_K_PACK( 176u,  551u,  198u),  /* idx=663, ratio=0.048270 */
    ALGO_RATIO_K_PACK( 172u,  555u,    0u),  /* idx=664, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 398u,  333u,    0u),  /* idx=665, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 380u,  352u,  605u),  /* idx=666, ratio=0.147630 */
    ALGO_RATIO_K_PACK( 363u,  369u, 1102u),  /* idx=667, ratio=0.269084 */
    ALGO_RATIO_K_PACK( 347u,  385u, 1518u),  /* idx=668, ratio=0.370758 */
    ALGO_RATIO_K_PACK( 333u,  399u, 1872u),  /* idx=669, ratio=0.457120 */
    ALGO_RATIO_K_PACK( 320u,  413u, 2176u),  /* idx=670, ratio=0.531387 */
    ALGO_RATIO_K_PACK( 308u,  425u, 2440u),  /* idx=671, ratio=0.595933 */
    ALGO_RATIO_K_PACK( 296u,  437u, 2672u),  /* idx=672, ratio=0.652550 */
    ALGO_RATIO_K_PACK( 286u,  448u, 2877u),  /* idx=673, ratio=0.702614 */
    ALGO_RATIO_K_PACK( 276u,  458u, 3060u),  /* idx=674, ratio=0.747200 */
    ALGO_RATIO_K_PACK( 267u,  467u, 3223u),  /* idx=675, ratio=0.787161 */
    ALGO_RATIO_K_PACK( 258u,  476u, 3371u),  /* idx=676, ratio=0.823182 */
    ALGO_RATIO_K_PACK( 250u,  484u, 3505u),  /* idx=677, ratio=0.855817 */
    ALGO_RATIO_K_PACK( 242u,  492u, 3209u),  /* idx=678, ratio=0.783712 */
    ALGO_RATIO_K_PACK( 235u,  499u, 2792u),  /* idx=679, ratio=0.681862 */
    ALGO_RATIO_K_PACK( 228u,  506u, 2410u),  /* idx=680, ratio=0.588401 */
    ALGO_RATIO_K_PACK( 222u,  513u, 2057u),  /* idx=681, ratio=0.502334 */
    ALGO_RATIO_K_PACK( 216u,  519u, 1731u),  /* idx=682, ratio=0.422817 */
    ALGO_RATIO_K_PACK( 210u,  525u, 1430u),  /* idx=683, ratio=0.349130 */
    ALGO_RATIO_K_PACK( 205u,  530u, 1149u),  /* idx=684, ratio=0.280654 */
    ALGO_RATIO_K_PACK( 200u,  536u,  888u),  /* idx=685, ratio=0.216855 */
    ALGO_RATIO_K_PACK( 195u,  541u,  644u),  /* idx=686, ratio=0.157270 */
    ALGO_RATIO_K_PACK( 190u,  545u,  416u),  /* idx=687, ratio=0.101495 */
    ALGO_RATIO_K_PACK( 185u,  550u,  201u),  /* idx=688, ratio=0.049175 */
    ALGO_RATIO_K_PACK( 181u,  554u,    0u),  /* idx=689, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 406u,  343u,    0u),  /* idx=690, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 388u,  361u,  582u),  /* idx=691, ratio=0.142174 */
    ALGO_RATIO_K_PACK( 370u,  378u, 1065u),  /* idx=692, ratio=0.259992 */
    ALGO_RATIO_K_PACK( 355u,  393u, 1471u),  /* idx=693, ratio=0.359219 */
    ALGO_RATIO_K_PACK( 340u,  407u, 1818u),  /* idx=694, ratio=0.443933 */
    ALGO_RATIO_K_PACK( 327u,  420u, 2118u),  /* idx=695, ratio=0.517102 */
    ALGO_RATIO_K_PACK( 315u,  432u, 2379u),  /* idx=696, ratio=0.580935 */
    ALGO_RATIO_K_PACK( 303u,  443u, 2609u),  /* idx=697, ratio=0.637111 */
    ALGO_RATIO_K_PACK( 293u,  454u, 2813u),  /* idx=698, ratio=0.686931 */
    ALGO_RATIO_K_PACK( 283u,  464u, 2995u),  /* idx=699, ratio=0.731416 */
    ALGO_RATIO_K_PACK( 273u,  473u, 3159u),  /* idx=700, ratio=0.771378 */
    ALGO_RATIO_K_PACK( 264u,  481u, 3307u),  /* idx=701, ratio=0.807475 */
    ALGO_RATIO_K_PACK( 256u,  489u, 3299u),  /* idx=702, ratio=0.805606 */
    ALGO_RATIO_K_PACK( 248u,  497u, 2867u),  /* idx=703, ratio=0.700070 */
    ALGO_RATIO_K_PACK( 241u,  504u, 2471u),  /* idx=704, ratio=0.603450 */
    ALGO_RATIO_K_PACK( 234u,  511u, 2108u),  /* idx=705, ratio=0.514661 */
    ALGO_RATIO_K_PACK( 228u,  517u, 1772u),  /* idx=706, ratio=0.432789 */
    ALGO_RATIO_K_PACK( 222u,  523u, 1462u),  /* idx=707, ratio=0.357055 */
    ALGO_RATIO_K_PACK( 216u,  529u, 1174u),  /* idx=708, ratio=0.286794 */
    ALGO_RATIO_K_PACK( 210u,  534u,  907u),  /* idx=709, ratio=0.221434 */
    ALGO_RATIO_K_PACK( 205u,  539u,  657u),  /* idx=710, ratio=0.160479 */
    ALGO_RATIO_K_PACK( 200u,  544u,  424u),  /* idx=711, ratio=0.103498 */
    ALGO_RATIO_K_PACK( 195u,  549u,  205u),  /* idx=712, ratio=0.050115 */
    ALGO_RATIO_K_PACK( 190u,  554u,    0u),  /* idx=713, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 414u,  352u,    0u),  /* idx=714, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 395u,  370u,  561u),  /* idx=715, ratio=0.137107 */
    ALGO_RATIO_K_PACK( 378u,  386u, 1030u),  /* idx=716, ratio=0.251494 */
    ALGO_RATIO_K_PACK( 362u,  401u, 1427u),  /* idx=717, ratio=0.348376 */
    ALGO_RATIO_K_PACK( 347u,  414u, 1767u),  /* idx=718, ratio=0.431485 */
    ALGO_RATIO_K_PACK( 334u,  427u, 2062u),  /* idx=719, ratio=0.503565 */
    ALGO_RATIO_K_PACK( 322u,  439u, 2321u),  /* idx=720, ratio=0.566673 */
    ALGO_RATIO_K_PACK( 310u,  450u, 2549u),  /* idx=721, ratio=0.622387 */
    ALGO_RATIO_K_PACK( 299u,  460u, 2752u),  /* idx=722, ratio=0.671934 */
    ALGO_RATIO_K_PACK( 289u,  469u, 2933u),  /* idx=723, ratio=0.716285 */
    ALGO_RATIO_K_PACK( 280u,  478u, 3097u),  /* idx=724, ratio=0.756215 */
    ALGO_RATIO_K_PACK( 271u,  486u, 3245u),  /* idx=725, ratio=0.792356 */
    ALGO_RATIO_K_PACK( 262u,  494u, 2945u),  /* idx=726, ratio=0.719278 */
    ALGO_RATIO_K_PACK( 254u,  502u, 2536u),  /* idx=727, ratio=0.619289 */
    ALGO_RATIO_K_PACK( 247u,  509u, 2161u),  /* idx=728, ratio=0.527608 */
    ALGO_RATIO_K_PACK( 240u,  515u, 1815u),  /* idx=729, ratio=0.443242 */
    ALGO_RATIO_K_PACK( 233u,  521u, 1496u),  /* idx=730, ratio=0.365348 */
    ALGO_RATIO_K_PACK( 227u,  527u, 1201u),  /* idx=731, ratio=0.293209 */
    ALGO_RATIO_K_PACK( 221u,  533u,  926u),  /* idx=732, ratio=0.226211 */
    ALGO_RATIO_K_PACK( 215u,  538u,  671u),  /* idx=733, ratio=0.163822 */
    ALGO_RATIO_K_PACK( 210u,  543u,  432u),  /* idx=734, ratio=0.105582 */
    ALGO_RATIO_K_PACK( 205u,  548u,  209u),  /* idx=735, ratio=0.051092 */
    ALGO_RATIO_K_PACK( 200u,  553u,    0u),  /* idx=736, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 421u,  362u,    0u),  /* idx=737, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 402u,  379u,  542u),  /* idx=738, ratio=0.132389 */
    ALGO_RATIO_K_PACK( 385u,  394u,  997u),  /* idx=739, ratio=0.243533 */
    ALGO_RATIO_K_PACK( 369u,  409u, 1385u),  /* idx=740, ratio=0.338168 */
    ALGO_RATIO_K_PACK( 354u,  422u, 1719u),  /* idx=741, ratio=0.419717 */
    ALGO_RATIO_K_PACK( 341u,  434u, 2009u),  /* idx=742, ratio=0.490718 */
    ALGO_RATIO_K_PACK( 328u,  445u, 2265u),  /* idx=743, ratio=0.553095 */
    ALGO_RATIO_K_PACK( 317u,  456u, 2491u),  /* idx=744, ratio=0.608327 */
    ALGO_RATIO_K_PACK( 306u,  466u, 2693u),  /* idx=745, ratio=0.657577 */
    ALGO_RATIO_K_PACK( 295u,  475u, 2874u),  /* idx=746, ratio=0.701767 */
    ALGO_RATIO_K_PACK( 286u,  484u, 3037u),  /* idx=747, ratio=0.741637 */
    ALGO_RATIO_K_PACK( 277u,  492u, 3029u),  /* idx=748, ratio=0.739569 */
    ALGO_RATIO_K_PACK( 268u,  499u, 2604u),  /* idx=749, ratio=0.635982 */
    ALGO_RATIO_K_PACK( 260u,  506u, 2216u),  /* idx=750, ratio=0.541224 */
    ALGO_RATIO_K_PACK( 253u,  513u, 1860u),  /* idx=751, ratio=0.454213 */
    ALGO_RATIO_K_PACK( 246u,  520u, 1532u),  /* idx=752, ratio=0.374036 */
    ALGO_RATIO_K_PACK( 239u,  526u, 1228u),  /* idx=753, ratio=0.299918 */
    ALGO_RATIO_K_PACK( 233u,  531u,  947u),  /* idx=754, ratio=0.231198 */
    ALGO_RATIO_K_PACK( 226u,  537u,  685u),  /* idx=755, ratio=0.167307 */
    ALGO_RATIO_K_PACK( 221u,  542u,  441u),  /* idx=756, ratio=0.107752 */
    ALGO_RATIO_K_PACK( 215u,  547u,  213u),  /* idx=757, ratio=0.052108 */
    ALGO_RATIO_K_PACK( 210u,  552u,    0u),  /* idx=758, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 429u,  371u,    0u),  /* idx=759, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 410u,  387u,  524u),  /* idx=760, ratio=0.127984 */
    ALGO_RATIO_K_PACK( 392u,  402u,  967u),  /* idx=761, ratio=0.236062 */
    ALGO_RATIO_K_PACK( 376u,  416u, 1345u),  /* idx=762, ratio=0.328541 */
    ALGO_RATIO_K_PACK( 361u,  429u, 1673u),  /* idx=763, ratio=0.408573 */
    ALGO_RATIO_K_PACK( 348u,  441u, 1960u),  /* idx=764, ratio=0.478511 */
    ALGO_RATIO_K_PACK( 335u,  452u, 2212u),  /* idx=765, ratio=0.540152 */
    ALGO_RATIO_K_PACK( 323u,  462u, 2436u),  /* idx=766, ratio=0.594889 */
    ALGO_RATIO_K_PACK( 312u,  472u, 2636u),  /* idx=767, ratio=0.643822 */
    ALGO_RATIO_K_PACK( 302u,  480u, 2817u),  /* idx=768, ratio=0.687826 */
    ALGO_RATIO_K_PACK( 292u,  489u, 2980u),  /* idx=769, ratio=0.727610 */
    ALGO_RATIO_K_PACK( 283u,  497u, 2676u),  /* idx=770, ratio=0.653599 */
    ALGO_RATIO_K_PACK( 274u,  504u, 2275u),  /* idx=771, ratio=0.555561 */
    ALGO_RATIO_K_PACK( 266u,  511u, 1907u),  /* idx=772, ratio=0.465740 */
    ALGO_RATIO_K_PACK( 258u,  518u, 1569u),  /* idx=773, ratio=0.383147 */
    ALGO_RATIO_K_PACK( 251u,  524u, 1257u),  /* idx=774, ratio=0.306941 */
    ALGO_RATIO_K_PACK( 244u,  530u,  968u),  /* idx=775, ratio=0.236410 */
    ALGO_RATIO_K_PACK( 238u,  535u,  700u),  /* idx=776, ratio=0.170943 */
    ALGO_RATIO_K_PACK( 232u,  541u,  451u),  /* idx=777, ratio=0.110013 */
    ALGO_RATIO_K_PACK( 226u,  546u,  218u),  /* idx=778, ratio=0.053164 */
    ALGO_RATIO_K_PACK( 220u,  551u,    0u),  /* idx=779, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 436u,  380u,    0u),  /* idx=780, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 417u,  396u,  507u),  /* idx=781, ratio=0.123864 */
    ALGO_RATIO_K_PACK( 399u,  410u,  938u),  /* idx=782, ratio=0.229035 */
    ALGO_RATIO_K_PACK( 383u,  424u, 1308u),  /* idx=783, ratio=0.319448 */
    ALGO_RATIO_K_PACK( 368u,  436u, 1630u),  /* idx=784, ratio=0.398006 */
    ALGO_RATIO_K_PACK( 354u,  447u, 1912u),  /* idx=785, ratio=0.466896 */
    ALGO_RATIO_K_PACK( 341u,  458u, 2161u),  /* idx=786, ratio=0.527801 */
    ALGO_RATIO_K_PACK( 329u,  468u, 2383u),  /* idx=787, ratio=0.582032 */
    ALGO_RATIO_K_PACK( 318u,  477u, 2582u),  /* idx=788, ratio=0.630629 */
    ALGO_RATIO_K_PACK( 308u,  486u, 2762u),  /* idx=789, ratio=0.674428 */
    ALGO_RATIO_K_PACK( 298u,  494u, 2753u),  /* idx=790, ratio=0.672221 */
    ALGO_RATIO_K_PACK( 289u,  502u, 2337u),  /* idx=791, ratio=0.570678 */
    ALGO_RATIO_K_PACK( 280u,  509u, 1957u),  /* idx=792, ratio=0.477868 */
    ALGO_RATIO_K_PACK( 272u,  516u, 1608u),  /* idx=793, ratio=0.392713 */
    ALGO_RATIO_K_PACK( 264u,  522u, 1287u),  /* idx=794, ratio=0.314301 */
    ALGO_RATIO_K_PACK( 257u,  528u,  990u),  /* idx=795, ratio=0.241863 */
    ALGO_RATIO_K_PACK( 250u,  534u,  716u),  /* idx=796, ratio=0.174741 */
    ALGO_RATIO_K_PACK( 243u,  539u,  460u),  /* idx=797, ratio=0.112371 */
    ALGO_RATIO_K_PACK( 237u,  545u,  222u),  /* idx=798, ratio=0.054265 */
    ALGO_RATIO_K_PACK( 231u,  549u,    0u),  /* idx=799, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 443u,  389u,    0u),  /* idx=800, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 424u,  404u,  491u),  /* idx=801, ratio=0.120000 */
    ALGO_RATIO_K_PACK( 406u,  418u,  911u),  /* idx=802, ratio=0.222414 */
    ALGO_RATIO_K_PACK( 390u,  431u, 1273u),  /* idx=803, ratio=0.310844 */
    ALGO_RATIO_K_PACK( 375u,  443u, 1589u),  /* idx=804, ratio=0.387971 */
    ALGO_RATIO_K_PACK( 361u,  454u, 1867u),  /* idx=805, ratio=0.455832 */
    ALGO_RATIO_K_PACK( 348u,  464u, 2113u),  /* idx=806, ratio=0.516002 */
    ALGO_RATIO_K_PACK( 336u,  474u, 2333u),  /* idx=807, ratio=0.569719 */
    ALGO_RATIO_K_PACK( 324u,  483u, 2531u),  /* idx=808, ratio=0.617967 */
    ALGO_RATIO_K_PACK( 314u,  491u, 2709u),  /* idx=809, ratio=0.661542 */
    ALGO_RATIO_K_PACK( 304u,  499u, 2402u),  /* idx=810, ratio=0.586640 */
    ALGO_RATIO_K_PACK( 294u,  507u, 2009u),  /* idx=811, ratio=0.490645 */
    ALGO_RATIO_K_PACK( 286u,  514u, 1649u),  /* idx=812, ratio=0.402768 */
    ALGO_RATIO_K_PACK( 277u,  520u, 1319u),  /* idx=813, ratio=0.322022 */
    ALGO_RATIO_K_PACK( 270u,  526u, 1014u),  /* idx=814, ratio=0.247573 */
    ALGO_RATIO_K_PACK( 262u,  532u,  732u),  /* idx=815, ratio=0.178712 */
    ALGO_RATIO_K_PACK( 255u,  538u,  470u),  /* idx=816, ratio=0.114832 */
    ALGO_RATIO_K_PACK( 248u,  543u,  227u),  /* idx=817, ratio=0.055412 */
    ALGO_RATIO_K_PACK( 242u,  548u,    0u),  /* idx=818, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 450u,  397u,    0u),  /* idx=819, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 430u,  412u,  477u),  /* idx=820, ratio=0.116370 */
    ALGO_RATIO_K_PACK( 413u,  426u,  885u),  /* idx=821, ratio=0.216166 */
    ALGO_RATIO_K_PACK( 396u,  438u, 1240u),  /* idx=822, ratio=0.302692 */
    ALGO_RATIO_K_PACK( 381u,  450u, 1550u),  /* idx=823, ratio=0.378430 */
    ALGO_RATIO_K_PACK( 367u,  460u, 1823u),  /* idx=824, ratio=0.445280 */
    ALGO_RATIO_K_PACK( 354u,  470u, 2067u),  /* idx=825, ratio=0.504719 */
    ALGO_RATIO_K_PACK( 342u,  480u, 2285u),  /* idx=826, ratio=0.557915 */
    ALGO_RATIO_K_PACK( 330u,  488u, 2481u),  /* idx=827, ratio=0.605803 */
    ALGO_RATIO_K_PACK( 320u,  497u, 2471u),  /* idx=828, ratio=0.603522 */
    ALGO_RATIO_K_PACK( 310u,  504u, 2064u),  /* idx=829, ratio=0.504123 */
    ALGO_RATIO_K_PACK( 300u,  512u, 1693u),  /* idx=830, ratio=0.413352 */
    ALGO_RATIO_K_PACK( 291u,  518u, 1352u),  /* idx=831, ratio=0.330133 */
    ALGO_RATIO_K_PACK( 283u,  525u, 1038u),  /* idx=832, ratio=0.253560 */
    ALGO_RATIO_K_PACK( 275u,  531u,  749u),  /* idx=833, ratio=0.182867 */
    ALGO_RATIO_K_PACK( 267u,  537u,  481u),  /* idx=834, ratio=0.117403 */
    ALGO_RATIO_K_PACK( 260u,  542u,  232u),  /* idx=835, ratio=0.056608 */
    ALGO_RATIO_K_PACK( 254u,  547u,    0u),  /* idx=836, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 456u,  406u,    0u),  /* idx=837, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 437u,  420u,  463u),  /* idx=838, ratio=0.112954 */
    ALGO_RATIO_K_PACK( 419u,  433u,  861u),  /* idx=839, ratio=0.210258 */
    ALGO_RATIO_K_PACK( 403u,  445u, 1208u),  /* idx=840, ratio=0.294956 */
    ALGO_RATIO_K_PACK( 387u,  456u, 1512u),  /* idx=841, ratio=0.369347 */
    ALGO_RATIO_K_PACK( 373u,  467u, 1782u),  /* idx=842, ratio=0.435205 */
    ALGO_RATIO_K_PACK( 360u,  476u, 2023u),  /* idx=843, ratio=0.493919 */
    ALGO_RATIO_K_PACK( 348u,  485u, 2238u),  /* idx=844, ratio=0.546591 */
    ALGO_RATIO_K_PACK( 336u,  494u, 2433u),  /* idx=845, ratio=0.594109 */
    ALGO_RATIO_K_PACK( 325u,  502u, 2123u),  /* idx=846, ratio=0.518363 */
    ALGO_RATIO_K_PACK( 315u,  509u, 1738u),  /* idx=847, ratio=0.424508 */
    ALGO_RATIO_K_PACK( 306u,  516u, 1387u),  /* idx=848, ratio=0.338662 */
    ALGO_RATIO_K_PACK( 297u,  523u, 1064u),  /* idx=849, ratio=0.259842 */
    ALGO_RATIO_K_PACK( 288u,  529u,  767u),  /* idx=850, ratio=0.187220 */
    ALGO_RATIO_K_PACK( 280u,  535u,  492u),  /* idx=851, ratio=0.120092 */
    ALGO_RATIO_K_PACK( 273u,  541u,  237u),  /* idx=852, ratio=0.057858 */
    ALGO_RATIO_K_PACK( 266u,  546u,    0u),  /* idx=853, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 463u,  414u,    0u),  /* idx=854, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 443u,  428u,  449u),  /* idx=855, ratio=0.109732 */
    ALGO_RATIO_K_PACK( 426u,  440u,  838u),  /* idx=856, ratio=0.204666 */
    ALGO_RATIO_K_PACK( 409u,  452u, 1178u),  /* idx=857, ratio=0.287606 */
    ALGO_RATIO_K_PACK( 394u,  463u, 1477u),  /* idx=858, ratio=0.360690 */
    ALGO_RATIO_K_PACK( 379u,  473u, 1743u),  /* idx=859, ratio=0.425577 */
    ALGO_RATIO_K_PACK( 366u,  482u, 1980u),  /* idx=860, ratio=0.483572 */
    ALGO_RATIO_K_PACK( 354u,  491u, 2194u),  /* idx=861, ratio=0.535718 */
    ALGO_RATIO_K_PACK( 342u,  499u, 2184u),  /* idx=862, ratio=0.533431 */
    ALGO_RATIO_K_PACK( 331u,  507u, 1787u),  /* idx=863, ratio=0.436282 */
    ALGO_RATIO_K_PACK( 321u,  514u, 1424u),  /* idx=864, ratio=0.347644 */
    ALGO_RATIO_K_PACK( 311u,  521u, 1091u),  /* idx=865, ratio=0.266445 */
    ALGO_RATIO_K_PACK( 302u,  527u,  785u),  /* idx=866, ratio=0.191786 */
    ALGO_RATIO_K_PACK( 294u,  534u,  503u),  /* idx=867, ratio=0.122907 */
    ALGO_RATIO_K_PACK( 286u,  539u,  242u),  /* idx=868, ratio=0.059163 */
    ALGO_RATIO_K_PACK( 278u,  545u,    0u),  /* idx=869, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 469u,  422u,    0u),  /* idx=870, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 450u,  435u,  437u),  /* idx=871, ratio=0.106689 */
    ALGO_RATIO_K_PACK( 432u,  447u,  816u),  /* idx=872, ratio=0.199363 */
    ALGO_RATIO_K_PACK( 415u,  459u, 1149u),  /* idx=873, ratio=0.280613 */
    ALGO_RATIO_K_PACK( 400u,  469u, 1443u),  /* idx=874, ratio=0.352429 */
    ALGO_RATIO_K_PACK( 385u,  479u, 1705u),  /* idx=875, ratio=0.416365 */
    ALGO_RATIO_K_PACK( 372u,  488u, 1940u),  /* idx=876, ratio=0.473649 */
    ALGO_RATIO_K_PACK( 360u,  497u, 2151u),  /* idx=877, ratio=0.525269 */
    ALGO_RATIO_K_PACK( 348u,  505u, 1838u),  /* idx=878, ratio=0.448728 */
    ALGO_RATIO_K_PACK( 337u,  512u, 1462u),  /* idx=879, ratio=0.357115 */
    ALGO_RATIO_K_PACK( 327u,  519u, 1120u),  /* idx=880, ratio=0.273391 */
    ALGO_RATIO_K_PACK( 317u,  526u,  805u),  /* idx=881, ratio=0.196579 */
    ALGO_RATIO_K_PACK( 308u,  532u,  515u),  /* idx=882, ratio=0.125858 */
    ALGO_RATIO_K_PACK( 299u,  538u,  248u),  /* idx=883, ratio=0.060529 */
    ALGO_RATIO_K_PACK( 291u,  543u,    0u),  /* idx=884, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 475u,  430u,    0u),  /* idx=885, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 456u,  442u,  425u),  /* idx=886, ratio=0.103810 */
    ALGO_RATIO_K_PACK( 438u,  454u,  796u),  /* idx=887, ratio=0.194327 */
    ALGO_RATIO_K_PACK( 421u,  465u, 1122u),  /* idx=888, ratio=0.273952 */
    ALGO_RATIO_K_PACK( 406u,  475u, 1411u),  /* idx=889, ratio=0.344539 */
    ALGO_RATIO_K_PACK( 391u,  485u, 1669u),  /* idx=890, ratio=0.407543 */
    ALGO_RATIO_K_PACK( 378u,  494u, 1901u),  /* idx=891, ratio=0.464125 */
    ALGO_RATIO_K_PACK( 365u,  502u, 1892u),  /* idx=892, ratio=0.461905 */
    ALGO_RATIO_K_PACK( 354u,  510u, 1503u),  /* idx=893, ratio=0.367117 */
    ALGO_RATIO_K_PACK( 342u,  517u, 1150u),  /* idx=894, ratio=0.280710 */
    ALGO_RATIO_K_PACK( 332u,  524u,  826u),  /* idx=895, ratio=0.201619 */
    ALGO_RATIO_K_PACK( 322u,  530u,  528u),  /* idx=896, ratio=0.128953 */
    ALGO_RATIO_K_PACK( 313u,  536u,  254u),  /* idx=897, ratio=0.061960 */
    ALGO_RATIO_K_PACK( 304u,  542u,    0u),  /* idx=898, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 482u,  437u,    0u),  /* idx=899, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 462u,  450u,  414u),  /* idx=900, ratio=0.101082 */
    ALGO_RATIO_K_PACK( 444u,  461u,  776u),  /* idx=901, ratio=0.189540 */
    ALGO_RATIO_K_PACK( 427u,  472u, 1096u),  /* idx=902, ratio=0.267600 */
    ALGO_RATIO_K_PACK( 412u,  482u, 1380u),  /* idx=903, ratio=0.336994 */
    ALGO_RATIO_K_PACK( 397u,  491u, 1634u),  /* idx=904, ratio=0.399088 */
    ALGO_RATIO_K_PACK( 384u,  499u, 1863u),  /* idx=905, ratio=0.454977 */
    ALGO_RATIO_K_PACK( 371u,  507u, 1547u),  /* idx=906, ratio=0.377695 */
    ALGO_RATIO_K_PACK( 359u,  515u, 1181u),  /* idx=907, ratio=0.288431 */
    ALGO_RATIO_K_PACK( 348u,  522u,  847u),  /* idx=908, ratio=0.206923 */
    ALGO_RATIO_K_PACK( 337u,  529u,  541u),  /* idx=909, ratio=0.132204 */
    ALGO_RATIO_K_PACK( 328u,  535u,  260u),  /* idx=910, ratio=0.063460 */
    ALGO_RATIO_K_PACK( 318u,  541u,    0u),  /* idx=911, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 488u,  445u,    0u),  /* idx=912, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 468u,  457u,  403u),  /* idx=913, ratio=0.098494 */
    ALGO_RATIO_K_PACK( 450u,  468u,  758u),  /* idx=914, ratio=0.184983 */
    ALGO_RATIO_K_PACK( 433u,  478u, 1071u),  /* idx=915, ratio=0.261536 */
    ALGO_RATIO_K_PACK( 417u,  488u, 1350u),  /* idx=916, ratio=0.329772 */
    ALGO_RATIO_K_PACK( 403u,  497u, 1601u),  /* idx=917, ratio=0.390976 */
    ALGO_RATIO_K_PACK( 389u,  505u, 1593u),  /* idx=918, ratio=0.388901 */
    ALGO_RATIO_K_PACK( 376u,  513u, 1215u),  /* idx=919, ratio=0.296588 */
    ALGO_RATIO_K_PACK( 365u,  520u,  870u),  /* idx=920, ratio=0.212515 */
    ALGO_RATIO_K_PACK( 353u,  527u,  555u),  /* idx=921, ratio=0.135624 */
    ALGO_RATIO_K_PACK( 343u,  533u,  266u),  /* idx=922, ratio=0.065034 */
    ALGO_RATIO_K_PACK( 333u,  539u,    0u),  /* idx=923, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 493u,  452u,    0u),  /* idx=924, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 474u,  464u,  393u),  /* idx=925, ratio=0.096036 */
    ALGO_RATIO_K_PACK( 456u,  474u,  740u),  /* idx=926, ratio=0.180640 */
    ALGO_RATIO_K_PACK( 439u,  484u, 1047u),  /* idx=927, ratio=0.255741 */
    ALGO_RATIO_K_PACK( 423u,  494u, 1322u),  /* idx=928, ratio=0.322853 */
    ALGO_RATIO_K_PACK( 408u,  502u, 1569u),  /* idx=929, ratio=0.383187 */
    ALGO_RATIO_K_PACK( 395u,  510u, 1250u),  /* idx=930, ratio=0.305221 */
    ALGO_RATIO_K_PACK( 382u,  518u,  894u),  /* idx=931, ratio=0.218416 */
    ALGO_RATIO_K_PACK( 370u,  525u,  570u),  /* idx=932, ratio=0.139225 */
    ALGO_RATIO_K_PACK( 359u,  531u,  273u),  /* idx=933, ratio=0.066688 */
    ALGO_RATIO_K_PACK( 348u,  538u,    0u),  /* idx=934, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 499u,  459u,    0u),  /* idx=935, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 479u,  470u,  384u),  /* idx=936, ratio=0.093697 */
    ALGO_RATIO_K_PACK( 461u,  481u,  723u),  /* idx=937, ratio=0.176497 */
    ALGO_RATIO_K_PACK( 444u,  490u, 1025u),  /* idx=938, ratio=0.250197 */
    ALGO_RATIO_K_PACK( 429u,  499u, 1295u),  /* idx=939, ratio=0.316219 */
    ALGO_RATIO_K_PACK( 414u,  508u, 1287u),  /* idx=940, ratio=0.314371 */
    ALGO_RATIO_K_PACK( 400u,  516u,  920u),  /* idx=941, ratio=0.224655 */
    ALGO_RATIO_K_PACK( 387u,  523u,  586u),  /* idx=942, ratio=0.143023 */
    ALGO_RATIO_K_PACK( 375u,  530u,  280u),  /* idx=943, ratio=0.068429 */
    ALGO_RATIO_K_PACK( 364u,  536u,    0u),  /* idx=944, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 505u,  466u,    0u),  /* idx=945, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 485u,  477u,  375u),  /* idx=946, ratio=0.091469 */
    ALGO_RATIO_K_PACK( 467u,  487u,  707u),  /* idx=947, ratio=0.172539 */
    ALGO_RATIO_K_PACK( 450u,  496u, 1003u),  /* idx=948, ratio=0.244888 */
    ALGO_RATIO_K_PACK( 434u,  505u, 1269u),  /* idx=949, ratio=0.309852 */
    ALGO_RATIO_K_PACK( 419u,  513u,  947u),  /* idx=950, ratio=0.231261 */
    ALGO_RATIO_K_PACK( 405u,  521u,  602u),  /* idx=951, ratio=0.147034 */
    ALGO_RATIO_K_PACK( 393u,  528u,  288u),  /* idx=952, ratio=0.070263 */
    ALGO_RATIO_K_PACK( 380u,  534u,    0u),  /* idx=953, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 510u,  473u,    0u),  /* idx=954, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 490u,  484u,  366u),  /* idx=955, ratio=0.089345 */
    ALGO_RATIO_K_PACK( 472u,  493u,  691u),  /* idx=956, ratio=0.168755 */
    ALGO_RATIO_K_PACK( 455u,  502u,  982u),  /* idx=957, ratio=0.239800 */
    ALGO_RATIO_K_PACK( 439u,  511u,  976u),  /* idx=958, ratio=0.238268 */
    ALGO_RATIO_K_PACK( 425u,  519u,  619u),  /* idx=959, ratio=0.151276 */
    ALGO_RATIO_K_PACK( 411u,  526u,  296u),  /* idx=960, ratio=0.072198 */
    ALGO_RATIO_K_PACK( 398u,  533u,    0u),  /* idx=961, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 516u,  480u,    0u),  /* idx=962, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 496u,  490u,  358u),  /* idx=963, ratio=0.087317 */
    ALGO_RATIO_K_PACK( 478u,  499u,  676u),  /* idx=964, ratio=0.165133 */
    ALGO_RATIO_K_PACK( 460u,  508u,  962u),  /* idx=965, ratio=0.234919 */
    ALGO_RATIO_K_PACK( 445u,  516u,  638u),  /* idx=966, ratio=0.155770 */
    ALGO_RATIO_K_PACK( 430u,  524u,  304u),  /* idx=967, ratio=0.074243 */
    ALGO_RATIO_K_PACK( 416u,  531u,    0u),  /* idx=968, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 521u,  487u,    0u),  /* idx=969, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 501u,  496u,  350u),  /* idx=970, ratio=0.085379 */
    ALGO_RATIO_K_PACK( 483u,  505u,  662u),  /* idx=971, ratio=0.161663 */
    ALGO_RATIO_K_PACK( 466u,  514u,  657u),  /* idx=972, ratio=0.160540 */
    ALGO_RATIO_K_PACK( 450u,  522u,  313u),  /* idx=973, ratio=0.076406 */
    ALGO_RATIO_K_PACK( 435u,  529u,    0u),  /* idx=974, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 526u,  493u,    0u),  /* idx=975, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 506u,  503u,  342u),  /* idx=976, ratio=0.083525 */
    ALGO_RATIO_K_PACK( 488u,  511u,  648u),  /* idx=977, ratio=0.158336 */
    ALGO_RATIO_K_PACK( 471u,  519u,  322u),  /* idx=978, ratio=0.078700 */
    ALGO_RATIO_K_PACK( 455u,  527u,    0u),  /* idx=979, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 531u,  499u,    0u),  /* idx=980, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 511u,  509u,  327u),  /* idx=981, ratio=0.079920 */
    ALGO_RATIO_K_PACK( 493u,  517u,  332u),  /* idx=982, ratio=0.081136 */
    ALGO_RATIO_K_PACK( 476u,  525u,    0u),  /* idx=983, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 536u,  506u,    0u),  /* idx=984, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 516u,  515u,  250u),  /* idx=985, ratio=0.061010 */
    ALGO_RATIO_K_PACK( 498u,  523u,    0u),  /* idx=986, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 541u,  512u,    0u),  /* idx=987, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 521u,  520u,    0u),  /* idx=988, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 546u,  518u,    0u),  /* idx=989, ratio=0.000000 */
};

typedef struct
{
    uint16_t u;
    uint16_t v;
    uint16_t ratioQ10000;
    uint32_t distance2;
} AlgoRatioK_NearestPointType;

static uint16_t AlgoRatioK_UnpackU(uint32_t packedValue)
{
    return (uint16_t)(packedValue & ALGO_RATIO_K_U_MASK);
}

static uint16_t AlgoRatioK_UnpackV(uint32_t packedValue)
{
    return (uint16_t)((packedValue & ALGO_RATIO_K_V_MASK) >> ALGO_RATIO_K_V_SHIFT);
}

static uint16_t AlgoRatioK_UnpackRatioQ10000(uint32_t packedValue)
{
    uint16_t ratio12;
    uint32_t numerator;
    uint32_t value;

    ratio12 = (uint16_t)((packedValue & ALGO_RATIO_K_RATIO_MASK) >> ALGO_RATIO_K_RATIO_SHIFT);
    numerator = ((uint32_t)ratio12 * ALGO_RATIO_K_RATIO_Q10000_MAX) + (ALGO_RATIO_K_RATIO12_MAX / 2u);
    value = numerator / ALGO_RATIO_K_RATIO12_MAX;
    if (value > ALGO_RATIO_K_RATIO_Q10000_MAX)
    {
        value = ALGO_RATIO_K_RATIO_Q10000_MAX;
    }

    return (uint16_t)value;
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
        if (nearest[i].distance2 == 0u)
        {
            *whiteRatioQ10000 = nearest[i].ratioQ10000;
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
        pointX = (int32_t)nearest[i].u - (int32_t)targetU;
        pointY = (int32_t)nearest[i].v - (int32_t)targetV;
        weightQ = AlgoRatioK_ReciprocalWeightQ(nearest[i].distance2);

        weightFix16 = AlgoRatioK_DivS64ToFix16((int64_t)weightQ, (int64_t)maxWeightQ);
        xFix16 = AlgoRatioK_ScaledIntToFix16(pointX, coordNormalizer);
        yFix16 = AlgoRatioK_ScaledIntToFix16(pointY, coordNormalizer);
        zFix16 = AlgoRatioK_ScaledIntToFix16((int32_t)nearest[i].ratioQ10000, ALGO_RATIO_K_RATIO_NORMALIZER);

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
    uint8_t i;

    if ((nearest == (void *)0) || (whiteRatioQ10000 == (void *)0) || (count == 0u))
    {
        return 0u;
    }

    weightedRatioSum = 0;
    weightSum = 0;

    for (i = 0u; i < count; i++)
    {
        if (nearest[i].distance2 == 0u)
        {
            *whiteRatioQ10000 = nearest[i].ratioQ10000;
            return 1u;
        }

        weightQ = AlgoRatioK_ReciprocalWeightQ(nearest[i].distance2);
        weightedRatioSum += (int64_t)weightQ * (int64_t)nearest[i].ratioQ10000;
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

static void AlgoRatioK_ResetNearestPoints(AlgoRatioK_NearestPointType *nearest)
{
    uint8_t i;

    for (i = 0u; i < ALGO_RATIO_K_AMOUNT; i++)
    {
        nearest[i].u = 0u;
        nearest[i].v = 0u;
        nearest[i].ratioQ10000 = 0u;
        nearest[i].distance2 = ALGO_RATIO_K_DISTANCE_MAX;
    }
}

static uint8_t AlgoRatioK_TryInsertPointByIndex(uint16_t lutIndex,
                                                uint16_t targetU,
                                                uint16_t targetV,
                                                AlgoRatioK_NearestPointType *nearest,
                                                uint8_t *nearestCount,
                                                uint16_t *directRatioQ10000)
{
    AlgoRatioK_NearestPointType point;
    uint32_t packedValue;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    packedValue = g_algoRatioKPackedLut[lutIndex];
    point.u = AlgoRatioK_UnpackU(packedValue);
    point.v = AlgoRatioK_UnpackV(packedValue);
    point.ratioQ10000 = AlgoRatioK_UnpackRatioQ10000(packedValue);
    point.distance2 = AlgoRatioK_CalcDistance2(targetU, targetV, point.u, point.v);

    if (point.distance2 == 0u)
    {
        if (directRatioQ10000 != (void *)0)
        {
            *directRatioQ10000 = point.ratioQ10000;
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
    const int32_t vertexKU = (int32_t)ALGO_RATIO_K_VERTEX_K_U;
    const int32_t vertexKV = (int32_t)ALGO_RATIO_K_VERTEX_K_V;
    const int32_t vertexIU = (int32_t)ALGO_RATIO_K_VERTEX_I_U;
    const int32_t vertexIV = (int32_t)ALGO_RATIO_K_VERTEX_I_V;
    const int32_t vertexJU = (int32_t)ALGO_RATIO_K_VERTEX_J_U;
    const int32_t vertexJV = (int32_t)ALGO_RATIO_K_VERTEX_J_V;
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
    uint16_t lutIndex;
    uint8_t status;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    axisIU = vertexIU - vertexKU;
    axisIV = vertexIV - vertexKV;
    axisJU = vertexJU - vertexKU;
    axisJV = vertexJV - vertexKV;
    relU = (int32_t)targetU - vertexKU;
    relV = (int32_t)targetV - vertexKV;

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

            lutIndex = AlgoRatioK_GridIndexFromIJ((uint16_t)candidateI, (uint16_t)candidateJ);
            status = AlgoRatioK_TryInsertPointByIndex(lutIndex,
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
    uint16_t lutIndex;
    uint8_t status;

    if ((nearest == (void *)0) || (nearestCount == (void *)0))
    {
        return 0u;
    }

    *nearestCount = 0u;

    for (lutIndex = 0u; lutIndex < ALGO_RATIO_K_LUT_COUNT; lutIndex++)
    {
        status = AlgoRatioK_TryInsertPointByIndex(lutIndex,
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

#if (ALGO_RATIO_K_FAST_NEAREST_ENABLE != 0u)
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
        AlgoRatioK_ResetNearestPoints(nearest);
        status = AlgoRatioK_FindNearestPointsFullScan(targetU,
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
    }
#else
    status = AlgoRatioK_FindNearestPointsFullScan(targetU,
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
#endif

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