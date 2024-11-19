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
buffer DB 20 DUP(0)          ; Буфер для строки результата
var DWORD  42
.CODE
main PROC
    ; Преобразуем число в строку вручную
    mov eax, var             ; Загружаем число в eax
    mov ecx, 10              ; База системы (десятичная)
    mov edi, OFFSET buffer   ; Указатель на буфер
convert_loop:
    xor edx, edx             ; Очищаем edx перед делением
    div ecx                  ; Делим eax на 10 (результат в eax, остаток в edx)
    add dl, '0'              ; Преобразуем остаток в символ
    mov [edi], dl            ; Сохраняем символ в буфере
    inc edi                  ; Переходим к следующей ячейке в буфере
    test eax, eax            ; Проверяем, если eax == 0
    jnz convert_loop         ; Если eax не ноль, продолжаем цикл
    ; Переворачиваем строку
    mov esi, OFFSET buffer   ; Указатель на начало строки
    dec edi                  ; Переходим к последнему символу
reverse_loop:
    cmp esi, edi             ; Проверяем, если указатели не встретились
    jge reverse_done         ; Если указатели встретились или перепутались, заканчиваем
    mov al, [esi]            ; Загружаем символ с начала
    mov bl, [edi]            ; Загружаем символ с конца
    mov [esi], bl            ; Сохраняем символ с конца на начало
    mov [edi], al            ; Сохраняем символ с начала на конец
    inc esi                  ; Перемещаем указатель в начало
    dec edi                  ; Перемещаем указатель в конец
    jmp reverse_loop         ; Повторяем цикл
reverse_done:
INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET Titel, MB_OK
INVOKE ExitProcess, 0
main ENDP
END main