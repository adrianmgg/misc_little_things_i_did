.macro dosyscall (%id)
li $v0, %id
syscall
.end_macro


#.macro exit
#dosyscall(10)
#.end_macro

#.macro exit (%exitcode)
#i $a0, %exitcode
#dosyscall(17)
#.end_macro

## duration - milliseconds
#.macro sleep (%duration)
#li $a0, %duration
#dosyscall(32)
#.end_macro

#.macro midi_out_async
#dosyscall(31)
#.end_macro

#.macro midi_out_blocking
#dosyscall(33)
#.end_macro

