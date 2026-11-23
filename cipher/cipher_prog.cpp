#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cryptopp/camellia.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Использование: " << argv[0] 
             << " <enc|dec> <входной_файл> <выходной_файл> <пароль>" << endl;
        return 1;
    }

    string mode     = argv[1];
    string inFile   = argv[2];
    string outFile  = argv[3];
    string password = argv[4];

    try {
        if (mode == "enc") {
            CryptoPP::SecByteBlock key(CryptoPP::Camellia::MAX_KEYLENGTH);
            CryptoPP::SHA256().CalculateDigest(key, (const CryptoPP::byte*)password.data(), password.size());

            CryptoPP::SecByteBlock iv(CryptoPP::Camellia::BLOCKSIZE);
            for (size_t i = 0; i < iv.size(); i++) {
                iv[i] = 0x00;
            }

            ofstream outStream(outFile, ios::binary);
            outStream.write((char*)iv.data(), iv.size());

            CryptoPP::CBC_Mode<CryptoPP::Camellia>::Encryption enc;
            enc.SetKeyWithIV(key, key.size(), iv);

            CryptoPP::FileSource(inFile.c_str(), true, 
                new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::FileSink(outStream)));

            cout << "Файл был успешно зашифрован: " << outFile << endl;

        } else if (mode == "dec") {
            CryptoPP::SecByteBlock key(CryptoPP::Camellia::MAX_KEYLENGTH);
            CryptoPP::SHA256().CalculateDigest(key, (const CryptoPP::byte*)password.data(), password.size());

            ifstream inStream(inFile, ios::binary);
            CryptoPP::SecByteBlock iv(CryptoPP::Camellia::BLOCKSIZE);
            inStream.read((char*)iv.data(), iv.size());

            CryptoPP::CBC_Mode<CryptoPP::Camellia>::Decryption dec;
            dec.SetKeyWithIV(key, key.size(), iv);

            CryptoPP::FileSource(inStream, true, 
                new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::FileSink(outFile.c_str())));

            cout << "Файл был успешно расшифрован: " << outFile << endl;

        } else {
            cerr << "Ошибка: неверный режим. Используйте 'enc' или 'dec'." << endl;
            return 1;
        }
    } catch (const CryptoPP::Exception& e) {
        cerr << "Ошибка Crypto++: " << e.what() << endl;
        return 1;
    }

    return 0;
}