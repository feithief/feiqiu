/**
 * @file
 * @brief Amalthea Software Platform
 * @internal
 *
 * @copyright (C) 2015-2018 Melexis N.V.
 * git flash 137e5489
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup amalthea_platform
 *
 * @details
 */

#ifndef PLTF_VERSION_H_
#define PLTF_VERSION_H_

/* SW platform version */
#define MLX_PLTF_VERSION_MAJOR              1
#define MLX_PLTF_VERSION_MINOR              1
#define MLX_PLTF_VERSION_REVISION           12
#define MLX_PLTF_VERSION_CUSTOMER_BUILD     0

#define MLX_PLTF_VERSION_STRING             "1.1.12.0"

/* Validate version range */
#if    (MLX_PLTF_VERSION_MAJOR > 255)    \
    || (MLX_PLTF_VERSION_MINOR > 255)    \
    || (MLX_PLTF_VERSION_REVISION       > 255) \
    || (MLX_PLTF_VERSION_CUSTOMER_BUILD > 255)
#error "Maximum version number is 255"
#endif


#endif /* PLTF_VERSION_H_ */
