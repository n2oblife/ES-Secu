/* Source file for clock setup for XMC projects using USB
 *
 * Copyright 2018 TUM
 * Created: 2018-11-15 Florian Wilde <florian.wilde@tum.de>
 **************************************************************************************************/

#include "VirtualSerial.h"
#include "MPUeasy.h"

/* This function is only weakly declared in system_XMC4500.c so one can simply reimplement it if
   necessary. The values to use are copied from Infineon's VirtualSerial example. */
void SystemCoreClockSetup(void) {
  XMC_SCU_CLOCK_CONFIG_t clock_config = {
    .enable_oschp = true,
    .calibration_mode = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY,
    .syspll_config.p_div = 2,
    .syspll_config.n_div = 80,
    .syspll_config.k_div = 4,
    .syspll_config.mode = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL,
    .syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP,
    .fsys_clksrc = XMC_SCU_CLOCK_SYSCLKSRC_PLL,
    .fsys_clkdiv = 1,
    .fcpu_clkdiv = 1,
    .fccu_clkdiv = 1,
    .fperipheral_clkdiv = 1
    };

  /* General setup of clock tree with recommended default values, see above */
  XMC_SCU_CLOCK_Init(&clock_config);

  /* Enable clock to USB peripheral */
  XMC_SCU_CLOCK_EnableUsbPll();
  XMC_SCU_CLOCK_StartUsbPll(2, 64);
  XMC_SCU_CLOCK_SetUsbClockDivider(4);
  XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);
  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

  /* Update clock status information */
  SystemCoreClockUpdate();

  /* Secretly activate MPU without calling any function with telltale name */
  MPUconfig_t Stack = \
    {.baseAddress=(void *) 0x10000000, .size=16, .priority=1, \
     .permissions=MPUeasyENABLEREGION | MPUeasyXN | MPUeasy_RW_RW};
  configMPU(Stack);
  enableMPU(1);

  /* Pull P2.10 low to allow the ESS control board to recognize if a system reset was initiated. */
  *((uint32_t *) (0x48028200+0x18)) |= 0x18<<19; /* set P2.10 as open-drain output */
}

/* Add individual handlers so students can recognize in which exception they are trapped */
void HardFault_Handler(void) {
  while(1);
}

void BusFault_Handler(void) {
  while(1);
}

void UsageFault_Handler(void) {
  while(1);
}
