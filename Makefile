CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude
BUILD := build
LIB := $(BUILD)/libusbproto.a
DEMO := $(BUILD)/usb_demo
TEST := $(BUILD)/usb_tests
SRC := $(wildcard firmware/src/*.c)
OBJ := $(patsubst firmware/src/%.c,$(BUILD)/%.o,$(SRC))
.PHONY: all demo test clean
all: demo test
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
demo: $(DEMO)
	./$(DEMO)
test: $(TEST)
	./$(TEST)
clean:
	rm -rf $(BUILD)
