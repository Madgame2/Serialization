#include<iostream>
#include<fstream>
#include <cstdarg>


using namespace std;

struct data_type {
    int8_t type = 0;   
    int16_t length=0;  
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

    ~data_type() {
        delete[] value; 
    }
};


void var_serialization(const data_type& var, const char* path) {
    ofstream out_file(path, ios::binary);

    if (!out_file.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    // Записываем данные в бинарном формате
    out_file.write(reinterpret_cast<const char*>(&var.type), sizeof(var.type));
    out_file.write(reinterpret_cast<const char*>(&var.length), sizeof(var.length));

    if (var.value != nullptr && var.length > 0) {
        out_file.write(reinterpret_cast<const char*>(var.value), sizeof(int32_t) * var.length);
    }

    out_file.close();  // Закрываем файл
    if (!out_file) {
        cout << "Ошибка записи в файл" << endl;
    }
}


int main() {


	short some = 0x01;
	data_type integer(0x01, 4, 2,1,2,3);


	var_serialization(integer, "out.bin");
}