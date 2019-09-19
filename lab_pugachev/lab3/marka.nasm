[BITS 16]
[ORG 0x7c00]

_start:
	;xor cx, cx
	;mov cx, 512
	;mov ax,0x7e00
	;mov ds, ax
	;xor di, di
	;mov ax, 0x2020
	;loop_str:
		;sub [di], ax
		;add si, 2
		;loop loop_str
	xor si, si
	xor ax,ax
	mov es,ax
	mov cx,2
	;mov dl, 0
	mov dh,0
	mov bx, 0x7e00
	mov ax, 0x0202
	;int 0x10
	int 0x13
	jnc .m1
	.m2:
	jmp .m2
	.m1:
	xor cx, cx
	mov cx, 512
	mov ax, 0x7e0
	mov ds, ax
	xor di, di
	mov ax, 0x1234
	loop_str:
		xor [di], ax
		add di, 2
		loop loop_str
	jmp 0:0x7e00
	
TIMES 510-($-_start) db 0
dw 0xaa55
	;jmp 0x7e00
	jmp main
;TIMES 2879*512 db 0

main:
	mov ax, 0xb800
	mov ds, ax
	xor si, si
	mov cx, 8
	mov bx, 77
	loop_st:
		mov ax, 0x0f07
		mov dx, cx
		mov bx,80
		inc bx
		mov cx, bx
		loop_start:
			cmp cx, 77
			je symbl
			cmp cx, 76
			je probel
			cmp cx, 73
			je symbl
			cmp cx, 72
			je probel
			cmp cx, 69
			je symbl
			cmp cx, 68
			je probel
			cmp cx, 65
			je symbl
			cmp cx, 64
			je probel
			cmp cx, 61
			je symbl
			cmp cx, 60
			je probel
			cmp cx, 57
			je symbl
			cmp cx, 56
			je probel
			cmp cx, 53
			je symbl
			cmp cx, 52
			je probel
			cmp cx, 51
			je pust
			vozvr:
			mov [si], ax
			add si, 2
			loop loop_start
		jmp meta
		pust:
			mov ax, 0x0000
			jmp vozvr
		symbl:
			mov ax, 0xbf0f
			sub bx, 1
			jmp vozvr
		probel:
			mov ax, 0x0f07
			sub bx,3
			jmp vozvr
		meta:
			mov dx,dx
		mov cx, dx
		loop loop_st
;nachalo podpisi...
	mov ax, 0x0f53
	mov [si], ax
	add si, 2
	mov ax, 0x0f45
	mov [si], ax
	add si, 2
	mov ax, 0x0f56
	mov [si], ax
	add si, 2
	mov ax, 0x0f4f
	mov [si], ax
	add si, 2
	mov ax, 0x0f53
	mov [si], ax
	add si, 2
	mov ax, 0x0f54
	mov [si], ax
	add si, 2
	mov ax, 0x0f49
	mov [si], ax
	add si, 2
	mov ax, 0x0f41
	mov [si], ax
	add si, 2
	mov ax, 0x0f4e
	mov [si], ax
	add si, 2
	mov ax, 0x0f4f
	mov [si], ax
	add si, 2
	mov ax, 0x0f56
	mov [si], ax
	add si, 2
	mov ax, 0x0000
	mov [si], ax
	add si, 2
	mov ax, 0x0f41
	mov [si], ax
	add si, 2
	mov ax, 0x0f4e
	mov [si], ax
	add si, 2
	mov ax, 0x0f44
	mov [si], ax
	add si, 2
	mov ax, 0x0f52
	mov [si], ax
	add si, 2
	mov ax, 0x0f45
	mov [si], ax
	add si, 2
	mov ax, 0x0f49
	mov [si], ax
	add si, 2

		metka:
			jmp $

;TIMES 510-($-_start) db 0
;dw 0xaa55
TIMES 2879*512 db 0
int 10h
