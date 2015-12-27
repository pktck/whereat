port = $(shell python get_prog_port.py)

all: build/whereat.ino.bin

upload: build/whereat.ino.bin
	echo Port: $(port)
	stty -F /dev/$(port) speed 1200 cs8 -cstopb -parenb
	/home/pktck/.arduino15/packages/arduino/tools/bossac/1.6.1-arduino/bossac -i --port=$(port) -U false -e -w -v -b /home/pktck/whereat/arduino/whereat/./build/whereat.ino.bin -R

build/whereat.ino.bin: whereat.ino
	make clean
	/home/pktck/arduino-1.6.7/arduino-builder -logger=machine -hardware "/home/pktck/arduino-1.6.7/hardware" -hardware "/home/pktck/.arduino15/packages" -tools "/home/pktck/arduino-1.6.7/tools-builder" -tools "/home/pktck/arduino-1.6.7/hardware/tools/avr" -tools "/home/pktck/.arduino15/packages" -built-in-libraries "/home/pktck/arduino-1.6.7/libraries" -libraries "/home/pktck/Arduino/libraries" -fqbn=arduino:sam:arduino_due_x_dbg -vid-pid=0X2341_0X003D -ide-version=10607 -build-path "/home/pktck/whereat/arduino/whereat/./build" -warnings=null -prefs=build.warn_data_percentage=75 "/home/pktck/whereat/arduino/whereat/whereat.ino"

clean:
	rm -fr build/*
