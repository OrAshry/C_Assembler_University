macr mmacsssteomr3
       cmp r3, #-6
       bne END
endmacr

LALALAL: mov r3, r1
LABEL2: .string "Hello, "" World!"

; Another comment
LABEL3: .entry EXTERNLABEL
LABEL1: mov r2,r4

LABEL4: .entry ENTRY_LABEL

; Comment before instructions
mov r1, r2
cmp #5, r3

add r4, r5

sub r6, LABEL5
lea LABEL6, r7

clr r8
not r9

inc r10

dec r11
jmp LABEL7

mmacsssteomr3

bne LABEL8

red r12
prn #3

jsr LABEL9
rts
stop

LABEL10: .data 1, 2, 3, 4, 5

LABEL11: cmp r3, #-6