#include<iostream>
#include<fstream>

using namespace std;

struct data_type {
    int8_t type = 0;
    int16_t length = 0;
    int32_t value = 0;

    data_type() {}
    data_type(int16_t type, int16_t length) {
        this->type = type;
        this->length = length;
    }
    data_type(int16_t type, int16_t length, int32_t value) {
        this->type = type;
        this->length = length;
        this->value = value;
    }
};


data_type var_deserialization(const char path[]) {
    ifstream in_file(path, ios::binary);

    if (!in_file.is_open()) {
        exit(-1);
    }

    data_type var;

    in_file.read(reinterpret_cast<char*>(&var.type), sizeof(var.type));
    in_file.read(reinterpret_cast<char*>(&var.length), sizeof(var.length));
    in_file.read(reinterpret_cast<char*>(&var.value), sizeof(var.value));

    in_file.close();

    return var;
}

void generate_assembly(const char path[], data_type var) {
    ofstream out_file(path);

    if (!out_file.is_open()) {
        cout << "create file error";
        return;
    }

    out_file << ".686P\n";
    out_file << ".MODEL FLAT, STDCALL\n";
    out_file << "includelib kernel32.lib\n";
    out_file << "ExitProcess PROTO : DWORD\n";
    out_file << "MessageBoxA PROTO : DWORD, : DWORD, : DWORD, :DWORD\n";

    out_file << ".STACK 4096\n";
    out_file << ".DATA\n";

    out_file << "Titel DB \"Title\", 0\n";
    out_file << "MB_OK EQU 0\n";
    out_file << "HW DD ? \n";
    out_file << "buffer DB 20 DUP(0)          ; Буфер для строки результата\n";


    if (var.length == 4) {
        out_file << "var DWORD  " << var.value << "\n";
    }
    else if (var.length == 2) {
        out_file << "var WORD  "<< var.value<<"\n";
    }
    else if (var.length == 1) {
        out_file << "var byte  " << var.value << "\n";
    }
    else {
        cout << "wrong var lenght";
        exit(-1);
    }

    out_file << ".CODE\n";
    out_file << "main PROC\n";

    out_file << "    ; Преобразуем число в строку вручную\n"
        "    mov eax, var             ; Загружаем число в eax\n"
        "    mov ecx, 10              ; База системы (десятичная)\n"
        "    mov edi, OFFSET buffer   ; Указатель на буфер\n"
        "convert_loop:\n"
        "    xor edx, edx             ; Очищаем edx перед делением\n"
        "    div ecx                  ; Делим eax на 10 (результат в eax, остаток в edx)\n"
        "    add dl, '0'              ; Преобразуем остаток в символ\n"
        "    mov [edi], dl            ; Сохраняем символ в буфере\n"
        "    inc edi                  ; Переходим к следующей ячейке в буфере\n"
        "    test eax, eax            ; Проверяем, если eax == 0\n"
        "    jnz convert_loop         ; Если eax не ноль, продолжаем цикл\n"
        "    ; Переворачиваем строку\n"
        "    mov esi, OFFSET buffer   ; Указатель на начало строки\n"
        "    dec edi                  ; Переходим к последнему символу\n"
        "reverse_loop:\n"
        "    cmp esi, edi             ; Проверяем, если указатели не встретились\n"
        "    jge reverse_done         ; Если указатели встретились или перепутались, заканчиваем\n"
        "    mov al, [esi]            ; Загружаем символ с начала\n"
        "    mov bl, [edi]            ; Загружаем символ с конца\n"
        "    mov [esi], bl            ; Сохраняем символ с конца на начало\n"
        "    mov [edi], al            ; Сохраняем символ с начала на конец\n"
        "    inc esi                  ; Перемещаем указатель в начало\n"
        "    dec edi                  ; Перемещаем указатель в конец\n"
        "    jmp reverse_loop         ; Повторяем цикл\n"
        "reverse_done:\n";

    out_file << "INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET Titel, MB_OK\n";
    out_file << "INVOKE ExitProcess, 0\n";
    out_file << "main ENDP\n";
    out_file << "END main";

    out_file.close();
}

int main() {
	

    data_type var = var_deserialization("..\\SE_asm_04\\out.bin");

    generate_assembly("..\\assembly\\ams_04.asm", var);
}