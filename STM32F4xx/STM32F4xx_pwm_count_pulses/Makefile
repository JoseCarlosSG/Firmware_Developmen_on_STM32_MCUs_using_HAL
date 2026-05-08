PROJECT_DIR=/home/carlos/Documents/Firmware_Developmen_on_STM32_MCUs_using_HAL/STM32F4xx/
# Directorio de la implementación.
APP_DIR=$(PROJECT_DIR)/STM32F4xx_pwm_count_pulses
# Rename bin and elf output file here
TARGET_NAME=pwm_setup
# Microcontroller Abstraction Layer.
MCAL_DIR=$(PROJECT_DIR)/STM32F4xx_MCAL
# Directorio de salida.
BUILD_DIR=$(APP_DIR)
# Archivo de linker
LINKER_FILE=$(MCAL_DIR)/Device/STM32F411RETX_FLASH.ld

# Compilador y opciones
CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -O0
CPPFLAGS=-DSTM32F411xE \
	 -DUSE_HAL_DRIVER \
	 -I$(APP_DIR) \
	 -I$(MCAL_DIR)/CMSIS/Core/Include \
	 -I$(MCAL_DIR)/Device/Inc \
	 -I$(MCAL_DIR)/HAL/Inc 
LDFLAGS=-T $(LINKER_FILE) -specs=nosys.specs

# Objetivo principal
all: $(BUILD_DIR)/$(TARGET_NAME).elf $(BUILD_DIR)/$(TARGET_NAME).bin

$(BUILD_DIR)/$(TARGET_NAME).bin: $(BUILD_DIR)/$(TARGET_NAME).elf
	arm-none-eabi-objcopy -O binary $(BUILD_DIR)/$(TARGET_NAME).elf $(BUILD_DIR)/$(TARGET_NAME).bin

# Objetivo .elf
$(BUILD_DIR)/$(TARGET_NAME).elf: $(BUILD_DIR)/main.o $(BUILD_DIR)/stm32f4xx_hal_msp.o \
	$(BUILD_DIR)/startup_stm32f411xe.o $(BUILD_DIR)/system_stm32f4xx.o \
	$(BUILD_DIR)/stm32f4xx_it.o $(BUILD_DIR)/stm32f4xx_hal.o \
	$(BUILD_DIR)/stm32f4xx_hal_rcc.o $(BUILD_DIR)/stm32f4xx_hal_cortex.o \
	$(BUILD_DIR)/stm32f4xx_hal_gpio.o $(BUILD_DIR)/stm32f4xx_hal_tim.o \
	$(BUILD_DIR)/stm32f4xx_hal_tim_ex.o $(BUILD_DIR)/stm32f4xx_hal_dma.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

# Compilación de archivos propios de la aplicación
$(BUILD_DIR)/main.o: $(APP_DIR)/main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f3xx_hal_msp.o: $(APP_DIR)/stm32f4xx_hal_msp.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilacion de archivos propios de cada microcontrolador
$(BUILD_DIR)/startup_stm32f411xe.o: $(MCAL_DIR)/Device/Src/startup_stm32f411xe.s
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/system_stm32f4xx.o: $(MCAL_DIR)/Device/Src/system_stm32f4xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f4xx_it.o: $(MCAL_DIR)/Device/Src/stm32f4xx_it.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
# Compilación de archivos fuentes de la libreria HAL
$(BUILD_DIR)/stm32f4xx_hal.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f4xx_hal_rcc.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_rcc.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
$(BUILD_DIR)/stm32f4xx_hal_cortex.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_cortex.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilación de archicos fuente correspondientes a un periferico
$(BUILD_DIR)/stm32f4xx_hal_gpio.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_gpio.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f4xx_hal_tim.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_tim.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f4xx_hal_tim_ex.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_tim_ex.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f4xx_hal_dma.o: $(MCAL_DIR)/HAL/Src/stm32f4xx_hal_dma.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Limpieza de archivos generados
clean: 
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin

# Cargar el archivo en el microcontrolador
load:

