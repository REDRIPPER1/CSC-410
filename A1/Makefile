CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BUILD_DIR := build

PROGRAMS := \
	$(BUILD_DIR)/arraysum \
	$(BUILD_DIR)/numIntegrate \
	$(BUILD_DIR)/matrixMul \
	$(BUILD_DIR)/nqueens \
	$(BUILD_DIR)/sorts

.PHONY: all clean run time

all: $(PROGRAMS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/arraysum: arraysum.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/numIntegrate: numIntegrate.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -lm -o $@

$(BUILD_DIR)/matrixMul: matrixMul/mainM.c matrixMul/matrixMul.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) matrixMul/mainM.c -o $@

$(BUILD_DIR)/nqueens: Nqueens/mainQ.c Nqueens/nqueens.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) Nqueens/mainQ.c -o $@

$(BUILD_DIR)/sorts: sorts/mainSorts.c sorts/sorts.c sorts/sorts.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) sorts/mainSorts.c sorts/sorts.c -o $@

run: all
	./$(BUILD_DIR)/arraysum
	./$(BUILD_DIR)/numIntegrate
	./$(BUILD_DIR)/matrixMul
	./$(BUILD_DIR)/nqueens
	./$(BUILD_DIR)/sorts

time: all
	/usr/bin/time -f 'array sum: %e s, %M KB' ./$(BUILD_DIR)/arraysum > /dev/null
	/usr/bin/time -f 'integration: %e s, %M KB' ./$(BUILD_DIR)/numIntegrate > /dev/null
	/usr/bin/time -f 'matrix multiplication: %e s, %M KB' ./$(BUILD_DIR)/matrixMul > /dev/null
	/usr/bin/time -f 'N queens: %e s, %M KB' ./$(BUILD_DIR)/nqueens > /dev/null
	/usr/bin/time -f 'sorting: %e s, %M KB' ./$(BUILD_DIR)/sorts > /dev/null

clean:
	rm -rf $(BUILD_DIR)