CC = gcc-8

DEPS := wpp.h \
	   wpp_test.h

TEST_DIR := ./test
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_EXES := $(patsubst $(TEST_DIR)/%.c,$(TEST_DIR)/%.out,$(TEST_FILES))

CFLAGS := -g -O0 -std=gnu11

%.out: %.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<
	./$@

clean:
	rm -f $(TEST_DIR)/*.out

build: $(TEST_EXES)
