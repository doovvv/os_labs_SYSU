;org 0x7c00
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

;clear screen
mov ah, 0x06     ; 功能号0x06  
mov    al, 0     ; 上卷全部行，即清屏  
mov    ch, 0     ; 左上角列    
mov    dh, 24     ; 右下角行  
mov    dl, 79     ; 右下角列  
mov    bh, 0x07     ; 空白区域属性  
int     0x10

;set cursor
mov ah, 02h
mov bh, 0x0
mov dh, 0
mov dl, 0
int 10h

loop:
    call keyboard_input
    call display_charcters
    call set_cursor
    jmp loop ; 

keyboard_input:
    mov ah, 00h
    int 16h
    ret

display_charcters:
    mov ah, 09h
    mov bh, 0
    mov bl, 0x01
    mov cx, 1
    int 10h
    ret

set_cursor:
    mov ah, 02h
    mov bh, 0x0
    mov dh, 0
    add dl, 1
    int 10h
    ret

times 510 - ($ - $$) db 0
db 0x55, 0xaa