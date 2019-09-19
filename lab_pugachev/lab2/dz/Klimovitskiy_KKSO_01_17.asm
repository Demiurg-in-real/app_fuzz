;	nasm Klimovitskiy_KKSO_01_17.asm -fbin -ofloppy.img
;	qemu-system-i386 -fda floppy.img

[BITS 16]
[ORG 0x7c00]

    xor ax,ax
    mov es,ax

    mov ah, 0x2              ; ax,0x0202
    mov al, 0x2
    mov ch, 0
    xor dh, dh
    mov dh, 0
    mov cl, 2
    ;mov dl, 0xF ;0xF
    mov bx, 0x7E00             ;0x7E00
    int 0x13

    xor cx,cx
    mov di,0x7E00

    mov cx,512
lp0:
    xor ax,ax
    mov al,byte [di]
    cmp al,3
    jae m0
    add ax,0x100
    m0:
    sub ax,3
    mov byte [di],al
    inc di
    loop lp0

    jmp 0:0x7E00

    times 510 - ($-$$) db 0
    dw 0xaa55



    xor cx,cx
    mov cx, 44
    push cx
    begin:
    pop cx
    mov dx, cx              ; for increase
    dec cx
    cmp cx, 0
    jz halt
    push cx

    mov ax, 0x0002            ;clean screen
    int 0x10

    cli
    mov ah, 0x0e

mov si, lucky_cat       ; cat is here, really
loop:
lodsb
cmp al, ' '             ; if ' ' then print a lot of ' '
jnz noIncrease

push cx
mov cx, dx
loop_print_increase:
int 0x10
loop loop_print_increase
    pop cx

noIncrease:
	or al, al
	jz next_step
	int 0x10                ; print
	jmp loop

next_step:
    mov al, 10
    int 0x10

    mov si, secret          ;my name - it's a secret

lp3:
    lodsb
	or al, al
    jz waiting              ;go and wait a lot
	int 0x10
	jmp lp3

waiting:
    mov     cx, 2
    mov     ah, 86h
    int     15h

    mov cx,0FFFFh ;one more method of waiting
    loop  $
jmp begin

halt:
	hlt

secret: db 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A, 0x0D, "Klimovitskiy Maxim KKSO-01-17", 0

lucky_cat: db ' ', "/\_/\", 0x0A, 0x0D, ' ', "|0s0|", 0x0A, 0x0D, ' ', "|####\_____/|", 0x0A, 0x0D, ' ', "|_|_|______)", 0x0A, 0x0D, ' ', "|_|_|...|_|_|#", 0

    times 2880*512 - ($-$$) db 0

;/\_/\
;|0s0|
;|""""\_____/|
;|_|_|______)
;|_|_|...|_|_|
