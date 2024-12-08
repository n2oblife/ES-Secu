/*
 * MPUeasy.h
 *
 *  Created on: 25.11.2015
 *      Author: Florian Wilde
 */

#ifndef MPUEASY_H_
#define MPUEASY_H_

typedef struct {
  int R0;
  int R1;
  int R2;
  int R3;
  int R12;
  int LR;
  int PC;
  int xPSR;
} EXC_FRAME_t;

enum MPUeasyPermissions {MPUeasy_None_None = 0, MPUeasy_RW_None = 1, \
                         MPUeasy_RW_R = 2, MPUeasy_RW_RW = 3, \
                         MPUeasy_R_None = 5, MPUeasy_R_R = 6};
#define MPUeasyENABLEREGION (0x1 << 7)
#define MPUeasyXN (0x1 << 4)

typedef struct {
  void *baseAddress; 	/* will be aligned according to size */
  int permissions; 	/* bit 7 enables region,
                         other options auto set according to manual
                         bit 4 disables execution
                         bits 2-0 according to table 2-18:
                         value	privileged	unprivileged
                         0		none		none
                         1		rw			none
                         2		rw			r
                         3		rw			rw
                         5		r			none
                         6		r			r             */
  uint8_t size;		/* as power of 2, so e.g. 10 = 1 KiB, 20 = 1 MiB,
                   * NOTE: Differs from SIZE field in RASR by 1 */
  uint8_t priority;	/* exclusive, i.e. only one region per priority */
} MPUconfig_t;

void enableMPU(int enableBackgroundRegion);
void disableMPU(void);
void configMPU(MPUconfig_t config);

#endif /* MPUEASY_H_ */
