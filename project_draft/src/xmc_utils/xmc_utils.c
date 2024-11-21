#include <xmc_common.h>
#include <utils.h>

void initCCU4() {
    // Release the reset of the CCU40
    SCU_RESET->PRCLR0 = SCU_RESET_PRCLR0_CCU40RS_Msk;
    // Enable the clock for the CCU40 (step 3)
    SCU_CLK->CLKSET = SCU_CLK_CLKSET_CCUCEN_Msk;
    // Disable the CCU40 slice protection (step 4)
    CCU40->GIDLC = CCU4_GIDLC_SPRB_Msk;
    // Set the period and prescaler for the timer slice (step 6)
    CCU40_CC42->PRS = 0xffff;               // Set period for dimming
    CCU40_CC42->PSC = 0b1010;               // Set prescaler for dimming
    CCU40_CC42->CRS = 0xffff * pwm_rate;   // Set duty cycle for dimming (...%)
    // Enable CCU40 slice (step 7)
    CCU40->GCSS = CCU4_GCSS_S2SE_Msk;
    // Clear slice input selection (step 8)
    CCU40->GIDLC = CCU4_GIDLC_CS2I_Msk;
    // Set the run bit to start the timer (step 9)
    CCU40_CC42->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
}

// void connectLED() {
//     const static uint8_t ALT3 = 0b10011;
//     // Set P1.1 as output and connect to CCU40 channel
//     PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk) | (ALT3 << PORT1_IOCR0_PC1_Pos);
// }

void initSysTick() {
    SYST_CSR->ENABLE = PPB_SYST_CSR_ENABLE_Msk;
    // Set the reload value for 100 ms resolution
    SysTick->LOAD = (SystemCoreClock / 1000) * TIMER_RESOLUTION;
    // Set the priority of the SysTick interrupt
    NVIC_SetPriority(SysTick_IRQn, 0);
    // Enable the SysTick interrupt
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    ticks++;
    if (ticks == TIMER_RESOLUTION)
        ticks = 0;
}

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++){}
        // __NOP();  // TODO No Operation; Replace with timer-based delay if available
}

uint32_t get_time_ms() {
    // TODO
    // Placeholder: Implement timer-based time retrieval
}

void button1_wrapper(void (*funct)(void)) {
    if (is_transmitting) return;  // Ignore button presses during transmission
    button1_is_pressed = 1;

    (*funct)();  // Call the function to be wrapped
    
    button1_is_pressed = 0;
}

void button2_wrapper(void (*funct)(void)) {
    if (is_transmitting) return;  // Ignore button presses during transmission
    button2_is_pressed = 1;
    
    (*funct)();  // Call the function to be wrapped
    
    button2_is_pressed = 0;
}
