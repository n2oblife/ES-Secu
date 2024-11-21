#include <xmc_common.h>
// #include "../../opt/XMClib/XMC_Peripheral_Library_v2.1.16/XMCLib/inc/xmc_common.h"
// #include <sstdint.h>

void initCCU4(void);
void connectLED(void);
void LED_On(void);
void LED_Off(void);

//*****************************************************************************
// DEFINING CONSTANTS AND ADRESS
//*****************************************************************************

const float pwm_rate = 0.1;

// // Initialize the RCU control structure pointer to the SCU base address
// #define my_RCU ((volatile rcu_ctrl_t *)SCU_BASE_ADDRESS+RCU_OFFSET)

int main(void) {
  
  initCCU4();
  // connectLED();
  LED_On();

  while(1);
  return 0;
}

void initCCU4() {
  // release the reset of the CCU40
  // my_RCU->CCU40RS2 = 0b1;
  SCU_RESET->PRCLR0 = SCU_RESET_PRCLR0_CCU40RS_Msk;
  // enable the clock for the CCU40 (step 3)
  SCU_CLK->CLKSET = SCU_CLK_CLKSET_CCUCEN_Msk;
  // (step 4)
  CCU40->GIDLC = CCU4_GIDLC_SPRB_Msk;
  // (step 6)
  CCU40_CC42->PRS = 0xffff; // Set period for dimming
  CCU40_CC42->PSC = 0b1010; // Set prescaler for dimming
  // CCU40_CC42->CRS = 0xffff * (1-0.5); // Set duty cycle for dimming (100%)
  CCU40_CC42->CRS = 0x0000; // Set duty cycle for dimming (0%)
  // connectLED();
  // (step 7)
  CCU40->GCSS = CCU4_GCSS_S2SE_Msk;
  // (step 8)
  CCU40->GIDLC = CCU4_GIDLC_CS2I_Msk;
  // (step 9)
  CCU40_CC42->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
}

void connectLED() {
  const static uint8_t ALT3 = 0b10011;
  PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk) | (ALT3 << PORT1_IOCR0_PC1_Pos);
  // PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk);
}

// Turn LED On by setting the output high on PORT1.1
void LED_On() {
  // CCU40_CC42->CRS = 0x0000; // Set duty cycle for dimming (100%)
  const static uint8_t ALT3 = 0b10011;
  PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk)| (ALT3 << PORT1_IOCR0_PC1_Pos);
  // CCU40_CC42->CRS = 0xffff; // Set duty cycle for dimming (100%)
}

// Turn LED Off by clearing the output on PORT1.1
void LED_Off() {
  // CCU40_CC42->CRS = 0x0000; // Set duty cycle for dimming (100%)
  const static uint8_t ALT3 = 0b10011;
  PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk);
  // CCU40_CC42->CRS = 0x0000; // Set duty cycle for dimming (0%)
}