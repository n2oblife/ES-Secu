#include <xmc_common.h>

const unsigned int __stack_chk_guard = 0xdeadbeef;

void __stack_chk_fail(void) __attribute__ ((alias ("__NVIC_SystemReset")));
/*  NVIC_SystemReset();
}*/
