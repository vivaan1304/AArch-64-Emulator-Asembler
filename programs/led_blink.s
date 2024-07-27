ldr w1, setPin
ldr w2, count
ldr w3, onAdr
ldr w4, offAdr
ldr w5, set21
ldr w6, pinNum
str w6, [w1]
loop:
    str w5, [w3]
    ldr w10, zero
    str w10, [w4]
    on:
        add w10, w10, #1
        cmp w10, w2	
        b.ne on
    str w5, [w4]
    ldr w10, zero
    off:
        add w10, w10, #1
        cmp w10, w2	
        b.ne off
b loop

count:
    .int 0xf0000
set21:
    .int 0x200000
zero:
    .int 0x0
setPin:
    .int 0x3f200008 
onAdr:
    .int 0x3f20001c
offAdr:
    .int 0x3f200028
pinNum:
    .int 0x8