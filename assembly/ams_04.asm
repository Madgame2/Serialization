.686P
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, :DWORD
.STACK 4096
.DATA
Titel DB "Title", 0
MB_OK EQU 0
HW DD ? 
buffer DB 20 DUP(0)          ; ����� ��� ������ ����������
var DWORD  42
.CODE
main PROC
    ; ����������� ����� � ������ �������
    mov eax, var             ; ��������� ����� � eax
    mov ecx, 10              ; ���� ������� (����������)
    mov edi, OFFSET buffer   ; ��������� �� �����
convert_loop:
    xor edx, edx             ; ������� edx ����� ��������
    div ecx                  ; ����� eax �� 10 (��������� � eax, ������� � edx)
    add dl, '0'              ; ����������� ������� � ������
    mov [edi], dl            ; ��������� ������ � ������
    inc edi                  ; ��������� � ��������� ������ � ������
    test eax, eax            ; ���������, ���� eax == 0
    jnz convert_loop         ; ���� eax �� ����, ���������� ����
    ; �������������� ������
    mov esi, OFFSET buffer   ; ��������� �� ������ ������
    dec edi                  ; ��������� � ���������� �������
reverse_loop:
    cmp esi, edi             ; ���������, ���� ��������� �� �����������
    jge reverse_done         ; ���� ��������� ����������� ��� ������������, �����������
    mov al, [esi]            ; ��������� ������ � ������
    mov bl, [edi]            ; ��������� ������ � �����
    mov [esi], bl            ; ��������� ������ � ����� �� ������
    mov [edi], al            ; ��������� ������ � ������ �� �����
    inc esi                  ; ���������� ��������� � ������
    dec edi                  ; ���������� ��������� � �����
    jmp reverse_loop         ; ��������� ����
reverse_done:
INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET Titel, MB_OK
INVOKE ExitProcess, 0
main ENDP
END main