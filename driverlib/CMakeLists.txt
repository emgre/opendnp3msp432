cmake_minimum_required(VERSION 2.8)

set(SRC
    adc14.c
    aes256.c
    comp_e.c
    cpu.c
    crc32.c
    cs.c
    dma.c
    flash.c
    fpu.c
    gpio.c
    i2c.c
    interrupt.c
    mpu.c
    pcm.c
    pmap.c
    pss.c
    ref_a.c
    reset.c
    rtc_c.c
    spi.c
    sysctl.c
    systick.c
    timer32.c
    timer_a.c
    uart.c
    wdt_a.c
)

include_directories(.)
add_library(driverlib ${SRC})