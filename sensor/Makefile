FILE=sensor.ino
TARGET=arduino:sam:arduino_due_x_dbg
PORT=/dev/ttyACM0

verify: $(FILE)
	@arduino $(FILE) --verify --board $(TARGET) --port $(PORT)

program: $(FILE)
	@arduino $(FILE) --upload --board $(TARGET) --port $(PORT)

serial:
	picocom $(PORT)
