TARGET = summarize
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic -Os -lcurl

TARGET_DEBUG = summarize_debug
CFLAGS_DEBUG ?= -Wall -Wextra -pedantic -fsanitize=address -fsanitize=undefined -lcurl



$(TARGET): main.c
	clang $(CFLAGS) main.c jsmn.c -o $@ 

$(TARGET_DEBUG): main.c
	clang $(CFLAGS) main.c jsmn.c -o $@ 

clean:
	rm -f $(TARGET)
	rm -f $(TARGET_DEBUG)
	