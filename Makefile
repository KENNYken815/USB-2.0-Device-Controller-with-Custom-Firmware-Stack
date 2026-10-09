CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Ifirmware/include
BUILD := build
LIB := $(BUILD)/libusbproto.a
DEMO := $(BUILD)/usb_demo
TEST := $(BUILD)/usb_tests
CTRL_TEST := $(BUILD)/usb_control_tests
SRC := $(wildcard firmware/src/*.c)
OBJ := $(patsubst firmware/src/%.c,$(BUILD)/%.o,$(SRC))
.PHONY: all demo test control-test clean
all: demo test control-test
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/%.o: firmware/src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(LIB): $(OBJ)
	ar rcs $@ $^
$(DEMO): firmware/examples/usb_demo.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@
$(TEST): firmware/tests/test_usb.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@
$(CTRL_TEST): firmware/tests/test_control.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@
demo: $(DEMO)
	./$(DEMO)
test: $(TEST)
	./$(TEST)
control-test: $(CTRL_TEST)
	./$(CTRL_TEST)
clean:
	rm -rf $(BUILD)
