#ifndef ENCRYPTDECRYPT_H
#define ENCRYPTDECRYPT_H

#include <QString>

class EncryptDecrypt {
public:
    static QByteArray encryptAES(const QByteArray &plaintext, QByteArray &fullCiphertext);
    static QByteArray decryptAES(const QByteArray &fullCiphertext);
    static QString getMachineID();

private:
    static QByteArray generate20CharCode(const QByteArray &data);
};

#endif // ENCRYPTDECRYPT_H
