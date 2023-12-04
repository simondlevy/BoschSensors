SKETCH = BMP388_I2C

PORT = /dev/ttyACM0

LIB = $(HOME)/Documents/Arduino/libraries

BOARD = teensy:avr:teensy40

build:
	arduino-cli compile  --libraries $(LIB),.. --fqbn $(BOARD) $(SKETCH).ino

flash:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) .

edit:
	vim $(SKETCH).ino

listen:
	miniterm $(PORT) 115200
