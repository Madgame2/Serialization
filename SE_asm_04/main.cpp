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


void var_serialization(const data_type& var, const char* path) {
    ofstream out_file(path, ios::binary);

    if (!out_file.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    // Записываем данные в бинарном формате
    out_file.write(reinterpret_cast<const char*>(&var.type), sizeof(var.type));
    out_file.write(reinterpret_cast<const char*>(&var.length), sizeof(var.length));
    out_file.write(reinterpret_cast<const char*>(&var.value), sizeof(var.value));

    out_file.close();  // Закрываем файл
    if (!out_file) {
        cout << "Ошибка записи в файл" << endl;
    }
}


int main() {


	short some = 0x01;
	data_type integer(0x01, 4, 42);


	var_serialization(integer, "out.bin");
}