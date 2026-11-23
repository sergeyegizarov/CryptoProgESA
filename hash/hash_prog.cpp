#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Использование: " << argv[0] << " <имя_файла>" << endl;
        return 1;
    }

    string filename = argv[1];
    // Открываем файл в бинарном режиме
    ifstream file(filename, ios::binary); 
    if (!file) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return 1;
    }

    SHA384 hash;
    byte digest[SHA384::DIGESTSIZE];
    byte buffer[4096];

    // Читаем файл частями (для поддержки больших файлов)
    while (file.read((char*)buffer, sizeof(buffer))) {
        hash.Update(buffer, file.gcount());
    }
    // Обрабатываем остаток файла
    if (file.gcount() > 0) {
        hash.Update(buffer, file.gcount());
    }
    hash.Final(digest);

    // Преобразуем бинарный хэш в hex-строку
    string hexDigest;
    HexEncoder encoder(new StringSink(hexDigest));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    cout << "SHA-384: " << hexDigest << endl;

    return 0;
}
