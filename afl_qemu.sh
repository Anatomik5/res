#!/bin/bash
# output for first mutator random without save dict
INPUT_DIR="input"
OUTPUT_DIR="output"
WRAPPER_SCRIPT="run_qemu"
WRAPPER_SOURCE="run_qemu.c"

cat <<EOF > $WRAPPER_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_elf>\n", argv[0]);
        return 1;
    }
    char *args[] = {
        "/home/alisher/arm_sim_fuzz/qemu/build/qemu-system-aarch64",
        "-machine", "raspi4b",
        "-display", "none", "-serial","stdio",
        "-kernel", argv[1],
        NULL
    };
     execvp(args[0], args);

    return 1;
}
EOF
gcc $WRAPPER_SOURCE -o $WRAPPER_SCRIPT 
export AFL_SKIP_BIN_CHECK=1
export AFL_SKIP_CPUFREQ=1
export AFL_CUSTOM_MUTATOR_LIBRARY="/home/alisher/test/AFLplusplus-4.21c/custom_mutators/aflpp/aflpp-mutator1.so"

export AFL_CUSTOM_MUTATOR_ONLY=1
AFL_MAP_SIZE=10000000 AFL_DISABLE_TRIM=1 /home/alisher/test/AFLplusplus-4.21c/afl-fuzz  -P explore -i $INPUT_DIR -o $OUTPUT_DIR  -- ./run_qemu @@


