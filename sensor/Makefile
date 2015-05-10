FILE=sensor.ino
TARGET=arduino:sam:due
PORT=/dev/ttyACM2

verify: $(FILE)
	@arduino $(FILE) --verify --board $(TARGET) --port $(PORT)

program: $(FILE)
	@arduino $(FILE) --upload --board $(TARGET) --port $(PORT)

serial:
	picocom $(PORT)
