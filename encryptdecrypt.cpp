#include "encryptdecrypt.h"

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <QNetworkInterface>

QByteArray EncryptDecrypt::encryptAES(const QByteArray &plaintext, QByteArray &fullCiphertext)
{
    QByteArray key = "12345678901234567890123456789012", iv = "1234567890123456";
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QByteArray();

    int outlen, ciphertext_len;
    QByteArray ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH, Qt::Uninitialized);

    // AES-256-CBC encryption
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), (unsigned char*)iv.data());
    EVP_EncryptUpdate(ctx, (unsigned char*)ciphertext.data(), &outlen, (unsigned char*)plaintext.data(), plaintext.size());
    ciphertext_len = outlen;

    EVP_EncryptFinal_ex(ctx, (unsigned char*)ciphertext.data() + outlen, &outlen);
    ciphertext_len += outlen;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);

    fullCiphertext = ciphertext.toBase64();  // Store full encrypted data

    return generate20CharCode(fullCiphertext);  // Generate 20-char uppercase code
}

QByteArray EncryptDecrypt::decryptAES(const QByteArray &fullCiphertext)
{
    QByteArray key = "12345678901234567890123456789012", iv = "1234567890123456";
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QByteArray();

    int outlen, plaintext_len;
    QByteArray decodedCiphertext = QByteArray::fromBase64(fullCiphertext);
    QByteArray plaintext(decodedCiphertext.size(), Qt::Uninitialized);

    // AES-256-CBC decryption
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), (unsigned char*)iv.data());
    EVP_DecryptUpdate(ctx, (unsigned char*)plaintext.data(), &outlen, (unsigned char*)decodedCiphertext.data(), decodedCiphertext.size());
    plaintext_len = outlen;

    EVP_DecryptFinal_ex(ctx, (unsigned char*)plaintext.data() + outlen, &outlen);
    plaintext_len += outlen;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return plaintext;
}

QByteArray EncryptDecrypt::generate20CharCode(const QByteArray &data)
{
    QByteArray hash(SHA256_DIGEST_LENGTH, Qt::Uninitialized);
    SHA256((unsigned char*)data.data(), data.size(), (unsigned char*)hash.data());

    QByteArray base64Hash = hash.toBase64();  // Convert hash to Base64
    QByteArray transformed;

    // Extract only uppercase letters & numbers
    for (char c : base64Hash) {
        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            transformed.append(c);
        } else if (c >= 'a' && c <= 'z') {
            transformed.append(c - 32);  // Convert lowercase to uppercase
        }
        if (transformed.size() == 20) break;
    }

    while (transformed.size() < 20) {
        transformed.append('X'); // Padding if needed
    }

    return transformed;
}

QString EncryptDecrypt::getMachineID()
{
    QString rawText = "";
    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces())
    {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) && !netInterface.hardwareAddress().isEmpty())
        {
            rawText = QString(netInterface.hardwareAddress());
        }
    }

    QByteArray inputBytes = rawText.toUtf8();

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    EVP_Digest(inputBytes.data(), inputBytes.size(), hash, &hashLen, EVP_sha256(), nullptr);
    QByteArray hashedData(reinterpret_cast<const char*>(hash), 16); // Use first 16 bytes

    QString hexString = hashedData.toHex().toUpper().left(16); // Ensure exactly 16 characters
    QString formatted;
    for (int i = 0; i < hexString.length(); ++i) {
        if (i > 0 && i % 4 == 0) {
            formatted.append('-');
        }
        formatted.append(hexString[i]);
    }

    return formatted;
}
