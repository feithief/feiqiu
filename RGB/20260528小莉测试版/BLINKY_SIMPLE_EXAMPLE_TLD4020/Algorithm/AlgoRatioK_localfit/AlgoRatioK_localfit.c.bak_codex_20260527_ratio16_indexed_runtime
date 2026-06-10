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

#define ALGO_RATIO_K_VERTEX_K_U          (200u)
#define ALGO_RATIO_K_VERTEX_K_V          (85u)
#define ALGO_RATIO_K_VERTEX_I_U          (547u)
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
    ALGO_RATIO_K_PACK( 200u,   85u,    0u),  /* idx=  0, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 188u,  124u,    0u),  /* idx=  1, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 177u,  159u,    0u),  /* idx=  2, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 167u,  190u,    0u),  /* idx=  3, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 158u,  217u,    0u),  /* idx=  4, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 151u,  243u,    0u),  /* idx=  5, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 143u,  266u,    0u),  /* idx=  6, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 137u,  287u,    0u),  /* idx=  7, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 131u,  306u,    0u),  /* idx=  8, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 125u,  323u,    0u),  /* idx=  9, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 120u,  340u,    0u),  /* idx= 10, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 116u,  355u,    0u),  /* idx= 11, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  369u,    0u),  /* idx= 12, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 107u,  382u,    0u),  /* idx= 13, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  394u,    0u),  /* idx= 14, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  405u,    0u),  /* idx= 15, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  416u,    0u),  /* idx= 16, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  93u,  426u,    0u),  /* idx= 17, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  436u,    0u),  /* idx= 18, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  445u,    0u),  /* idx= 19, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  453u,    0u),  /* idx= 20, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  461u,    0u),  /* idx= 21, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  468u,    0u),  /* idx= 22, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  476u,    0u),  /* idx= 23, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  482u,    0u),  /* idx= 24, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  489u,    0u),  /* idx= 25, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  495u,    0u),  /* idx= 26, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  501u,    0u),  /* idx= 27, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  506u,    0u),  /* idx= 28, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  512u,    0u),  /* idx= 29, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  517u,    0u),  /* idx= 30, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  64u,  522u,    0u),  /* idx= 31, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  62u,  527u,    0u),  /* idx= 32, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  61u,  531u,    0u),  /* idx= 33, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  535u,    0u),  /* idx= 34, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  58u,  540u,    0u),  /* idx= 35, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  57u,  544u,    0u),  /* idx= 36, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  56u,  547u,    0u),  /* idx= 37, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  54u,  551u,    0u),  /* idx= 38, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  53u,  555u,    0u),  /* idx= 39, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  52u,  558u,    0u),  /* idx= 40, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  51u,  561u,    0u),  /* idx= 41, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  50u,  565u,    0u),  /* idx= 42, ratio=-0.000000 */
    ALGO_RATIO_K_PACK(  49u,  568u,    0u),  /* idx= 43, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 213u,  102u,    0u),  /* idx= 44, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 200u,  139u,    0u),  /* idx= 45, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 189u,  173u,    0u),  /* idx= 46, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 179u,  202u,    0u),  /* idx= 47, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 170u,  229u,    0u),  /* idx= 48, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 161u,  254u,    0u),  /* idx= 49, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 154u,  276u,    0u),  /* idx= 50, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 147u,  296u,    0u),  /* idx= 51, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 141u,  315u,    0u),  /* idx= 52, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 135u,  332u,    0u),  /* idx= 53, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 129u,  348u,    0u),  /* idx= 54, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 124u,  362u,    0u),  /* idx= 55, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 120u,  376u,    0u),  /* idx= 56, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 116u,  389u,    0u),  /* idx= 57, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  400u,    0u),  /* idx= 58, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  412u,    0u),  /* idx= 59, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  422u,    0u),  /* idx= 60, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  432u,    0u),  /* idx= 61, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  441u,    0u),  /* idx= 62, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  450u,    0u),  /* idx= 63, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  458u,    0u),  /* idx= 64, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  466u,    0u),  /* idx= 65, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  473u,    0u),  /* idx= 66, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  480u,    0u),  /* idx= 67, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  487u,    0u),  /* idx= 68, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  493u,    0u),  /* idx= 69, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  499u,    0u),  /* idx= 70, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  505u,    0u),  /* idx= 71, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  510u,    0u),  /* idx= 72, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  515u,    0u),  /* idx= 73, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  520u,    0u),  /* idx= 74, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  525u,    0u),  /* idx= 75, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  530u,    0u),  /* idx= 76, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  534u,    0u),  /* idx= 77, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  64u,  539u,    0u),  /* idx= 78, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  543u,    0u),  /* idx= 79, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  62u,  547u,    0u),  /* idx= 80, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  550u,    0u),  /* idx= 81, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  554u,    0u),  /* idx= 82, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  58u,  557u,    0u),  /* idx= 83, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  57u,  561u,    0u),  /* idx= 84, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  56u,  564u,    0u),  /* idx= 85, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  55u,  567u,    0u),  /* idx= 86, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 226u,  118u,    0u),  /* idx= 87, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 213u,  154u, 1558u),  /* idx= 88, ratio=0.380482 */
    ALGO_RATIO_K_PACK( 201u,  186u,  983u),  /* idx= 89, ratio=0.239936 */
    ALGO_RATIO_K_PACK( 190u,  215u,  517u),  /* idx= 90, ratio=0.126300 */
    ALGO_RATIO_K_PACK( 181u,  241u,  147u),  /* idx= 91, ratio=0.035988 */
    ALGO_RATIO_K_PACK( 172u,  264u,    0u),  /* idx= 92, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 164u,  286u,    0u),  /* idx= 93, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 157u,  305u,    0u),  /* idx= 94, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 150u,  323u,    0u),  /* idx= 95, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 144u,  340u,    0u),  /* idx= 96, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 138u,  355u,    0u),  /* idx= 97, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 133u,  370u,    0u),  /* idx= 98, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 128u,  383u,    0u),  /* idx= 99, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 124u,  395u,    0u),  /* idx=100, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 119u,  407u,    0u),  /* idx=101, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 115u,  418u,    0u),  /* idx=102, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 112u,  428u,    0u),  /* idx=103, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  437u,    0u),  /* idx=104, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  446u,    0u),  /* idx=105, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  455u,    0u),  /* idx=106, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  463u,    0u),  /* idx=107, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  470u,    0u),  /* idx=108, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  477u,    0u),  /* idx=109, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  484u,    0u),  /* idx=110, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  491u,    0u),  /* idx=111, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  497u,    0u),  /* idx=112, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  503u,    0u),  /* idx=113, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  508u,    0u),  /* idx=114, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  514u,    0u),  /* idx=115, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  519u,    0u),  /* idx=116, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  524u,    0u),  /* idx=117, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  529u,    0u),  /* idx=118, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  533u,    0u),  /* idx=119, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  537u,    0u),  /* idx=120, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  542u,    0u),  /* idx=121, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  546u,    0u),  /* idx=122, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  549u,    0u),  /* idx=123, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  553u,    0u),  /* idx=124, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  64u,  557u,    0u),  /* idx=125, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  560u,    0u),  /* idx=126, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  61u,  563u,    0u),  /* idx=127, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  567u,    0u),  /* idx=128, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 239u,  134u,    0u),  /* idx=129, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 225u,  168u, 1552u),  /* idx=130, ratio=0.378961 */
    ALGO_RATIO_K_PACK( 213u,  199u, 2483u),  /* idx=131, ratio=0.606441 */
    ALGO_RATIO_K_PACK( 202u,  227u, 2004u),  /* idx=132, ratio=0.489319 */
    ALGO_RATIO_K_PACK( 192u,  252u, 1570u),  /* idx=133, ratio=0.383307 */
    ALGO_RATIO_K_PACK( 182u,  275u, 1174u),  /* idx=134, ratio=0.286644 */
    ALGO_RATIO_K_PACK( 174u,  295u,  863u),  /* idx=135, ratio=0.210743 */
    ALGO_RATIO_K_PACK( 167u,  314u,  640u),  /* idx=136, ratio=0.156387 */
    ALGO_RATIO_K_PACK( 160u,  332u,  485u),  /* idx=137, ratio=0.118349 */
    ALGO_RATIO_K_PACK( 153u,  348u,  370u),  /* idx=138, ratio=0.090438 */
    ALGO_RATIO_K_PACK( 147u,  363u,  287u),  /* idx=139, ratio=0.070144 */
    ALGO_RATIO_K_PACK( 142u,  377u,  188u),  /* idx=140, ratio=0.045972 */
    ALGO_RATIO_K_PACK( 136u,  390u,   98u),  /* idx=141, ratio=0.023820 */
    ALGO_RATIO_K_PACK( 132u,  402u,   24u),  /* idx=142, ratio=0.005741 */
    ALGO_RATIO_K_PACK( 127u,  413u,    0u),  /* idx=143, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 123u,  423u,    0u),  /* idx=144, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 119u,  433u,    0u),  /* idx=145, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 115u,  443u,    0u),  /* idx=146, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 112u,  451u,    0u),  /* idx=147, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  460u,    0u),  /* idx=148, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  467u,    0u),  /* idx=149, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  475u,    0u),  /* idx=150, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  482u,    0u),  /* idx=151, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  489u,    0u),  /* idx=152, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  495u,    0u),  /* idx=153, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  501u,    0u),  /* idx=154, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  507u,    0u),  /* idx=155, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  512u,    0u),  /* idx=156, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  517u,    0u),  /* idx=157, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  523u,    0u),  /* idx=158, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  527u,    0u),  /* idx=159, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  532u,    0u),  /* idx=160, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  536u,    0u),  /* idx=161, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  541u,    0u),  /* idx=162, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  545u,    0u),  /* idx=163, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  549u,    0u),  /* idx=164, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  552u,    0u),  /* idx=165, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  556u,    0u),  /* idx=166, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  559u,    0u),  /* idx=167, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  563u,    0u),  /* idx=168, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  566u,    0u),  /* idx=169, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 251u,  149u,    0u),  /* idx=170, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 237u,  182u, 1413u),  /* idx=171, ratio=0.345032 */
    ALGO_RATIO_K_PACK( 224u,  212u, 2302u),  /* idx=172, ratio=0.562206 */
    ALGO_RATIO_K_PACK( 212u,  239u, 2914u),  /* idx=173, ratio=0.711482 */
    ALGO_RATIO_K_PACK( 202u,  263u, 2677u),  /* idx=174, ratio=0.653822 */
    ALGO_RATIO_K_PACK( 193u,  285u, 2277u),  /* idx=175, ratio=0.555949 */
    ALGO_RATIO_K_PACK( 184u,  305u, 1902u),  /* idx=176, ratio=0.464470 */
    ALGO_RATIO_K_PACK( 176u,  323u, 1571u),  /* idx=177, ratio=0.383591 */
    ALGO_RATIO_K_PACK( 169u,  340u, 1317u),  /* idx=178, ratio=0.321660 */
    ALGO_RATIO_K_PACK( 162u,  356u, 1129u),  /* idx=179, ratio=0.275603 */
    ALGO_RATIO_K_PACK( 156u,  370u,  987u),  /* idx=180, ratio=0.240969 */
    ALGO_RATIO_K_PACK( 150u,  384u,  849u),  /* idx=181, ratio=0.207389 */
    ALGO_RATIO_K_PACK( 145u,  396u,  724u),  /* idx=182, ratio=0.176921 */
    ALGO_RATIO_K_PACK( 140u,  408u,  620u),  /* idx=183, ratio=0.151345 */
    ALGO_RATIO_K_PACK( 135u,  419u,  524u),  /* idx=184, ratio=0.127965 */
    ALGO_RATIO_K_PACK( 130u,  429u,  434u),  /* idx=185, ratio=0.106000 */
    ALGO_RATIO_K_PACK( 126u,  439u,  338u),  /* idx=186, ratio=0.082547 */
    ALGO_RATIO_K_PACK( 122u,  448u,  205u),  /* idx=187, ratio=0.050012 */
    ALGO_RATIO_K_PACK( 119u,  456u,   85u),  /* idx=188, ratio=0.020824 */
    ALGO_RATIO_K_PACK( 115u,  465u,    0u),  /* idx=189, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 112u,  472u,    0u),  /* idx=190, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 109u,  479u,    0u),  /* idx=191, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 106u,  486u,    0u),  /* idx=192, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  493u,    0u),  /* idx=193, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  499u,    0u),  /* idx=194, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  505u,    0u),  /* idx=195, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  511u,    0u),  /* idx=196, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  516u,    0u),  /* idx=197, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  521u,    0u),  /* idx=198, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  526u,    0u),  /* idx=199, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  531u,    0u),  /* idx=200, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  535u,    0u),  /* idx=201, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  540u,    0u),  /* idx=202, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  544u,    0u),  /* idx=203, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  548u,    0u),  /* idx=204, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  552u,    0u),  /* idx=205, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  555u,    0u),  /* idx=206, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  559u,    0u),  /* idx=207, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  562u,    0u),  /* idx=208, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  566u,    0u),  /* idx=209, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 263u,  164u,    0u),  /* idx=210, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 248u,  196u, 1297u),  /* idx=211, ratio=0.316679 */
    ALGO_RATIO_K_PACK( 235u,  224u, 2146u),  /* idx=212, ratio=0.523985 */
    ALGO_RATIO_K_PACK( 223u,  250u, 2745u),  /* idx=213, ratio=0.670235 */
    ALGO_RATIO_K_PACK( 212u,  273u, 3190u),  /* idx=214, ratio=0.778941 */
    ALGO_RATIO_K_PACK( 202u,  295u, 3146u),  /* idx=215, ratio=0.768284 */
    ALGO_RATIO_K_PACK( 193u,  314u, 2767u),  /* idx=216, ratio=0.675823 */
    ALGO_RATIO_K_PACK( 185u,  332u, 2443u),  /* idx=217, ratio=0.596610 */
    ALGO_RATIO_K_PACK( 178u,  348u, 2111u),  /* idx=218, ratio=0.515614 */
    ALGO_RATIO_K_PACK( 171u,  364u, 1847u),  /* idx=219, ratio=0.450953 */
    ALGO_RATIO_K_PACK( 164u,  378u, 1648u),  /* idx=220, ratio=0.402388 */
    ALGO_RATIO_K_PACK( 158u,  391u, 1474u),  /* idx=221, ratio=0.359987 */
    ALGO_RATIO_K_PACK( 152u,  403u, 1323u),  /* idx=222, ratio=0.322999 */
    ALGO_RATIO_K_PACK( 147u,  414u, 1188u),  /* idx=223, ratio=0.290056 */
    ALGO_RATIO_K_PACK( 142u,  425u, 1060u),  /* idx=224, ratio=0.258825 */
    ALGO_RATIO_K_PACK( 138u,  435u,  919u),  /* idx=225, ratio=0.224434 */
    ALGO_RATIO_K_PACK( 133u,  444u,  754u),  /* idx=226, ratio=0.184101 */
    ALGO_RATIO_K_PACK( 129u,  453u,  602u),  /* idx=227, ratio=0.147064 */
    ALGO_RATIO_K_PACK( 125u,  461u,  466u),  /* idx=228, ratio=0.113770 */
    ALGO_RATIO_K_PACK( 122u,  469u,  338u),  /* idx=229, ratio=0.082660 */
    ALGO_RATIO_K_PACK( 118u,  477u,  224u),  /* idx=230, ratio=0.054769 */
    ALGO_RATIO_K_PACK( 115u,  484u,  117u),  /* idx=231, ratio=0.028621 */
    ALGO_RATIO_K_PACK( 112u,  491u,   17u),  /* idx=232, ratio=0.004042 */
    ALGO_RATIO_K_PACK( 109u,  497u,    0u),  /* idx=233, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 106u,  503u,    0u),  /* idx=234, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  509u,    0u),  /* idx=235, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  514u,    0u),  /* idx=236, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  520u,    0u),  /* idx=237, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  525u,    0u),  /* idx=238, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  529u,    0u),  /* idx=239, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  534u,    0u),  /* idx=240, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  538u,    0u),  /* idx=241, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  543u,    0u),  /* idx=242, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  547u,    0u),  /* idx=243, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  551u,    0u),  /* idx=244, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  554u,    0u),  /* idx=245, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  558u,    0u),  /* idx=246, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  562u,    0u),  /* idx=247, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  565u,    0u),  /* idx=248, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 274u,  178u,    0u),  /* idx=249, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 259u,  209u, 1198u),  /* idx=250, ratio=0.292632 */
    ALGO_RATIO_K_PACK( 246u,  236u, 2009u),  /* idx=251, ratio=0.490630 */
    ALGO_RATIO_K_PACK( 233u,  261u, 2594u),  /* idx=252, ratio=0.633509 */
    ALGO_RATIO_K_PACK( 222u,  284u, 3036u),  /* idx=253, ratio=0.741474 */
    ALGO_RATIO_K_PACK( 212u,  304u, 3382u),  /* idx=254, ratio=0.825928 */
    ALGO_RATIO_K_PACK( 203u,  323u, 3487u),  /* idx=255, ratio=0.851465 */
    ALGO_RATIO_K_PACK( 194u,  340u, 3126u),  /* idx=256, ratio=0.763419 */
    ALGO_RATIO_K_PACK( 186u,  356u, 2836u),  /* idx=257, ratio=0.692513 */
    ALGO_RATIO_K_PACK( 179u,  371u, 2532u),  /* idx=258, ratio=0.618305 */
    ALGO_RATIO_K_PACK( 172u,  385u, 2273u),  /* idx=259, ratio=0.555066 */
    ALGO_RATIO_K_PACK( 166u,  397u, 2065u),  /* idx=260, ratio=0.504256 */
    ALGO_RATIO_K_PACK( 160u,  409u, 1890u),  /* idx=261, ratio=0.461427 */
    ALGO_RATIO_K_PACK( 155u,  420u, 1718u),  /* idx=262, ratio=0.419599 */
    ALGO_RATIO_K_PACK( 150u,  431u, 1530u),  /* idx=263, ratio=0.373720 */
    ALGO_RATIO_K_PACK( 145u,  440u, 1330u),  /* idx=264, ratio=0.324728 */
    ALGO_RATIO_K_PACK( 140u,  450u, 1145u),  /* idx=265, ratio=0.279720 */
    ALGO_RATIO_K_PACK( 136u,  458u,  979u),  /* idx=266, ratio=0.239047 */
    ALGO_RATIO_K_PACK( 132u,  466u,  827u),  /* idx=267, ratio=0.201908 */
    ALGO_RATIO_K_PACK( 128u,  474u,  686u),  /* idx=268, ratio=0.167549 */
    ALGO_RATIO_K_PACK( 125u,  481u,  558u),  /* idx=269, ratio=0.136186 */
    ALGO_RATIO_K_PACK( 121u,  488u,  437u),  /* idx=270, ratio=0.106694 */
    ALGO_RATIO_K_PACK( 118u,  495u,  325u),  /* idx=271, ratio=0.079380 */
    ALGO_RATIO_K_PACK( 115u,  501u,  218u),  /* idx=272, ratio=0.053127 */
    ALGO_RATIO_K_PACK( 112u,  507u,  116u),  /* idx=273, ratio=0.028292 */
    ALGO_RATIO_K_PACK( 109u,  513u,   17u),  /* idx=274, ratio=0.004255 */
    ALGO_RATIO_K_PACK( 107u,  518u,    0u),  /* idx=275, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  523u,    0u),  /* idx=276, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  528u,    0u),  /* idx=277, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  533u,    0u),  /* idx=278, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  537u,    0u),  /* idx=279, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  542u,    0u),  /* idx=280, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  546u,    0u),  /* idx=281, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  550u,    0u),  /* idx=282, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  554u,    0u),  /* idx=283, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  557u,    0u),  /* idx=284, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  561u,    0u),  /* idx=285, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  564u,    0u),  /* idx=286, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 286u,  192u,    0u),  /* idx=287, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 270u,  222u, 1114u),  /* idx=288, ratio=0.271980 */
    ALGO_RATIO_K_PACK( 256u,  248u, 1889u),  /* idx=289, ratio=0.461268 */
    ALGO_RATIO_K_PACK( 244u,  272u, 2459u),  /* idx=290, ratio=0.600599 */
    ALGO_RATIO_K_PACK( 232u,  294u, 2897u),  /* idx=291, ratio=0.707445 */
    ALGO_RATIO_K_PACK( 222u,  314u, 3243u),  /* idx=292, ratio=0.791981 */
    ALGO_RATIO_K_PACK( 212u,  332u, 3524u),  /* idx=293, ratio=0.860533 */
    ALGO_RATIO_K_PACK( 203u,  349u, 3754u),  /* idx=294, ratio=0.916768 */
    ALGO_RATIO_K_PACK( 195u,  364u, 3409u),  /* idx=295, ratio=0.832538 */
    ALGO_RATIO_K_PACK( 188u,  379u, 3123u),  /* idx=296, ratio=0.762737 */
    ALGO_RATIO_K_PACK( 181u,  392u, 2856u),  /* idx=297, ratio=0.697439 */
    ALGO_RATIO_K_PACK( 174u,  404u, 2610u),  /* idx=298, ratio=0.637327 */
    ALGO_RATIO_K_PACK( 168u,  416u, 2409u),  /* idx=299, ratio=0.588171 */
    ALGO_RATIO_K_PACK( 162u,  426u, 2167u),  /* idx=300, ratio=0.529294 */
    ALGO_RATIO_K_PACK( 157u,  437u, 1930u),  /* idx=301, ratio=0.471324 */
    ALGO_RATIO_K_PACK( 152u,  446u, 1714u),  /* idx=302, ratio=0.418441 */
    ALGO_RATIO_K_PACK( 147u,  455u, 1515u),  /* idx=303, ratio=0.370001 */
    ALGO_RATIO_K_PACK( 143u,  463u, 1333u),  /* idx=304, ratio=0.325504 */
    ALGO_RATIO_K_PACK( 139u,  471u, 1167u),  /* idx=305, ratio=0.285027 */
    ALGO_RATIO_K_PACK( 135u,  479u, 1015u),  /* idx=306, ratio=0.247852 */
    ALGO_RATIO_K_PACK( 131u,  486u,  873u),  /* idx=307, ratio=0.213279 */
    ALGO_RATIO_K_PACK( 127u,  493u,  742u),  /* idx=308, ratio=0.181112 */
    ALGO_RATIO_K_PACK( 124u,  499u,  617u),  /* idx=309, ratio=0.150686 */
    ALGO_RATIO_K_PACK( 121u,  505u,  499u),  /* idx=310, ratio=0.121845 */
    ALGO_RATIO_K_PACK( 118u,  511u,  385u),  /* idx=311, ratio=0.093911 */
    ALGO_RATIO_K_PACK( 115u,  516u,  276u),  /* idx=312, ratio=0.067309 */
    ALGO_RATIO_K_PACK( 112u,  522u,  172u),  /* idx=313, ratio=0.041967 */
    ALGO_RATIO_K_PACK( 110u,  527u,   68u),  /* idx=314, ratio=0.016593 */
    ALGO_RATIO_K_PACK( 107u,  532u,    0u),  /* idx=315, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  536u,    0u),  /* idx=316, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  541u,    0u),  /* idx=317, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  545u,    0u),  /* idx=318, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  549u,    0u),  /* idx=319, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  553u,    0u),  /* idx=320, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  557u,    0u),  /* idx=321, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  560u,    0u),  /* idx=322, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  564u,    0u),  /* idx=323, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 297u,  206u,    0u),  /* idx=324, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 281u,  234u, 1040u),  /* idx=325, ratio=0.254050 */
    ALGO_RATIO_K_PACK( 266u,  260u, 1782u),  /* idx=326, ratio=0.435221 */
    ALGO_RATIO_K_PACK( 253u,  283u, 2338u),  /* idx=327, ratio=0.570939 */
    ALGO_RATIO_K_PACK( 242u,  304u, 2770u),  /* idx=328, ratio=0.676403 */
    ALGO_RATIO_K_PACK( 231u,  323u, 3115u),  /* idx=329, ratio=0.760714 */
    ALGO_RATIO_K_PACK( 221u,  341u, 3397u),  /* idx=330, ratio=0.829657 */
    ALGO_RATIO_K_PACK( 212u,  357u, 3633u),  /* idx=331, ratio=0.887082 */
    ALGO_RATIO_K_PACK( 203u,  372u, 3832u),  /* idx=332, ratio=0.935653 */
    ALGO_RATIO_K_PACK( 196u,  386u, 3573u),  /* idx=333, ratio=0.872618 */
    ALGO_RATIO_K_PACK( 189u,  399u, 3335u),  /* idx=334, ratio=0.814334 */
    ALGO_RATIO_K_PACK( 182u,  411u, 3126u),  /* idx=335, ratio=0.763348 */
    ALGO_RATIO_K_PACK( 176u,  422u, 2826u),  /* idx=336, ratio=0.690038 */
    ALGO_RATIO_K_PACK( 170u,  432u, 2554u),  /* idx=337, ratio=0.623622 */
    ALGO_RATIO_K_PACK( 164u,  442u, 2302u),  /* idx=338, ratio=0.562266 */
    ALGO_RATIO_K_PACK( 159u,  451u, 2071u),  /* idx=339, ratio=0.505830 */
    ALGO_RATIO_K_PACK( 154u,  460u, 1861u),  /* idx=340, ratio=0.454497 */
    ALGO_RATIO_K_PACK( 150u,  468u, 1667u),  /* idx=341, ratio=0.406997 */
    ALGO_RATIO_K_PACK( 145u,  476u, 1488u),  /* idx=342, ratio=0.363405 */
    ALGO_RATIO_K_PACK( 141u,  483u, 1325u),  /* idx=343, ratio=0.323500 */
    ALGO_RATIO_K_PACK( 137u,  490u, 1172u),  /* idx=344, ratio=0.286145 */
    ALGO_RATIO_K_PACK( 134u,  497u, 1028u),  /* idx=345, ratio=0.251148 */
    ALGO_RATIO_K_PACK( 130u,  503u,  892u),  /* idx=346, ratio=0.217824 */
    ALGO_RATIO_K_PACK( 127u,  509u,  764u),  /* idx=347, ratio=0.186580 */
    ALGO_RATIO_K_PACK( 124u,  515u,  640u),  /* idx=348, ratio=0.156174 */
    ALGO_RATIO_K_PACK( 121u,  520u,  520u),  /* idx=349, ratio=0.127055 */
    ALGO_RATIO_K_PACK( 118u,  525u,  406u),  /* idx=350, ratio=0.099152 */
    ALGO_RATIO_K_PACK( 115u,  530u,  294u),  /* idx=351, ratio=0.071740 */
    ALGO_RATIO_K_PACK( 112u,  535u,  190u),  /* idx=352, ratio=0.046367 */
    ALGO_RATIO_K_PACK( 110u,  539u,   91u),  /* idx=353, ratio=0.022229 */
    ALGO_RATIO_K_PACK( 107u,  544u,    0u),  /* idx=354, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  548u,    0u),  /* idx=355, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  552u,    0u),  /* idx=356, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  556u,    0u),  /* idx=357, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  559u,    0u),  /* idx=358, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  563u,    0u),  /* idx=359, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 307u,  219u,    0u),  /* idx=360, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 291u,  246u,  976u),  /* idx=361, ratio=0.238338 */
    ALGO_RATIO_K_PACK( 276u,  271u, 1687u),  /* idx=362, ratio=0.411959 */
    ALGO_RATIO_K_PACK( 263u,  293u, 2228u),  /* idx=363, ratio=0.544071 */
    ALGO_RATIO_K_PACK( 251u,  314u, 2653u),  /* idx=364, ratio=0.647970 */
    ALGO_RATIO_K_PACK( 240u,  332u, 2997u),  /* idx=365, ratio=0.731823 */
    ALGO_RATIO_K_PACK( 230u,  349u, 3280u),  /* idx=366, ratio=0.800919 */
    ALGO_RATIO_K_PACK( 220u,  365u, 3517u),  /* idx=367, ratio=0.858840 */
    ALGO_RATIO_K_PACK( 212u,  379u, 3719u),  /* idx=368, ratio=0.908094 */
    ALGO_RATIO_K_PACK( 204u,  393u, 3892u),  /* idx=369, ratio=0.950490 */
    ALGO_RATIO_K_PACK( 196u,  405u, 3655u),  /* idx=370, ratio=0.892654 */
    ALGO_RATIO_K_PACK( 189u,  417u, 3432u),  /* idx=371, ratio=0.838109 */
    ALGO_RATIO_K_PACK( 183u,  428u, 3211u),  /* idx=372, ratio=0.784095 */
    ALGO_RATIO_K_PACK( 177u,  438u, 2907u),  /* idx=373, ratio=0.709833 */
    ALGO_RATIO_K_PACK( 171u,  448u, 2644u),  /* idx=374, ratio=0.645612 */
    ALGO_RATIO_K_PACK( 166u,  457u, 2402u),  /* idx=375, ratio=0.586663 */
    ALGO_RATIO_K_PACK( 161u,  465u, 2182u),  /* idx=376, ratio=0.532836 */
    ALGO_RATIO_K_PACK( 156u,  473u, 1978u),  /* idx=377, ratio=0.482977 */
    ALGO_RATIO_K_PACK( 152u,  481u, 1789u),  /* idx=378, ratio=0.436765 */
    ALGO_RATIO_K_PACK( 147u,  488u, 1615u),  /* idx=379, ratio=0.394468 */
    ALGO_RATIO_K_PACK( 143u,  495u, 1452u),  /* idx=380, ratio=0.354549 */
    ALGO_RATIO_K_PACK( 140u,  501u, 1300u),  /* idx=381, ratio=0.317341 */
    ALGO_RATIO_K_PACK( 136u,  507u, 1153u),  /* idx=382, ratio=0.281672 */
    ALGO_RATIO_K_PACK( 132u,  513u, 1013u),  /* idx=383, ratio=0.247366 */
    ALGO_RATIO_K_PACK( 129u,  519u,  880u),  /* idx=384, ratio=0.214908 */
    ALGO_RATIO_K_PACK( 126u,  524u,  752u),  /* idx=385, ratio=0.183639 */
    ALGO_RATIO_K_PACK( 123u,  529u,  629u),  /* idx=386, ratio=0.153578 */
    ALGO_RATIO_K_PACK( 120u,  534u,  512u),  /* idx=387, ratio=0.124926 */
    ALGO_RATIO_K_PACK( 117u,  538u,  400u),  /* idx=388, ratio=0.097739 */
    ALGO_RATIO_K_PACK( 115u,  543u,  297u),  /* idx=389, ratio=0.072494 */
    ALGO_RATIO_K_PACK( 112u,  547u,  201u),  /* idx=390, ratio=0.048969 */
    ALGO_RATIO_K_PACK( 110u,  551u,  114u),  /* idx=391, ratio=0.027745 */
    ALGO_RATIO_K_PACK( 108u,  555u,   37u),  /* idx=392, ratio=0.009046 */
    ALGO_RATIO_K_PACK( 105u,  559u,    0u),  /* idx=393, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  562u,    0u),  /* idx=394, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 317u,  232u,    0u),  /* idx=395, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 301u,  258u,  919u),  /* idx=396, ratio=0.224457 */
    ALGO_RATIO_K_PACK( 286u,  282u, 1601u),  /* idx=397, ratio=0.391057 */
    ALGO_RATIO_K_PACK( 272u,  303u, 2128u),  /* idx=398, ratio=0.519618 */
    ALGO_RATIO_K_PACK( 260u,  323u, 2546u),  /* idx=399, ratio=0.621832 */
    ALGO_RATIO_K_PACK( 249u,  341u, 2887u),  /* idx=400, ratio=0.705045 */
    ALGO_RATIO_K_PACK( 238u,  358u, 3170u),  /* idx=401, ratio=0.774106 */
    ALGO_RATIO_K_PACK( 229u,  373u, 3408u),  /* idx=402, ratio=0.832341 */
    ALGO_RATIO_K_PACK( 220u,  387u, 3612u),  /* idx=403, ratio=0.882111 */
    ALGO_RATIO_K_PACK( 212u,  400u, 3788u),  /* idx=404, ratio=0.925137 */
    ALGO_RATIO_K_PACK( 204u,  412u, 3942u),  /* idx=405, ratio=0.962703 */
    ALGO_RATIO_K_PACK( 197u,  423u, 3724u),  /* idx=406, ratio=0.909357 */
    ALGO_RATIO_K_PACK( 190u,  434u, 3514u),  /* idx=407, ratio=0.858152 */
    ALGO_RATIO_K_PACK( 184u,  444u, 3221u),  /* idx=408, ratio=0.786540 */
    ALGO_RATIO_K_PACK( 178u,  453u, 2955u),  /* idx=409, ratio=0.721573 */
    ALGO_RATIO_K_PACK( 173u,  462u, 2706u),  /* idx=410, ratio=0.660740 */
    ALGO_RATIO_K_PACK( 167u,  470u, 2478u),  /* idx=411, ratio=0.605142 */
    ALGO_RATIO_K_PACK( 163u,  478u, 2266u),  /* idx=412, ratio=0.553444 */
    ALGO_RATIO_K_PACK( 158u,  485u, 2070u),  /* idx=413, ratio=0.505403 */
    ALGO_RATIO_K_PACK( 154u,  492u, 1887u),  /* idx=414, ratio=0.460904 */
    ALGO_RATIO_K_PACK( 149u,  499u, 1714u),  /* idx=415, ratio=0.418450 */
    ALGO_RATIO_K_PACK( 145u,  505u, 1552u),  /* idx=416, ratio=0.378893 */
    ALGO_RATIO_K_PACK( 142u,  511u, 1395u),  /* idx=417, ratio=0.340579 */
    ALGO_RATIO_K_PACK( 138u,  517u, 1247u),  /* idx=418, ratio=0.304536 */
    ALGO_RATIO_K_PACK( 135u,  522u, 1105u),  /* idx=419, ratio=0.269730 */
    ALGO_RATIO_K_PACK( 131u,  528u,  970u),  /* idx=420, ratio=0.236819 */
    ALGO_RATIO_K_PACK( 128u,  532u,  841u),  /* idx=421, ratio=0.205349 */
    ALGO_RATIO_K_PACK( 125u,  537u,  720u),  /* idx=422, ratio=0.175702 */
    ALGO_RATIO_K_PACK( 123u,  542u,  605u),  /* idx=423, ratio=0.147711 */
    ALGO_RATIO_K_PACK( 120u,  546u,  499u),  /* idx=424, ratio=0.121811 */
    ALGO_RATIO_K_PACK( 117u,  550u,  401u),  /* idx=425, ratio=0.097943 */
    ALGO_RATIO_K_PACK( 115u,  554u,  315u),  /* idx=426, ratio=0.076877 */
    ALGO_RATIO_K_PACK( 112u,  558u,  171u),  /* idx=427, ratio=0.041876 */
    ALGO_RATIO_K_PACK( 110u,  562u,    0u),  /* idx=428, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 327u,  244u,    0u),  /* idx=429, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 311u,  269u,  869u),  /* idx=430, ratio=0.212103 */
    ALGO_RATIO_K_PACK( 295u,  292u, 1524u),  /* idx=431, ratio=0.372174 */
    ALGO_RATIO_K_PACK( 282u,  313u, 2036u),  /* idx=432, ratio=0.497268 */
    ALGO_RATIO_K_PACK( 269u,  332u, 2448u),  /* idx=433, ratio=0.597720 */
    ALGO_RATIO_K_PACK( 257u,  350u, 2785u),  /* idx=434, ratio=0.680158 */
    ALGO_RATIO_K_PACK( 247u,  366u, 3067u),  /* idx=435, ratio=0.749030 */
    ALGO_RATIO_K_PACK( 237u,  380u, 3306u),  /* idx=436, ratio=0.807428 */
    ALGO_RATIO_K_PACK( 228u,  394u, 3512u),  /* idx=437, ratio=0.857574 */
    ALGO_RATIO_K_PACK( 219u,  407u, 3690u),  /* idx=438, ratio=0.901102 */
    ALGO_RATIO_K_PACK( 212u,  419u, 3846u),  /* idx=439, ratio=0.939240 */
    ALGO_RATIO_K_PACK( 204u,  430u, 3984u),  /* idx=440, ratio=0.972931 */
    ALGO_RATIO_K_PACK( 197u,  440u, 3782u),  /* idx=441, ratio=0.923496 */
    ALGO_RATIO_K_PACK( 191u,  450u, 3527u),  /* idx=442, ratio=0.861342 */
    ALGO_RATIO_K_PACK( 185u,  459u, 3232u),  /* idx=443, ratio=0.789300 */
    ALGO_RATIO_K_PACK( 179u,  467u, 2981u),  /* idx=444, ratio=0.728047 */
    ALGO_RATIO_K_PACK( 174u,  475u, 2749u),  /* idx=445, ratio=0.671280 */
    ALGO_RATIO_K_PACK( 169u,  483u, 2531u),  /* idx=446, ratio=0.618104 */
    ALGO_RATIO_K_PACK( 164u,  490u, 2329u),  /* idx=447, ratio=0.568749 */
    ALGO_RATIO_K_PACK( 160u,  497u, 2139u),  /* idx=448, ratio=0.522421 */
    ALGO_RATIO_K_PACK( 155u,  503u, 1958u),  /* idx=449, ratio=0.478135 */
    ALGO_RATIO_K_PACK( 151u,  509u, 1787u),  /* idx=450, ratio=0.436300 */
    ALGO_RATIO_K_PACK( 147u,  515u, 1623u),  /* idx=451, ratio=0.396385 */
    ALGO_RATIO_K_PACK( 144u,  521u, 1469u),  /* idx=452, ratio=0.358627 */
    ALGO_RATIO_K_PACK( 140u,  526u, 1320u),  /* idx=453, ratio=0.322284 */
    ALGO_RATIO_K_PACK( 137u,  531u, 1179u),  /* idx=454, ratio=0.288016 */
    ALGO_RATIO_K_PACK( 134u,  536u, 1046u),  /* idx=455, ratio=0.255408 */
    ALGO_RATIO_K_PACK( 131u,  541u,  920u),  /* idx=456, ratio=0.224724 */
    ALGO_RATIO_K_PACK( 128u,  545u,  758u),  /* idx=457, ratio=0.185189 */
    ALGO_RATIO_K_PACK( 125u,  549u,  553u),  /* idx=458, ratio=0.134941 */
    ALGO_RATIO_K_PACK( 122u,  553u,  358u),  /* idx=459, ratio=0.087472 */
    ALGO_RATIO_K_PACK( 120u,  557u,  174u),  /* idx=460, ratio=0.042559 */
    ALGO_RATIO_K_PACK( 117u,  561u,    0u),  /* idx=461, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 337u,  256u,    0u),  /* idx=462, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 320u,  281u,  823u),  /* idx=463, ratio=0.201038 */
    ALGO_RATIO_K_PACK( 305u,  303u, 1454u),  /* idx=464, ratio=0.355031 */
    ALGO_RATIO_K_PACK( 290u,  323u, 1952u),  /* idx=465, ratio=0.476762 */
    ALGO_RATIO_K_PACK( 278u,  341u, 2356u),  /* idx=466, ratio=0.575409 */
    ALGO_RATIO_K_PACK( 266u,  358u, 2690u),  /* idx=467, ratio=0.656968 */
    ALGO_RATIO_K_PACK( 255u,  374u, 2971u),  /* idx=468, ratio=0.725527 */
    ALGO_RATIO_K_PACK( 245u,  388u, 3210u),  /* idx=469, ratio=0.783963 */
    ALGO_RATIO_K_PACK( 236u,  401u, 3417u),  /* idx=470, ratio=0.834366 */
    ALGO_RATIO_K_PACK( 227u,  414u, 3597u),  /* idx=471, ratio=0.878284 */
    ALGO_RATIO_K_PACK( 219u,  425u, 3755u),  /* idx=472, ratio=0.916893 */
    ALGO_RATIO_K_PACK( 211u,  436u, 3895u),  /* idx=473, ratio=0.951102 */
    ALGO_RATIO_K_PACK( 204u,  446u, 4020u),  /* idx=474, ratio=0.981622 */
    ALGO_RATIO_K_PACK( 198u,  455u, 3785u),  /* idx=475, ratio=0.924228 */
    ALGO_RATIO_K_PACK( 192u,  464u, 3473u),  /* idx=476, ratio=0.848068 */
    ALGO_RATIO_K_PACK( 186u,  472u, 3227u),  /* idx=477, ratio=0.788012 */
    ALGO_RATIO_K_PACK( 180u,  480u, 2993u),  /* idx=478, ratio=0.730926 */
    ALGO_RATIO_K_PACK( 175u,  488u, 2773u),  /* idx=479, ratio=0.677267 */
    ALGO_RATIO_K_PACK( 170u,  495u, 2566u),  /* idx=480, ratio=0.626648 */
    ALGO_RATIO_K_PACK( 166u,  501u, 2370u),  /* idx=481, ratio=0.578718 */
    ALGO_RATIO_K_PACK( 161u,  508u, 2185u),  /* idx=482, ratio=0.533541 */
    ALGO_RATIO_K_PACK( 157u,  513u, 2006u),  /* idx=483, ratio=0.489880 */
    ALGO_RATIO_K_PACK( 153u,  519u, 1837u),  /* idx=484, ratio=0.448700 */
    ALGO_RATIO_K_PACK( 149u,  525u, 1676u),  /* idx=485, ratio=0.409344 */
    ALGO_RATIO_K_PACK( 146u,  530u, 1484u),  /* idx=486, ratio=0.362343 */
    ALGO_RATIO_K_PACK( 142u,  535u, 1232u),  /* idx=487, ratio=0.300734 */
    ALGO_RATIO_K_PACK( 139u,  539u,  995u),  /* idx=488, ratio=0.242912 */
    ALGO_RATIO_K_PACK( 136u,  544u,  772u),  /* idx=489, ratio=0.188537 */
    ALGO_RATIO_K_PACK( 133u,  548u,  562u),  /* idx=490, ratio=0.137309 */
    ALGO_RATIO_K_PACK( 130u,  552u,  364u),  /* idx=491, ratio=0.088964 */
    ALGO_RATIO_K_PACK( 127u,  556u,  177u),  /* idx=492, ratio=0.043265 */
    ALGO_RATIO_K_PACK( 124u,  560u,    0u),  /* idx=493, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 347u,  268u,    0u),  /* idx=494, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 329u,  292u,  782u),  /* idx=495, ratio=0.191071 */
    ALGO_RATIO_K_PACK( 314u,  313u, 1390u),  /* idx=496, ratio=0.339397 */
    ALGO_RATIO_K_PACK( 299u,  332u, 1875u),  /* idx=497, ratio=0.457880 */
    ALGO_RATIO_K_PACK( 286u,  350u, 2272u),  /* idx=498, ratio=0.554703 */
    ALGO_RATIO_K_PACK( 274u,  366u, 2602u),  /* idx=499, ratio=0.635308 */
    ALGO_RATIO_K_PACK( 263u,  381u, 2881u),  /* idx=500, ratio=0.703454 */
    ALGO_RATIO_K_PACK( 253u,  395u, 3120u),  /* idx=501, ratio=0.761824 */
    ALGO_RATIO_K_PACK( 243u,  408u, 3327u),  /* idx=502, ratio=0.812380 */
    ALGO_RATIO_K_PACK( 234u,  420u, 3508u),  /* idx=503, ratio=0.856593 */
    ALGO_RATIO_K_PACK( 226u,  431u, 3667u),  /* idx=504, ratio=0.895586 */
    ALGO_RATIO_K_PACK( 219u,  442u, 3809u),  /* idx=505, ratio=0.930231 */
    ALGO_RATIO_K_PACK( 211u,  452u, 3936u),  /* idx=506, ratio=0.961219 */
    ALGO_RATIO_K_PACK( 205u,  461u, 3988u),  /* idx=507, ratio=0.973989 */
    ALGO_RATIO_K_PACK( 198u,  469u, 3691u),  /* idx=508, ratio=0.901400 */
    ALGO_RATIO_K_PACK( 192u,  477u, 3441u),  /* idx=509, ratio=0.840322 */
    ALGO_RATIO_K_PACK( 187u,  485u, 3210u),  /* idx=510, ratio=0.783866 */
    ALGO_RATIO_K_PACK( 181u,  492u, 2990u),  /* idx=511, ratio=0.730261 */
    ALGO_RATIO_K_PACK( 176u,  499u, 2782u),  /* idx=512, ratio=0.679447 */
    ALGO_RATIO_K_PACK( 172u,  506u, 2583u),  /* idx=513, ratio=0.630758 */
    ALGO_RATIO_K_PACK( 167u,  512u, 2392u),  /* idx=514, ratio=0.584229 */
    ALGO_RATIO_K_PACK( 163u,  518u, 2086u),  /* idx=515, ratio=0.509326 */
    ALGO_RATIO_K_PACK( 159u,  523u, 1790u),  /* idx=516, ratio=0.437091 */
    ALGO_RATIO_K_PACK( 155u,  528u, 1514u),  /* idx=517, ratio=0.369681 */
    ALGO_RATIO_K_PACK( 151u,  533u, 1256u),  /* idx=518, ratio=0.306628 */
    ALGO_RATIO_K_PACK( 147u,  538u, 1014u),  /* idx=519, ratio=0.247523 */
    ALGO_RATIO_K_PACK( 144u,  543u,  786u),  /* idx=520, ratio=0.192007 */
    ALGO_RATIO_K_PACK( 141u,  547u,  572u),  /* idx=521, ratio=0.139762 */
    ALGO_RATIO_K_PACK( 138u,  552u,  371u),  /* idx=522, ratio=0.090508 */
    ALGO_RATIO_K_PACK( 135u,  556u,  180u),  /* idx=523, ratio=0.043995 */
    ALGO_RATIO_K_PACK( 132u,  559u,    0u),  /* idx=524, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 356u,  279u,    0u),  /* idx=525, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 338u,  302u,  745u),  /* idx=526, ratio=0.182045 */
    ALGO_RATIO_K_PACK( 322u,  323u, 1331u),  /* idx=527, ratio=0.325083 */
    ALGO_RATIO_K_PACK( 308u,  342u, 1804u),  /* idx=528, ratio=0.440437 */
    ALGO_RATIO_K_PACK( 294u,  359u, 2193u),  /* idx=529, ratio=0.535435 */
    ALGO_RATIO_K_PACK( 282u,  375u, 2519u),  /* idx=530, ratio=0.615030 */
    ALGO_RATIO_K_PACK( 271u,  389u, 2796u),  /* idx=531, ratio=0.682685 */
    ALGO_RATIO_K_PACK( 260u,  403u, 3034u),  /* idx=532, ratio=0.740901 */
    ALGO_RATIO_K_PACK( 251u,  415u, 3241u),  /* idx=533, ratio=0.791523 */
    ALGO_RATIO_K_PACK( 242u,  427u, 3423u),  /* idx=534, ratio=0.835947 */
    ALGO_RATIO_K_PACK( 233u,  438u, 3584u),  /* idx=535, ratio=0.875245 */
    ALGO_RATIO_K_PACK( 226u,  448u, 3728u),  /* idx=536, ratio=0.910257 */
    ALGO_RATIO_K_PACK( 218u,  457u, 3856u),  /* idx=537, ratio=0.941646 */
    ALGO_RATIO_K_PACK( 211u,  466u, 3972u),  /* idx=538, ratio=0.969949 */
    ALGO_RATIO_K_PACK( 205u,  474u, 3871u),  /* idx=539, ratio=0.945288 */
    ALGO_RATIO_K_PACK( 199u,  482u, 3619u),  /* idx=540, ratio=0.883766 */
    ALGO_RATIO_K_PACK( 193u,  490u, 3400u),  /* idx=541, ratio=0.830381 */
    ALGO_RATIO_K_PACK( 187u,  497u, 3183u),  /* idx=542, ratio=0.777402 */
    ALGO_RATIO_K_PACK( 182u,  503u, 2811u),  /* idx=543, ratio=0.686553 */
    ALGO_RATIO_K_PACK( 177u,  510u, 2459u),  /* idx=544, ratio=0.600438 */
    ALGO_RATIO_K_PACK( 173u,  516u, 2132u),  /* idx=545, ratio=0.520624 */
    ALGO_RATIO_K_PACK( 168u,  521u, 1828u),  /* idx=546, ratio=0.446444 */
    ALGO_RATIO_K_PACK( 164u,  527u, 1545u),  /* idx=547, ratio=0.377322 */
    ALGO_RATIO_K_PACK( 160u,  532u, 1281u),  /* idx=548, ratio=0.312757 */
    ALGO_RATIO_K_PACK( 156u,  537u, 1033u),  /* idx=549, ratio=0.252313 */
    ALGO_RATIO_K_PACK( 153u,  542u,  801u),  /* idx=550, ratio=0.195607 */
    ALGO_RATIO_K_PACK( 149u,  546u,  583u),  /* idx=551, ratio=0.142304 */
    ALGO_RATIO_K_PACK( 146u,  551u,  377u),  /* idx=552, ratio=0.092107 */
    ALGO_RATIO_K_PACK( 143u,  555u,  183u),  /* idx=553, ratio=0.044750 */
    ALGO_RATIO_K_PACK( 140u,  559u,    0u),  /* idx=554, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 365u,  291u,    0u),  /* idx=555, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 347u,  313u,  712u),  /* idx=556, ratio=0.173833 */
    ALGO_RATIO_K_PACK( 331u,  332u, 1277u),  /* idx=557, ratio=0.311926 */
    ALGO_RATIO_K_PACK( 316u,  351u, 1737u),  /* idx=558, ratio=0.424274 */
    ALGO_RATIO_K_PACK( 303u,  367u, 2119u),  /* idx=559, ratio=0.517462 */
    ALGO_RATIO_K_PACK( 290u,  383u, 2441u),  /* idx=560, ratio=0.596006 */
    ALGO_RATIO_K_PACK( 279u,  397u, 2715u),  /* idx=561, ratio=0.663107 */
    ALGO_RATIO_K_PACK( 268u,  410u, 2953u),  /* idx=562, ratio=0.721096 */
    ALGO_RATIO_K_PACK( 258u,  422u, 3160u),  /* idx=563, ratio=0.771710 */
    ALGO_RATIO_K_PACK( 249u,  433u, 3343u),  /* idx=564, ratio=0.816273 */
    ALGO_RATIO_K_PACK( 240u,  444u, 3505u),  /* idx=565, ratio=0.855809 */
    ALGO_RATIO_K_PACK( 232u,  453u, 3649u),  /* idx=566, ratio=0.891122 */
    ALGO_RATIO_K_PACK( 225u,  463u, 3779u),  /* idx=567, ratio=0.922855 */
    ALGO_RATIO_K_PACK( 218u,  471u, 3897u),  /* idx=568, ratio=0.951526 */
    ALGO_RATIO_K_PACK( 211u,  479u, 4003u),  /* idx=569, ratio=0.977558 */
    ALGO_RATIO_K_PACK( 205u,  487u, 3704u),  /* idx=570, ratio=0.904428 */
    ALGO_RATIO_K_PACK( 199u,  494u, 3275u),  /* idx=571, ratio=0.799671 */
    ALGO_RATIO_K_PACK( 194u,  501u, 2880u),  /* idx=572, ratio=0.703388 */
    ALGO_RATIO_K_PACK( 188u,  508u, 2517u),  /* idx=573, ratio=0.614589 */
    ALGO_RATIO_K_PACK( 183u,  514u, 2180u),  /* idx=574, ratio=0.532435 */
    ALGO_RATIO_K_PACK( 178u,  520u, 1868u),  /* idx=575, ratio=0.456207 */
    ALGO_RATIO_K_PACK( 174u,  525u, 1578u),  /* idx=576, ratio=0.385285 */
    ALGO_RATIO_K_PACK( 170u,  531u, 1307u),  /* idx=577, ratio=0.319136 */
    ALGO_RATIO_K_PACK( 165u,  536u, 1054u),  /* idx=578, ratio=0.257291 */
    ALGO_RATIO_K_PACK( 162u,  541u,  816u),  /* idx=579, ratio=0.199346 */
    ALGO_RATIO_K_PACK( 158u,  545u,  594u),  /* idx=580, ratio=0.144941 */
    ALGO_RATIO_K_PACK( 154u,  550u,  384u),  /* idx=581, ratio=0.093762 */
    ALGO_RATIO_K_PACK( 151u,  554u,  186u),  /* idx=582, ratio=0.045531 */
    ALGO_RATIO_K_PACK( 148u,  558u,    0u),  /* idx=583, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 374u,  302u,    0u),  /* idx=584, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 356u,  323u,  681u),  /* idx=585, ratio=0.166331 */
    ALGO_RATIO_K_PACK( 339u,  342u, 1228u),  /* idx=586, ratio=0.299794 */
    ALGO_RATIO_K_PACK( 324u,  359u, 1676u),  /* idx=587, ratio=0.409255 */
    ALGO_RATIO_K_PACK( 311u,  376u, 2050u),  /* idx=588, ratio=0.500655 */
    ALGO_RATIO_K_PACK( 298u,  390u, 2367u),  /* idx=589, ratio=0.578124 */
    ALGO_RATIO_K_PACK( 286u,  404u, 2640u),  /* idx=590, ratio=0.644621 */
    ALGO_RATIO_K_PACK( 275u,  417u, 2876u),  /* idx=591, ratio=0.702322 */
    ALGO_RATIO_K_PACK( 265u,  428u, 3083u),  /* idx=592, ratio=0.752865 */
    ALGO_RATIO_K_PACK( 256u,  439u, 3266u),  /* idx=593, ratio=0.797504 */
    ALGO_RATIO_K_PACK( 247u,  450u, 3428u),  /* idx=594, ratio=0.837217 */
    ALGO_RATIO_K_PACK( 239u,  459u, 3574u),  /* idx=595, ratio=0.872775 */
    ALGO_RATIO_K_PACK( 232u,  468u, 3705u),  /* idx=596, ratio=0.904799 */
    ALGO_RATIO_K_PACK( 224u,  477u, 3824u),  /* idx=597, ratio=0.933791 */
    ALGO_RATIO_K_PACK( 218u,  485u, 3805u),  /* idx=598, ratio=0.929217 */
    ALGO_RATIO_K_PACK( 211u,  492u, 3361u),  /* idx=599, ratio=0.820642 */
    ALGO_RATIO_K_PACK( 205u,  499u, 2953u),  /* idx=600, ratio=0.721068 */
    ALGO_RATIO_K_PACK( 199u,  506u, 2577u),  /* idx=601, ratio=0.629423 */
    ALGO_RATIO_K_PACK( 194u,  512u, 2231u),  /* idx=602, ratio=0.544794 */
    ALGO_RATIO_K_PACK( 189u,  518u, 1910u),  /* idx=603, ratio=0.466406 */
    ALGO_RATIO_K_PACK( 184u,  524u, 1612u),  /* idx=604, ratio=0.393593 */
    ALGO_RATIO_K_PACK( 179u,  529u, 1334u),  /* idx=605, ratio=0.325780 */
    ALGO_RATIO_K_PACK( 175u,  534u, 1075u),  /* idx=606, ratio=0.262470 */
    ALGO_RATIO_K_PACK( 171u,  539u,  832u),  /* idx=607, ratio=0.203229 */
    ALGO_RATIO_K_PACK( 167u,  544u,  605u),  /* idx=608, ratio=0.147677 */
    ALGO_RATIO_K_PACK( 163u,  549u,  391u),  /* idx=609, ratio=0.095479 */
    ALGO_RATIO_K_PACK( 159u,  553u,  190u),  /* idx=610, ratio=0.046340 */
    ALGO_RATIO_K_PACK( 156u,  557u,    0u),  /* idx=611, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 382u,  312u,    0u),  /* idx=612, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 364u,  333u,  653u),  /* idx=613, ratio=0.159449 */
    ALGO_RATIO_K_PACK( 347u,  351u, 1182u),  /* idx=614, ratio=0.288569 */
    ALGO_RATIO_K_PACK( 332u,  368u, 1619u),  /* idx=615, ratio=0.395263 */
    ALGO_RATIO_K_PACK( 318u,  384u, 1986u),  /* idx=616, ratio=0.484906 */
    ALGO_RATIO_K_PACK( 306u,  398u, 2298u),  /* idx=617, ratio=0.561284 */
    ALGO_RATIO_K_PACK( 294u,  411u, 2568u),  /* idx=618, ratio=0.627137 */
    ALGO_RATIO_K_PACK( 283u,  423u, 2803u),  /* idx=619, ratio=0.684502 */
    ALGO_RATIO_K_PACK( 273u,  435u, 3009u),  /* idx=620, ratio=0.734919 */
    ALGO_RATIO_K_PACK( 263u,  446u, 3192u),  /* idx=621, ratio=0.779579 */
    ALGO_RATIO_K_PACK( 254u,  455u, 3356u),  /* idx=622, ratio=0.819415 */
    ALGO_RATIO_K_PACK( 246u,  465u, 3502u),  /* idx=623, ratio=0.855169 */
    ALGO_RATIO_K_PACK( 238u,  473u, 3634u),  /* idx=624, ratio=0.887436 */
    ALGO_RATIO_K_PACK( 231u,  482u, 3754u),  /* idx=625, ratio=0.916705 */
    ALGO_RATIO_K_PACK( 224u,  489u, 3451u),  /* idx=626, ratio=0.842741 */
    ALGO_RATIO_K_PACK( 217u,  497u, 3029u),  /* idx=627, ratio=0.739661 */
    ALGO_RATIO_K_PACK( 211u,  504u, 2641u),  /* idx=628, ratio=0.644990 */
    ALGO_RATIO_K_PACK( 205u,  510u, 2284u),  /* idx=629, ratio=0.557740 */
    ALGO_RATIO_K_PACK( 200u,  516u, 1954u),  /* idx=630, ratio=0.477071 */
    ALGO_RATIO_K_PACK( 195u,  522u, 1647u),  /* idx=631, ratio=0.402266 */
    ALGO_RATIO_K_PACK( 190u,  528u, 1362u),  /* idx=632, ratio=0.332707 */
    ALGO_RATIO_K_PACK( 185u,  533u, 1097u),  /* idx=633, ratio=0.267862 */
    ALGO_RATIO_K_PACK( 180u,  538u,  849u),  /* idx=634, ratio=0.207267 */
    ALGO_RATIO_K_PACK( 176u,  543u,  616u),  /* idx=635, ratio=0.150518 */
    ALGO_RATIO_K_PACK( 172u,  548u,  398u),  /* idx=636, ratio=0.097259 */
    ALGO_RATIO_K_PACK( 168u,  552u,  193u),  /* idx=637, ratio=0.047179 */
    ALGO_RATIO_K_PACK( 164u,  556u,    0u),  /* idx=638, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 390u,  323u,    0u),  /* idx=639, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 372u,  342u,  627u),  /* idx=640, ratio=0.153114 */
    ALGO_RATIO_K_PACK( 355u,  360u, 1139u),  /* idx=641, ratio=0.278155 */
    ALGO_RATIO_K_PACK( 340u,  376u, 1565u),  /* idx=642, ratio=0.382197 */
    ALGO_RATIO_K_PACK( 326u,  392u, 1925u),  /* idx=643, ratio=0.470118 */
    ALGO_RATIO_K_PACK( 313u,  405u, 2233u),  /* idx=644, ratio=0.545397 */
    ALGO_RATIO_K_PACK( 301u,  418u, 2500u),  /* idx=645, ratio=0.610577 */
    ALGO_RATIO_K_PACK( 290u,  430u, 2734u),  /* idx=646, ratio=0.667563 */
    ALGO_RATIO_K_PACK( 279u,  441u, 2939u),  /* idx=647, ratio=0.717808 */
    ALGO_RATIO_K_PACK( 270u,  452u, 3122u),  /* idx=648, ratio=0.762442 */
    ALGO_RATIO_K_PACK( 261u,  461u, 3286u),  /* idx=649, ratio=0.802355 */
    ALGO_RATIO_K_PACK( 252u,  470u, 3433u),  /* idx=650, ratio=0.838258 */
    ALGO_RATIO_K_PACK( 244u,  479u, 3566u),  /* idx=651, ratio=0.870727 */
    ALGO_RATIO_K_PACK( 237u,  487u, 3547u),  /* idx=652, ratio=0.866064 */
    ALGO_RATIO_K_PACK( 230u,  494u, 3109u),  /* idx=653, ratio=0.759237 */
    ALGO_RATIO_K_PACK( 223u,  501u, 2708u),  /* idx=654, ratio=0.661347 */
    ALGO_RATIO_K_PACK( 217u,  508u, 2340u),  /* idx=655, ratio=0.571316 */
    ALGO_RATIO_K_PACK( 211u,  514u, 1999u),  /* idx=656, ratio=0.488236 */
    ALGO_RATIO_K_PACK( 205u,  521u, 1684u),  /* idx=657, ratio=0.411330 */
    ALGO_RATIO_K_PACK( 200u,  526u, 1392u),  /* idx=658, ratio=0.339936 */
    ALGO_RATIO_K_PACK( 195u,  532u, 1120u),  /* idx=659, ratio=0.273481 */
    ALGO_RATIO_K_PACK( 190u,  537u,  866u),  /* idx=660, ratio=0.211469 */
    ALGO_RATIO_K_PACK( 186u,  542u,  628u),  /* idx=661, ratio=0.153471 */
    ALGO_RATIO_K_PACK( 181u,  547u,  406u),  /* idx=662, ratio=0.099107 */
    ALGO_RATIO_K_PACK( 177u,  551u,  197u),  /* idx=663, ratio=0.048048 */
    ALGO_RATIO_K_PACK( 173u,  555u,    0u),  /* idx=664, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 399u,  333u,    0u),  /* idx=665, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 380u,  352u,  603u),  /* idx=666, ratio=0.147263 */
    ALGO_RATIO_K_PACK( 363u,  369u, 1099u),  /* idx=667, ratio=0.268467 */
    ALGO_RATIO_K_PACK( 348u,  385u, 1515u),  /* idx=668, ratio=0.369966 */
    ALGO_RATIO_K_PACK( 333u,  399u, 1868u),  /* idx=669, ratio=0.456205 */
    ALGO_RATIO_K_PACK( 320u,  413u, 2172u),  /* idx=670, ratio=0.530385 */
    ALGO_RATIO_K_PACK( 308u,  425u, 2436u),  /* idx=671, ratio=0.594869 */
    ALGO_RATIO_K_PACK( 297u,  437u, 2668u),  /* idx=672, ratio=0.651442 */
    ALGO_RATIO_K_PACK( 286u,  448u, 2873u),  /* idx=673, ratio=0.701476 */
    ALGO_RATIO_K_PACK( 277u,  458u, 3055u),  /* idx=674, ratio=0.746042 */
    ALGO_RATIO_K_PACK( 267u,  467u, 3219u),  /* idx=675, ratio=0.785991 */
    ALGO_RATIO_K_PACK( 259u,  476u, 3366u),  /* idx=676, ratio=0.822004 */
    ALGO_RATIO_K_PACK( 251u,  484u, 3500u),  /* idx=677, ratio=0.854636 */
    ALGO_RATIO_K_PACK( 243u,  492u, 3194u),  /* idx=678, ratio=0.779879 */
    ALGO_RATIO_K_PACK( 236u,  499u, 2779u),  /* idx=679, ratio=0.678555 */
    ALGO_RATIO_K_PACK( 229u,  506u, 2398u),  /* idx=680, ratio=0.585570 */
    ALGO_RATIO_K_PACK( 223u,  513u, 2047u),  /* idx=681, ratio=0.499935 */
    ALGO_RATIO_K_PACK( 217u,  519u, 1723u),  /* idx=682, ratio=0.420812 */
    ALGO_RATIO_K_PACK( 211u,  525u, 1423u),  /* idx=683, ratio=0.347485 */
    ALGO_RATIO_K_PACK( 206u,  530u, 1144u),  /* idx=684, ratio=0.279339 */
    ALGO_RATIO_K_PACK( 200u,  536u,  884u),  /* idx=685, ratio=0.215845 */
    ALGO_RATIO_K_PACK( 195u,  541u,  641u),  /* idx=686, ratio=0.156541 */
    ALGO_RATIO_K_PACK( 191u,  545u,  414u),  /* idx=687, ratio=0.101027 */
    ALGO_RATIO_K_PACK( 186u,  550u,  200u),  /* idx=688, ratio=0.048950 */
    ALGO_RATIO_K_PACK( 182u,  554u,    0u),  /* idx=689, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 406u,  343u,    0u),  /* idx=690, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 388u,  361u,  581u),  /* idx=691, ratio=0.141843 */
    ALGO_RATIO_K_PACK( 371u,  378u, 1062u),  /* idx=692, ratio=0.259430 */
    ALGO_RATIO_K_PACK( 355u,  393u, 1468u),  /* idx=693, ratio=0.358494 */
    ALGO_RATIO_K_PACK( 341u,  407u, 1814u),  /* idx=694, ratio=0.443092 */
    ALGO_RATIO_K_PACK( 327u,  420u, 2114u),  /* idx=695, ratio=0.516176 */
    ALGO_RATIO_K_PACK( 315u,  432u, 2375u),  /* idx=696, ratio=0.579949 */
    ALGO_RATIO_K_PACK( 304u,  443u, 2605u),  /* idx=697, ratio=0.636081 */
    ALGO_RATIO_K_PACK( 293u,  454u, 2809u),  /* idx=698, ratio=0.685870 */
    ALGO_RATIO_K_PACK( 283u,  463u, 2991u),  /* idx=699, ratio=0.730333 */
    ALGO_RATIO_K_PACK( 274u,  473u, 3154u),  /* idx=700, ratio=0.770280 */
    ALGO_RATIO_K_PACK( 265u,  481u, 3302u),  /* idx=701, ratio=0.806368 */
    ALGO_RATIO_K_PACK( 257u,  489u, 3283u),  /* idx=702, ratio=0.801673 */
    ALGO_RATIO_K_PACK( 249u,  497u, 2853u),  /* idx=703, ratio=0.696683 */
    ALGO_RATIO_K_PACK( 242u,  504u, 2459u),  /* idx=704, ratio=0.600554 */
    ALGO_RATIO_K_PACK( 235u,  511u, 2097u),  /* idx=705, ratio=0.512210 */
    ALGO_RATIO_K_PACK( 229u,  517u, 1764u),  /* idx=706, ratio=0.430742 */
    ALGO_RATIO_K_PACK( 222u,  523u, 1455u),  /* idx=707, ratio=0.355377 */
    ALGO_RATIO_K_PACK( 217u,  529u, 1169u),  /* idx=708, ratio=0.285455 */
    ALGO_RATIO_K_PACK( 211u,  534u,  903u),  /* idx=709, ratio=0.220406 */
    ALGO_RATIO_K_PACK( 206u,  539u,  654u),  /* idx=710, ratio=0.159738 */
    ALGO_RATIO_K_PACK( 201u,  544u,  422u),  /* idx=711, ratio=0.103023 */
    ALGO_RATIO_K_PACK( 196u,  549u,  204u),  /* idx=712, ratio=0.049886 */
    ALGO_RATIO_K_PACK( 191u,  553u,    0u),  /* idx=713, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 414u,  352u,    0u),  /* idx=714, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 395u,  370u,  560u),  /* idx=715, ratio=0.136807 */
    ALGO_RATIO_K_PACK( 378u,  386u, 1028u),  /* idx=716, ratio=0.250982 */
    ALGO_RATIO_K_PACK( 362u,  401u, 1424u),  /* idx=717, ratio=0.347712 */
    ALGO_RATIO_K_PACK( 348u,  414u, 1764u),  /* idx=718, ratio=0.430711 */
    ALGO_RATIO_K_PACK( 335u,  427u, 2059u),  /* idx=719, ratio=0.502710 */
    ALGO_RATIO_K_PACK( 322u,  439u, 2317u),  /* idx=720, ratio=0.565759 */
    ALGO_RATIO_K_PACK( 310u,  450u, 2545u),  /* idx=721, ratio=0.621429 */
    ALGO_RATIO_K_PACK( 300u,  460u, 2748u),  /* idx=722, ratio=0.670944 */
    ALGO_RATIO_K_PACK( 290u,  469u, 2929u),  /* idx=723, ratio=0.715271 */
    ALGO_RATIO_K_PACK( 280u,  478u, 3092u),  /* idx=724, ratio=0.755186 */
    ALGO_RATIO_K_PACK( 271u,  486u, 3240u),  /* idx=725, ratio=0.791315 */
    ALGO_RATIO_K_PACK( 263u,  494u, 2931u),  /* idx=726, ratio=0.715806 */
    ALGO_RATIO_K_PACK( 255u,  501u, 2524u),  /* idx=727, ratio=0.616324 */
    ALGO_RATIO_K_PACK( 248u,  508u, 2150u),  /* idx=728, ratio=0.525102 */
    ALGO_RATIO_K_PACK( 241u,  515u, 1807u),  /* idx=729, ratio=0.441151 */
    ALGO_RATIO_K_PACK( 234u,  521u, 1489u),  /* idx=730, ratio=0.363636 */
    ALGO_RATIO_K_PACK( 228u,  527u, 1195u),  /* idx=731, ratio=0.291844 */
    ALGO_RATIO_K_PACK( 222u,  533u,  922u),  /* idx=732, ratio=0.225163 */
    ALGO_RATIO_K_PACK( 216u,  538u,  668u),  /* idx=733, ratio=0.163067 */
    ALGO_RATIO_K_PACK( 211u,  543u,  430u),  /* idx=734, ratio=0.105099 */
    ALGO_RATIO_K_PACK( 206u,  548u,  208u),  /* idx=735, ratio=0.050859 */
    ALGO_RATIO_K_PACK( 201u,  552u,    0u),  /* idx=736, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 422u,  362u,    0u),  /* idx=737, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 403u,  379u,  541u),  /* idx=738, ratio=0.132117 */
    ALGO_RATIO_K_PACK( 386u,  394u,  995u),  /* idx=739, ratio=0.243067 */
    ALGO_RATIO_K_PACK( 370u,  408u, 1382u),  /* idx=740, ratio=0.337560 */
    ALGO_RATIO_K_PACK( 355u,  422u, 1716u),  /* idx=741, ratio=0.419004 */
    ALGO_RATIO_K_PACK( 341u,  434u, 2006u),  /* idx=742, ratio=0.489928 */
    ALGO_RATIO_K_PACK( 329u,  445u, 2261u),  /* idx=743, ratio=0.552246 */
    ALGO_RATIO_K_PACK( 317u,  456u, 2487u),  /* idx=744, ratio=0.607436 */
    ALGO_RATIO_K_PACK( 306u,  466u, 2689u),  /* idx=745, ratio=0.656653 */
    ALGO_RATIO_K_PACK( 296u,  475u, 2870u),  /* idx=746, ratio=0.700819 */
    ALGO_RATIO_K_PACK( 286u,  483u, 3033u),  /* idx=747, ratio=0.740672 */
    ALGO_RATIO_K_PACK( 277u,  491u, 3014u),  /* idx=748, ratio=0.736008 */
    ALGO_RATIO_K_PACK( 269u,  499u, 2592u),  /* idx=749, ratio=0.632945 */
    ALGO_RATIO_K_PACK( 261u,  506u, 2206u),  /* idx=750, ratio=0.538659 */
    ALGO_RATIO_K_PACK( 254u,  513u, 1851u),  /* idx=751, ratio=0.452076 */
    ALGO_RATIO_K_PACK( 246u,  519u, 1525u),  /* idx=752, ratio=0.372288 */
    ALGO_RATIO_K_PACK( 240u,  525u, 1222u),  /* idx=753, ratio=0.298525 */
    ALGO_RATIO_K_PACK( 233u,  531u,  942u),  /* idx=754, ratio=0.230131 */
    ALGO_RATIO_K_PACK( 227u,  537u,  682u),  /* idx=755, ratio=0.166538 */
    ALGO_RATIO_K_PACK( 222u,  542u,  439u),  /* idx=756, ratio=0.107260 */
    ALGO_RATIO_K_PACK( 216u,  547u,  212u),  /* idx=757, ratio=0.051871 */
    ALGO_RATIO_K_PACK( 211u,  551u,    0u),  /* idx=758, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 429u,  371u,    0u),  /* idx=759, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 410u,  387u,  523u),  /* idx=760, ratio=0.127738 */
    ALGO_RATIO_K_PACK( 393u,  402u,  965u),  /* idx=761, ratio=0.235636 */
    ALGO_RATIO_K_PACK( 377u,  416u, 1343u),  /* idx=762, ratio=0.327984 */
    ALGO_RATIO_K_PACK( 362u,  429u, 1670u),  /* idx=763, ratio=0.407916 */
    ALGO_RATIO_K_PACK( 348u,  441u, 1957u),  /* idx=764, ratio=0.477780 */
    ALGO_RATIO_K_PACK( 335u,  452u, 2209u),  /* idx=765, ratio=0.539364 */
    ALGO_RATIO_K_PACK( 324u,  462u, 2433u),  /* idx=766, ratio=0.594059 */
    ALGO_RATIO_K_PACK( 313u,  471u, 2633u),  /* idx=767, ratio=0.642959 */
    ALGO_RATIO_K_PACK( 302u,  480u, 2813u),  /* idx=768, ratio=0.686938 */
    ALGO_RATIO_K_PACK( 293u,  489u, 2976u),  /* idx=769, ratio=0.726705 */
    ALGO_RATIO_K_PACK( 284u,  497u, 2664u),  /* idx=770, ratio=0.650487 */
    ALGO_RATIO_K_PACK( 275u,  504u, 2264u),  /* idx=771, ratio=0.552936 */
    ALGO_RATIO_K_PACK( 267u,  511u, 1898u),  /* idx=772, ratio=0.463556 */
    ALGO_RATIO_K_PACK( 259u,  518u, 1562u),  /* idx=773, ratio=0.381362 */
    ALGO_RATIO_K_PACK( 252u,  524u, 1251u),  /* idx=774, ratio=0.305520 */
    ALGO_RATIO_K_PACK( 245u,  530u,  964u),  /* idx=775, ratio=0.235322 */
    ALGO_RATIO_K_PACK( 239u,  535u,  697u),  /* idx=776, ratio=0.170161 */
    ALGO_RATIO_K_PACK( 233u,  541u,  448u),  /* idx=777, ratio=0.109512 */
    ALGO_RATIO_K_PACK( 227u,  546u,  217u),  /* idx=778, ratio=0.052923 */
    ALGO_RATIO_K_PACK( 221u,  550u,    0u),  /* idx=779, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 436u,  380u,    0u),  /* idx=780, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 417u,  396u,  506u),  /* idx=781, ratio=0.123640 */
    ALGO_RATIO_K_PACK( 400u,  410u,  936u),  /* idx=782, ratio=0.228646 */
    ALGO_RATIO_K_PACK( 384u,  424u, 1306u),  /* idx=783, ratio=0.318936 */
    ALGO_RATIO_K_PACK( 369u,  436u, 1627u),  /* idx=784, ratio=0.397400 */
    ALGO_RATIO_K_PACK( 355u,  447u, 1909u),  /* idx=785, ratio=0.466220 */
    ALGO_RATIO_K_PACK( 342u,  458u, 2158u),  /* idx=786, ratio=0.527070 */
    ALGO_RATIO_K_PACK( 330u,  468u, 2380u),  /* idx=787, ratio=0.581259 */
    ALGO_RATIO_K_PACK( 319u,  477u, 2579u),  /* idx=788, ratio=0.629824 */
    ALGO_RATIO_K_PACK( 308u,  486u, 2758u),  /* idx=789, ratio=0.673597 */
    ALGO_RATIO_K_PACK( 299u,  494u, 2740u),  /* idx=790, ratio=0.669029 */
    ALGO_RATIO_K_PACK( 289u,  502u, 2326u),  /* idx=791, ratio=0.567990 */
    ALGO_RATIO_K_PACK( 281u,  509u, 1948u),  /* idx=792, ratio=0.475634 */
    ALGO_RATIO_K_PACK( 273u,  516u, 1601u),  /* idx=793, ratio=0.390889 */
    ALGO_RATIO_K_PACK( 265u,  522u, 1281u),  /* idx=794, ratio=0.312850 */
    ALGO_RATIO_K_PACK( 258u,  528u,  986u),  /* idx=795, ratio=0.240753 */
    ALGO_RATIO_K_PACK( 251u,  534u,  712u),  /* idx=796, ratio=0.173944 */
    ALGO_RATIO_K_PACK( 244u,  539u,  458u),  /* idx=797, ratio=0.111861 */
    ALGO_RATIO_K_PACK( 238u,  544u,  221u),  /* idx=798, ratio=0.054020 */
    ALGO_RATIO_K_PACK( 232u,  549u,    0u),  /* idx=799, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 443u,  388u,    0u),  /* idx=800, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 424u,  404u,  491u),  /* idx=801, ratio=0.119796 */
    ALGO_RATIO_K_PACK( 406u,  418u,  909u),  /* idx=802, ratio=0.222058 */
    ALGO_RATIO_K_PACK( 390u,  431u, 1271u),  /* idx=803, ratio=0.310374 */
    ALGO_RATIO_K_PACK( 375u,  443u, 1586u),  /* idx=804, ratio=0.387413 */
    ALGO_RATIO_K_PACK( 361u,  454u, 1864u),  /* idx=805, ratio=0.455206 */
    ALGO_RATIO_K_PACK( 348u,  464u, 2110u),  /* idx=806, ratio=0.515323 */
    ALGO_RATIO_K_PACK( 336u,  474u, 2330u),  /* idx=807, ratio=0.568999 */
    ALGO_RATIO_K_PACK( 325u,  483u, 2527u),  /* idx=808, ratio=0.617215 */
    ALGO_RATIO_K_PACK( 315u,  491u, 2706u),  /* idx=809, ratio=0.660765 */
    ALGO_RATIO_K_PACK( 305u,  499u, 2391u),  /* idx=810, ratio=0.583886 */
    ALGO_RATIO_K_PACK( 295u,  507u, 2000u),  /* idx=811, ratio=0.488358 */
    ALGO_RATIO_K_PACK( 287u,  514u, 1642u),  /* idx=812, ratio=0.400904 */
    ALGO_RATIO_K_PACK( 278u,  520u, 1313u),  /* idx=813, ratio=0.320541 */
    ALGO_RATIO_K_PACK( 270u,  526u, 1009u),  /* idx=814, ratio=0.246441 */
    ALGO_RATIO_K_PACK( 263u,  532u,  728u),  /* idx=815, ratio=0.177899 */
    ALGO_RATIO_K_PACK( 256u,  538u,  468u),  /* idx=816, ratio=0.114312 */
    ALGO_RATIO_K_PACK( 249u,  543u,  226u),  /* idx=817, ratio=0.055162 */
    ALGO_RATIO_K_PACK( 243u,  548u,    0u),  /* idx=818, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 450u,  397u,    0u),  /* idx=819, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 431u,  412u,  476u),  /* idx=820, ratio=0.116185 */
    ALGO_RATIO_K_PACK( 413u,  425u,  884u),  /* idx=821, ratio=0.215840 */
    ALGO_RATIO_K_PACK( 397u,  438u, 1238u),  /* idx=822, ratio=0.302259 */
    ALGO_RATIO_K_PACK( 382u,  450u, 1548u),  /* idx=823, ratio=0.377915 */
    ALGO_RATIO_K_PACK( 368u,  460u, 1821u),  /* idx=824, ratio=0.444700 */
    ALGO_RATIO_K_PACK( 355u,  470u, 2064u),  /* idx=825, ratio=0.504089 */
    ALGO_RATIO_K_PACK( 343u,  480u, 2282u),  /* idx=826, ratio=0.557245 */
    ALGO_RATIO_K_PACK( 331u,  488u, 2478u),  /* idx=827, ratio=0.605101 */
    ALGO_RATIO_K_PACK( 320u,  497u, 2460u),  /* idx=828, ratio=0.600698 */
    ALGO_RATIO_K_PACK( 310u,  504u, 2055u),  /* idx=829, ratio=0.501782 */
    ALGO_RATIO_K_PACK( 301u,  511u, 1685u),  /* idx=830, ratio=0.411446 */
    ALGO_RATIO_K_PACK( 292u,  518u, 1346u),  /* idx=831, ratio=0.328620 */
    ALGO_RATIO_K_PACK( 284u,  525u, 1034u),  /* idx=832, ratio=0.252404 */
    ALGO_RATIO_K_PACK( 276u,  531u,  745u),  /* idx=833, ratio=0.182039 */
    ALGO_RATIO_K_PACK( 268u,  536u,  479u),  /* idx=834, ratio=0.116874 */
    ALGO_RATIO_K_PACK( 261u,  542u,  231u),  /* idx=835, ratio=0.056354 */
    ALGO_RATIO_K_PACK( 255u,  547u,    0u),  /* idx=836, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 457u,  405u,    0u),  /* idx=837, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 438u,  420u,  462u),  /* idx=838, ratio=0.112784 */
    ALGO_RATIO_K_PACK( 420u,  433u,  860u),  /* idx=839, ratio=0.209960 */
    ALGO_RATIO_K_PACK( 403u,  445u, 1206u),  /* idx=840, ratio=0.294558 */
    ALGO_RATIO_K_PACK( 388u,  456u, 1511u),  /* idx=841, ratio=0.368872 */
    ALGO_RATIO_K_PACK( 374u,  467u, 1780u),  /* idx=842, ratio=0.434669 */
    ALGO_RATIO_K_PACK( 361u,  476u, 2020u),  /* idx=843, ratio=0.493334 */
    ALGO_RATIO_K_PACK( 349u,  485u, 2236u),  /* idx=844, ratio=0.545967 */
    ALGO_RATIO_K_PACK( 337u,  494u, 2430u),  /* idx=845, ratio=0.593453 */
    ALGO_RATIO_K_PACK( 326u,  502u, 2113u),  /* idx=846, ratio=0.515965 */
    ALGO_RATIO_K_PACK( 316u,  509u, 1730u),  /* idx=847, ratio=0.422557 */
    ALGO_RATIO_K_PACK( 307u,  516u, 1380u),  /* idx=848, ratio=0.337116 */
    ALGO_RATIO_K_PACK( 298u,  523u, 1059u),  /* idx=849, ratio=0.258663 */
    ALGO_RATIO_K_PACK( 289u,  529u,  763u),  /* idx=850, ratio=0.186375 */
    ALGO_RATIO_K_PACK( 281u,  535u,  490u),  /* idx=851, ratio=0.119553 */
    ALGO_RATIO_K_PACK( 274u,  541u,  236u),  /* idx=852, ratio=0.057599 */
    ALGO_RATIO_K_PACK( 267u,  546u,    0u),  /* idx=853, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 463u,  414u,    0u),  /* idx=854, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 444u,  427u,  449u),  /* idx=855, ratio=0.109578 */
    ALGO_RATIO_K_PACK( 426u,  440u,  837u),  /* idx=856, ratio=0.204392 */
    ALGO_RATIO_K_PACK( 410u,  452u, 1176u),  /* idx=857, ratio=0.287240 */
    ALGO_RATIO_K_PACK( 394u,  463u, 1475u),  /* idx=858, ratio=0.360251 */
    ALGO_RATIO_K_PACK( 380u,  473u, 1741u),  /* idx=859, ratio=0.425080 */
    ALGO_RATIO_K_PACK( 367u,  482u, 1978u),  /* idx=860, ratio=0.483028 */
    ALGO_RATIO_K_PACK( 355u,  491u, 2191u),  /* idx=861, ratio=0.535137 */
    ALGO_RATIO_K_PACK( 343u,  499u, 2174u),  /* idx=862, ratio=0.530972 */
    ALGO_RATIO_K_PACK( 332u,  507u, 1778u),  /* idx=863, ratio=0.434285 */
    ALGO_RATIO_K_PACK( 322u,  514u, 1417u),  /* idx=864, ratio=0.346063 */
    ALGO_RATIO_K_PACK( 312u,  521u, 1086u),  /* idx=865, ratio=0.265240 */
    ALGO_RATIO_K_PACK( 303u,  527u,  782u),  /* idx=866, ratio=0.190923 */
    ALGO_RATIO_K_PACK( 295u,  533u,  501u),  /* idx=867, ratio=0.122358 */
    ALGO_RATIO_K_PACK( 287u,  539u,  241u),  /* idx=868, ratio=0.058900 */
    ALGO_RATIO_K_PACK( 279u,  545u,    0u),  /* idx=869, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 470u,  422u,    0u),  /* idx=870, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 450u,  435u,  436u),  /* idx=871, ratio=0.106548 */
    ALGO_RATIO_K_PACK( 432u,  447u,  815u),  /* idx=872, ratio=0.199112 */
    ALGO_RATIO_K_PACK( 416u,  459u, 1148u),  /* idx=873, ratio=0.280276 */
    ALGO_RATIO_K_PACK( 401u,  469u, 1442u),  /* idx=874, ratio=0.352024 */
    ALGO_RATIO_K_PACK( 386u,  479u, 1703u),  /* idx=875, ratio=0.415905 */
    ALGO_RATIO_K_PACK( 373u,  488u, 1938u),  /* idx=876, ratio=0.473144 */
    ALGO_RATIO_K_PACK( 360u,  496u, 2149u),  /* idx=877, ratio=0.524727 */
    ALGO_RATIO_K_PACK( 349u,  504u, 1829u),  /* idx=878, ratio=0.446683 */
    ALGO_RATIO_K_PACK( 338u,  512u, 1456u),  /* idx=879, ratio=0.355498 */
    ALGO_RATIO_K_PACK( 328u,  519u, 1114u),  /* idx=880, ratio=0.272161 */
    ALGO_RATIO_K_PACK( 318u,  526u,  801u),  /* idx=881, ratio=0.195699 */
    ALGO_RATIO_K_PACK( 309u,  532u,  513u),  /* idx=882, ratio=0.125297 */
    ALGO_RATIO_K_PACK( 300u,  538u,  247u),  /* idx=883, ratio=0.060261 */
    ALGO_RATIO_K_PACK( 292u,  543u,    0u),  /* idx=884, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 476u,  429u,    0u),  /* idx=885, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 457u,  442u,  425u),  /* idx=886, ratio=0.103681 */
    ALGO_RATIO_K_PACK( 439u,  454u,  795u),  /* idx=887, ratio=0.194098 */
    ALGO_RATIO_K_PACK( 422u,  465u, 1121u),  /* idx=888, ratio=0.273643 */
    ALGO_RATIO_K_PACK( 407u,  475u, 1409u),  /* idx=889, ratio=0.344165 */
    ALGO_RATIO_K_PACK( 392u,  485u, 1667u),  /* idx=890, ratio=0.407117 */
    ALGO_RATIO_K_PACK( 379u,  494u, 1899u),  /* idx=891, ratio=0.463657 */
    ALGO_RATIO_K_PACK( 366u,  502u, 1883u),  /* idx=892, ratio=0.459810 */
    ALGO_RATIO_K_PACK( 354u,  510u, 1497u),  /* idx=893, ratio=0.365462 */
    ALGO_RATIO_K_PACK( 343u,  517u, 1144u),  /* idx=894, ratio=0.279452 */
    ALGO_RATIO_K_PACK( 333u,  524u,  822u),  /* idx=895, ratio=0.200720 */
    ALGO_RATIO_K_PACK( 323u,  530u,  526u),  /* idx=896, ratio=0.128381 */
    ALGO_RATIO_K_PACK( 314u,  536u,  253u),  /* idx=897, ratio=0.061686 */
    ALGO_RATIO_K_PACK( 305u,  542u,    0u),  /* idx=898, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 482u,  437u,    0u),  /* idx=899, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 463u,  449u,  413u),  /* idx=900, ratio=0.100965 */
    ALGO_RATIO_K_PACK( 445u,  461u,  775u),  /* idx=901, ratio=0.189330 */
    ALGO_RATIO_K_PACK( 428u,  472u, 1095u),  /* idx=902, ratio=0.267316 */
    ALGO_RATIO_K_PACK( 412u,  481u, 1379u),  /* idx=903, ratio=0.336649 */
    ALGO_RATIO_K_PACK( 398u,  491u, 1633u),  /* idx=904, ratio=0.398694 */
    ALGO_RATIO_K_PACK( 384u,  499u, 1861u),  /* idx=905, ratio=0.454542 */
    ALGO_RATIO_K_PACK( 372u,  507u, 1540u),  /* idx=906, ratio=0.376001 */
    ALGO_RATIO_K_PACK( 360u,  515u, 1176u),  /* idx=907, ratio=0.287144 */
    ALGO_RATIO_K_PACK( 349u,  522u,  844u),  /* idx=908, ratio=0.206005 */
    ALGO_RATIO_K_PACK( 338u,  528u,  539u),  /* idx=909, ratio=0.131621 */
    ALGO_RATIO_K_PACK( 329u,  535u,  259u),  /* idx=910, ratio=0.063181 */
    ALGO_RATIO_K_PACK( 319u,  541u,    0u),  /* idx=911, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 488u,  445u,    0u),  /* idx=912, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 469u,  457u,  403u),  /* idx=913, ratio=0.098387 */
    ALGO_RATIO_K_PACK( 451u,  468u,  757u),  /* idx=914, ratio=0.184791 */
    ALGO_RATIO_K_PACK( 434u,  478u, 1070u),  /* idx=915, ratio=0.261274 */
    ALGO_RATIO_K_PACK( 418u,  487u, 1349u),  /* idx=916, ratio=0.329454 */
    ALGO_RATIO_K_PACK( 404u,  496u, 1600u),  /* idx=917, ratio=0.390611 */
    ALGO_RATIO_K_PACK( 390u,  505u, 1585u),  /* idx=918, ratio=0.387165 */
    ALGO_RATIO_K_PACK( 377u,  512u, 1209u),  /* idx=919, ratio=0.295272 */
    ALGO_RATIO_K_PACK( 366u,  520u,  866u),  /* idx=920, ratio=0.211577 */
    ALGO_RATIO_K_PACK( 354u,  527u,  553u),  /* idx=921, ratio=0.135028 */
    ALGO_RATIO_K_PACK( 344u,  533u,  265u),  /* idx=922, ratio=0.064749 */
    ALGO_RATIO_K_PACK( 334u,  539u,    0u),  /* idx=923, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 494u,  452u,    0u),  /* idx=924, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 475u,  463u,  393u),  /* idx=925, ratio=0.095938 */
    ALGO_RATIO_K_PACK( 456u,  474u,  739u),  /* idx=926, ratio=0.180464 */
    ALGO_RATIO_K_PACK( 440u,  484u, 1046u),  /* idx=927, ratio=0.255500 */
    ALGO_RATIO_K_PACK( 424u,  493u, 1321u),  /* idx=928, ratio=0.322560 */
    ALGO_RATIO_K_PACK( 409u,  502u, 1568u),  /* idx=929, ratio=0.382850 */
    ALGO_RATIO_K_PACK( 396u,  510u, 1244u),  /* idx=930, ratio=0.303874 */
    ALGO_RATIO_K_PACK( 383u,  518u,  890u),  /* idx=931, ratio=0.217457 */
    ALGO_RATIO_K_PACK( 371u,  525u,  568u),  /* idx=932, ratio=0.138617 */
    ALGO_RATIO_K_PACK( 360u,  531u,  272u),  /* idx=933, ratio=0.066398 */
    ALGO_RATIO_K_PACK( 349u,  538u,    0u),  /* idx=934, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 500u,  459u,    0u),  /* idx=935, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 480u,  470u,  383u),  /* idx=936, ratio=0.093608 */
    ALGO_RATIO_K_PACK( 462u,  481u,  722u),  /* idx=937, ratio=0.176336 */
    ALGO_RATIO_K_PACK( 445u,  490u, 1024u),  /* idx=938, ratio=0.249976 */
    ALGO_RATIO_K_PACK( 430u,  499u, 1294u),  /* idx=939, ratio=0.315949 */
    ALGO_RATIO_K_PACK( 415u,  508u, 1282u),  /* idx=940, ratio=0.312991 */
    ALGO_RATIO_K_PACK( 401u,  515u,  916u),  /* idx=941, ratio=0.223675 */
    ALGO_RATIO_K_PACK( 388u,  523u,  583u),  /* idx=942, ratio=0.142402 */
    ALGO_RATIO_K_PACK( 376u,  530u,  279u),  /* idx=943, ratio=0.068133 */
    ALGO_RATIO_K_PACK( 365u,  536u,    0u),  /* idx=944, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 506u,  466u,    0u),  /* idx=945, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 486u,  477u,  374u),  /* idx=946, ratio=0.091388 */
    ALGO_RATIO_K_PACK( 468u,  487u,  706u),  /* idx=947, ratio=0.172391 */
    ALGO_RATIO_K_PACK( 451u,  496u, 1002u),  /* idx=948, ratio=0.244685 */
    ALGO_RATIO_K_PACK( 435u,  505u, 1268u),  /* idx=949, ratio=0.309603 */
    ALGO_RATIO_K_PACK( 420u,  513u,  943u),  /* idx=950, ratio=0.230258 */
    ALGO_RATIO_K_PACK( 407u,  521u,  600u),  /* idx=951, ratio=0.146399 */
    ALGO_RATIO_K_PACK( 394u,  528u,  286u),  /* idx=952, ratio=0.069961 */
    ALGO_RATIO_K_PACK( 382u,  534u,    0u),  /* idx=953, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 511u,  473u,    0u),  /* idx=954, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 492u,  483u,  366u),  /* idx=955, ratio=0.089271 */
    ALGO_RATIO_K_PACK( 473u,  493u,  690u),  /* idx=956, ratio=0.168620 */
    ALGO_RATIO_K_PACK( 456u,  502u,  981u),  /* idx=957, ratio=0.239614 */
    ALGO_RATIO_K_PACK( 440u,  511u,  971u),  /* idx=958, ratio=0.237240 */
    ALGO_RATIO_K_PACK( 426u,  518u,  617u),  /* idx=959, ratio=0.150627 */
    ALGO_RATIO_K_PACK( 412u,  526u,  294u),  /* idx=960, ratio=0.071889 */
    ALGO_RATIO_K_PACK( 399u,  533u,    0u),  /* idx=961, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 517u,  480u,    0u),  /* idx=962, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 497u,  490u,  357u),  /* idx=963, ratio=0.087250 */
    ALGO_RATIO_K_PACK( 479u,  499u,  676u),  /* idx=964, ratio=0.165010 */
    ALGO_RATIO_K_PACK( 462u,  508u,  961u),  /* idx=965, ratio=0.234749 */
    ALGO_RATIO_K_PACK( 446u,  516u,  635u),  /* idx=966, ratio=0.155106 */
    ALGO_RATIO_K_PACK( 431u,  524u,  303u),  /* idx=967, ratio=0.073927 */
    ALGO_RATIO_K_PACK( 417u,  531u,    0u),  /* idx=968, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 522u,  486u,    0u),  /* idx=969, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 502u,  496u,  349u),  /* idx=970, ratio=0.085318 */
    ALGO_RATIO_K_PACK( 484u,  505u,  662u),  /* idx=971, ratio=0.161551 */
    ALGO_RATIO_K_PACK( 467u,  514u,  655u),  /* idx=972, ratio=0.159860 */
    ALGO_RATIO_K_PACK( 451u,  521u,  312u),  /* idx=973, ratio=0.076084 */
    ALGO_RATIO_K_PACK( 436u,  529u,    0u),  /* idx=974, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 527u,  493u,    0u),  /* idx=975, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 507u,  502u,  342u),  /* idx=976, ratio=0.083470 */
    ALGO_RATIO_K_PACK( 489u,  511u,  648u),  /* idx=977, ratio=0.158235 */
    ALGO_RATIO_K_PACK( 472u,  519u,  321u),  /* idx=978, ratio=0.078370 */
    ALGO_RATIO_K_PACK( 456u,  527u,    0u),  /* idx=979, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 532u,  499u,    0u),  /* idx=980, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 513u,  508u,  335u),  /* idx=981, ratio=0.081701 */
    ALGO_RATIO_K_PACK( 494u,  517u,  331u),  /* idx=982, ratio=0.080799 */
    ALGO_RATIO_K_PACK( 477u,  525u,    0u),  /* idx=983, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 537u,  506u,    0u),  /* idx=984, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 518u,  514u,  265u),  /* idx=985, ratio=0.064666 */
    ALGO_RATIO_K_PACK( 499u,  522u,    0u),  /* idx=986, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 542u,  512u,    0u),  /* idx=987, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 522u,  520u,    0u),  /* idx=988, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 547u,  518u,    0u),  /* idx=989, ratio=0.000000 */
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