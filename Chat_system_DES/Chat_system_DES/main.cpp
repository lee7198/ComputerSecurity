#include<openssl/rsa.h>

#include<openssl/evp.h>

#include<openssl/pem.h>

#include <string.h>

 

int encryptUCHAR(unsigned char* key, unsigned char* text, int textLen, unsigned char* encryptedChar)

{

    EVP_CIPHER_CTX* ctx;

 

    int len;

    int tempLen;

    //unsigned char key[] = "Test_Secret_Key";

    unsigned char iv[] = "Test_iv";

 

    if (!(ctx = EVP_CIPHER_CTX_new()))

    {

        printf("ENCRYPT FAILED"); return 0;

    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL /*iv*/))

    {

        printf("ENCRYPT FAILED"); return 0;

    }

    if (1 != EVP_EncryptUpdate(ctx, encryptedChar, &tempLen, text, textLen))

    {

        printf("ENCRYPT FAILED"); return 0;

    }

    len = tempLen;

 

    if (1 != EVP_EncryptFinal_ex(ctx, encryptedChar + len, &tempLen))

    {

        printf("ENCRYPT FAILED"); return 0;

    }

    len += tempLen;

 

    EVP_CIPHER_CTX_free(ctx);

 

    return len;

}

 

int decryptUCHAR(unsigned char* key, unsigned char* encryptedChar, int encryptedLen, unsigned char* decrypedChar)

{

    EVP_CIPHER_CTX* ctx;

    int len;

    int tempLen;

 

    if (!(ctx = EVP_CIPHER_CTX_new()))

    {

        printf("DECRYPT FAILED"); return -1;

    }

    int result;

    //unsigned char key[] = "Test_Secret_Key";

    unsigned char iv[] = "Test_iv";

    result = EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL/*iv*/);

    result = EVP_DecryptUpdate(ctx, decrypedChar, &tempLen, encryptedChar, encryptedLen);

    len = tempLen;

 

    result = EVP_DecryptFinal_ex(ctx, decrypedChar + len, &tempLen);

    len += tempLen;

 

    EVP_CIPHER_CTX_free(ctx);

 

    return len;

}

 

int encryptRsaWithPublicKey(unsigned char* inData, int inLen, unsigned char* outData)

{

    char publicKeyText[] =

       "-----BEGIN PUBLIC KEY-----\n"

  "MIIBITANBgkqhkiG9w0BAQEFAAOCAQ4AMIIBCQKCAQB384AQg2Kk30nIjVK64xtl\n"

  "xAgFsdKz4ZZmbZFMH0k798ITQ5WHaiNwd72i21yolEUoOcKuDl4GwdgUGdy7Hx9Z\n"

  "KHpf/vt/RHxEYyqp2fhIxYo3O+US3w2qYqVYvHlmUql0cGSPq1hx64jdprZ+ifFZ\n"

  "Xfh+/o8EqFv1wtY65vDNA8Ecn/CGNuIVRZoffvQSDFup+a4HA1FPhbvY/bzfycxP\n"

  "EudoJ241bzw3tJ6ss3sdCkYGEedv6yI59ilBkPn7IznL6N7uPMujt9NGmU1AeY8I\n"

  "A39rRtPy7tTTgn5AUKLUTBjGfmTDsd7K0GnG3cSjSu+YKpLHO4uZMRL1pWJN8lhH\n"

  "AgMBAAE=\n"

  "-----END PUBLIC KEY-----\n";

 

    BIO* bio = BIO_new_mem_buf(publicKeyText, -1);

    RSA* rsaPublicKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);

    BIO_free(bio);

 

    int len = RSA_public_encrypt(inLen, (const unsigned char*)inData, outData, rsaPublicKey, RSA_PKCS1_OAEP_PADDING);

    RSA_free(rsaPublicKey);

 

    return len;

}

 

int decryptRsaWithPrivateKey(unsigned char* inData, int inLen, unsigned char* outData)

{

    char privateKeyText[] =

     "-----BEGIN RSA PRIVATE KEY-----\n"

  "MIIEoQIBAAKCAQB384AQg2Kk30nIjVK64xtlxAgFsdKz4ZZmbZFMH0k798ITQ5WH\n"

  "aiNwd72i21yolEUoOcKuDl4GwdgUGdy7Hx9ZKHpf/vt/RHxEYyqp2fhIxYo3O+US\n"

  "3w2qYqVYvHlmUql0cGSPq1hx64jdprZ+ifFZXfh+/o8EqFv1wtY65vDNA8Ecn/CG\n"

  "NuIVRZoffvQSDFup+a4HA1FPhbvY/bzfycxPEudoJ241bzw3tJ6ss3sdCkYGEedv\n"

  "6yI59ilBkPn7IznL6N7uPMujt9NGmU1AeY8IA39rRtPy7tTTgn5AUKLUTBjGfmTD\n"

  "sd7K0GnG3cSjSu+YKpLHO4uZMRL1pWJN8lhHAgMBAAECggEAR07KYwa4LatQAZmC\n"

  "BPfd4bGX00rTJ3yEHIAmUEqJDs1K4oja65V9W+7THW2qt8ztD+zGfxKrcvjpq4l0\n"

  "XwkxaZ9WoJrEh/P+q+kIA9FAeIJ2mD9S/tT7Gr9p7EafJOUfN8lY/s+t1Itg4eOp\n"

  "ua/nemkSwOwzuWEcygz5rip8HGO6ZkLCQlpbXR6VMDt40k24N+vYMT6Ix3SxM27C\n"

  "cknKKOAx1ATCsHJ88oXqDNOK7V/UlT1a2bfWXOcNnpuUPOgt4Otngc7hVG6DBWrB\n"

  "o+/IxD/2p1A8Rzar3+VWfP3n8ibv3vhgb+Xy6JwpGdA4Kg/pr8vIPANnNgY+oBf4\n"

  "tvxEAQKBgQDWQ3vYWldM2RRG0DSrlWsDEZjJZRdbfdq8uqGQG2jmcOmJw60ts0aJ\n"

  "Xqw9AkpeyQD+rqUJ+nEZrDml4Xp+JaWsTrnJnPLAWdi4FcpsINJOUoLQmpzC5AOx\n"

  "LppOGs6iyrvjjWYaHb78X0WGT0KcvnNyk2N0VRYXix1MM8OXqnxyZwKBgQCPUQMG\n"

  "ym/XUKDbXDedcwUnWw8ZN+cp0hroJWd9m5yILSdnLpRi2lT6GGjPuO9Qrb/h9aNt\n"

  "wrttB7VLgyRXt6f0q5eH04H0WQMDTK7fWE+X1o/18VDI40TMPC+jCsZo2uQyPY+6\n"

  "I/Y687JNZwn0GcakchSnmAXkYXb96SiX+JX/IQKBgADUliBKiVgU50029V1M4PNM\n"

  "GCl/qM2D+VsN5h5m3Ms32M/d4EfzD+zsXwKFhcBESyqxhpY5UELnNfn8m7rtbmzy\n"

  "iSnM3+v67SoZe4VbwzTmgPdKAh2HQLRpF9QBqpda7ct9+s1THDBiNiTFHd75pfzl\n"

  "sVb8FW/V7DElajamRIE1AoGAYLaotjJyWvj8NEv88/GkrrRSchA5nQ0+Px7OwgL7\n"

  "FSJzjrvuQu9XnRZP1K3WQbme3ZtoWC+rT55+B6FOKyuy2ffZ4D0pW32d/OIIuIvd\n"

  "2nde2veoLoi/6ptLAlnJHwVoLUdB8LlH9uxxjK5WFzY1yM97hMWgZEgphpuLW1LT\n"

  "1kECgYBoZhHqn3TD98/JDTzMVV7N6ziHlx8ELOy6g5IrruI94qgVxw+Iaovla9Ez\n"

  "m+/jEO5aMiN0xb6mcSWcU/a7l4kjUyu6lopCmylMLMqlNwYf8DfOeKdj0Lv0RMsc\n"

  "s5Ld0WguFNxUxAc+XNfQZgcJdKGQ4V/v0VoXFwPbjoyjj1Ok7A==\n"

  "-----END RSA PRIVATE KEY-----\n";

 

    BIO* bio = BIO_new_mem_buf(privateKeyText, -1);

    RSA* rsaPrivateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

    BIO_free(bio);

 

    int len = RSA_private_decrypt(inLen, (const unsigned char*)inData, outData, rsaPrivateKey, RSA_PKCS1_OAEP_PADDING);

 

    return len;

}

 

int decryptRsaWithPublicKey(unsigned char* inData, int inLen, unsigned char* outData)

{

    char publicKeyText[] =

       "-----BEGIN PUBLIC KEY-----\n"

  "MIIBITANBgkqhkiG9w0BAQEFAAOCAQ4AMIIBCQKCAQB384AQg2Kk30nIjVK64xtl\n"

  "xAgFsdKz4ZZmbZFMH0k798ITQ5WHaiNwd72i21yolEUoOcKuDl4GwdgUGdy7Hx9Z\n"

  "KHpf/vt/RHxEYyqp2fhIxYo3O+US3w2qYqVYvHlmUql0cGSPq1hx64jdprZ+ifFZ\n"

  "Xfh+/o8EqFv1wtY65vDNA8Ecn/CGNuIVRZoffvQSDFup+a4HA1FPhbvY/bzfycxP\n"

  "EudoJ241bzw3tJ6ss3sdCkYGEedv6yI59ilBkPn7IznL6N7uPMujt9NGmU1AeY8I\n"

  "A39rRtPy7tTTgn5AUKLUTBjGfmTDsd7K0GnG3cSjSu+YKpLHO4uZMRL1pWJN8lhH\n"

  "AgMBAAE=\n"

  "-----END PUBLIC KEY-----\n";

 

    BIO* bio = BIO_new_mem_buf(publicKeyText, -1);

    RSA* rsaPublicKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);

    BIO_free(bio);

 

    //unsigned char encrypted[512];

    int len = RSA_public_decrypt(inLen, (const unsigned char*)inData, outData, rsaPublicKey, RSA_PKCS1_PADDING);

    RSA_free(rsaPublicKey);

 

    return len;

}

 

int encryptRsaWithPrivateKey(unsigned char* inData, int inLen, unsigned char* outData)

{

    char privateKeyText[] =

     "-----BEGIN RSA PRIVATE KEY-----\n"

  "MIIEoQIBAAKCAQB384AQg2Kk30nIjVK64xtlxAgFsdKz4ZZmbZFMH0k798ITQ5WH\n"

  "aiNwd72i21yolEUoOcKuDl4GwdgUGdy7Hx9ZKHpf/vt/RHxEYyqp2fhIxYo3O+US\n"

  "3w2qYqVYvHlmUql0cGSPq1hx64jdprZ+ifFZXfh+/o8EqFv1wtY65vDNA8Ecn/CG\n"

  "NuIVRZoffvQSDFup+a4HA1FPhbvY/bzfycxPEudoJ241bzw3tJ6ss3sdCkYGEedv\n"

  "6yI59ilBkPn7IznL6N7uPMujt9NGmU1AeY8IA39rRtPy7tTTgn5AUKLUTBjGfmTD\n"

  "sd7K0GnG3cSjSu+YKpLHO4uZMRL1pWJN8lhHAgMBAAECggEAR07KYwa4LatQAZmC\n"

  "BPfd4bGX00rTJ3yEHIAmUEqJDs1K4oja65V9W+7THW2qt8ztD+zGfxKrcvjpq4l0\n"

  "XwkxaZ9WoJrEh/P+q+kIA9FAeIJ2mD9S/tT7Gr9p7EafJOUfN8lY/s+t1Itg4eOp\n"

  "ua/nemkSwOwzuWEcygz5rip8HGO6ZkLCQlpbXR6VMDt40k24N+vYMT6Ix3SxM27C\n"

  "cknKKOAx1ATCsHJ88oXqDNOK7V/UlT1a2bfWXOcNnpuUPOgt4Otngc7hVG6DBWrB\n"

  "o+/IxD/2p1A8Rzar3+VWfP3n8ibv3vhgb+Xy6JwpGdA4Kg/pr8vIPANnNgY+oBf4\n"

  "tvxEAQKBgQDWQ3vYWldM2RRG0DSrlWsDEZjJZRdbfdq8uqGQG2jmcOmJw60ts0aJ\n"

  "Xqw9AkpeyQD+rqUJ+nEZrDml4Xp+JaWsTrnJnPLAWdi4FcpsINJOUoLQmpzC5AOx\n"

  "LppOGs6iyrvjjWYaHb78X0WGT0KcvnNyk2N0VRYXix1MM8OXqnxyZwKBgQCPUQMG\n"

  "ym/XUKDbXDedcwUnWw8ZN+cp0hroJWd9m5yILSdnLpRi2lT6GGjPuO9Qrb/h9aNt\n"

  "wrttB7VLgyRXt6f0q5eH04H0WQMDTK7fWE+X1o/18VDI40TMPC+jCsZo2uQyPY+6\n"

  "I/Y687JNZwn0GcakchSnmAXkYXb96SiX+JX/IQKBgADUliBKiVgU50029V1M4PNM\n"

  "GCl/qM2D+VsN5h5m3Ms32M/d4EfzD+zsXwKFhcBESyqxhpY5UELnNfn8m7rtbmzy\n"

  "iSnM3+v67SoZe4VbwzTmgPdKAh2HQLRpF9QBqpda7ct9+s1THDBiNiTFHd75pfzl\n"

  "sVb8FW/V7DElajamRIE1AoGAYLaotjJyWvj8NEv88/GkrrRSchA5nQ0+Px7OwgL7\n"

  "FSJzjrvuQu9XnRZP1K3WQbme3ZtoWC+rT55+B6FOKyuy2ffZ4D0pW32d/OIIuIvd\n"

  "2nde2veoLoi/6ptLAlnJHwVoLUdB8LlH9uxxjK5WFzY1yM97hMWgZEgphpuLW1LT\n"

  "1kECgYBoZhHqn3TD98/JDTzMVV7N6ziHlx8ELOy6g5IrruI94qgVxw+Iaovla9Ez\n"

  "m+/jEO5aMiN0xb6mcSWcU/a7l4kjUyu6lopCmylMLMqlNwYf8DfOeKdj0Lv0RMsc\n"

  "s5Ld0WguFNxUxAc+XNfQZgcJdKGQ4V/v0VoXFwPbjoyjj1Ok7A==\n"

  "-----END RSA PRIVATE KEY-----\n";

 

    BIO* bio = BIO_new_mem_buf(privateKeyText, -1);

    RSA* rsaPrivateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

    BIO_free(bio);

 

    int len = RSA_private_encrypt(inLen, (const unsigned char*)inData, outData, rsaPrivateKey, RSA_PKCS1_PADDING);

 

    return len;

}

 

void RSA_Test()

{

    RSA* key;

    unsigned char cipher_text[256];

    unsigned char plain_text_sender[] = "RSA Test 1";

    unsigned char plain_text_receiver[256];

    unsigned int num;

    BIGNUM* bn;

    bn = BN_new();

    BN_set_word(bn, 3);

 

    key = RSA_new();

    int res = RSA_generate_key_ex(key, 1024, bn, NULL);

    if (RSA_check_key(key) == 1)

        printf("validate key\n");

 

    printf("plaintext=%s\n", plain_text_sender);

    num = RSA_public_encrypt(sizeof(plain_text_sender) - 1, plain_text_sender, cipher_text, key, RSA_PKCS1_PADDING);

    for (int i = 0; i < 256; i++)

        printf("%x", cipher_text[i]);

    printf("\n");

 

    num = RSA_private_decrypt(num, cipher_text, plain_text_receiver, key, RSA_PKCS1_PADDING);

    plain_text_receiver[num] = '\0';

    printf("plaintext=%s\n", plain_text_receiver);

 

}

 

void EncDecTest()

{

    char testData[] = "This is a test!";

    char encBuf[100] = { 0, };

    char decBuf[100] = { 0, };

 

    printf("Org : %s\n", testData);

    unsigned char key[] = "This_Is_A_Secret_Key_Test(32ch)_"; // 32 characters

 

    int len = encryptUCHAR(key, (unsigned char *)testData, strlen(testData), (unsigned char*)encBuf);

 

    for (int i = 0; i < len; i++)

        printf("%x", encBuf[i]);

    printf("\n");

 

    len = decryptUCHAR(key, (unsigned char *)encBuf, len, (unsigned char*)decBuf);

    decBuf[len] = 0;

    printf("dec : %d - %s\n", len, decBuf);

 

}

 

void RSA_Test2()

{

    unsigned char encData[512] = { 0, };

    char decData[512] = { 0, };

    char InText[] = "RSA Test 2";

 

    printf("RSA TEST2 : %s\n", InText);

    int len = encryptRsaWithPrivateKey((unsigned char *)InText, strlen(InText), encData);

    for (int i = 0; i < len; i++)

        printf("%x", encData[i]);

    printf("\n");

 

    len = decryptRsaWithPublicKey(encData, len, (unsigned char *)decData);

    printf("dec : %s\n", decData);

}

 

int main()

{

    RSA_Test();

 

    EncDecTest();

 

    RSA_Test2();

    return 1;

}
