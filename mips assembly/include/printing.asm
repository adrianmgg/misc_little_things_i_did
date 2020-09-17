.include "syscalls.asm"

# $a0 - int to print
.eqv print_int dosyscall 1
.eqv print_int_hex dosyscall 34
.eqv print_int_bin dosyscall35
.eqv print_uint dosyscall36

# $f12 - float/double to print
.eqv print_float dosyscall 2
.eqv print_double dosyscall 3

# $a0 - address of (null-terminated) string to print
.eqv print_string dosyscall 4

# $a0 - character to print
# "Prints ASCII character corresponding to contents of low-order byte."
.eqv print_char dosyscall 11


.macro print_char_i (%char)
li $a0, %char
print_char
.end_macro
