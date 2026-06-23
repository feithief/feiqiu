/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2018, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
#ifndef LDRV_DEFINES_H
#define LDRV_DEFINES_H

/* XML Version 1.0.4 */
#if 1
/******************PWM 32000   35mA   fast********************/
#define LDRV_XML_VERSION (10004)

#define LDRV_DIAG_CFG (0x8) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG0 (0x0) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG1 (0x0) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG2 (0x0) /*decimal 0*/

#define LDRV_IEN0 (0x70001) /*decimal 0*/

#define LDRV_IEN1 (0x0) /*decimal 0*/

#define LDRV_MOD_CFG (0x1) /*decimal 1*/

#define LDRV_OSP_CFG (0x10) /*decimal 16*/

#define LDRV_PWM_DC_CFG0 (0x0) /*decimal 0*/

#define LDRV_PWM_DC_CFG1 (0x0) /*decimal 0*/

#define LDRV_PWM_DC_CFG2 (0x0) /*decimal 0*/

#define LDRV_PWM_FREQ (0x27D00) /*decimal 163072*/

#define LDRV_PWM_ISET_CFG0 (0x28) /*decimal 40*/

#define LDRV_PWM_ISET_CFG1 (0x28) /*decimal 40*/

#define LDRV_PWM_ISET_CFG2 (0x28) /*decimal 40*/

#define LDRV_PWM_PHASE_CFG0 (0x0) /*decimal 0*/

#define LDRV_PWM_PHASE_CFG1 (0x0) /*decimal 0*/

#define LDRV_PWM_PHASE_CFG2 (0x0) /*decimal 0*/

#define LDRV_PWM_SYNCH (0x0) /*decimal 0*/
/******************PWM 32000   35mA   fast********************/
#endif

#if 0
/******************PWM 65535   35mA   fast********************/
#define LDRV_XML_VERSION (10004)

#define LDRV_DIAG_CFG (0x0) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG0 (0x0) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG1 (0x0) /*decimal 0*/

#define LDRV_DIAG_PHASE_CFG2 (0x0) /*decimal 0*/

#define LDRV_IEN0 (0x0) /*decimal 0*/

#define LDRV_IEN1 (0x0) /*decimal 0*/

#define LDRV_MOD_CFG (0x1) /*decimal 1*/

#define LDRV_OSP_CFG (0x10) /*decimal 16*/

#define LDRV_PWM_DC_CFG0 (0x0) /*decimal 0*/

#define LDRV_PWM_DC_CFG1 (0x0) /*decimal 0*/

#define LDRV_PWM_DC_CFG2 (0x0) /*decimal 0*/

#define LDRV_PWM_FREQ (0xFFFF) /*decimal 65535*/

#define LDRV_PWM_ISET_CFG0 (0x28) /*decimal 40*/

#define LDRV_PWM_ISET_CFG1 (0x28) /*decimal 40*/

#define LDRV_PWM_ISET_CFG2 (0x28) /*decimal 40*/

#define LDRV_PWM_PHASE_CFG0 (0x0) /*decimal 0*/

#define LDRV_PWM_PHASE_CFG1 (0x0) /*decimal 0*/

#define LDRV_PWM_PHASE_CFG2 (0x0) /*decimal 0*/

#define LDRV_PWM_SYNCH (0x0) /*decimal 0*/
/******************PWM 65535   35mA   fast********************/
#endif
#endif /* LDRV_DEFINES_H */
