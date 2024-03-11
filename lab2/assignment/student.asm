; If you meet compile error, try 'sudo apt install gcc-multilib g++-multilib' first

%include "head.include"
; you code here


your_if:
; put your implementation here
    cmp byte[a1], 12
    jge else_if
    mov eax, [a1]
    mov bx, 2
    cdq
    idiv bx
    add eax, 1
    mov [if_flag],eax 
    jmp end
else_if:
    cmp byte[a1], 24
    jge else
    mov eax, [a1]
    sub eax, 24
    neg eax
    imul byte[a1]
    mov [if_flag], eax
    jmp end
else:
    mov eax, [a1]
    shl eax, 4
    mov [if_flag], eax
    jmp end
    
end:
your_while:
; put your implementation here
    cmp byte[a2], 12
    jl while_end
    call  my_random
    mov ebx, [a2]
    sub ebx, 12
    mov ecx, [while_flag] ;twice [] because while_flag is addresss in x86
    mov [ecx+ebx], al ;al,not eax, because while_flag is char array, only one byte, al is 8bit but eax is 32bit
    sub byte[a2], 1
    jmp your_while
while_end:
%include "end.include"

your_function:
; put your implementation here
    mov ecx, 0
    mov ebx, [your_string]
    for:
        mov al, [ebx+ecx]
        cmp al, 0 ;can't be '\0'
        je function_end
        pushad
        ;mov ax, ebx[ecx]
        push ax
        call print_a_char
        pop ax
        popad
        add ecx, 1
        jmp for
    function_end:
        ret
