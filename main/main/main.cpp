#include <iostream>
#include <string>
using namespace std;

struct DES_interface {
    string origin;
    string ascii;
    string bin;
    string hex;
};

class DES {
    int num[8];
    int arr[8][8];
    
public:
    DES_interface plainText;
    DES() {
        while(1) {
            cout << "8자리 이하의 문자를 입력해주세요 : ";
            cin >> plainText.origin;
            if(plainText.origin.length() <= 8)
                break;
            
            cout << "8자리를 초과합니다. 다시 입력해주세요." << endl;
            continue;
        }
        for(int i=0; i<8; i++)
            num[i] = plainText.origin[i];
    }
    
    void ASCII() {
        string result;
        for(int i=0; i<8; i++)
            num[i] = plainText.origin[i];
        
        cout << "Plaintext (ASCII) \t: \t";
        for(int i=0; i<8; i++) {
            plainText.ascii += to_string((int)(num[i]));
            cout << (int) num[i];
        }
        printf("\n");
    }
    
    void Binary() {
        string result;
        int bin[64], p = 0;
        
        for(int i = 0; i < 8; i++) {
            int bit = 128, j = 0;
            while(bit > 0) {
                arr[i][j++] = (num[i] / bit) ? 1 : 0;
                if(bit != 1) num[i] %= bit;
                bit /= 2;
            }
        }
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                bin[j + p] = arr[i][j];
            p += 8;
        
        cout << "Plaintext (Binary) \t: \t";
        for(int i=0; i<64; i++) {
            printf("%d", bin[i]);
            plainText.bin += bin[i];
            if((i+1) % 8 == 0)
                printf(" ");
        }
        printf("\n");
    }
    
    void Hex() {
        int a[4];
        int k = 0;
        int num = 0;
        printf("Plaintext (Hex) \t: \t");
        for(int i=0; i< 16; i++) {
            for(int j=0; j<4; j++) {
                a[j] = plainText.bin[j + k];
                num = (a[0] * 8) + (a[1] * 4) + (a[2] * 2) + (a[3] * 1);
            }
            plainText.hex += num;
            printf("%X", num);
            k = k + 4;
        }
        printf("\n");
    }
};


int main() {
    DES des;
    des.ASCII();
    des.Binary();
    des.Hex();
    
    
    return 0;
}
