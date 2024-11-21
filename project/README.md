# Architecture template of the project
├── src/
│   ├── app/                     # Application layer
│   │   ├── main.c               # Entry point
│   │   ├── state_machine.c      # State machine logic
│   │   ├── tasks.c              # Application-specific tasks
│   ├── drivers/                 # Device driver layer
│   │   ├── button_driver.c
│   │   ├── led_driver.c
│   │   ├── motor_driver.c
│   │   ├── sensor_driver.c
│   ├── hal/                     # HAL (Hardware Abstraction Layer)
│   │   ├── xmc_gpio.c
│   │   ├── xmc_timer.c
│   │   ├── xmc_pwm.c
│   │   ├── xmc_uart.c
│   ├── middleware/              # Middleware layer
│   │   ├── communication.c      # UART/CAN protocols
│   │   ├── math_utils.c         # Math algorithms
│   │   ├── scheduler.c          # Lightweight task scheduler
├── include/                     # Shared headers for all layers
│   ├── app/
│   │   ├── state_machine.h
│   │   ├── tasks.h
│   ├── drivers/
│   │   ├── button_driver.h
│   │   ├── led_driver.h
│   │   ├── motor_driver.h
│   │   ├── sensor_driver.h
│   ├── hal/
│   │   ├── xmc_gpio.h
│   │   ├── xmc_timer.h
│   │   ├── xmc_pwm.h
│   │   ├── xmc_uart.h
│   ├── middleware/
│       ├── communication.h
│       ├── math_utils.h
│       ├── scheduler.h
├── config/                      # Configuration files
│   ├── config.h                 # Global configurations
│   ├── pin_map.h                # Pin mappings for the board
├── build/                       # Build directory (compiled outputs)
├── Makefile                     # Build rules
└── README.md                    # Documentation
