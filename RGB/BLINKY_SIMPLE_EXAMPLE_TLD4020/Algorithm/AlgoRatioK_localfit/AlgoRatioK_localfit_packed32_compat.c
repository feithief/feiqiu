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
    ALGO_RATIO_K_PACK( 200u,   84u,    0u),  /* idx=  0, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 188u,  123u,    0u),  /* idx=  1, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 177u,  158u,    0u),  /* idx=  2, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 168u,  189u,    0u),  /* idx=  3, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 159u,  217u,    0u),  /* idx=  4, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 151u,  242u,    0u),  /* idx=  5, ratio=0.000000 */
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
    ALGO_RATIO_K_PACK(  87u,  444u,    0u),  /* idx= 19, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  453u,    0u),  /* idx= 20, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  461u,    0u),  /* idx= 21, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  468u,    0u),  /* idx= 22, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  78u,  476u,    0u),  /* idx= 23, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  75u,  482u,    0u),  /* idx= 24, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  489u,    0u),  /* idx= 25, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  495u,    0u),  /* idx= 26, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  501u,    0u),  /* idx= 27, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  506u,    0u),  /* idx= 28, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  66u,  512u,    0u),  /* idx= 29, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  517u,    0u),  /* idx= 30, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  522u,    0u),  /* idx= 31, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  62u,  527u,    0u),  /* idx= 32, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  531u,    0u),  /* idx= 33, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  535u,    0u),  /* idx= 34, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  58u,  540u,    0u),  /* idx= 35, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  56u,  544u,    0u),  /* idx= 36, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  55u,  547u,    0u),  /* idx= 37, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  54u,  551u,    0u),  /* idx= 38, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  53u,  555u,    0u),  /* idx= 39, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  52u,  558u,    0u),  /* idx= 40, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  51u,  561u,    0u),  /* idx= 41, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  50u,  565u,    0u),  /* idx= 42, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  49u,  568u,    0u),  /* idx= 43, ratio=-0.000000 */
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
    ALGO_RATIO_K_PACK( 115u,  388u,    0u),  /* idx= 57, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  400u,    0u),  /* idx= 58, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 107u,  411u,    0u),  /* idx= 59, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  422u,    0u),  /* idx= 60, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  432u,    0u),  /* idx= 61, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  441u,    0u),  /* idx= 62, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  450u,    0u),  /* idx= 63, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  458u,    0u),  /* idx= 64, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  466u,    0u),  /* idx= 65, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  473u,    0u),  /* idx= 66, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  480u,    0u),  /* idx= 67, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  487u,    0u),  /* idx= 68, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  493u,    0u),  /* idx= 69, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  499u,    0u),  /* idx= 70, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  505u,    0u),  /* idx= 71, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  510u,    0u),  /* idx= 72, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  515u,    0u),  /* idx= 73, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  70u,  520u,    0u),  /* idx= 74, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  525u,    0u),  /* idx= 75, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  530u,    0u),  /* idx= 76, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  534u,    0u),  /* idx= 77, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  64u,  538u,    0u),  /* idx= 78, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  63u,  543u,    0u),  /* idx= 79, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  61u,  546u,    0u),  /* idx= 80, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  60u,  550u,    0u),  /* idx= 81, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  59u,  554u,    0u),  /* idx= 82, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  57u,  557u,    0u),  /* idx= 83, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  56u,  561u,    0u),  /* idx= 84, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  55u,  564u,    0u),  /* idx= 85, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  54u,  567u,    0u),  /* idx= 86, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 227u,  117u,    0u),  /* idx= 87, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 213u,  153u, 1577u),  /* idx= 88, ratio=0.385033 */
    ALGO_RATIO_K_PACK( 201u,  185u,  991u),  /* idx= 89, ratio=0.242055 */
    ALGO_RATIO_K_PACK( 191u,  214u,  520u),  /* idx= 90, ratio=0.127067 */
    ALGO_RATIO_K_PACK( 181u,  240u,  147u),  /* idx= 91, ratio=0.036007 */
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
    ALGO_RATIO_K_PACK(  93u,  477u,    0u),  /* idx=109, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  484u,    0u),  /* idx=110, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  491u,    0u),  /* idx=111, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  497u,    0u),  /* idx=112, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  503u,    0u),  /* idx=113, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  508u,    0u),  /* idx=114, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  514u,    0u),  /* idx=115, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  519u,    0u),  /* idx=116, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  524u,    0u),  /* idx=117, ratio=0.000000 */
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
    ALGO_RATIO_K_PACK(  60u,  567u,    0u),  /* idx=128, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 239u,  133u,    0u),  /* idx=129, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 226u,  168u, 1562u),  /* idx=130, ratio=0.381537 */
    ALGO_RATIO_K_PACK( 213u,  199u, 2497u),  /* idx=131, ratio=0.609714 */
    ALGO_RATIO_K_PACK( 202u,  226u, 2013u),  /* idx=132, ratio=0.491464 */
    ALGO_RATIO_K_PACK( 192u,  251u, 1570u),  /* idx=133, ratio=0.383436 */
    ALGO_RATIO_K_PACK( 183u,  274u, 1174u),  /* idx=134, ratio=0.286573 */
    ALGO_RATIO_K_PACK( 174u,  295u,  860u),  /* idx=135, ratio=0.210056 */
    ALGO_RATIO_K_PACK( 167u,  314u,  638u),  /* idx=136, ratio=0.155717 */
    ALGO_RATIO_K_PACK( 160u,  332u,  480u),  /* idx=137, ratio=0.117255 */
    ALGO_RATIO_K_PACK( 153u,  348u,  368u),  /* idx=138, ratio=0.089853 */
    ALGO_RATIO_K_PACK( 147u,  363u,  282u),  /* idx=139, ratio=0.068790 */
    ALGO_RATIO_K_PACK( 141u,  377u,  177u),  /* idx=140, ratio=0.043160 */
    ALGO_RATIO_K_PACK( 136u,  390u,   88u),  /* idx=141, ratio=0.021535 */
    ALGO_RATIO_K_PACK( 131u,  402u,   15u),  /* idx=142, ratio=0.003585 */
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
    ALGO_RATIO_K_PACK(  85u,  517u,    0u),  /* idx=157, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  522u,    0u),  /* idx=158, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  527u,    0u),  /* idx=159, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  532u,    0u),  /* idx=160, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  536u,    0u),  /* idx=161, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  541u,    0u),  /* idx=162, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  545u,    0u),  /* idx=163, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  72u,  549u,    0u),  /* idx=164, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  552u,    0u),  /* idx=165, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  69u,  556u,    0u),  /* idx=166, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  68u,  559u,    0u),  /* idx=167, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  67u,  563u,    0u),  /* idx=168, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  65u,  566u,    0u),  /* idx=169, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 252u,  148u,    0u),  /* idx=170, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 237u,  182u, 1421u),  /* idx=171, ratio=0.347104 */
    ALGO_RATIO_K_PACK( 224u,  211u, 2313u),  /* idx=172, ratio=0.564935 */
    ALGO_RATIO_K_PACK( 213u,  238u, 2925u),  /* idx=173, ratio=0.714375 */
    ALGO_RATIO_K_PACK( 202u,  262u, 2684u),  /* idx=174, ratio=0.655368 */
    ALGO_RATIO_K_PACK( 193u,  284u, 2278u),  /* idx=175, ratio=0.556386 */
    ALGO_RATIO_K_PACK( 184u,  305u, 1898u),  /* idx=176, ratio=0.463544 */
    ALGO_RATIO_K_PACK( 176u,  323u, 1567u),  /* idx=177, ratio=0.382617 */
    ALGO_RATIO_K_PACK( 169u,  340u, 1313u),  /* idx=178, ratio=0.320687 */
    ALGO_RATIO_K_PACK( 162u,  356u, 1125u),  /* idx=179, ratio=0.274655 */
    ALGO_RATIO_K_PACK( 156u,  370u,  983u),  /* idx=180, ratio=0.240056 */
    ALGO_RATIO_K_PACK( 150u,  384u,  839u),  /* idx=181, ratio=0.204768 */
    ALGO_RATIO_K_PACK( 144u,  396u,  716u),  /* idx=182, ratio=0.174838 */
    ALGO_RATIO_K_PACK( 139u,  408u,  612u),  /* idx=183, ratio=0.149358 */
    ALGO_RATIO_K_PACK( 135u,  419u,  515u),  /* idx=184, ratio=0.125677 */
    ALGO_RATIO_K_PACK( 130u,  429u,  424u),  /* idx=185, ratio=0.103426 */
    ALGO_RATIO_K_PACK( 126u,  439u,  329u),  /* idx=186, ratio=0.080441 */
    ALGO_RATIO_K_PACK( 122u,  448u,  196u),  /* idx=187, ratio=0.047980 */
    ALGO_RATIO_K_PACK( 118u,  456u,   76u),  /* idx=188, ratio=0.018490 */
    ALGO_RATIO_K_PACK( 115u,  464u,    0u),  /* idx=189, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 111u,  472u,    0u),  /* idx=190, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 108u,  479u,    0u),  /* idx=191, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 105u,  486u,    0u),  /* idx=192, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  493u,    0u),  /* idx=193, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  499u,    0u),  /* idx=194, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  505u,    0u),  /* idx=195, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  95u,  511u,    0u),  /* idx=196, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  93u,  516u,    0u),  /* idx=197, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  521u,    0u),  /* idx=198, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  526u,    0u),  /* idx=199, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  86u,  531u,    0u),  /* idx=200, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  535u,    0u),  /* idx=201, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  539u,    0u),  /* idx=202, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  81u,  544u,    0u),  /* idx=203, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  548u,    0u),  /* idx=204, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  551u,    0u),  /* idx=205, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  76u,  555u,    0u),  /* idx=206, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  74u,  559u,    0u),  /* idx=207, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  73u,  562u,    0u),  /* idx=208, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  71u,  565u,    0u),  /* idx=209, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 263u,  163u,    0u),  /* idx=210, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 249u,  195u, 1304u),  /* idx=211, ratio=0.318371 */
    ALGO_RATIO_K_PACK( 235u,  224u, 2155u),  /* idx=212, ratio=0.526284 */
    ALGO_RATIO_K_PACK( 223u,  250u, 2755u),  /* idx=213, ratio=0.672725 */
    ALGO_RATIO_K_PACK( 212u,  273u, 3200u),  /* idx=214, ratio=0.781445 */
    ALGO_RATIO_K_PACK( 203u,  294u, 3147u),  /* idx=215, ratio=0.768510 */
    ALGO_RATIO_K_PACK( 194u,  314u, 2770u),  /* idx=216, ratio=0.676455 */
    ALGO_RATIO_K_PACK( 185u,  332u, 2436u),  /* idx=217, ratio=0.594878 */
    ALGO_RATIO_K_PACK( 178u,  348u, 2104u),  /* idx=218, ratio=0.513860 */
    ALGO_RATIO_K_PACK( 171u,  363u, 1842u),  /* idx=219, ratio=0.449713 */
    ALGO_RATIO_K_PACK( 164u,  377u, 1645u),  /* idx=220, ratio=0.401641 */
    ALGO_RATIO_K_PACK( 158u,  391u, 1464u),  /* idx=221, ratio=0.357436 */
    ALGO_RATIO_K_PACK( 152u,  403u, 1314u),  /* idx=222, ratio=0.320975 */
    ALGO_RATIO_K_PACK( 147u,  414u, 1180u),  /* idx=223, ratio=0.288102 */
    ALGO_RATIO_K_PACK( 142u,  425u, 1050u),  /* idx=224, ratio=0.256528 */
    ALGO_RATIO_K_PACK( 137u,  435u,  912u),  /* idx=225, ratio=0.222607 */
    ALGO_RATIO_K_PACK( 133u,  444u,  745u),  /* idx=226, ratio=0.181919 */
    ALGO_RATIO_K_PACK( 129u,  453u,  594u),  /* idx=227, ratio=0.144937 */
    ALGO_RATIO_K_PACK( 125u,  461u,  456u),  /* idx=228, ratio=0.111282 */
    ALGO_RATIO_K_PACK( 121u,  469u,  329u),  /* idx=229, ratio=0.080229 */
    ALGO_RATIO_K_PACK( 118u,  477u,  213u),  /* idx=230, ratio=0.051964 */
    ALGO_RATIO_K_PACK( 115u,  484u,  106u),  /* idx=231, ratio=0.025870 */
    ALGO_RATIO_K_PACK( 112u,  491u,    6u),  /* idx=232, ratio=0.001346 */
    ALGO_RATIO_K_PACK( 109u,  497u,    0u),  /* idx=233, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 106u,  503u,    0u),  /* idx=234, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  509u,    0u),  /* idx=235, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  514u,    0u),  /* idx=236, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  520u,    0u),  /* idx=237, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  525u,    0u),  /* idx=238, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  529u,    0u),  /* idx=239, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  91u,  534u,    0u),  /* idx=240, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  89u,  538u,    0u),  /* idx=241, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  543u,    0u),  /* idx=242, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  547u,    0u),  /* idx=243, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  84u,  551u,    0u),  /* idx=244, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  82u,  554u,    0u),  /* idx=245, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  80u,  558u,    0u),  /* idx=246, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  79u,  561u,    0u),  /* idx=247, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  77u,  565u,    0u),  /* idx=248, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 275u,  178u,    0u),  /* idx=249, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 260u,  208u, 1204u),  /* idx=250, ratio=0.294032 */
    ALGO_RATIO_K_PACK( 246u,  236u, 2017u),  /* idx=251, ratio=0.492583 */
    ALGO_RATIO_K_PACK( 234u,  261u, 2603u),  /* idx=252, ratio=0.635664 */
    ALGO_RATIO_K_PACK( 222u,  284u, 3045u),  /* idx=253, ratio=0.743671 */
    ALGO_RATIO_K_PACK( 212u,  304u, 3391u),  /* idx=254, ratio=0.828093 */
    ALGO_RATIO_K_PACK( 203u,  323u, 3483u),  /* idx=255, ratio=0.850621 */
    ALGO_RATIO_K_PACK( 194u,  340u, 3125u),  /* idx=256, ratio=0.763105 */
    ALGO_RATIO_K_PACK( 186u,  356u, 2834u),  /* idx=257, ratio=0.692090 */
    ALGO_RATIO_K_PACK( 179u,  371u, 2524u),  /* idx=258, ratio=0.616395 */
    ALGO_RATIO_K_PACK( 172u,  385u, 2269u),  /* idx=259, ratio=0.554076 */
    ALGO_RATIO_K_PACK( 166u,  397u, 2054u),  /* idx=260, ratio=0.501505 */
    ALGO_RATIO_K_PACK( 160u,  409u, 1880u),  /* idx=261, ratio=0.459175 */
    ALGO_RATIO_K_PACK( 155u,  420u, 1709u),  /* idx=262, ratio=0.417398 */
    ALGO_RATIO_K_PACK( 149u,  431u, 1521u),  /* idx=263, ratio=0.371548 */
    ALGO_RATIO_K_PACK( 145u,  440u, 1321u),  /* idx=264, ratio=0.322577 */
    ALGO_RATIO_K_PACK( 140u,  450u, 1137u),  /* idx=265, ratio=0.277598 */
    ALGO_RATIO_K_PACK( 136u,  458u,  969u),  /* idx=266, ratio=0.236525 */
    ALGO_RATIO_K_PACK( 132u,  466u,  815u),  /* idx=267, ratio=0.198973 */
    ALGO_RATIO_K_PACK( 128u,  474u,  676u),  /* idx=268, ratio=0.165103 */
    ALGO_RATIO_K_PACK( 124u,  481u,  546u),  /* idx=269, ratio=0.133317 */
    ALGO_RATIO_K_PACK( 121u,  488u,  425u),  /* idx=270, ratio=0.103863 */
    ALGO_RATIO_K_PACK( 118u,  495u,  314u),  /* idx=271, ratio=0.076590 */
    ALGO_RATIO_K_PACK( 115u,  501u,  206u),  /* idx=272, ratio=0.050378 */
    ALGO_RATIO_K_PACK( 112u,  507u,  105u),  /* idx=273, ratio=0.025587 */
    ALGO_RATIO_K_PACK( 109u,  513u,    7u),  /* idx=274, ratio=0.001593 */
    ALGO_RATIO_K_PACK( 106u,  518u,    0u),  /* idx=275, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  523u,    0u),  /* idx=276, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 101u,  528u,    0u),  /* idx=277, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  99u,  533u,    0u),  /* idx=278, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  97u,  537u,    0u),  /* idx=279, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  94u,  542u,    0u),  /* idx=280, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  92u,  546u,    0u),  /* idx=281, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  90u,  550u,    0u),  /* idx=282, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  88u,  554u,    0u),  /* idx=283, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  87u,  557u,    0u),  /* idx=284, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  85u,  561u,    0u),  /* idx=285, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  83u,  564u,    0u),  /* idx=286, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 286u,  192u,    0u),  /* idx=287, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 271u,  221u, 1119u),  /* idx=288, ratio=0.273150 */
    ALGO_RATIO_K_PACK( 256u,  248u, 1896u),  /* idx=289, ratio=0.462938 */
    ALGO_RATIO_K_PACK( 244u,  272u, 2467u),  /* idx=290, ratio=0.602473 */
    ALGO_RATIO_K_PACK( 232u,  294u, 2905u),  /* idx=291, ratio=0.709380 */
    ALGO_RATIO_K_PACK( 222u,  314u, 3251u),  /* idx=292, ratio=0.793907 */
    ALGO_RATIO_K_PACK( 212u,  332u, 3532u),  /* idx=293, ratio=0.862414 */
    ALGO_RATIO_K_PACK( 203u,  349u, 3750u),  /* idx=294, ratio=0.915709 */
    ALGO_RATIO_K_PACK( 195u,  364u, 3407u),  /* idx=295, ratio=0.831915 */
    ALGO_RATIO_K_PACK( 187u,  378u, 3121u),  /* idx=296, ratio=0.762038 */
    ALGO_RATIO_K_PACK( 180u,  392u, 2853u),  /* idx=297, ratio=0.696688 */
    ALGO_RATIO_K_PACK( 174u,  404u, 2602u),  /* idx=298, ratio=0.635337 */
    ALGO_RATIO_K_PACK( 168u,  416u, 2396u),  /* idx=299, ratio=0.585036 */
    ALGO_RATIO_K_PACK( 162u,  426u, 2158u),  /* idx=300, ratio=0.526918 */
    ALGO_RATIO_K_PACK( 157u,  436u, 1920u),  /* idx=301, ratio=0.468920 */
    ALGO_RATIO_K_PACK( 152u,  446u, 1704u),  /* idx=302, ratio=0.416024 */
    ALGO_RATIO_K_PACK( 147u,  455u, 1505u),  /* idx=303, ratio=0.367583 */
    ALGO_RATIO_K_PACK( 143u,  463u, 1323u),  /* idx=304, ratio=0.323095 */
    ALGO_RATIO_K_PACK( 138u,  471u, 1156u),  /* idx=305, ratio=0.282177 */
    ALGO_RATIO_K_PACK( 134u,  479u, 1003u),  /* idx=306, ratio=0.245016 */
    ALGO_RATIO_K_PACK( 131u,  486u,  862u),  /* idx=307, ratio=0.210461 */
    ALGO_RATIO_K_PACK( 127u,  493u,  730u),  /* idx=308, ratio=0.178319 */
    ALGO_RATIO_K_PACK( 124u,  499u,  606u),  /* idx=309, ratio=0.147919 */
    ALGO_RATIO_K_PACK( 120u,  505u,  486u),  /* idx=310, ratio=0.118569 */
    ALGO_RATIO_K_PACK( 117u,  511u,  373u),  /* idx=311, ratio=0.091208 */
    ALGO_RATIO_K_PACK( 114u,  516u,  262u),  /* idx=312, ratio=0.064063 */
    ALGO_RATIO_K_PACK( 112u,  522u,  156u),  /* idx=313, ratio=0.038138 */
    ALGO_RATIO_K_PACK( 109u,  527u,   55u),  /* idx=314, ratio=0.013384 */
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
    ALGO_RATIO_K_PACK( 281u,  234u, 1044u),  /* idx=325, ratio=0.255037 */
    ALGO_RATIO_K_PACK( 267u,  259u, 1788u),  /* idx=326, ratio=0.436659 */
    ALGO_RATIO_K_PACK( 254u,  283u, 2345u),  /* idx=327, ratio=0.572576 */
    ALGO_RATIO_K_PACK( 242u,  304u, 2777u),  /* idx=328, ratio=0.678112 */
    ALGO_RATIO_K_PACK( 231u,  323u, 3122u),  /* idx=329, ratio=0.762431 */
    ALGO_RATIO_K_PACK( 221u,  341u, 3404u),  /* idx=330, ratio=0.831345 */
    ALGO_RATIO_K_PACK( 212u,  357u, 3639u),  /* idx=331, ratio=0.888724 */
    ALGO_RATIO_K_PACK( 203u,  372u, 3838u),  /* idx=332, ratio=0.937239 */
    ALGO_RATIO_K_PACK( 196u,  386u, 3568u),  /* idx=333, ratio=0.871331 */
    ALGO_RATIO_K_PACK( 188u,  399u, 3329u),  /* idx=334, ratio=0.813039 */
    ALGO_RATIO_K_PACK( 182u,  411u, 3121u),  /* idx=335, ratio=0.762057 */
    ALGO_RATIO_K_PACK( 175u,  422u, 2813u),  /* idx=336, ratio=0.686900 */
    ALGO_RATIO_K_PACK( 169u,  432u, 2543u),  /* idx=337, ratio=0.621114 */
    ALGO_RATIO_K_PACK( 164u,  442u, 2292u),  /* idx=338, ratio=0.559687 */
    ALGO_RATIO_K_PACK( 159u,  451u, 2061u),  /* idx=339, ratio=0.503197 */
    ALGO_RATIO_K_PACK( 154u,  460u, 1850u),  /* idx=340, ratio=0.451826 */
    ALGO_RATIO_K_PACK( 149u,  468u, 1656u),  /* idx=341, ratio=0.404300 */
    ALGO_RATIO_K_PACK( 145u,  476u, 1477u),  /* idx=342, ratio=0.360694 */
    ALGO_RATIO_K_PACK( 141u,  483u, 1312u),  /* idx=343, ratio=0.320307 */
    ALGO_RATIO_K_PACK( 137u,  490u, 1161u),  /* idx=344, ratio=0.283438 */
    ALGO_RATIO_K_PACK( 133u,  497u, 1017u),  /* idx=345, ratio=0.248453 */
    ALGO_RATIO_K_PACK( 130u,  503u,  881u),  /* idx=346, ratio=0.215148 */
    ALGO_RATIO_K_PACK( 126u,  509u,  751u),  /* idx=347, ratio=0.183358 */
    ALGO_RATIO_K_PACK( 123u,  515u,  626u),  /* idx=348, ratio=0.152955 */
    ALGO_RATIO_K_PACK( 120u,  520u,  507u),  /* idx=349, ratio=0.123842 */
    ALGO_RATIO_K_PACK( 117u,  525u,  390u),  /* idx=350, ratio=0.095296 */
    ALGO_RATIO_K_PACK( 114u,  530u,  281u),  /* idx=351, ratio=0.068553 */
    ALGO_RATIO_K_PACK( 112u,  535u,  175u),  /* idx=352, ratio=0.042721 */
    ALGO_RATIO_K_PACK( 109u,  539u,   77u),  /* idx=353, ratio=0.018740 */
    ALGO_RATIO_K_PACK( 107u,  544u,    0u),  /* idx=354, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 104u,  548u,    0u),  /* idx=355, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 102u,  552u,    0u),  /* idx=356, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 100u,  556u,    0u),  /* idx=357, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  98u,  559u,    0u),  /* idx=358, ratio=0.000000 */
    ALGO_RATIO_K_PACK(  96u,  563u,    0u),  /* idx=359, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 308u,  218u,    0u),  /* idx=360, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 291u,  246u,  979u),  /* idx=361, ratio=0.239177 */
    ALGO_RATIO_K_PACK( 276u,  271u, 1692u),  /* idx=362, ratio=0.413203 */
    ALGO_RATIO_K_PACK( 263u,  293u, 2234u),  /* idx=363, ratio=0.545506 */
    ALGO_RATIO_K_PACK( 251u,  313u, 2660u),  /* idx=364, ratio=0.649485 */
    ALGO_RATIO_K_PACK( 240u,  332u, 3003u),  /* idx=365, ratio=0.733356 */
    ALGO_RATIO_K_PACK( 230u,  349u, 3286u),  /* idx=366, ratio=0.802437 */
    ALGO_RATIO_K_PACK( 220u,  365u, 3523u),  /* idx=367, ratio=0.860324 */
    ALGO_RATIO_K_PACK( 212u,  379u, 3725u),  /* idx=368, ratio=0.909534 */
    ALGO_RATIO_K_PACK( 204u,  393u, 3898u),  /* idx=369, ratio=0.951882 */
    ALGO_RATIO_K_PACK( 196u,  405u, 3649u),  /* idx=370, ratio=0.891155 */
    ALGO_RATIO_K_PACK( 189u,  417u, 3426u),  /* idx=371, ratio=0.836624 */
    ALGO_RATIO_K_PACK( 183u,  428u, 3192u),  /* idx=372, ratio=0.779508 */
    ALGO_RATIO_K_PACK( 177u,  438u, 2893u),  /* idx=373, ratio=0.706575 */
    ALGO_RATIO_K_PACK( 171u,  448u, 2633u),  /* idx=374, ratio=0.642898 */
    ALGO_RATIO_K_PACK( 166u,  457u, 2392u),  /* idx=375, ratio=0.584223 */
    ALGO_RATIO_K_PACK( 160u,  465u, 2170u),  /* idx=376, ratio=0.529936 */
    ALGO_RATIO_K_PACK( 156u,  473u, 1966u),  /* idx=377, ratio=0.480006 */
    ALGO_RATIO_K_PACK( 151u,  481u, 1778u),  /* idx=378, ratio=0.434189 */
    ALGO_RATIO_K_PACK( 147u,  488u, 1603u),  /* idx=379, ratio=0.391402 */
    ALGO_RATIO_K_PACK( 143u,  495u, 1439u),  /* idx=380, ratio=0.351452 */
    ALGO_RATIO_K_PACK( 139u,  501u, 1287u),  /* idx=381, ratio=0.314222 */
    ALGO_RATIO_K_PACK( 135u,  507u, 1141u),  /* idx=382, ratio=0.278538 */
    ALGO_RATIO_K_PACK( 132u,  513u, 1000u),  /* idx=383, ratio=0.244221 */
    ALGO_RATIO_K_PACK( 129u,  519u,  865u),  /* idx=384, ratio=0.211130 */
    ALGO_RATIO_K_PACK( 126u,  524u,  736u),  /* idx=385, ratio=0.179828 */
    ALGO_RATIO_K_PACK( 123u,  529u,  614u),  /* idx=386, ratio=0.149940 */
    ALGO_RATIO_K_PACK( 120u,  534u,  498u),  /* idx=387, ratio=0.121492 */
    ALGO_RATIO_K_PACK( 117u,  538u,  387u),  /* idx=388, ratio=0.094574 */
    ALGO_RATIO_K_PACK( 114u,  543u,  284u),  /* idx=389, ratio=0.069384 */
    ALGO_RATIO_K_PACK( 112u,  547u,  188u),  /* idx=390, ratio=0.045905 */
    ALGO_RATIO_K_PACK( 109u,  551u,  102u),  /* idx=391, ratio=0.024890 */
    ALGO_RATIO_K_PACK( 107u,  555u,   27u),  /* idx=392, ratio=0.006495 */
    ALGO_RATIO_K_PACK( 105u,  559u,    0u),  /* idx=393, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 103u,  562u,    0u),  /* idx=394, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 318u,  231u,    0u),  /* idx=395, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 301u,  258u,  922u),  /* idx=396, ratio=0.225175 */
    ALGO_RATIO_K_PACK( 286u,  281u, 1606u),  /* idx=397, ratio=0.392138 */
    ALGO_RATIO_K_PACK( 272u,  303u, 2133u),  /* idx=398, ratio=0.520880 */
    ALGO_RATIO_K_PACK( 260u,  323u, 2552u),  /* idx=399, ratio=0.623176 */
    ALGO_RATIO_K_PACK( 249u,  341u, 2893u),  /* idx=400, ratio=0.706416 */
    ALGO_RATIO_K_PACK( 238u,  357u, 3176u),  /* idx=401, ratio=0.775472 */
    ALGO_RATIO_K_PACK( 229u,  373u, 3414u),  /* idx=402, ratio=0.833684 */
    ALGO_RATIO_K_PACK( 220u,  387u, 3618u),  /* idx=403, ratio=0.883420 */
    ALGO_RATIO_K_PACK( 211u,  400u, 3794u),  /* idx=404, ratio=0.926407 */
    ALGO_RATIO_K_PACK( 204u,  412u, 3947u),  /* idx=405, ratio=0.963930 */
    ALGO_RATIO_K_PACK( 197u,  423u, 3717u),  /* idx=406, ratio=0.907677 */
    ALGO_RATIO_K_PACK( 190u,  434u, 3507u),  /* idx=407, ratio=0.856501 */
    ALGO_RATIO_K_PACK( 184u,  444u, 3205u),  /* idx=408, ratio=0.782712 */
    ALGO_RATIO_K_PACK( 178u,  453u, 2942u),  /* idx=409, ratio=0.718425 */
    ALGO_RATIO_K_PACK( 172u,  462u, 2695u),  /* idx=410, ratio=0.658088 */
    ALGO_RATIO_K_PACK( 167u,  470u, 2467u),  /* idx=411, ratio=0.602378 */
    ALGO_RATIO_K_PACK( 162u,  478u, 2255u),  /* idx=412, ratio=0.550587 */
    ALGO_RATIO_K_PACK( 158u,  485u, 2058u),  /* idx=413, ratio=0.502470 */
    ALGO_RATIO_K_PACK( 153u,  492u, 1875u),  /* idx=414, ratio=0.457912 */
    ALGO_RATIO_K_PACK( 149u,  499u, 1703u),  /* idx=415, ratio=0.415913 */
    ALGO_RATIO_K_PACK( 145u,  505u, 1539u),  /* idx=416, ratio=0.375821 */
    ALGO_RATIO_K_PACK( 141u,  511u, 1382u),  /* idx=417, ratio=0.337482 */
    ALGO_RATIO_K_PACK( 138u,  517u, 1234u),  /* idx=418, ratio=0.301430 */
    ALGO_RATIO_K_PACK( 134u,  522u, 1091u),  /* idx=419, ratio=0.266472 */
    ALGO_RATIO_K_PACK( 131u,  528u,  955u),  /* idx=420, ratio=0.233325 */
    ALGO_RATIO_K_PACK( 128u,  532u,  826u),  /* idx=421, ratio=0.201820 */
    ALGO_RATIO_K_PACK( 125u,  537u,  706u),  /* idx=422, ratio=0.172311 */
    ALGO_RATIO_K_PACK( 122u,  542u,  591u),  /* idx=423, ratio=0.144380 */
    ALGO_RATIO_K_PACK( 119u,  546u,  486u),  /* idx=424, ratio=0.118622 */
    ALGO_RATIO_K_PACK( 117u,  550u,  389u),  /* idx=425, ratio=0.094971 */
    ALGO_RATIO_K_PACK( 114u,  554u,  304u),  /* idx=426, ratio=0.074285 */
    ALGO_RATIO_K_PACK( 112u,  558u,  173u),  /* idx=427, ratio=0.042142 */
    ALGO_RATIO_K_PACK( 109u,  562u,    0u),  /* idx=428, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 328u,  244u,    0u),  /* idx=429, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 311u,  269u,  871u),  /* idx=430, ratio=0.212721 */
    ALGO_RATIO_K_PACK( 295u,  292u, 1528u),  /* idx=431, ratio=0.373117 */
    ALGO_RATIO_K_PACK( 282u,  313u, 2041u),  /* idx=432, ratio=0.498381 */
    ALGO_RATIO_K_PACK( 269u,  332u, 2453u),  /* idx=433, ratio=0.598916 */
    ALGO_RATIO_K_PACK( 257u,  350u, 2790u),  /* idx=434, ratio=0.681386 */
    ALGO_RATIO_K_PACK( 247u,  366u, 3072u),  /* idx=435, ratio=0.750260 */
    ALGO_RATIO_K_PACK( 237u,  380u, 3311u),  /* idx=436, ratio=0.808644 */
    ALGO_RATIO_K_PACK( 228u,  394u, 3517u),  /* idx=437, ratio=0.858764 */
    ALGO_RATIO_K_PACK( 219u,  407u, 3695u),  /* idx=438, ratio=0.902259 */
    ALGO_RATIO_K_PACK( 211u,  419u, 3851u),  /* idx=439, ratio=0.940362 */
    ALGO_RATIO_K_PACK( 204u,  430u, 3989u),  /* idx=440, ratio=0.974016 */
    ALGO_RATIO_K_PACK( 197u,  440u, 3774u),  /* idx=441, ratio=0.921657 */
    ALGO_RATIO_K_PACK( 191u,  450u, 3510u),  /* idx=442, ratio=0.857038 */
    ALGO_RATIO_K_PACK( 185u,  459u, 3219u),  /* idx=443, ratio=0.786043 */
    ALGO_RATIO_K_PACK( 179u,  467u, 2969u),  /* idx=444, ratio=0.725134 */
    ALGO_RATIO_K_PACK( 174u,  475u, 2736u),  /* idx=445, ratio=0.668187 */
    ALGO_RATIO_K_PACK( 169u,  483u, 2519u),  /* idx=446, ratio=0.615223 */
    ALGO_RATIO_K_PACK( 164u,  490u, 2317u),  /* idx=447, ratio=0.565768 */
    ALGO_RATIO_K_PACK( 159u,  497u, 2127u),  /* idx=448, ratio=0.519360 */
    ALGO_RATIO_K_PACK( 155u,  503u, 1945u),  /* idx=449, ratio=0.475010 */
    ALGO_RATIO_K_PACK( 151u,  509u, 1774u),  /* idx=450, ratio=0.433130 */
    ALGO_RATIO_K_PACK( 147u,  515u, 1610u),  /* idx=451, ratio=0.393069 */
    ALGO_RATIO_K_PACK( 143u,  521u, 1455u),  /* idx=452, ratio=0.355250 */
    ALGO_RATIO_K_PACK( 140u,  526u, 1306u),  /* idx=453, ratio=0.318934 */
    ALGO_RATIO_K_PACK( 136u,  531u, 1165u),  /* idx=454, ratio=0.284542 */
    ALGO_RATIO_K_PACK( 133u,  536u, 1032u),  /* idx=455, ratio=0.252004 */
    ALGO_RATIO_K_PACK( 130u,  541u,  907u),  /* idx=456, ratio=0.221406 */
    ALGO_RATIO_K_PACK( 127u,  545u,  763u),  /* idx=457, ratio=0.186376 */
    ALGO_RATIO_K_PACK( 124u,  549u,  556u),  /* idx=458, ratio=0.135803 */
    ALGO_RATIO_K_PACK( 122u,  553u,  360u),  /* idx=459, ratio=0.088030 */
    ALGO_RATIO_K_PACK( 119u,  557u,  175u),  /* idx=460, ratio=0.042830 */
    ALGO_RATIO_K_PACK( 117u,  561u,    0u),  /* idx=461, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 337u,  256u,    0u),  /* idx=462, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 320u,  280u,  825u),  /* idx=463, ratio=0.201572 */
    ALGO_RATIO_K_PACK( 305u,  303u, 1457u),  /* idx=464, ratio=0.355856 */
    ALGO_RATIO_K_PACK( 291u,  323u, 1956u),  /* idx=465, ratio=0.477746 */
    ALGO_RATIO_K_PACK( 278u,  341u, 2361u),  /* idx=466, ratio=0.576474 */
    ALGO_RATIO_K_PACK( 266u,  358u, 2695u),  /* idx=467, ratio=0.658069 */
    ALGO_RATIO_K_PACK( 255u,  374u, 2976u),  /* idx=468, ratio=0.726636 */
    ALGO_RATIO_K_PACK( 245u,  388u, 3215u),  /* idx=469, ratio=0.785064 */
    ALGO_RATIO_K_PACK( 235u,  401u, 3421u),  /* idx=470, ratio=0.835447 */
    ALGO_RATIO_K_PACK( 227u,  414u, 3601u),  /* idx=471, ratio=0.879339 */
    ALGO_RATIO_K_PACK( 219u,  425u, 3759u),  /* idx=472, ratio=0.917919 */
    ALGO_RATIO_K_PACK( 211u,  436u, 3899u),  /* idx=473, ratio=0.952096 */
    ALGO_RATIO_K_PACK( 204u,  446u, 4024u),  /* idx=474, ratio=0.982584 */
    ALGO_RATIO_K_PACK( 198u,  455u, 3769u),  /* idx=475, ratio=0.920402 */
    ALGO_RATIO_K_PACK( 191u,  464u, 3456u),  /* idx=476, ratio=0.844065 */
    ALGO_RATIO_K_PACK( 185u,  472u, 3214u),  /* idx=477, ratio=0.784936 */
    ALGO_RATIO_K_PACK( 180u,  480u, 2981u),  /* idx=478, ratio=0.727914 */
    ALGO_RATIO_K_PACK( 175u,  488u, 2762u),  /* idx=479, ratio=0.674431 */
    ALGO_RATIO_K_PACK( 170u,  495u, 2554u),  /* idx=480, ratio=0.623714 */
    ALGO_RATIO_K_PACK( 165u,  501u, 2359u),  /* idx=481, ratio=0.576178 */
    ALGO_RATIO_K_PACK( 161u,  508u, 2172u),  /* idx=482, ratio=0.530388 */
    ALGO_RATIO_K_PACK( 157u,  513u, 1994u),  /* idx=483, ratio=0.486827 */
    ALGO_RATIO_K_PACK( 153u,  519u, 1823u),  /* idx=484, ratio=0.445225 */
    ALGO_RATIO_K_PACK( 149u,  525u, 1662u),  /* idx=485, ratio=0.405856 */
    ALGO_RATIO_K_PACK( 145u,  530u, 1493u),  /* idx=486, ratio=0.364684 */
    ALGO_RATIO_K_PACK( 142u,  535u, 1239u),  /* idx=487, ratio=0.302671 */
    ALGO_RATIO_K_PACK( 138u,  539u, 1001u),  /* idx=488, ratio=0.244471 */
    ALGO_RATIO_K_PACK( 135u,  544u,  777u),  /* idx=489, ratio=0.189743 */
    ALGO_RATIO_K_PACK( 132u,  548u,  566u),  /* idx=490, ratio=0.138186 */
    ALGO_RATIO_K_PACK( 129u,  552u,  367u),  /* idx=491, ratio=0.089531 */
    ALGO_RATIO_K_PACK( 126u,  556u,  178u),  /* idx=492, ratio=0.043540 */
    ALGO_RATIO_K_PACK( 124u,  560u,    0u),  /* idx=493, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 347u,  268u,    0u),  /* idx=494, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 329u,  291u,  784u),  /* idx=495, ratio=0.191534 */
    ALGO_RATIO_K_PACK( 314u,  313u, 1393u),  /* idx=496, ratio=0.340122 */
    ALGO_RATIO_K_PACK( 299u,  332u, 1879u),  /* idx=497, ratio=0.458751 */
    ALGO_RATIO_K_PACK( 286u,  350u, 2275u),  /* idx=498, ratio=0.555653 */
    ALGO_RATIO_K_PACK( 274u,  366u, 2606u),  /* idx=499, ratio=0.636295 */
    ALGO_RATIO_K_PACK( 263u,  381u, 2885u),  /* idx=500, ratio=0.704454 */
    ALGO_RATIO_K_PACK( 253u,  395u, 3124u),  /* idx=501, ratio=0.762820 */
    ALGO_RATIO_K_PACK( 243u,  408u, 3331u),  /* idx=502, ratio=0.813362 */
    ALGO_RATIO_K_PACK( 234u,  420u, 3512u),  /* idx=503, ratio=0.857554 */
    ALGO_RATIO_K_PACK( 226u,  431u, 3671u),  /* idx=504, ratio=0.896522 */
    ALGO_RATIO_K_PACK( 218u,  442u, 3813u),  /* idx=505, ratio=0.931142 */
    ALGO_RATIO_K_PACK( 211u,  451u, 3940u),  /* idx=506, ratio=0.962101 */
    ALGO_RATIO_K_PACK( 204u,  461u, 3975u),  /* idx=507, ratio=0.970679 */
    ALGO_RATIO_K_PACK( 198u,  469u, 3674u),  /* idx=508, ratio=0.897141 */
    ALGO_RATIO_K_PACK( 192u,  477u, 3427u),  /* idx=509, ratio=0.836918 */
    ALGO_RATIO_K_PACK( 186u,  485u, 3198u),  /* idx=510, ratio=0.781063 */
    ALGO_RATIO_K_PACK( 181u,  492u, 2979u),  /* idx=511, ratio=0.727397 */
    ALGO_RATIO_K_PACK( 176u,  499u, 2770u),  /* idx=512, ratio=0.676552 */
    ALGO_RATIO_K_PACK( 171u,  506u, 2571u),  /* idx=513, ratio=0.627780 */
    ALGO_RATIO_K_PACK( 167u,  512u, 2380u),  /* idx=514, ratio=0.581099 */
    ALGO_RATIO_K_PACK( 162u,  518u, 2099u),  /* idx=515, ratio=0.512638 */
    ALGO_RATIO_K_PACK( 158u,  523u, 1801u),  /* idx=516, ratio=0.439921 */
    ALGO_RATIO_K_PACK( 154u,  528u, 1524u),  /* idx=517, ratio=0.372066 */
    ALGO_RATIO_K_PACK( 150u,  533u, 1264u),  /* idx=518, ratio=0.308599 */
    ALGO_RATIO_K_PACK( 147u,  538u, 1020u),  /* idx=519, ratio=0.249109 */
    ALGO_RATIO_K_PACK( 143u,  543u,  791u),  /* idx=520, ratio=0.193234 */
    ALGO_RATIO_K_PACK( 140u,  547u,  576u),  /* idx=521, ratio=0.140653 */
    ALGO_RATIO_K_PACK( 137u,  551u,  373u),  /* idx=522, ratio=0.091083 */
    ALGO_RATIO_K_PACK( 134u,  556u,  181u),  /* idx=523, ratio=0.044274 */
    ALGO_RATIO_K_PACK( 131u,  559u,    0u),  /* idx=524, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 356u,  279u,    0u),  /* idx=525, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 338u,  302u,  747u),  /* idx=526, ratio=0.182448 */
    ALGO_RATIO_K_PACK( 322u,  323u, 1334u),  /* idx=527, ratio=0.325719 */
    ALGO_RATIO_K_PACK( 308u,  341u, 1807u),  /* idx=528, ratio=0.441209 */
    ALGO_RATIO_K_PACK( 294u,  359u, 2196u),  /* idx=529, ratio=0.536283 */
    ALGO_RATIO_K_PACK( 282u,  374u, 2522u),  /* idx=530, ratio=0.615916 */
    ALGO_RATIO_K_PACK( 271u,  389u, 2799u),  /* idx=531, ratio=0.683587 */
    ALGO_RATIO_K_PACK( 260u,  403u, 3038u),  /* idx=532, ratio=0.741802 */
    ALGO_RATIO_K_PACK( 251u,  415u, 3245u),  /* idx=533, ratio=0.792415 */
    ALGO_RATIO_K_PACK( 242u,  427u, 3427u),  /* idx=534, ratio=0.836823 */
    ALGO_RATIO_K_PACK( 233u,  438u, 3588u),  /* idx=535, ratio=0.876101 */
    ALGO_RATIO_K_PACK( 225u,  448u, 3731u),  /* idx=536, ratio=0.911089 */
    ALGO_RATIO_K_PACK( 218u,  457u, 3859u),  /* idx=537, ratio=0.942455 */
    ALGO_RATIO_K_PACK( 211u,  466u, 3975u),  /* idx=538, ratio=0.970733 */
    ALGO_RATIO_K_PACK( 204u,  474u, 3856u),  /* idx=539, ratio=0.941603 */
    ALGO_RATIO_K_PACK( 198u,  482u, 3605u),  /* idx=540, ratio=0.880286 */
    ALGO_RATIO_K_PACK( 192u,  490u, 3388u),  /* idx=541, ratio=0.827313 */
    ALGO_RATIO_K_PACK( 187u,  497u, 3172u),  /* idx=542, ratio=0.774571 */
    ALGO_RATIO_K_PACK( 182u,  503u, 2830u),  /* idx=543, ratio=0.691050 */
    ALGO_RATIO_K_PACK( 177u,  510u, 2475u),  /* idx=544, ratio=0.604353 */
    ALGO_RATIO_K_PACK( 172u,  516u, 2146u),  /* idx=545, ratio=0.524004 */
    ALGO_RATIO_K_PACK( 168u,  521u, 1840u),  /* idx=546, ratio=0.449331 */
    ALGO_RATIO_K_PACK( 164u,  527u, 1555u),  /* idx=547, ratio=0.379752 */
    ALGO_RATIO_K_PACK( 160u,  532u, 1289u),  /* idx=548, ratio=0.314764 */
    ALGO_RATIO_K_PACK( 156u,  537u, 1040u),  /* idx=549, ratio=0.253927 */
    ALGO_RATIO_K_PACK( 152u,  542u,  806u),  /* idx=550, ratio=0.196855 */
    ALGO_RATIO_K_PACK( 149u,  546u,  586u),  /* idx=551, ratio=0.143210 */
    ALGO_RATIO_K_PACK( 145u,  551u,  380u),  /* idx=552, ratio=0.092691 */
    ALGO_RATIO_K_PACK( 142u,  555u,  184u),  /* idx=553, ratio=0.045033 */
    ALGO_RATIO_K_PACK( 139u,  559u,    0u),  /* idx=554, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 365u,  290u,    0u),  /* idx=555, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 347u,  312u,  713u),  /* idx=556, ratio=0.174185 */
    ALGO_RATIO_K_PACK( 331u,  332u, 1280u),  /* idx=557, ratio=0.312487 */
    ALGO_RATIO_K_PACK( 316u,  351u, 1740u),  /* idx=558, ratio=0.424959 */
    ALGO_RATIO_K_PACK( 302u,  367u, 2122u),  /* idx=559, ratio=0.518219 */
    ALGO_RATIO_K_PACK( 290u,  382u, 2444u),  /* idx=560, ratio=0.596802 */
    ALGO_RATIO_K_PACK( 278u,  397u, 2719u),  /* idx=561, ratio=0.663920 */
    ALGO_RATIO_K_PACK( 268u,  410u, 2956u),  /* idx=562, ratio=0.721911 */
    ALGO_RATIO_K_PACK( 258u,  422u, 3163u),  /* idx=563, ratio=0.772520 */
    ALGO_RATIO_K_PACK( 249u,  433u, 3346u),  /* idx=564, ratio=0.817070 */
    ALGO_RATIO_K_PACK( 240u,  444u, 3508u),  /* idx=565, ratio=0.856589 */
    ALGO_RATIO_K_PACK( 232u,  453u, 3652u),  /* idx=566, ratio=0.891883 */
    ALGO_RATIO_K_PACK( 225u,  463u, 3782u),  /* idx=567, ratio=0.923595 */
    ALGO_RATIO_K_PACK( 217u,  471u, 3899u),  /* idx=568, ratio=0.952245 */
    ALGO_RATIO_K_PACK( 211u,  480u, 3992u),  /* idx=569, ratio=0.974879 */
    ALGO_RATIO_K_PACK( 205u,  487u, 3728u),  /* idx=570, ratio=0.910404 */
    ALGO_RATIO_K_PACK( 199u,  494u, 3296u),  /* idx=571, ratio=0.804927 */
    ALGO_RATIO_K_PACK( 193u,  501u, 2899u),  /* idx=572, ratio=0.707987 */
    ALGO_RATIO_K_PACK( 188u,  508u, 2533u),  /* idx=573, ratio=0.618589 */
    ALGO_RATIO_K_PACK( 183u,  514u, 2194u),  /* idx=574, ratio=0.535886 */
    ALGO_RATIO_K_PACK( 178u,  520u, 1880u),  /* idx=575, ratio=0.459152 */
    ALGO_RATIO_K_PACK( 173u,  525u, 1588u),  /* idx=576, ratio=0.387763 */
    ALGO_RATIO_K_PACK( 169u,  531u, 1315u),  /* idx=577, ratio=0.321181 */
    ALGO_RATIO_K_PACK( 165u,  536u, 1060u),  /* idx=578, ratio=0.258935 */
    ALGO_RATIO_K_PACK( 161u,  541u,  822u),  /* idx=579, ratio=0.200615 */
    ALGO_RATIO_K_PACK( 157u,  545u,  597u),  /* idx=580, ratio=0.145861 */
    ALGO_RATIO_K_PACK( 154u,  550u,  386u),  /* idx=581, ratio=0.094356 */
    ALGO_RATIO_K_PACK( 150u,  554u,  188u),  /* idx=582, ratio=0.045819 */
    ALGO_RATIO_K_PACK( 147u,  558u,    0u),  /* idx=583, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 374u,  301u,    0u),  /* idx=584, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 356u,  323u,  682u),  /* idx=585, ratio=0.166639 */
    ALGO_RATIO_K_PACK( 339u,  342u, 1230u),  /* idx=586, ratio=0.300289 */
    ALGO_RATIO_K_PACK( 324u,  359u, 1678u),  /* idx=587, ratio=0.409864 */
    ALGO_RATIO_K_PACK( 310u,  375u, 2053u),  /* idx=588, ratio=0.501332 */
    ALGO_RATIO_K_PACK( 298u,  390u, 2370u),  /* idx=589, ratio=0.578838 */
    ALGO_RATIO_K_PACK( 286u,  404u, 2643u),  /* idx=590, ratio=0.645353 */
    ALGO_RATIO_K_PACK( 275u,  417u, 2879u),  /* idx=591, ratio=0.703059 */
    ALGO_RATIO_K_PACK( 265u,  428u, 3086u),  /* idx=592, ratio=0.753599 */
    ALGO_RATIO_K_PACK( 256u,  439u, 3269u),  /* idx=593, ratio=0.798228 */
    ALGO_RATIO_K_PACK( 247u,  450u, 3431u),  /* idx=594, ratio=0.837927 */
    ALGO_RATIO_K_PACK( 239u,  459u, 3577u),  /* idx=595, ratio=0.873469 */
    ALGO_RATIO_K_PACK( 231u,  468u, 3708u),  /* idx=596, ratio=0.905475 */
    ALGO_RATIO_K_PACK( 224u,  477u, 3827u),  /* idx=597, ratio=0.934448 */
    ALGO_RATIO_K_PACK( 217u,  485u, 3830u),  /* idx=598, ratio=0.935348 */
    ALGO_RATIO_K_PACK( 211u,  492u, 3383u),  /* idx=599, ratio=0.826027 */
    ALGO_RATIO_K_PACK( 205u,  499u, 2972u),  /* idx=600, ratio=0.725776 */
    ALGO_RATIO_K_PACK( 199u,  506u, 2594u),  /* idx=601, ratio=0.633513 */
    ALGO_RATIO_K_PACK( 194u,  512u, 2245u),  /* idx=602, ratio=0.548319 */
    ALGO_RATIO_K_PACK( 188u,  518u, 1922u),  /* idx=603, ratio=0.469411 */
    ALGO_RATIO_K_PACK( 183u,  524u, 1622u),  /* idx=604, ratio=0.396119 */
    ALGO_RATIO_K_PACK( 179u,  529u, 1343u),  /* idx=605, ratio=0.327864 */
    ALGO_RATIO_K_PACK( 174u,  534u, 1082u),  /* idx=606, ratio=0.264144 */
    ALGO_RATIO_K_PACK( 170u,  539u,  838u),  /* idx=607, ratio=0.204521 */
    ALGO_RATIO_K_PACK( 166u,  544u,  609u),  /* idx=608, ratio=0.148613 */
    ALGO_RATIO_K_PACK( 162u,  549u,  393u),  /* idx=609, ratio=0.096082 */
    ALGO_RATIO_K_PACK( 159u,  553u,  191u),  /* idx=610, ratio=0.046632 */
    ALGO_RATIO_K_PACK( 155u,  557u,    0u),  /* idx=611, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 382u,  312u,    0u),  /* idx=612, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 364u,  332u,  654u),  /* idx=613, ratio=0.159718 */
    ALGO_RATIO_K_PACK( 347u,  351u, 1183u),  /* idx=614, ratio=0.289006 */
    ALGO_RATIO_K_PACK( 332u,  368u, 1621u),  /* idx=615, ratio=0.395804 */
    ALGO_RATIO_K_PACK( 318u,  384u, 1988u),  /* idx=616, ratio=0.485510 */
    ALGO_RATIO_K_PACK( 305u,  398u, 2301u),  /* idx=617, ratio=0.561924 */
    ALGO_RATIO_K_PACK( 293u,  411u, 2571u),  /* idx=618, ratio=0.627796 */
    ALGO_RATIO_K_PACK( 282u,  423u, 2806u),  /* idx=619, ratio=0.685167 */
    ALGO_RATIO_K_PACK( 272u,  435u, 3012u),  /* idx=620, ratio=0.735583 */
    ALGO_RATIO_K_PACK( 263u,  446u, 3195u),  /* idx=621, ratio=0.780236 */
    ALGO_RATIO_K_PACK( 254u,  455u, 3358u),  /* idx=622, ratio=0.820061 */
    ALGO_RATIO_K_PACK( 245u,  465u, 3505u),  /* idx=623, ratio=0.855800 */
    ALGO_RATIO_K_PACK( 238u,  474u, 3637u),  /* idx=624, ratio=0.888053 */
    ALGO_RATIO_K_PACK( 230u,  482u, 3756u),  /* idx=625, ratio=0.917305 */
    ALGO_RATIO_K_PACK( 223u,  489u, 3474u),  /* idx=626, ratio=0.848263 */
    ALGO_RATIO_K_PACK( 217u,  497u, 3049u),  /* idx=627, ratio=0.744482 */
    ALGO_RATIO_K_PACK( 211u,  504u, 2658u),  /* idx=628, ratio=0.649174 */
    ALGO_RATIO_K_PACK( 205u,  510u, 2299u),  /* idx=629, ratio=0.561342 */
    ALGO_RATIO_K_PACK( 199u,  516u, 1966u),  /* idx=630, ratio=0.480140 */
    ALGO_RATIO_K_PACK( 194u,  522u, 1658u),  /* idx=631, ratio=0.404844 */
    ALGO_RATIO_K_PACK( 189u,  528u, 1371u),  /* idx=632, ratio=0.334832 */
    ALGO_RATIO_K_PACK( 184u,  533u, 1104u),  /* idx=633, ratio=0.269567 */
    ALGO_RATIO_K_PACK( 180u,  538u,  854u),  /* idx=634, ratio=0.208582 */
    ALGO_RATIO_K_PACK( 175u,  543u,  620u),  /* idx=635, ratio=0.151470 */
    ALGO_RATIO_K_PACK( 171u,  548u,  401u),  /* idx=636, ratio=0.097873 */
    ALGO_RATIO_K_PACK( 167u,  552u,  194u),  /* idx=637, ratio=0.047475 */
    ALGO_RATIO_K_PACK( 164u,  556u,    0u),  /* idx=638, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 390u,  322u,    0u),  /* idx=639, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 372u,  342u,  628u),  /* idx=640, ratio=0.153350 */
    ALGO_RATIO_K_PACK( 355u,  360u, 1141u),  /* idx=641, ratio=0.278541 */
    ALGO_RATIO_K_PACK( 340u,  376u, 1567u),  /* idx=642, ratio=0.382677 */
    ALGO_RATIO_K_PACK( 326u,  392u, 1927u),  /* idx=643, ratio=0.470657 */
    ALGO_RATIO_K_PACK( 313u,  405u, 2236u),  /* idx=644, ratio=0.545971 */
    ALGO_RATIO_K_PACK( 301u,  418u, 2503u),  /* idx=645, ratio=0.611170 */
    ALGO_RATIO_K_PACK( 290u,  430u, 2736u),  /* idx=646, ratio=0.668163 */
    ALGO_RATIO_K_PACK( 279u,  441u, 2942u),  /* idx=647, ratio=0.718408 */
    ALGO_RATIO_K_PACK( 269u,  452u, 3125u),  /* idx=648, ratio=0.763037 */
    ALGO_RATIO_K_PACK( 260u,  461u, 3288u),  /* idx=649, ratio=0.802940 */
    ALGO_RATIO_K_PACK( 252u,  470u, 3435u),  /* idx=650, ratio=0.838832 */
    ALGO_RATIO_K_PACK( 244u,  479u, 3568u),  /* idx=651, ratio=0.871288 */
    ALGO_RATIO_K_PACK( 236u,  487u, 3570u),  /* idx=652, ratio=0.871729 */
    ALGO_RATIO_K_PACK( 229u,  494u, 3129u),  /* idx=653, ratio=0.764177 */
    ALGO_RATIO_K_PACK( 223u,  501u, 2726u),  /* idx=654, ratio=0.665629 */
    ALGO_RATIO_K_PACK( 216u,  508u, 2355u),  /* idx=655, ratio=0.574999 */
    ALGO_RATIO_K_PACK( 211u,  514u, 2012u),  /* idx=656, ratio=0.491370 */
    ALGO_RATIO_K_PACK( 205u,  521u, 1695u),  /* idx=657, ratio=0.413961 */
    ALGO_RATIO_K_PACK( 200u,  526u, 1401u),  /* idx=658, ratio=0.342102 */
    ALGO_RATIO_K_PACK( 194u,  532u, 1127u),  /* idx=659, ratio=0.275218 */
    ALGO_RATIO_K_PACK( 190u,  537u,  871u),  /* idx=660, ratio=0.212808 */
    ALGO_RATIO_K_PACK( 185u,  542u,  632u),  /* idx=661, ratio=0.154440 */
    ALGO_RATIO_K_PACK( 181u,  547u,  408u),  /* idx=662, ratio=0.099731 */
    ALGO_RATIO_K_PACK( 176u,  551u,  198u),  /* idx=663, ratio=0.048350 */
    ALGO_RATIO_K_PACK( 172u,  555u,    0u),  /* idx=664, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 399u,  333u,    0u),  /* idx=665, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 380u,  352u,  604u),  /* idx=666, ratio=0.147470 */
    ALGO_RATIO_K_PACK( 363u,  369u, 1101u),  /* idx=667, ratio=0.268808 */
    ALGO_RATIO_K_PACK( 348u,  385u, 1517u),  /* idx=668, ratio=0.370393 */
    ALGO_RATIO_K_PACK( 333u,  399u, 1870u),  /* idx=669, ratio=0.456686 */
    ALGO_RATIO_K_PACK( 320u,  413u, 2174u),  /* idx=670, ratio=0.530898 */
    ALGO_RATIO_K_PACK( 308u,  425u, 2438u),  /* idx=671, ratio=0.595401 */
    ALGO_RATIO_K_PACK( 297u,  437u, 2670u),  /* idx=672, ratio=0.651982 */
    ALGO_RATIO_K_PACK( 286u,  448u, 2875u),  /* idx=673, ratio=0.702017 */
    ALGO_RATIO_K_PACK( 276u,  458u, 3057u),  /* idx=674, ratio=0.746579 */
    ALGO_RATIO_K_PACK( 267u,  467u, 3221u),  /* idx=675, ratio=0.786521 */
    ALGO_RATIO_K_PACK( 258u,  476u, 3368u),  /* idx=676, ratio=0.822524 */
    ALGO_RATIO_K_PACK( 250u,  484u, 3502u),  /* idx=677, ratio=0.855145 */
    ALGO_RATIO_K_PACK( 243u,  492u, 3214u),  /* idx=678, ratio=0.784943 */
    ALGO_RATIO_K_PACK( 235u,  499u, 2797u),  /* idx=679, ratio=0.682940 */
    ALGO_RATIO_K_PACK( 229u,  506u, 2413u),  /* idx=680, ratio=0.589338 */
    ALGO_RATIO_K_PACK( 222u,  513u, 2060u),  /* idx=681, ratio=0.503139 */
    ALGO_RATIO_K_PACK( 216u,  519u, 1734u),  /* idx=682, ratio=0.423498 */
    ALGO_RATIO_K_PACK( 210u,  525u, 1432u),  /* idx=683, ratio=0.349695 */
    ALGO_RATIO_K_PACK( 205u,  530u, 1151u),  /* idx=684, ratio=0.281110 */
    ALGO_RATIO_K_PACK( 200u,  536u,  889u),  /* idx=685, ratio=0.217209 */
    ALGO_RATIO_K_PACK( 195u,  541u,  645u),  /* idx=686, ratio=0.157528 */
    ALGO_RATIO_K_PACK( 190u,  545u,  416u),  /* idx=687, ratio=0.101662 */
    ALGO_RATIO_K_PACK( 186u,  550u,  202u),  /* idx=688, ratio=0.049257 */
    ALGO_RATIO_K_PACK( 181u,  554u,    0u),  /* idx=689, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 406u,  343u,    0u),  /* idx=690, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 388u,  361u,  582u),  /* idx=691, ratio=0.142025 */
    ALGO_RATIO_K_PACK( 371u,  378u, 1064u),  /* idx=692, ratio=0.259731 */
    ALGO_RATIO_K_PACK( 355u,  393u, 1470u),  /* idx=693, ratio=0.358873 */
    ALGO_RATIO_K_PACK( 341u,  407u, 1816u),  /* idx=694, ratio=0.443520 */
    ALGO_RATIO_K_PACK( 327u,  420u, 2116u),  /* idx=695, ratio=0.516635 */
    ALGO_RATIO_K_PACK( 315u,  432u, 2377u),  /* idx=696, ratio=0.580425 */
    ALGO_RATIO_K_PACK( 303u,  443u, 2607u),  /* idx=697, ratio=0.636567 */
    ALGO_RATIO_K_PACK( 293u,  454u, 2811u),  /* idx=698, ratio=0.686357 */
    ALGO_RATIO_K_PACK( 283u,  463u, 2993u),  /* idx=699, ratio=0.730817 */
    ALGO_RATIO_K_PACK( 273u,  473u, 3156u),  /* idx=700, ratio=0.770759 */
    ALGO_RATIO_K_PACK( 265u,  481u, 3304u),  /* idx=701, ratio=0.806838 */
    ALGO_RATIO_K_PACK( 256u,  489u, 3304u),  /* idx=702, ratio=0.806869 */
    ALGO_RATIO_K_PACK( 249u,  497u, 2871u),  /* idx=703, ratio=0.701176 */
    ALGO_RATIO_K_PACK( 241u,  504u, 2475u),  /* idx=704, ratio=0.604410 */
    ALGO_RATIO_K_PACK( 235u,  511u, 2111u),  /* idx=705, ratio=0.515485 */
    ALGO_RATIO_K_PACK( 228u,  517u, 1775u),  /* idx=706, ratio=0.433485 */
    ALGO_RATIO_K_PACK( 222u,  523u, 1465u),  /* idx=707, ratio=0.357632 */
    ALGO_RATIO_K_PACK( 216u,  529u, 1176u),  /* idx=708, ratio=0.287260 */
    ALGO_RATIO_K_PACK( 210u,  534u,  908u),  /* idx=709, ratio=0.221796 */
    ALGO_RATIO_K_PACK( 205u,  539u,  658u),  /* idx=710, ratio=0.160742 */
    ALGO_RATIO_K_PACK( 200u,  544u,  425u),  /* idx=711, ratio=0.103669 */
    ALGO_RATIO_K_PACK( 195u,  549u,  206u),  /* idx=712, ratio=0.050198 */
    ALGO_RATIO_K_PACK( 191u,  553u,    0u),  /* idx=713, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 414u,  352u,    0u),  /* idx=714, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 395u,  370u,  561u),  /* idx=715, ratio=0.136967 */
    ALGO_RATIO_K_PACK( 378u,  386u, 1029u),  /* idx=716, ratio=0.251248 */
    ALGO_RATIO_K_PACK( 362u,  401u, 1425u),  /* idx=717, ratio=0.348047 */
    ALGO_RATIO_K_PACK( 348u,  414u, 1765u),  /* idx=718, ratio=0.431092 */
    ALGO_RATIO_K_PACK( 334u,  427u, 2060u),  /* idx=719, ratio=0.503119 */
    ALGO_RATIO_K_PACK( 322u,  439u, 2319u),  /* idx=720, ratio=0.566185 */
    ALGO_RATIO_K_PACK( 310u,  450u, 2547u),  /* idx=721, ratio=0.621863 */
    ALGO_RATIO_K_PACK( 299u,  460u, 2749u),  /* idx=722, ratio=0.671381 */
    ALGO_RATIO_K_PACK( 289u,  469u, 2931u),  /* idx=723, ratio=0.715707 */
    ALGO_RATIO_K_PACK( 280u,  478u, 3094u),  /* idx=724, ratio=0.755616 */
    ALGO_RATIO_K_PACK( 271u,  486u, 3242u),  /* idx=725, ratio=0.791739 */
    ALGO_RATIO_K_PACK( 263u,  494u, 2950u),  /* idx=726, ratio=0.720412 */
    ALGO_RATIO_K_PACK( 255u,  502u, 2540u),  /* idx=727, ratio=0.620272 */
    ALGO_RATIO_K_PACK( 247u,  508u, 2164u),  /* idx=728, ratio=0.528452 */
    ALGO_RATIO_K_PACK( 240u,  515u, 1818u),  /* idx=729, ratio=0.443955 */
    ALGO_RATIO_K_PACK( 234u,  521u, 1499u),  /* idx=730, ratio=0.365939 */
    ALGO_RATIO_K_PACK( 227u,  527u, 1203u),  /* idx=731, ratio=0.293686 */
    ALGO_RATIO_K_PACK( 221u,  533u,  928u),  /* idx=732, ratio=0.226580 */
    ALGO_RATIO_K_PACK( 216u,  538u,  672u),  /* idx=733, ratio=0.164090 */
    ALGO_RATIO_K_PACK( 210u,  543u,  433u),  /* idx=734, ratio=0.105756 */
    ALGO_RATIO_K_PACK( 205u,  548u,  210u),  /* idx=735, ratio=0.051176 */
    ALGO_RATIO_K_PACK( 200u,  552u,    0u),  /* idx=736, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 422u,  362u,    0u),  /* idx=737, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 403u,  379u,  542u),  /* idx=738, ratio=0.132257 */
    ALGO_RATIO_K_PACK( 385u,  394u,  996u),  /* idx=739, ratio=0.243301 */
    ALGO_RATIO_K_PACK( 369u,  409u, 1384u),  /* idx=740, ratio=0.337856 */
    ALGO_RATIO_K_PACK( 355u,  422u, 1717u),  /* idx=741, ratio=0.419342 */
    ALGO_RATIO_K_PACK( 341u,  434u, 2008u),  /* idx=742, ratio=0.490292 */
    ALGO_RATIO_K_PACK( 328u,  445u, 2263u),  /* idx=743, ratio=0.552626 */
    ALGO_RATIO_K_PACK( 317u,  456u, 2489u),  /* idx=744, ratio=0.607824 */
    ALGO_RATIO_K_PACK( 306u,  466u, 2691u),  /* idx=745, ratio=0.657044 */
    ALGO_RATIO_K_PACK( 296u,  475u, 2871u),  /* idx=746, ratio=0.701208 */
    ALGO_RATIO_K_PACK( 286u,  483u, 3035u),  /* idx=747, ratio=0.741057 */
    ALGO_RATIO_K_PACK( 277u,  492u, 3033u),  /* idx=748, ratio=0.740734 */
    ALGO_RATIO_K_PACK( 269u,  499u, 2608u),  /* idx=749, ratio=0.636990 */
    ALGO_RATIO_K_PACK( 261u,  506u, 2220u),  /* idx=750, ratio=0.542088 */
    ALGO_RATIO_K_PACK( 253u,  513u, 1863u),  /* idx=751, ratio=0.454942 */
    ALGO_RATIO_K_PACK( 246u,  519u, 1534u),  /* idx=752, ratio=0.374640 */
    ALGO_RATIO_K_PACK( 239u,  526u, 1230u),  /* idx=753, ratio=0.300405 */
    ALGO_RATIO_K_PACK( 233u,  531u,  948u),  /* idx=754, ratio=0.231575 */
    ALGO_RATIO_K_PACK( 227u,  537u,  686u),  /* idx=755, ratio=0.167581 */
    ALGO_RATIO_K_PACK( 221u,  542u,  442u),  /* idx=756, ratio=0.107929 */
    ALGO_RATIO_K_PACK( 215u,  547u,  214u),  /* idx=757, ratio=0.052194 */
    ALGO_RATIO_K_PACK( 210u,  551u,    0u),  /* idx=758, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 429u,  371u,    0u),  /* idx=759, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 410u,  387u,  524u),  /* idx=760, ratio=0.127860 */
    ALGO_RATIO_K_PACK( 392u,  402u,  966u),  /* idx=761, ratio=0.235841 */
    ALGO_RATIO_K_PACK( 376u,  416u, 1344u),  /* idx=762, ratio=0.328245 */
    ALGO_RATIO_K_PACK( 362u,  429u, 1672u),  /* idx=763, ratio=0.408215 */
    ALGO_RATIO_K_PACK( 348u,  441u, 1958u),  /* idx=764, ratio=0.478102 */
    ALGO_RATIO_K_PACK( 335u,  452u, 2210u),  /* idx=765, ratio=0.539701 */
    ALGO_RATIO_K_PACK( 323u,  462u, 2434u),  /* idx=766, ratio=0.594404 */
    ALGO_RATIO_K_PACK( 312u,  471u, 2634u),  /* idx=767, ratio=0.643307 */
    ALGO_RATIO_K_PACK( 302u,  480u, 2814u),  /* idx=768, ratio=0.687286 */
    ALGO_RATIO_K_PACK( 292u,  489u, 2977u),  /* idx=769, ratio=0.727049 */
    ALGO_RATIO_K_PACK( 283u,  497u, 2681u),  /* idx=770, ratio=0.654635 */
    ALGO_RATIO_K_PACK( 274u,  504u, 2279u),  /* idx=771, ratio=0.556447 */
    ALGO_RATIO_K_PACK( 266u,  511u, 1910u),  /* idx=772, ratio=0.466488 */
    ALGO_RATIO_K_PACK( 259u,  518u, 1572u),  /* idx=773, ratio=0.383765 */
    ALGO_RATIO_K_PACK( 251u,  524u, 1259u),  /* idx=774, ratio=0.307439 */
    ALGO_RATIO_K_PACK( 245u,  530u,  970u),  /* idx=775, ratio=0.236796 */
    ALGO_RATIO_K_PACK( 238u,  535u,  701u),  /* idx=776, ratio=0.171223 */
    ALGO_RATIO_K_PACK( 232u,  541u,  451u),  /* idx=777, ratio=0.110194 */
    ALGO_RATIO_K_PACK( 226u,  546u,  218u),  /* idx=778, ratio=0.053252 */
    ALGO_RATIO_K_PACK( 221u,  550u,    0u),  /* idx=779, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 436u,  380u,    0u),  /* idx=780, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 417u,  396u,  507u),  /* idx=781, ratio=0.123747 */
    ALGO_RATIO_K_PACK( 399u,  410u,  937u),  /* idx=782, ratio=0.228826 */
    ALGO_RATIO_K_PACK( 383u,  424u, 1307u),  /* idx=783, ratio=0.319165 */
    ALGO_RATIO_K_PACK( 368u,  436u, 1628u),  /* idx=784, ratio=0.397663 */
    ALGO_RATIO_K_PACK( 354u,  447u, 1910u),  /* idx=785, ratio=0.466504 */
    ALGO_RATIO_K_PACK( 342u,  458u, 2160u),  /* idx=786, ratio=0.527368 */
    ALGO_RATIO_K_PACK( 330u,  468u, 2382u),  /* idx=787, ratio=0.581564 */
    ALGO_RATIO_K_PACK( 318u,  477u, 2580u),  /* idx=788, ratio=0.630132 */
    ALGO_RATIO_K_PACK( 308u,  486u, 2760u),  /* idx=789, ratio=0.673905 */
    ALGO_RATIO_K_PACK( 298u,  494u, 2757u),  /* idx=790, ratio=0.673284 */
    ALGO_RATIO_K_PACK( 289u,  502u, 2341u),  /* idx=791, ratio=0.571587 */
    ALGO_RATIO_K_PACK( 280u,  509u, 1960u),  /* idx=792, ratio=0.478634 */
    ALGO_RATIO_K_PACK( 272u,  516u, 1611u),  /* idx=793, ratio=0.393346 */
    ALGO_RATIO_K_PACK( 264u,  522u, 1289u),  /* idx=794, ratio=0.314811 */
    ALGO_RATIO_K_PACK( 257u,  528u,  992u),  /* idx=795, ratio=0.242257 */
    ALGO_RATIO_K_PACK( 250u,  534u,  717u),  /* idx=796, ratio=0.175027 */
    ALGO_RATIO_K_PACK( 243u,  539u,  461u),  /* idx=797, ratio=0.112556 */
    ALGO_RATIO_K_PACK( 237u,  544u,  223u),  /* idx=798, ratio=0.054354 */
    ALGO_RATIO_K_PACK( 231u,  549u,    0u),  /* idx=799, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 443u,  389u,    0u),  /* idx=800, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 424u,  404u,  491u),  /* idx=801, ratio=0.119889 */
    ALGO_RATIO_K_PACK( 406u,  418u,  910u),  /* idx=802, ratio=0.222215 */
    ALGO_RATIO_K_PACK( 390u,  431u, 1272u),  /* idx=803, ratio=0.310574 */
    ALGO_RATIO_K_PACK( 375u,  443u, 1587u),  /* idx=804, ratio=0.387643 */
    ALGO_RATIO_K_PACK( 361u,  454u, 1865u),  /* idx=805, ratio=0.455456 */
    ALGO_RATIO_K_PACK( 348u,  464u, 2111u),  /* idx=806, ratio=0.515585 */
    ALGO_RATIO_K_PACK( 336u,  474u, 2331u),  /* idx=807, ratio=0.569267 */
    ALGO_RATIO_K_PACK( 325u,  483u, 2529u),  /* idx=808, ratio=0.617486 */
    ALGO_RATIO_K_PACK( 314u,  491u, 2707u),  /* idx=809, ratio=0.661036 */
    ALGO_RATIO_K_PACK( 304u,  499u, 2406u),  /* idx=810, ratio=0.587574 */
    ALGO_RATIO_K_PACK( 295u,  507u, 2012u),  /* idx=811, ratio=0.491431 */
    ALGO_RATIO_K_PACK( 286u,  514u, 1652u),  /* idx=812, ratio=0.403417 */
    ALGO_RATIO_K_PACK( 278u,  520u, 1321u),  /* idx=813, ratio=0.322544 */
    ALGO_RATIO_K_PACK( 270u,  526u, 1015u),  /* idx=814, ratio=0.247976 */
    ALGO_RATIO_K_PACK( 262u,  532u,  733u),  /* idx=815, ratio=0.179004 */
    ALGO_RATIO_K_PACK( 255u,  538u,  471u),  /* idx=816, ratio=0.115021 */
    ALGO_RATIO_K_PACK( 249u,  543u,  227u),  /* idx=817, ratio=0.055503 */
    ALGO_RATIO_K_PACK( 242u,  548u,    0u),  /* idx=818, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 450u,  397u,    0u),  /* idx=819, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 431u,  412u,  476u),  /* idx=820, ratio=0.116265 */
    ALGO_RATIO_K_PACK( 413u,  425u,  884u),  /* idx=821, ratio=0.215976 */
    ALGO_RATIO_K_PACK( 396u,  438u, 1238u),  /* idx=822, ratio=0.302434 */
    ALGO_RATIO_K_PACK( 381u,  450u, 1548u),  /* idx=823, ratio=0.378116 */
    ALGO_RATIO_K_PACK( 367u,  460u, 1822u),  /* idx=824, ratio=0.444918 */
    ALGO_RATIO_K_PACK( 354u,  470u, 2065u),  /* idx=825, ratio=0.504318 */
    ALGO_RATIO_K_PACK( 342u,  480u, 2283u),  /* idx=826, ratio=0.557480 */
    ALGO_RATIO_K_PACK( 331u,  488u, 2479u),  /* idx=827, ratio=0.605338 */
    ALGO_RATIO_K_PACK( 320u,  497u, 2475u),  /* idx=828, ratio=0.604481 */
    ALGO_RATIO_K_PACK( 310u,  504u, 2068u),  /* idx=829, ratio=0.504930 */
    ALGO_RATIO_K_PACK( 300u,  511u, 1695u),  /* idx=830, ratio=0.414018 */
    ALGO_RATIO_K_PACK( 292u,  518u, 1354u),  /* idx=831, ratio=0.330667 */
    ALGO_RATIO_K_PACK( 283u,  525u, 1040u),  /* idx=832, ratio=0.253972 */
    ALGO_RATIO_K_PACK( 275u,  531u,  750u),  /* idx=833, ratio=0.183166 */
    ALGO_RATIO_K_PACK( 268u,  536u,  482u),  /* idx=834, ratio=0.117596 */
    ALGO_RATIO_K_PACK( 261u,  542u,  232u),  /* idx=835, ratio=0.056702 */
    ALGO_RATIO_K_PACK( 254u,  547u,    0u),  /* idx=836, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 457u,  405u,    0u),  /* idx=837, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 437u,  420u,  462u),  /* idx=838, ratio=0.112854 */
    ALGO_RATIO_K_PACK( 419u,  433u,  860u),  /* idx=839, ratio=0.210078 */
    ALGO_RATIO_K_PACK( 403u,  445u, 1207u),  /* idx=840, ratio=0.294709 */
    ALGO_RATIO_K_PACK( 388u,  456u, 1511u),  /* idx=841, ratio=0.369045 */
    ALGO_RATIO_K_PACK( 374u,  467u, 1781u),  /* idx=842, ratio=0.434857 */
    ALGO_RATIO_K_PACK( 360u,  476u, 2021u),  /* idx=843, ratio=0.493532 */
    ALGO_RATIO_K_PACK( 348u,  485u, 2237u),  /* idx=844, ratio=0.546170 */
    ALGO_RATIO_K_PACK( 337u,  494u, 2431u),  /* idx=845, ratio=0.593659 */
    ALGO_RATIO_K_PACK( 326u,  502u, 2126u),  /* idx=846, ratio=0.519191 */
    ALGO_RATIO_K_PACK( 316u,  509u, 1741u),  /* idx=847, ratio=0.425191 */
    ALGO_RATIO_K_PACK( 306u,  516u, 1389u),  /* idx=848, ratio=0.339210 */
    ALGO_RATIO_K_PACK( 297u,  523u, 1066u),  /* idx=849, ratio=0.260265 */
    ALGO_RATIO_K_PACK( 289u,  529u,  768u),  /* idx=850, ratio=0.187526 */
    ALGO_RATIO_K_PACK( 281u,  535u,  493u),  /* idx=851, ratio=0.120289 */
    ALGO_RATIO_K_PACK( 273u,  541u,  237u),  /* idx=852, ratio=0.057953 */
    ALGO_RATIO_K_PACK( 266u,  546u,    0u),  /* idx=853, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 463u,  414u,    0u),  /* idx=854, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 444u,  427u,  449u),  /* idx=855, ratio=0.109637 */
    ALGO_RATIO_K_PACK( 426u,  440u,  837u),  /* idx=856, ratio=0.204493 */
    ALGO_RATIO_K_PACK( 409u,  452u, 1177u),  /* idx=857, ratio=0.287369 */
    ALGO_RATIO_K_PACK( 394u,  463u, 1476u),  /* idx=858, ratio=0.360400 */
    ALGO_RATIO_K_PACK( 380u,  473u, 1741u),  /* idx=859, ratio=0.425241 */
    ALGO_RATIO_K_PACK( 366u,  482u, 1979u),  /* idx=860, ratio=0.483198 */
    ALGO_RATIO_K_PACK( 354u,  491u, 2192u),  /* idx=861, ratio=0.535311 */
    ALGO_RATIO_K_PACK( 342u,  499u, 2188u),  /* idx=862, ratio=0.534282 */
    ALGO_RATIO_K_PACK( 332u,  507u, 1789u),  /* idx=863, ratio=0.436983 */
    ALGO_RATIO_K_PACK( 321u,  514u, 1426u),  /* idx=864, ratio=0.348206 */
    ALGO_RATIO_K_PACK( 312u,  521u, 1093u),  /* idx=865, ratio=0.266878 */
    ALGO_RATIO_K_PACK( 303u,  527u,  787u),  /* idx=866, ratio=0.192099 */
    ALGO_RATIO_K_PACK( 294u,  533u,  504u),  /* idx=867, ratio=0.123109 */
    ALGO_RATIO_K_PACK( 286u,  539u,  243u),  /* idx=868, ratio=0.059261 */
    ALGO_RATIO_K_PACK( 278u,  545u,    0u),  /* idx=869, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 470u,  422u,    0u),  /* idx=870, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 450u,  435u,  437u),  /* idx=871, ratio=0.106598 */
    ALGO_RATIO_K_PACK( 432u,  447u,  816u),  /* idx=872, ratio=0.199197 */
    ALGO_RATIO_K_PACK( 415u,  459u, 1148u),  /* idx=873, ratio=0.280386 */
    ALGO_RATIO_K_PACK( 400u,  469u, 1442u),  /* idx=874, ratio=0.352150 */
    ALGO_RATIO_K_PACK( 386u,  479u, 1704u),  /* idx=875, ratio=0.416042 */
    ALGO_RATIO_K_PACK( 372u,  488u, 1938u),  /* idx=876, ratio=0.473288 */
    ALGO_RATIO_K_PACK( 360u,  497u, 2149u),  /* idx=877, ratio=0.524875 */
    ALGO_RATIO_K_PACK( 348u,  504u, 1840u),  /* idx=878, ratio=0.449449 */
    ALGO_RATIO_K_PACK( 337u,  512u, 1465u),  /* idx=879, ratio=0.357692 */
    ALGO_RATIO_K_PACK( 327u,  519u, 1121u),  /* idx=880, ratio=0.273835 */
    ALGO_RATIO_K_PACK( 317u,  526u,  806u),  /* idx=881, ratio=0.196900 */
    ALGO_RATIO_K_PACK( 308u,  532u,  516u),  /* idx=882, ratio=0.126064 */
    ALGO_RATIO_K_PACK( 299u,  538u,  248u),  /* idx=883, ratio=0.060629 */
    ALGO_RATIO_K_PACK( 291u,  543u,    0u),  /* idx=884, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 476u,  430u,    0u),  /* idx=885, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 456u,  442u,  425u),  /* idx=886, ratio=0.103723 */
    ALGO_RATIO_K_PACK( 438u,  454u,  795u),  /* idx=887, ratio=0.194169 */
    ALGO_RATIO_K_PACK( 421u,  465u, 1121u),  /* idx=888, ratio=0.273734 */
    ALGO_RATIO_K_PACK( 406u,  475u, 1410u),  /* idx=889, ratio=0.344270 */
    ALGO_RATIO_K_PACK( 392u,  485u, 1668u),  /* idx=890, ratio=0.407231 */
    ALGO_RATIO_K_PACK( 378u,  494u, 1899u),  /* idx=891, ratio=0.463776 */
    ALGO_RATIO_K_PACK( 366u,  502u, 1895u),  /* idx=892, ratio=0.462646 */
    ALGO_RATIO_K_PACK( 354u,  510u, 1506u),  /* idx=893, ratio=0.367709 */
    ALGO_RATIO_K_PACK( 343u,  517u, 1151u),  /* idx=894, ratio=0.281165 */
    ALGO_RATIO_K_PACK( 332u,  524u,  827u),  /* idx=895, ratio=0.201948 */
    ALGO_RATIO_K_PACK( 323u,  530u,  529u),  /* idx=896, ratio=0.129165 */
    ALGO_RATIO_K_PACK( 313u,  536u,  254u),  /* idx=897, ratio=0.062062 */
    ALGO_RATIO_K_PACK( 305u,  542u,    0u),  /* idx=898, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 482u,  437u,    0u),  /* idx=899, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 462u,  450u,  414u),  /* idx=900, ratio=0.100999 */
    ALGO_RATIO_K_PACK( 444u,  461u,  776u),  /* idx=901, ratio=0.189389 */
    ALGO_RATIO_K_PACK( 427u,  472u, 1095u),  /* idx=902, ratio=0.267391 */
    ALGO_RATIO_K_PACK( 412u,  482u, 1379u),  /* idx=903, ratio=0.336735 */
    ALGO_RATIO_K_PACK( 397u,  491u, 1633u),  /* idx=904, ratio=0.398787 */
    ALGO_RATIO_K_PACK( 384u,  499u, 1862u),  /* idx=905, ratio=0.454639 */
    ALGO_RATIO_K_PACK( 371u,  507u, 1549u),  /* idx=906, ratio=0.378304 */
    ALGO_RATIO_K_PACK( 359u,  515u, 1183u),  /* idx=907, ratio=0.288899 */
    ALGO_RATIO_K_PACK( 348u,  522u,  849u),  /* idx=908, ratio=0.207261 */
    ALGO_RATIO_K_PACK( 338u,  528u,  542u),  /* idx=909, ratio=0.132421 */
    ALGO_RATIO_K_PACK( 328u,  535u,  260u),  /* idx=910, ratio=0.063564 */
    ALGO_RATIO_K_PACK( 319u,  541u,    0u),  /* idx=911, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 488u,  445u,    0u),  /* idx=912, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 468u,  457u,  403u),  /* idx=913, ratio=0.098415 */
    ALGO_RATIO_K_PACK( 450u,  468u,  757u),  /* idx=914, ratio=0.184838 */
    ALGO_RATIO_K_PACK( 433u,  478u, 1070u),  /* idx=915, ratio=0.261334 */
    ALGO_RATIO_K_PACK( 418u,  488u, 1349u),  /* idx=916, ratio=0.329522 */
    ALGO_RATIO_K_PACK( 403u,  496u, 1600u),  /* idx=917, ratio=0.390685 */
    ALGO_RATIO_K_PACK( 389u,  505u, 1595u),  /* idx=918, ratio=0.389527 */
    ALGO_RATIO_K_PACK( 377u,  513u, 1216u),  /* idx=919, ratio=0.297069 */
    ALGO_RATIO_K_PACK( 365u,  520u,  872u),  /* idx=920, ratio=0.212861 */
    ALGO_RATIO_K_PACK( 354u,  527u,  556u),  /* idx=921, ratio=0.135846 */
    ALGO_RATIO_K_PACK( 343u,  533u,  267u),  /* idx=922, ratio=0.065141 */
    ALGO_RATIO_K_PACK( 333u,  539u,    0u),  /* idx=923, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 494u,  452u,    0u),  /* idx=924, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 474u,  464u,  393u),  /* idx=925, ratio=0.095960 */
    ALGO_RATIO_K_PACK( 456u,  474u,  739u),  /* idx=926, ratio=0.180500 */
    ALGO_RATIO_K_PACK( 439u,  484u, 1046u),  /* idx=927, ratio=0.255546 */
    ALGO_RATIO_K_PACK( 423u,  493u, 1321u),  /* idx=928, ratio=0.322612 */
    ALGO_RATIO_K_PACK( 409u,  502u, 1568u),  /* idx=929, ratio=0.382906 */
    ALGO_RATIO_K_PACK( 395u,  510u, 1252u),  /* idx=930, ratio=0.305715 */
    ALGO_RATIO_K_PACK( 382u,  518u,  896u),  /* idx=931, ratio=0.218772 */
    ALGO_RATIO_K_PACK( 370u,  525u,  571u),  /* idx=932, ratio=0.139454 */
    ALGO_RATIO_K_PACK( 359u,  531u,  274u),  /* idx=933, ratio=0.066798 */
    ALGO_RATIO_K_PACK( 348u,  538u,    0u),  /* idx=934, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 499u,  459u,    0u),  /* idx=935, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 480u,  470u,  383u),  /* idx=936, ratio=0.093624 */
    ALGO_RATIO_K_PACK( 462u,  481u,  722u),  /* idx=937, ratio=0.176362 */
    ALGO_RATIO_K_PACK( 445u,  490u, 1024u),  /* idx=938, ratio=0.250009 */
    ALGO_RATIO_K_PACK( 429u,  499u, 1294u),  /* idx=939, ratio=0.315986 */
    ALGO_RATIO_K_PACK( 414u,  508u, 1289u),  /* idx=940, ratio=0.314880 */
    ALGO_RATIO_K_PACK( 400u,  515u,  921u),  /* idx=941, ratio=0.225021 */
    ALGO_RATIO_K_PACK( 388u,  523u,  587u),  /* idx=942, ratio=0.143257 */
    ALGO_RATIO_K_PACK( 376u,  530u,  281u),  /* idx=943, ratio=0.068542 */
    ALGO_RATIO_K_PACK( 364u,  536u,    0u),  /* idx=944, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 505u,  466u,    0u),  /* idx=945, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 485u,  477u,  374u),  /* idx=946, ratio=0.091399 */
    ALGO_RATIO_K_PACK( 467u,  487u,  706u),  /* idx=947, ratio=0.172409 */
    ALGO_RATIO_K_PACK( 450u,  496u, 1002u),  /* idx=948, ratio=0.244707 */
    ALGO_RATIO_K_PACK( 434u,  505u, 1268u),  /* idx=949, ratio=0.309626 */
    ALGO_RATIO_K_PACK( 420u,  513u,  949u),  /* idx=950, ratio=0.231638 */
    ALGO_RATIO_K_PACK( 406u,  521u,  603u),  /* idx=951, ratio=0.147275 */
    ALGO_RATIO_K_PACK( 393u,  528u,  288u),  /* idx=952, ratio=0.070379 */
    ALGO_RATIO_K_PACK( 381u,  534u,    0u),  /* idx=953, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 511u,  473u,    0u),  /* idx=954, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 491u,  484u,  366u),  /* idx=955, ratio=0.089277 */
    ALGO_RATIO_K_PACK( 473u,  493u,  691u),  /* idx=956, ratio=0.168630 */
    ALGO_RATIO_K_PACK( 456u,  502u,  981u),  /* idx=957, ratio=0.239625 */
    ALGO_RATIO_K_PACK( 440u,  511u,  977u),  /* idx=958, ratio=0.238655 */
    ALGO_RATIO_K_PACK( 425u,  518u,  620u),  /* idx=959, ratio=0.151524 */
    ALGO_RATIO_K_PACK( 411u,  526u,  296u),  /* idx=960, ratio=0.072317 */
    ALGO_RATIO_K_PACK( 398u,  533u,    0u),  /* idx=961, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 516u,  480u,    0u),  /* idx=962, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 496u,  490u,  357u),  /* idx=963, ratio=0.087252 */
    ALGO_RATIO_K_PACK( 478u,  499u,  676u),  /* idx=964, ratio=0.165012 */
    ALGO_RATIO_K_PACK( 461u,  508u,  961u),  /* idx=965, ratio=0.234750 */
    ALGO_RATIO_K_PACK( 445u,  516u,  639u),  /* idx=966, ratio=0.156025 */
    ALGO_RATIO_K_PACK( 430u,  524u,  305u),  /* idx=967, ratio=0.074365 */
    ALGO_RATIO_K_PACK( 416u,  531u,    0u),  /* idx=968, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 521u,  487u,    0u),  /* idx=969, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 501u,  496u,  349u),  /* idx=970, ratio=0.085316 */
    ALGO_RATIO_K_PACK( 483u,  505u,  662u),  /* idx=971, ratio=0.161547 */
    ALGO_RATIO_K_PACK( 466u,  514u,  658u),  /* idx=972, ratio=0.160802 */
    ALGO_RATIO_K_PACK( 450u,  522u,  313u),  /* idx=973, ratio=0.076532 */
    ALGO_RATIO_K_PACK( 435u,  529u,    0u),  /* idx=974, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 526u,  493u,    0u),  /* idx=975, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 507u,  502u,  342u),  /* idx=976, ratio=0.083465 */
    ALGO_RATIO_K_PACK( 488u,  511u,  648u),  /* idx=977, ratio=0.158224 */
    ALGO_RATIO_K_PACK( 471u,  519u,  323u),  /* idx=978, ratio=0.078829 */
    ALGO_RATIO_K_PACK( 455u,  527u,    0u),  /* idx=979, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 531u,  499u,    0u),  /* idx=980, ratio=-0.000000 */
    ALGO_RATIO_K_PACK( 512u,  509u,  331u),  /* idx=981, ratio=0.080931 */
    ALGO_RATIO_K_PACK( 493u,  517u,  333u),  /* idx=982, ratio=0.081269 */
    ALGO_RATIO_K_PACK( 476u,  525u,    0u),  /* idx=983, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 536u,  506u,    0u),  /* idx=984, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 517u,  514u,  253u),  /* idx=985, ratio=0.061890 */
    ALGO_RATIO_K_PACK( 498u,  523u,    0u),  /* idx=986, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 541u,  512u,    0u),  /* idx=987, ratio=0.000000 */
    ALGO_RATIO_K_PACK( 522u,  520u,    0u),  /* idx=988, ratio=0.000000 */
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