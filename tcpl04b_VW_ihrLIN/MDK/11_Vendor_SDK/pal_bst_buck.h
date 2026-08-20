/**
 *****************************************************************************
 * @brief   pal bst buck header file.
 *
 * @file    pal_bst_buck.h
 * @author  AE/FAE team
 * @date    2026.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2026 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */
#ifndef __PAL_BST_BUCK_H__
#define __PAL_BST_BUCK_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

void pal_bst_buck_init(void);
void pal_bst_buck_lvd_protect(void);
#ifdef __cplusplus
}
#endif
#endif /*__PAL_BST_BUCK_H__*/

