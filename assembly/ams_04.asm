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
buffer DB 20 DUP(0)          ; Буфер для строки результата
data DB 2, 1, 2, 3
arraySize EQU ($ - data)            ; Размер массива (вычисляется автоматически)
.CODE
main PROC
    mov esi, OFFSET data            ; Указатель на массив
    mov ecx, arraySize              ; Количество элементов массива
process_array:
    cmp ecx, 0                      ; Проверяем, есть ли ещё элементы
    je end_loop                     ; Если нет, завершаем цикл
    movzx eax, BYTE PTR [esi]       ; Загружаем текущий элемент массива
    call convert_to_string          ; Преобразуем число в строку
    INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET head, MB_OK
    inc esi                         ; Переходим к следующему элементу массива
    dec ecx                         ; Уменьшаем счётчик
    jmp process_array               ; Повторяем цикл
end_loop:
    INVOKE ExitProcess, 0
main ENDP
convert_to_string PROC
    mov ecx, 10                     ; База системы (десятичная)
    mov edi, OFFSET buffer          ; Указатель на буфер
convert_loop:
    xor edx, edx                    ; Очищаем остаток
    div ecx                         ; Делим eax на 10
    add dl, '0'                     ; Преобразуем остаток в символ
    mov [edi], dl                   ; Сохраняем символ в буфер
    inc edi                         ; Переходим к следующей позиции
    test eax, eax                   ; Проверяем, если eax == 0
    jnz convert_loop                ; Если не 0, продолжаем
    mov BYTE PTR [edi], 0           ; Завершаем строку нулевым байтом
    ret
convert_to_string ENDP
END main
