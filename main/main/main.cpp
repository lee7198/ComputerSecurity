#include <iostream>
#include <string>
using namespace std;

void getASCII(int arr[][8], string plainText) {
    int num[8];

    cout << "convert ASCII";
    for(int i=0; i<8; i++) {
        num[i] = plainText[i];
    }
    
    // print ASCII
    cout << "Plaintext (ASC) : ";
    for(int i=0; i<8; i++) {
        printf("%d", num[i]);
    }

    for(int i = 0; i < 8; i++) {
        int bit = 128;
        while(bit != 1) {
            arr[i][0] = (num[i] / bit) ? 1 : 0;
            num[i] %= bit;
        }
    }
}

void des() {
    string plaintext;
    int arr[8][8];

    while(1) {
        cout << "8자리 이하의 문자를 입력해주세요 : ";
        cin >> plaintext;
        if(plaintext.length() <= 8) {
            cout << "OK";
            break;
        }
        cout << "8자리를 초과합니다. 다시 입력해주세요." << endl;
        continue;
    }
    getASCII(arr, plaintext);
}

int main() {
    des();
    
    return 0;
}
