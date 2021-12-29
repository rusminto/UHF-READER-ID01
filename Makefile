-include arduino.mk

#---- START OF CONFIG, only applied if config file doesn't exist -----#

#~ Port where Arduino is connected
PORT?=/dev/cu.usbmodem14101

#~ Board name, ex: nano, uno, etc.
BOARD?=mega

#~ specific CPU if needed, especially on nano, ex: atmega328old, atmega328, atmega168, etc.
CPU?=atmega2560

#~ Arduino uC type, ex: avr, samd, etc.
TYPE?=avr

#----- END OF CONFIG ------#

CORE=arduino\:$(TYPE)
PROC=$(BOARD)\:cpu=$(CPU)

#~ if CPU not defined, then empty PROC as it's not needed
ifeq ($(CPU),)
	PROC=$(BOARD)
endif

TARGET=$(CORE)\:$(PROC)
DIR=.
BIN:=$(shell which arduino-cli)

all: upload clean

clean:
	rm -rf build/ $(DIR)/.*.hex $(DIR)/.*.elf

compile: clean
	$(BIN) compile --export-binaries --fqbn $(TARGET) $(DIR)

upload: compile
	$(BIN) upload --port $(PORT) --fqbn $(TARGET) $(DIR) --verify --verbose

monitor:
	$(BIN) monitor --port $(PORT) --config baudrate=9600

get-fqbn:
	@echo $(TARGET)
