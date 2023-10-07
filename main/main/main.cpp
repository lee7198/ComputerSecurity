#include <iostream>
#include <string>
using namespace std;

const int SIZE = 64;

struct DES_interface {
    string origin;
    string ascii;
    int bin[SIZE];
    string hex;
};

class DES {
    int num[8];
    int arr[8][8];
    int DES_IP_TABLE[SIZE] = {58,50,42,34,26,18,10,2,
                            60,52,44,36,28,20,12,4,
                            62,54,46,38,30,22,14,6,
                            64,56,48,40,32,24,16,8,
                            57,49,41,33,25,17,9,1,
                            59,51,43,35,27,19,11,3,
                            61,53,45,37,29,21,13,5,
                            63,55,47,39,31,23,15,7 };
    
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
    
    void printAscii(int num[]) {
        for(int i=0; i<8; i++)
            printf("%d", num[i]);
    }
    
    void printBinary(int bin[]) {
        for(int i=0; i<SIZE; i++) {
            printf("%d", bin[i]);
            if((i+1) % 8 == 0)
                printf(" ");
        }
    }
    
    void printHex(int bin[]) {
        int a[4];
        int k = 0, num = 0;
        
        for(int i=0; i< SIZE/4; i++) {
            for(int j=0; j<4; j++) {
                a[j] = bin[j + k];
                num = (a[0] * 8) + (a[1] * 4) + (a[2] * 2) + (a[3] * 1);
            }
            printf("%X", num);
            
            if((i+1) % 2 == 0)
                printf(" ");
            k = k + 4;
        }
    }
    
    void Ascii() {
        for(int i=0; i<8; i++) {
            num[i] = plainText.origin[i];
            plainText.ascii += to_string((int)(num[i]));
        }
        
        cout << "Plaintext (ASCII) \t: \t";
        printAscii(num);
        printf("\n");
    }
    
    void Binary() {
        int bin[64], p = 0;
        
        for(int i = 0; i < 8; i++) {
            int bit = 128, j = 0;
            while(bit > 0) {
                arr[i][j++] = (num[i] / bit) ? 1 : 0;
                if(bit != 1) num[i] %= bit;
                bit /= 2;
            }
        }
        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                bin[j + p] = arr[i][j];
                plainText.bin[j + p] = arr[i][j];
            }
            p += 8;
        }
        
        cout << "Plaintext (Binary) \t: \t";
        printBinary(bin);
        printf("\n");
    }
    
    void Hex() {
        int a[4];
        int k = 0, num = 0;
        
        for(int i=0; i< SIZE/4; i++) {
            for(int j=0; j<4; j++) {
                a[j] = plainText.bin[j + k];
                num = (a[0] * 8) + (a[1] * 4) + (a[2] * 2) + (a[3] * 1);
            }
            plainText.hex += num;
            k = k + 4;
        }
        
        printf("Plaintext (Hex) \t: \t");
        printHex(plainText.bin);
        printf("\n");
    }
    
    void init() {
        Ascii();
        Binary();
        Hex();
    }
    
    
    // 1. Initial Permution
    void Plaintext_AfterPermutaion() {
        int temp;
        int q[64];
        int *p = plainText.bin;
        
        for(int i=0; i<SIZE; i++)
            q[i] = p[i];
        for(int i=0; i<SIZE; i++) {
            temp = DES_IP_TABLE[i];
            p[i] = q[temp - 1];
        }
        
        cout << "\nAfter initial permutation(Bin) \t: \t";
        printBinary(p);
        cout << "\nAfter initial permutation(Hex) \t: \t";
        printHex(p);
        
        
        printf("\n");
    }
    
    // 2. round
    void round() {
        
    }
    
    void encryption() {
        Plaintext_AfterPermutaion();
    }
};


int main() {
    DES des;
    des.init();
    
    cout << "\n< Encryption >\n";
    cout << "\n1. Initial Permution\n";
    des.encryption();
    
    return 0;
}
