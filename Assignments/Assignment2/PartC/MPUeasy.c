/*
 * MPUeasy.c
 *
 *  Created on: 25.11.2015
 *      Author: Florian Wilde
 */

#include <xmc_common.h>
#include "MPUeasy.h"

void enableMPU(int enableBackgroundRegion) {
	__DSB();
	__ISB();
	PPB->MPU_CTRL |= (enableBackgroundRegion ? 0x4 : 0x0) | 0x1;
	__DSB();
	__ISB();
}

void disableMPU(void) {
	__DSB();
	__ISB();
	PPB->MPU_CTRL = 0;
	__DSB();
	__ISB();
}

void configMPU(MPUconfig_t config) {
	/* to align baseAddress, we shift right and then left again,
	 * with at least the position of the ADDR bitfield
	 * so other bitfields don't get changed */
	uint8_t addrShift = config.size > PPB_MPU_RBAR_ADDR_Pos ? \
	                    config.size : PPB_MPU_RBAR_ADDR_Pos;
	/* TEX, S, C, and B field will be set according to recommendation
	 * in table 2-17 of manual */
	uint8_t autoSet = config.baseAddress < (void *)0x10000000 ? 0x2 : \
	                  config.baseAddress < (void *)0x40000000 ? 0x6 : \
	                  config.baseAddress < (void *)0x60000000 ? 0x5 : \
	                                                            0x7;
	/* switch to correct priority slot */
	PPB->MPU_RNR = config.priority & 0x7;
	/* disable region before changing parameters to avoid glitches */
	PPB->MPU_RASR &= ~PPB_MPU_RASR_ENABLE_Msk;

	PPB->MPU_RBAR = ((uint32_t) config.baseAddress >> addrShift) \
	                                               << addrShift;
	PPB->MPU_RASR = (config.permissions << PPB_MPU_RASR_AP_Pos \
                & (PPB_MPU_RASR_XN_Msk | PPB_MPU_RASR_AP_Msk))  | \
	                (autoSet            << PPB_MPU_RASR_B_Pos  )  | \
            ((config.size > 0 ? config.size - 1 : config.size)\
	                                    << PPB_MPU_RASR_SIZE_Pos\
	                                     & PPB_MPU_RASR_SIZE_Msk) | \
	                (config.permissions >> 7 \
	                                     & PPB_MPU_RASR_ENABLE_Msk);
}

void MemManage_Handler(void) {
  uint8_t MMFSR = PPB->CFSR;
  void *MMFAR = (MMFSR & PPB_CFSR_MMARVALID_Msk) ? (void *) PPB->MMFAR : NULL;
  /* use EXC_FRAME_t with appropriate base address to inspect stacked CPU status before exception */
  EXC_FRAME_t *status = NULL; /* set to correct position using debugger */
  while(1);
}
