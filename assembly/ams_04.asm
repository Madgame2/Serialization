.686P
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD
.STACK 4096
.DATA
head DB "Array Elements", 0
MB_OK EQU 0
HW DD ?
buffer DB 20 DUP(0)          ; ����� ��� ������ ����������
data DB 2, 1, 2, 3
arraySize EQU ($ - data)            ; ������ ������� (����������� �������������)
.CODE
main PROC
    mov esi, OFFSET data            ; ��������� �� ������
    mov ecx, arraySize              ; ���������� ��������� �������
process_array:
    cmp ecx, 0                      ; ���������, ���� �� ��� ��������
    je end_loop                     ; ���� ���, ��������� ����
    movzx eax, BYTE PTR [esi]       ; ��������� ������� ������� �������
    call convert_to_string          ; ����������� ����� � ������
    INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET head, MB_OK
    inc esi                         ; ��������� � ���������� �������� �������
    dec ecx                         ; ��������� �������
    jmp process_array               ; ��������� ����
end_loop:
    INVOKE ExitProcess, 0
main ENDP
convert_to_string PROC
    mov ecx, 10                     ; ���� ������� (����������)
    mov edi, OFFSET buffer          ; ��������� �� �����
convert_loop:
    xor edx, edx                    ; ������� �������
    div ecx                         ; ����� eax �� 10
    add dl, '0'                     ; ����������� ������� � ������
    mov [edi], dl                   ; ��������� ������ � �����
    inc edi                         ; ��������� � ��������� �������
    test eax, eax                   ; ���������, ���� eax == 0
    jnz convert_loop                ; ���� �� 0, ����������
    mov BYTE PTR [edi], 0           ; ��������� ������ ������� ������
    ret
convert_to_string ENDP
END main
