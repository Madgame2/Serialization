#include<iostream>
#include<fstream>
#include <cstdarg>

using namespace std;

struct data_type {
    int8_t type = 0;
    int16_t length = 0;
    int32_t* value = nullptr;

    data_type() {}
    data_type(int8_t type, int32_t value) {
        this->type = type;
        this->length = 1;
        this->value = new int32_t(value);
    }
    data_type(int16_t type, int16_t length, int32_t value...) {
        this->type = type;
        this->length = length;
        this->value = new int32_t[length];

        this->value[0] = value;

        va_list args;
        va_start(args, value);

        for (int i = 1; i < length; i++) {
            this->value[i] = va_arg(args, int32_t);
        }

        va_end(args);
    }

};


data_type var_deserialization(const char path[]) {
    ifstream in_file(path, ios::binary);

    if (!in_file.is_open()) {
        cout << "������ �������� �����" << endl;
        exit(-1);
    }

    data_type var;

    // ��������� �������� ����
    in_file.read(reinterpret_cast<char*>(&var.type), sizeof(var.type));
    in_file.read(reinterpret_cast<char*>(&var.length), sizeof(var.length));

    // ��������� ����� ������� � ��������� ������, ���� ��� ����
    if (var.length > 0) {
        var.value = new int32_t[var.length];
        in_file.read(reinterpret_cast<char*>(var.value), sizeof(int32_t) * var.length);
    }

    in_file.close();

    if (!in_file) {
        cout << "������ ������ �� �����" << endl;
        exit(-1);
    }

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
    out_file << "MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD\n";

    out_file << ".STACK 4096\n";
    out_file << ".DATA\n";

    out_file << "head DB \"Array Elements\", 0\n";
    out_file << "MB_OK EQU 0\n";
    out_file << "HW DD ?\n";
    out_file << "buffer DB 20 DUP(0)          ; ����� ��� ������ ����������\n";

    out_file << "data DB " << *var.value;  // ������ ������� �������
    for (int i = 1; i < var.length; i++) {
        out_file << ", " << var.value[i]; // ��������� ��������
    }
    out_file << "\n";

    out_file << "arraySize EQU ($ - data)            ; ������ ������� (����������� �������������)\n";

    out_file << ".CODE\n";
    out_file << "main PROC\n";

    out_file << "    mov esi, OFFSET data            ; ��������� �� ������\n";
    out_file << "    mov ecx, arraySize              ; ���������� ��������� �������\n";

    out_file << "process_array:\n";
    out_file << "    cmp ecx, 0                      ; ���������, ���� �� ��� ��������\n";
    out_file << "    je end_loop                     ; ���� ���, ��������� ����\n";

    out_file << "    movzx eax, BYTE PTR [esi]       ; ��������� ������� ������� �������\n";
    out_file << "    call convert_to_string          ; ����������� ����� � ������\n";

    out_file << "    INVOKE MessageBoxA, HW, OFFSET buffer, OFFSET head, MB_OK\n";

    out_file << "    inc esi                         ; ��������� � ���������� �������� �������\n";
    out_file << "    dec ecx                         ; ��������� �������\n";
    out_file << "    jmp process_array               ; ��������� ����\n";

    out_file << "end_loop:\n";
    out_file << "    INVOKE ExitProcess, 0\n";
    out_file << "main ENDP\n";

    out_file << "convert_to_string PROC\n";
    out_file << "    mov ecx, 10                     ; ���� ������� (����������)\n";
    out_file << "    mov edi, OFFSET buffer          ; ��������� �� �����\n";
    out_file << "convert_loop:\n";
    out_file << "    xor edx, edx                    ; ������� �������\n";
    out_file << "    div ecx                         ; ����� eax �� 10\n";
    out_file << "    add dl, '0'                     ; ����������� ������� � ������\n";
    out_file << "    mov [edi], dl                   ; ��������� ������ � �����\n";
    out_file << "    inc edi                         ; ��������� � ��������� �������\n";
    out_file << "    test eax, eax                   ; ���������, ���� eax == 0\n";
    out_file << "    jnz convert_loop                ; ���� �� 0, ����������\n";
    out_file << "    mov BYTE PTR [edi], 0           ; ��������� ������ ������� ������\n";
    out_file << "    ret\n";
    out_file << "convert_to_string ENDP\n";

    out_file << "END main\n";



    out_file.close();
}

int main() {
	

    data_type var = var_deserialization("..\\SE_asm_04\\out.bin");

    generate_assembly("..\\assembly\\ams_04.asm", var);
}