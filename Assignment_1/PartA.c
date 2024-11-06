#include <xmc_common.h>
// #include "../../opt/XMClib/XMC_Peripheral_Library_v2.1.16/XMCLib/inc/xmc_common.h"
// #include <sstdint.h>

void initCCU4(void);
void connectLED(void);

//*****************************************************************************
// DEFINING CONSTANTS AND ADRESS
//*****************************************************************************

const float pwm_rate = 0.1;

// /// Base address for System Control Unit (SCU)
// const uint32_t SCU_BASE_ADDRESS = 0x50004000;
// /// Offset for RCU Control Registers
// const uint32_t RCU_OFFSET = 0x414;

// /// RCU Control Registers structure definition
// typedef struct {
//     union {
//         uint32_t reset_control; // Full 32-bit reset control register
//         struct {
//             uint32_t VADCRS0    : 1;  // VADC reset clear bit (0: no effect, 1: de-assert reset)
//             uint32_t DSDRS1     : 1;  // DSD reset clear bit
//             uint32_t CCU40RS2   : 1;  // CCU40 reset clear bit
//             uint32_t CCU41RS3   : 1;  // CCU41 reset clear bit
//             uint32_t CCU42RS4   : 1;  // CCU42 reset clear bit
//             uint32_t reserved0  : 1;  // Reserved bit [5]
//             uint32_t CCU80RS7   : 1;  // CCU80 reset clear bit
//             uint32_t CCU81RS8   : 1;  // CCU81 reset clear bit
//             uint32_t POSIF0RS9  : 1;  // POSIF0 reset clear bit
//             uint32_t POSIF1RS10 : 1;  // POSIF1 reset clear bit
//             uint32_t USIC0RS11  : 1;  // USIC0 reset clear bit
//             uint32_t reserved1  : 4;  // Reserved bits [15:12]
//             uint32_t ERU1RS16   : 1;  // ERU1 reset clear bit
//             uint32_t reserved2  : 6;  // Reserved bits [22:17]
//             uint32_t reserved3  : 1;  // Reserved bit [23]
//             uint32_t reserved4  : 8;  // Reserved bits [31:24]
//         };
//     };
// } __attribute__((packed)) rcu_ctrl_t;

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

// /// Function to initialize the peripherals by clearing the reset bits
// void initialize_peripherals_reset() {
//     my_RCU->VADCRS0 = 1;   // De-assert VADC reset
//     my_RCU->DSDRS1 = 1;    // De-assert DSD reset
//     my_RCU->CCU40RS2 = 1;  // De-assert CCU40 reset
//     my_RCU->CCU41RS3 = 1;  // De-assert CCU41 reset
//     my_RCU->CCU42RS4 = 1;  // De-assert CCU42 reset
//     my_RCU->CCU80RS7 = 1;  // De-assert CCU80 reset
//     my_RCU->CCU81RS8 = 1;  // De-assert CCU81 reset
//     my_RCU->POSIF0RS9 = 1; // De-assert POSIF0 reset
//     my_RCU->POSIF1RS10 = 1;// De-assert POSIF1 reset
//     my_RCU->USIC0RS11 = 1; // De-assert USIC0 reset
//     my_RCU->ERU1RS16 = 1;  // De-assert ERU1 reset
// }