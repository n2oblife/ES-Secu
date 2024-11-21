#include <xmc_common.h>

void initCCU4(void);
void connectLED(void);

//*****************************************************************************
// DEFINING CONSTANTS AND ADRESS
//*****************************************************************************

const float pwm_rate = 0.1;

// // Initialize the RCU control structure pointer to the SCU base address
// #define my_RCU ((volatile rcu_ctrl_t *)SCU_BASE_ADDRESS+RCU_OFFSET)

int main(void) {
  
  initCCU4();

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
  CCU40_CC42->PRS = 0xffff;
  CCU40_CC42->PSC = 0b1010;
  CCU40_CC42->CRS = 0xffff*(1-0.5);
  connectLED();
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
}
