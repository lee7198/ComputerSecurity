#include <iostream>
#include <string>
using namespace std;

struct DES_interface {
    string plaintext;
    string ascii;
};

class DES {
    int num[8];
    int arr[8][8];
    
public:
    DES_interface text;
    DES() {
        while(1) {
            cout << "8자리 이하의 문자를 입력해주세요 : ";
            cin >> text.plaintext;
            if(text.plaintext.length() <= 8)
                break;
            
            cout << "8자리를 초과합니다. 다시 입력해주세요." << endl;
            continue;
        }
        for(int i=0; i<8; i++)
            num[i] = text.plaintext[i];
    }
    
    void ASCII() {
        string result;
        for(int i=0; i<8; i++)
            num[i] = text.plaintext[i];
        
        // print ASCII
        cout << "Plaintext (ASCII) : ";
        for(int i=0; i<8; i++) {
            text.ascii += to_string((int)(num[i]));
            cout << (int) num[i];
        }
        printf("\n");
    }
    
    string Binary() {
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
        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                bin[j + p] = arr[i][j];
            }
            p += 8;
        }
        // print Binary
        cout << "Plaintext (Binary) : ";
        for(int i=0; i<64; i++) {
            printf("%d", bin[i]);
            if((i+1) % 8 == 0)
                printf(" ");
        }
        return "";
    }
};


int main() {
    DES des;
    des.ASCII();
    des.Binary();
    
    
    return 0;
}

//void Print_Hex(int w, int Arr[]) {
//int i, j, num; int k = 0; int a[4];
//for (i = 0; i < w / 4; i++) {
//for (j = 0; j < 4; j++) {
//a[j] = Arr[j + k];
//num = (a[0] * 8) + (a[1] * 4) + (a[2] * 2) + (a[3] * 1);
//} printf("%X", num); k = k + 4;
//} printf("\n");
//}
