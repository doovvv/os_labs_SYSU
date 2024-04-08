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
mov cl, 2
mov bx, 0x7e00           ; bootloader的加载地址
load_bootloader:
    call asm_read_hard_disk  ; 读取硬盘
    inc cl
    cmp cl, 6
    jle load_bootloader
jmp 0x0000:0x7e00        ; 跳转到bootloader

jmp $ ; 死循环

asm_read_hard_disk:
    mov ah, 02h ;read disk
    mov al, 1
    mov ch, 0
    mov dh, 0
    mov dl, 80h
    int 13h
    cmp ah, 00h ;check
    jne asm_read_hard_disk
    add bx,512
    ret
times 510 - ($ - $$) db 0
db 0x55, 0xaa