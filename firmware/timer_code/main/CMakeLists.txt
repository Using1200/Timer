idf_component_register(SRCS "main.c" "buttons.c" "app_state.c" "buzzer.c" "timer_logic.c" "potentiometer.c" "display.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver st7789)
