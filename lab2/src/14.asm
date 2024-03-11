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

;clear screen
mov ah, 0x06     ; 功能号0x06  
mov    al, 0     ; 上卷全部行，即清屏  
mov    ch, 0     ; 左上角列    
mov    dh, 24     ; 右下角行  
mov    dl, 79     ; 右下角列  
mov    bh, 0x07     ; 空白区域属性  
int     0x10

var1 db -1
var2 db 1
mov al, '0'
mov dh, 0
mov dl, 2
mov bl, 0x00
mov ecx, 0 ;init argument
loop:
    cmp ecx, 100000000 ;add time to observe
    je function
    add ecx,1
    jmp loop
    function:
        call set_cursor
        call display_num ;even num
        sub dh, 24
        neg dh
        sub dl, 80
        neg dl
        add al, 1
        call set_cursor
        call display_num ;odd num
        sub al, 1
        neg dl
        add dl, 80
        neg dh
        add dh, 24
        cmp dh, 24
        je change_var1
        cmp dh, 0
        je change_var1 ;change direction
        continue1:
        add dh,[var1]
        cmp dl, 80
        je change_var2
        cmp dl, 0
        je change_var2 ;change direction
        continue2:
        add dl,[var2]
        add bl,1
        mov ecx, 0
        cmp al, '8'
        je update_al
        add al, 2
        jmp loop
        update_al:
            mov al,'0'
            jmp loop

change_var1:
    neg byte[var1]
    jmp continue1
change_var2:
    neg byte[var2]
    jmp continue2

set_cursor:
    mov ah, 02h
    mov bh, 0x0
    int 10h
    ret

display_num:
    mov ah, 09h
    ;mov al, '2'
    mov bh, 0
    ;mov bl, 0x01
    mov cx, 1
    int 10h
    ret