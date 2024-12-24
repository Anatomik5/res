Command line used to find this crash:

/home/alisher/test/AFLplusplus-4.21c/afl-fuzz -P explore -i input_elf/ -o no_nop/ -- /home/alisher/arm_sim_fuzz/qemu/build/qemu-system-aarch64 -M virt -cpu cortex-a72 -display none -kernel @@

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 0 B.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please post
to https://github.com/AFLplusplus/AFLplusplus/issues/286 once the issues
 are fixed :)

