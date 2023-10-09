#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
using namespace std;

struct DES_interface {
    string origin;
    string ascii;
    int bin[64];
    string hex;
};

struct Divided {
    int L[8][4];
    int R[8][4];
};

class DES {
    int num[8];
    int arr[8][8];
    
    const string keyString = "133457799BBCDFF";
    const bitset<64> key = hexToBitset(keyString);
    // encrption
    int DES_IP_TABLE[64] =
    {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
    };
    // decrption
    int DES_IIP_TABLE[64] =
    {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };
    int P[32] =
    {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };
    int SBox_DES[8][4][16] =
    {
        /* S-Box [1] */
        { { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
        { 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
        { 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
        { 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } },
        /* S-Box [2] */
        { { 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
        { 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
        { 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
        { 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 } },
        /* S-Box [3] */
        { { 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
        { 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
        { 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
        { 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 } },
        /* S-Box [4] */
        { { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
        { 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
        { 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
        { 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 } },
        /* S-Box [5] */
        { { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
        { 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
        { 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
        { 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 } },
        /* S-Box [6] */
        { { 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
        { 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
        { 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
        { 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 } },
        /* S-Box [7] */
        { { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
        { 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
        { 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
        { 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 } },
        /* S-Box [8] */
        { { 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
        { 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
        { 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
        { 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 } }
    };
    int PC_1[64] =
    {
        /* Left Half */
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        /* Right Half */
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };
    int PC_2[48] =
    {
        14,17,11,24,1,5,3,28,
        15,6,21,10,23,19,12,4,
        26,8,16,7,27,20,13,2,
        41,52,31,37,47,55,30,40,
        51,45,33,48,44,49,39,56,
        34,53,46,42,50,36,29,32,
    };
    int shifts[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2 };
    
    bitset<64> hexToBitset(const string& hexString) {
        unsigned long long value = stoull(hexString, nullptr, 16);
        return bitset<64>(value);
    }
    
    void Ascii() {
        for(int i=0; i<8; i++) {
            this->num[i] = this->plainText.origin[i];
            this->plainText.ascii += to_string((int)(this->num[i]));
        }
        
        cout << "\tPlaintext (ASCII) \t: \t";
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
                bin[j + p] = this->arr[i][j];
                this->plainText.bin[j + p] = this->arr[i][j];
            }
            p += 8;
        }
        
        cout << "\tPlaintext (Binary) \t: \t";
        printBinary(bin);
        printf("\n");
    }
    
    void Hex() {
        int a[4];
        int k = 0, num = 0;
        
        for(int i=0; i<16; i++) {
            for(int j=0; j<4; j++) {
                a[j] = this->plainText.bin[j + k];
                num = (a[0] * 8) + (a[1] * 4) + (a[2] * 2) + (a[3] * 1);
            }
            this->plainText.hex += num;
            k = k + 4;
        }
        
        printf("\tPlaintext (Hex) \t: \t");
        printHex(this->plainText.bin);
        printf("\n");
    }
    
    // 1. Initial Permution
    int* InitialPermutaion(int* p) {
        int q[64];
        
        for(int i=0; i<64; i++)
            q[i] = p[i];
        for(int i=0; i<64; i++) {
            p[i] = q[this->DES_IP_TABLE[i] - 1];
        }
//        cout << "\tAfter initial permutation(Bin) \t: \t";
//        printBinary(p);
//        cout << "\n\tAfter initial permutation(Hex) \t: \t";
//        printHex(p);
//        printf("\n");
        
        return p;
    }
    
    int* InverseInitialPermutaion(int* p) {
        int q[64];
        
        for(int i=0; i<64; i++)
            q[i] = p[i];
        for(int i=0; i<64; i++) {
            p[i] = q[this->DES_IIP_TABLE[i] - 1];
        }
        
        return p;
    }
    
    
    // 2. Round
    Divided Divide_L_R(int* permuted_arr) {
        Divided result;
        int k = 0;
        for(int i=0; i<8; i++) {
            for(int j=0; j<4; j++) {
                result.L[i][j] = permuted_arr[j + k];
                result.R[i][j] = permuted_arr[j + 32 + k];
            }
            k += 4;
        }
        return result;
    }
    
    int* Combine_8_4bit_to_32bit(int arr[8][4]) {
        int k = 0;
        int* result = new int[32];
        for(int i=0; i<8; i++) {
            for(int j=0; j<4; j++)
                result[j] = arr[i][j];
            k += 4;
        }
        return result;
    }
    
    void Extend_32_to_48(int arr[8][4], int exArr[8][6]) {
        for(int i=0; i<8; i++) {
            exArr[i][5] = arr[(i + 1) % 8][0];
            exArr[i][0] = arr[(7 + i) % 8][3];
            
            for(int j=1; j<5; j++)
                exArr[i][j] = arr[i][j-1];
        }
    }
    
    int* Combine_8_6bit_to_48(int arr2[8][6]) {
        int k = 0;
        int* result = new int[48];
        
        for(int i=0; i<8; i++) {
            for(int j=0; j<6; j++)
                result[j + k] = arr2[i][j];
            k += 6;
        }
        return result;
    }
    
    int* XOR(int s, int* arr, int* arr2) {
        int* result = new int[s];
        for (int i = 0; i < s; i++) {
            result[i] = arr[i] ^ arr2[i];
        }
        return result;
    }


    void Divide_48bit_to_8_6(int arr[], int arr2[][6]) {
        int k = 0;
        
        for(int i=0; i<8; i++) {
            for(int j=0; j<6; j++)
                arr2[i][j] = arr[j + k];
        }
        k += 6;
    }
    
    void Key_Schedule(bitset<48>* subkey_arr, bool dec=false) {
        bitset<56> permutedKey;
        bitset<48> subkey;
        // Permutation Choice1
        for (int i =0; i <56; ++i) {
            permutedKey[i] = this->key[PC_1[i]-1];
        }
        
        // C, D 초기화
        bitset<28> C = (permutedKey >> 28).to_ullong();
        bitset<28> D = (permutedKey & bitset<56>(0x000000000FFFFFFF)).to_ullong();
        
        for(int round=0; round <16; ++round){
            int shiftAmount = (round < 16) ? this->shifts[round] : this->shifts[15];
            
            // shift
            C = (C << shiftAmount) | (C >> (28 - shiftAmount));
            D = (D << shiftAmount) | (D >> (28 - shiftAmount));
            
            // C, D 합치기
            bitset<56> CD;
            CD |= C.to_ullong();
            CD <<= 28;
            CD |= D.to_ullong();
            
            // Permutation Choice2
            for(int i=0; i<48; ++i)
                subkey[i] = CD[this->PC_2[i] - 1];
                
            subkey_arr[round] = subkey;
        }
    }
    
    void Sbox(int input[][6], int result[8][4]) {
        int num, j = 0, row = 0, col = 0;
        
        for(int i=0; i<8; i++) {
            row = (input[i][0] * 2) + (input[i][5] * 1);
            col = (input[i][1] * 8) + (input[i][2] * 4) + (input[i][3] * 2) + (input[i][4] * 1);
            num = this->SBox_DES[j][row][col];
            result[i][0] = (num / 8) ? 1 : 0;
            num %= 8;
            result[i][1] = (num / 4) ? 1 : 0;
            num %= 4;
            result[i][2] = (num / 2) ? 1 : 0;
            num %= 2;
            result[i][3] = (num / 1) ? 1 : 0;
            j++;
            if(i == 8) j = 0;
        }
    }
    
    int* Pbox(int input[8][4]) {
        int* result = new int[32];
        
        for (int i=0; i<8; ++i) {
            for (int j=0; j<4; ++j) {
                int bit_position = input[i][j];
                result[31 - (i * 4 + j)] = bit_position;
            }
        }
        return result;
    }
    
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
        for(int i=0; i<64; i++) {
            printf("%d", bin[i]);
            if((i+1) % 8 == 0)
                printf(" ");
        }
    }
    
    void printHex(int bin[]) {
        int a[4];
        int k = 0, num = 0;
        
        for(int i=0; i< 16; i++) {
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
    
    void init() {
        this->Ascii();
        this->Binary();
        this->Hex();
    }
    
    int* encryption() {
        int* chipertext = new int[64];
//        cout << "\n1. Initial Permution\n";
        int* permuted_arr = InitialPermutaion(this->plainText.bin);
        
        // Division
        Divided divided = Divide_L_R(permuted_arr);
//        cout << "\n2. Rounds\n";
        
        // Key Schedule
        bitset<48>* subKey_arr = new bitset<48>[16];
        Key_Schedule(subKey_arr);
        
        // Expansion Permutation
        int exArr_R[8][6];
        int* XOR_to_key[16];
        int* XOR_to_p[32];
        int* tempL = new int[32];
        int* tempR = new int[32];
        
        // Rounds
        for(int i=0; i<16; i++) {
            // Expansion Right Array for each round
            Extend_32_to_48(divided.R, exArr_R);
            int* arr48_R = Combine_8_6bit_to_48(exArr_R);

            // convert subkey to array
            int subkey_[48];
            for(int j=0; j<48; j++)
                subkey_[j] = subKey_arr[i][j];

            XOR_to_key[i] = XOR(48, arr48_R, subkey_);

            // S BOX (Subtitutaion)
            int XoR_8_6[8][6];
            Divide_48bit_to_8_6(XOR_to_key[i], XoR_8_6);
            int SBOX_arr[8][4];
            Sbox(XoR_8_6, SBOX_arr);

            // P BOX (Permutation)
            XOR_to_p[i] = Pbox(SBOX_arr);

            if(i < 15) {
                tempL = XOR(32, Combine_8_4bit_to_32bit(divided.L) ,XOR_to_p[i]);
                swap(divided.L ,divided.R);
           } else {
               tempR = XOR(32, Combine_8_4bit_to_32bit(divided.L) ,XOR_to_p[i]);
           }
       }
       for(int i=0; i<32; i++) {
          chipertext[i] = tempL[i];
          chipertext[i+32] = tempR[i];
       }

       return InverseInitialPermutaion(chipertext);
    }

    int* decryption(int* chipertext) {
        int* plaintext = new int[64];
        // Initial Permutation
        int* permuted_arr = InitialPermutaion(chipertext);
        
        // Division
        Divided divided = Divide_L_R(permuted_arr);
        
        // Key Schedule
        bitset<48>* subKey_arr = new bitset<48>[16];
        Key_Schedule(subKey_arr, true);
        
        // Expansion Permutation
        int exArr_R[8][6];
        int* XOR_to_key[16];
        int* XOR_to_p[32];
        int* tempL = new int[32];
        int* tempR = new int[32];
        
        // Rounds
        for(int i=0; i<16; i++) {
            // Expansion Right Array for each round
            Extend_32_to_48(divided.R, exArr_R);
            int* arr48_R = Combine_8_6bit_to_48(exArr_R);

            // convert subkey to array
            int subkey_[48];
            for(int j=0; j<48; j++)
                subkey_[j] = subKey_arr[i][j];

            XOR_to_key[i] = XOR(48, arr48_R, subkey_);

            // S BOX (Subtitutaion)
            int XoR_8_6[8][6];
            Divide_48bit_to_8_6(XOR_to_key[i], XoR_8_6);
            int SBOX_arr[8][4];
            Sbox(XoR_8_6, SBOX_arr);

            // P BOX (Permutation)
            XOR_to_p[i] = Pbox(SBOX_arr);

            if(i < 15) {
                tempL = XOR(32, Combine_8_4bit_to_32bit(divided.L) ,XOR_to_p[i]);
                swap(divided.L ,divided.R);
           } else {
               tempR = XOR(32, Combine_8_4bit_to_32bit(divided.L) ,XOR_to_p[i]);
           }
       }
       for(int i=0; i<32; i++) {
           plaintext[i] = tempL[i];
           plaintext[i+32] = tempR[i];
       }

        return InverseInitialPermutaion(plaintext);
    }
};

int main() {
    DES des;
    des.init();
    
    cout << "\n< Encryption >\n";
    int* chipertext = des.encryption();
    printf("\tChipertext (Binary) : \t");
    des.printBinary(chipertext);
    printf("\n\tChipertext (Hex) \t: \t");
    des.printHex(chipertext);
    cout << endl;
    
    cout << "\n< Decryption >\n";
    int* plaintext = des.decryption(chipertext);
    printf("\tPlaintext (Binary) \t: \t");
    des.printBinary(plaintext);
    printf("\n\tPlaintext (Hex) \t: \t");
    des.printHex(plaintext);
    
    cout << "\n\n";
    
    return 0;
}
