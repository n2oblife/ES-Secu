#include <xmc_gpio.h>

const XMC_GPIO_CONFIG_t LED_config = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
        .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
        .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

int main(void){
XMC_GPIO_Init(XMC_GPIO_PORT1, 0, &LED_config);
XMC_GPIO_Init(XMC_GPIO_PORT1, 1, &LED_config);
XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 0);
XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
while(1);
return 0;
}
