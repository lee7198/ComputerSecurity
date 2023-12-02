
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <string>

#include <unistd.h>

#include <iostream>
#include <sys/socket.h>
#include <sstream>

#include "des.h"

#define PORT 5000

using namespace std;

int main(void) {
    /* unsigned long */
    DWORD mData[8] = { 0, };
    DWORD mRoundKey[32] = { 0, };

    /* Key 생성 */ 
    KeySchedule("hello_keimyung", mRoundKey);

    /* 암호화할 내용 입력 */
    cout << "● 암호화될 내용을 입력 → ";
    fscanf(stdin, "%s", mData);

    /* Encryption */
    Cryption_EnDe(1, mData, mRoundKey);
    cout << endl << "========== Encryption ==========" << endl;
    printf("● 암호화 된 내용 : %s \n", mData);

    /* Decryption */
    Cryption_EnDe(2, mData, mRoundKey);
    cout << endl << "========== Decryption ==========" << endl;
    printf("● 복호화 된 내용 : %s \n", mData);
    // printf("hello");

    return 0;
}
