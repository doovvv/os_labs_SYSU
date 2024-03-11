org 0x7c00
[bits 16]
xor ax, ax ; eax = 0
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; 初始化栈指针
mov sp, 0x7c00
mov ax, 0xb800
mov gs, ax

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 0
int 10h

;display num
mov ah, 09h
mov al, '2'
mov bh, 0
mov bl, 0x01
mov cx, 2
int 10h

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 2
int 10h

mov ah, 09h
mov al, '3'
mov bh, 0
mov bl, 0x01
mov cx, 2
int 10h

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 4
int 10h

;display num
mov ah, 09h
mov al, '6'
mov bh, 0
mov bl, 0x01
mov cx, 1
int 10h

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 5
int 10h

;display num
mov ah, 09h
mov al, '2'
mov bh, 0
mov bl, 0x01
mov cx, 1
int 10h

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 6
int 10h

;display num
mov ah, 09h
mov al, '9'
mov bh, 0
mov bl, 0x01
mov cx, 1
int 10h

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 7
int 10h

;display num
mov ah, 09h
mov al, '0'
mov bh, 0
mov bl, 0x01
mov cx, 1
int 10h
jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa
