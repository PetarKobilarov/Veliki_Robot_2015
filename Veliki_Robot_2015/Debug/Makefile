################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../can.c \
../fuse.c \
../main.c \
../odometry.c \
../system.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
can.o \
fuse.o \
main.o \
odometry.o \
system.o

OBJS_AS_ARGS +=  \
can.o \
fuse.o \
main.o \
odometry.o \
system.o

C_DEPS +=  \
can.d \
fuse.d \
main.d \
odometry.d \
system.d

C_DEPS_AS_ARGS +=  \
can.d \
fuse.d \
main.d \
odometry.d \
system.d

OUTPUT_FILE_PATH +=Veliki_Robot_2015.elf

OUTPUT_FILE_PATH_AS_ARGS +=Veliki_Robot_2015.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler











./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=10000000ULL  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -Wall -pedantic -mmcu=at90can128 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Veliki_Robot_2015.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=at90can128  
	@echo Finished building target: $@
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Veliki_Robot_2015.elf" "Veliki_Robot_2015.hex"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Veliki_Robot_2015.elf" "Veliki_Robot_2015.eep" || exit 0
	"C:\WinAVR-20100110\bin\avr-objdump.exe" -h -S "Veliki_Robot_2015.elf" > "Veliki_Robot_2015.lss"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Veliki_Robot_2015.elf" "Veliki_Robot_2015.srec"
	"C:\WinAVR-20100110\bin\avr-size.exe" "Veliki_Robot_2015.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Veliki_Robot_2015.elf" "Veliki_Robot_2015.a" "Veliki_Robot_2015.hex" "Veliki_Robot_2015.lss" "Veliki_Robot_2015.eep" "Veliki_Robot_2015.map" "Veliki_Robot_2015.srec" "Veliki_Robot_2015.usersignatures"
	