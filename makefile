CC = gcc

DEPS := wpp.h \
	    wpp_test.h

CURRENT_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

TEST_DIR := $(CURRENT_DIR)test
EXMP_DIR := $(CURRENT_DIR)examples
EXES_DIR := $(CURRENT_DIR)output
EXES_TEST_DIR := $(EXES_DIR)/test
EXES_EXMP_DIR := $(EXES_DIR)/examples
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
EXMP_FILES := $(wildcard $(EXMP_DIR)/*.c)
TEST_EXES := $(patsubst $(TEST_DIR)/%.c,$(EXES_TEST_DIR)/%.out,$(TEST_FILES))
EXMP_EXES := $(patsubst $(EXMP_DIR)/%.c,$(EXES_EXMP_DIR)/%.out,$(EXMP_FILES))

CFLAGS := -g -O0 -std=gnu11

$(EXES_DIR)/%.out: $(CURRENT_DIR)%.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

prebuild:
	if [ ! -d "$(EXES_DIR)" ]; then \
		mkdir -p $(EXES_DIR); mkdir -p $(EXES_TEST_DIR); mkdir -p $(EXES_EXMP_DIR); \
	else \
		if [ ! -d "$(EXES_TEST_DIR)" ]; then mkdir -p $(EXES_TEST_DIR); fi; \
		if [ ! -d "$(EXES_EXMP_DIR)" ]; then mkdir -p $(EXES_EXMP_DIR); fi; \
	fi

clean:
	rm -f $(EXES_DIR)/*.out
	rm -f $(EXES_TEST_DIR)/*.out
	rm -f $(EXES_EXMP_DIR)/*.out
	rm -rf $(EXES_TEST_DIR)
	rm -rf $(EXES_EXMP_DIR)
	rm -rf $(EXES_DIR)

build-test: prebuild $(TEST_EXES)
build-example: prebuild $(EXMP_EXES)
build: prebuild $(TEST_EXES) $(EXMP_EXES)

execute-test:
	for FILE in $(TEST_EXES); do $$FILE; done
execute-example:
	for FILE in $(EXMP_EXES); do $$FILE; done
execute: execute-test execute-example

test: build-test execute-test
example: build-example execute-example
all: build execute