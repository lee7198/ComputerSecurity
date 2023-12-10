#include "des.h"

void InitialPermutation(DWORD * Data)
{
    /* 입력 된 64Bit를 32Bit 씩 나누는 배열 변수 */
    DWORD NewData[2] = { 0, 0 };
    /* NewData[1]에 31번째 인덱스에서 0번째 인덱스까지 데이터 저장 (32Bit) */
    for (int i = 31; i >= 0; i--)
    {
        DWORD tmpMask = 0x00000001;
        DWORD setMask = 0x00000001 << (31 - i);
        if ( (IP[i] <= 32) && (Data[1] & (tmpMask << (32 - IP[i]))) )
        {
            NewData[1] = NewData[1] | setMask;
        }
        else if ( (IP[i] > 32) && (Data[0] & (tmpMask << (64 - IP[i]))) )
        {
            NewData[1] = NewData[1] | setMask;
        }
    }
    /* NewData[0]에 63번째 인덱스에서 32번째 인덱스까지 데이터 저장 (32Bit) */
    for (int i = 63; i >= 32; i--)
    {
        DWORD tmpMask = 0x00000001;
        DWORD setMask = 0x00000001 << (63 - i);
        
        if ( (IP[i] <= 32) && (Data[1] & (tmpMask << (32 - IP[i]))) )
        {
            NewData[0] = NewData[0] | setMask;
        }
        else if ( (IP[i] > 32) && (Data[0] & (tmpMask << (64 - IP[i]))) )
        {
            NewData[0] = NewData[0] | setMask;
        }
    }
    Data[1] = NewData[1]; /* Left : 32비트 블록 */
    Data[0] = NewData[0]; /* Right : 32비트 블록 */
}
/* 변경 된 Bit를 IIP(Inverse Initial Permutation)를 하여 Bit 변경 */
void InverseInitialPermutation(DWORD * Data)
{
    /* 입력 된 64Bit를 32Bit 씩 나누는 배열 변수 */
    DWORD NewData[2] = { 0, 0 };
    /* NewData[1]에 31번째 인덱스에서 0번째 인덱스까지 데이터 저장 (32Bit) */
    for (int i = 31; i >= 0; i--)
    {
        DWORD tmpMask = 0x00000001; /* 0x00000001 = 1 */
        DWORD setMask = 0x00000001 << (31 - i);
        if ( (IIP[i] <= 32) && (Data[1] & (tmpMask << (32 - IIP[i]))) )
        {
            NewData[1] = NewData[1] | setMask;
        }
        else if ( (IIP[i] > 32) && (Data[0] & (tmpMask << (64 - IIP[i]))) )
        {
            NewData[1] = NewData[1] | setMask;
        }
    }
    /* NewData[0]에 63번째 인덱스에서 32번째 인덱스까지 데이터 저장 (32Bit) */
    for (int i = 63; i >= 32; i--)
    {
        DWORD tmpMask = 0x00000001; /* 0x00000001 = 1 */
        DWORD setMask = 0x00000001 << (63 - i);
        if ( (IIP[i] <= 32) && (Data[1] & (tmpMask << (32 - IIP[i]))) )
        {
            NewData[0] = NewData[0] | setMask;
        }
        else if ( (IIP[i] > 32) && (Data[0] & (tmpMask << (64 - IIP[i]))) )
        {
            NewData[0] = NewData[0] | setMask;
        }
    }
    Data[1] = NewData[1]; /* Left : 32비트 블록 */
    Data[0] = NewData[0]; /* Right : 32비트 블록 */
}

/* 확장 치환 (ExpansionPermutation) Right(RightHalf) 의 32Bit를 48Bit 확장  */
void ExpansionPermutation(DWORD RightHalf, DWORD * eWord)
{
    /*                ExpansionPermutation            */
    // ● 32 1 2 3 4 5 => 1 2 3 4 5 6                //
    // ● 4 5 6 7 8 9 => 7 8 9 10 11 12                //
    // ● 8 9 10 11 12 13 => 13 14 15 16 17 18        //
    // ● 12 13 14 15 16 17 => 19 20 21 22 23 24    //
    // ● 16 17 18 19 20 21 => 25 26 27 28 29 30    //
    // ● 20 21 22 23 24 25 => 31 32 33 34 35 36    //
    // ● 24 25 26 27 28 29 => 37 38 39 40 41 42    //
    // ● 28 29 30 31 32 1 => 43 44 45 46 47 48        //
    DWORD New[2] = { 0,0 };
    New[1] |= ((RightHalf & 0x00000001) << 23);
    New[1] |= (((RightHalf & 0xF8000000) >> 9) & 0x007C0000);
    New[1] |= ((RightHalf & 0x1F800000) >> 11);
    New[1] |= ((RightHalf & 0x01F80000) >> 13);
    New[1] |= ((RightHalf & 0x001F8000) >> 15);
    New[0] |= ((RightHalf & 0x0001F800) << 7);
    New[0] |= ((RightHalf & 0x00001F80) << 5);
    New[0] |= ((RightHalf & 0x000001F8) << 3);
    New[0] |= ((RightHalf & 0x0000001F) << 1);
    New[0] |= (((RightHalf & 0x80000000) >> 31) & 0x00000001);
    eWord[1] = New[1];
    eWord[0] = New[0];
}
/* 치환 함수 (PermutationFunction) : P(Permutation)을 이용하여 치환 */
DWORD PermutationFunction(DWORD dWord)
{
    DWORD tmpMask = 0x00000001;
    DWORD keyMask = 0x00000001;
    DWORD dwTmp = 0;
    for (int i = 0; i < 32; i++)
    {
        if ( dWord & (tmpMask << (32 - P[i])) )
            dwTmp |= (keyMask << (31 - i));
    }
    return dwTmp;
}
/* S-Box를 이용하여 6Bit 블록을 4Bit 블록으로 치환   */
DWORD SubstitutionChoice(DWORD * eWord)
{
    short row = 0; /* Row : 0번 Bit와 5번 Bit를 통하여 행의 번호 저장 변수 */
    short col = 0; /* Col : 1번 Bit ~ 4번 Bit를 통하여 열의 번호 저장 변수 */
    
    DWORD tmp0, tmp1;
    tmp0 = eWord[0];
    tmp1 = eWord[1];
    int shift;
    eWord[0] = 0;
    eWord[1] = 0;
    for (int i = 0; i < 4; i++)
    {
        shift = (4 * (3 - i));
        /* S-Box 6Bit Input */
        tmp1 <<= 6;
        row = ((tmp1 & 0x20000000) >> 28) | ((tmp1 & 0x01000000) >> 24);
        col = (tmp1 & 0x1E000000) >> 25;
        eWord[1] |= (SBox_DES[i][row][col] << shift);
        /* S-Box 4Bit Output */
        tmp0 <<= 6;
        row = ((tmp0 & 0x20000000) >> 28) | ((tmp0 & 0x01000000) >> 24);
        col = (tmp0 & 0x1E000000) >> 25;
        eWord[0] |= (SBox_DES[i + 4][row][col] << shift);
    }
    
    eWord[0] |= (eWord[1] << 16);
    return eWord[0];
}
/* RoundKey를 통하여 암호키 K로 부터 보조키열을 차래대로 개입  */
void Round(short mSwitch, DWORD * dWord, DWORD RoundKey1, DWORD RoundKey0)
{
    DWORD tmp[2] = { 0,0 };
    /* Right(RightHalf) 48Bit 확장 */
    mSwitch == 1 ? ExpansionPermutation(dWord[0], tmp) : ExpansionPermutation(dWord[1], tmp);
    /* Right(RightHalf)를 RoundKeyd와 XOR 연산  */
    tmp[1] = tmp[1] ^ RoundKey1; /* RoundKey1 XOR 연산 */
    tmp[0] = tmp[0] ^ RoundKey0; /* RoundKey0 XOR 연산 */
    /* RoundKeyd와 XOR 연산 된 값은 6Bit 단위로 8개씩 분할 (SBox_DES [8])  */
    tmp[0] = SubstitutionChoice(tmp);
    /* P-Box를 이용하여 치환 */
    tmp[0] = PermutationFunction(tmp[0]);
    /* dWord[1] : LeftHalf */
    /* dWord[0] : RightHalf */
    switch (mSwitch)
    {
        /* Encryption */
        case (1) :
        {
            /* RoundKeyd로 연산 된 RightHalf와 연산되지 않은 RightHalf XOR 연산  */
            tmp[0] = tmp[0] ^ dWord[1];
            dWord[1] = dWord[0]; /* LeftHalf에 RightHalf를 저장 */
            dWord[0] = tmp[0]; /* RightHalf에 RoundKeyd 연산 값 저장 */
            break;
        }
        /* Decryption */
        case (2) :
        {
            /* RoundKeyd로 연산 된 RightHalf와 연산되지 않은 RightHalf XOR 연산  */
            tmp[0] = tmp[0] ^ dWord[0];
            dWord[0] = dWord[1]; /* RightHalf에 LeftHalf를 저장 */
            dWord[1] = tmp[0]; /* LeftHalf에 RoundKeyd 연산 값 저장 */
            break;
        }
    }
}
/* PC_1 (Permuted choice 1)을 이용하여 64Bit Key를 (8, 16, 24, 32, 48, 56, 64) 위치를 삭제하여 56 Bit Key 생성 */
void PermutedChoice1(DWORD * Key)
{
    DWORD tmpMask;
    DWORD KeyMask;
    DWORD NewKey[2] = { 0,0 };
    tmpMask = 0x00000001;
    /* [8], [16], [24] Parity Bit 제거 후 28Bit Key 분할 */
    for (int i = 0; i < 28; i++)
    {
        KeyMask = 0x08000000 >> i;
        if ((PC_1[i] <= 32) && (Key[1] & (tmpMask << (32 - PC_1[i]))))
        {
            NewKey[1] = NewKey[1] | KeyMask;
        }
        else if ((PC_1[i] > 32) && (Key[0] & (tmpMask << (64 - PC_1[i]))))
        {
            NewKey[1] = NewKey[1] | KeyMask;
        }
    }
    /* [32], [48], [56], [64] Parity Bit 제거 후 28Bit Key 분할 */
    for (int i = 28; i < 56; i++)
    {
        KeyMask = 0x08000000 >> (i - 28);
        if ((PC_1[i] <= 32) && (Key[1] & (tmpMask << (32 - PC_1[i]))))
        {
            NewKey[0] = NewKey[0] | KeyMask;
        }
        else if ((PC_1[i] > 32) && (Key[0] & (tmpMask << (64 - PC_1[i]))))
        {
            NewKey[0] = NewKey[0] | KeyMask;
        }
    }
    Key[0] = NewKey[0];
    Key[1] = NewKey[1];
}
/* PC_2 (Permuted choice 2)를 이용하여 56Bit Key를 매 라운드마다 좌우측 순환시켜 48Bit 서브키 생성 */
void PermutedChoice2(DWORD *Key)
{
    DWORD tmpMask;
    DWORD KeyMask;
    DWORD NewKey[2] = { 0,0 };
    tmpMask = 0x00000001;
    /* [0] ~ [24]까지 24Bit 분할 */
    for (int i = 0; i < 24; i++)
    {
        KeyMask = 0x00800000 >> i;
        if ( (PC_2[i] <= 28) && (Key[1] & (tmpMask << (28 - PC_2[i]))) )
        {
            NewKey[1] = NewKey[1] | KeyMask;
        }
        else if ( (PC_2[i] > 28) && (Key[0] & (tmpMask << (56 - PC_2[i]))) )
        {
            NewKey[1] = NewKey[1] | KeyMask;
        }
    }
    /* [24] ~ [48]까지 24Bit 분할 */
    for (int i = 24; i < 48; i++)
    {
        KeyMask = 0x00800000 >> (i - 24);
        if ( (PC_2[i] <= 28) && (Key[1] & (tmpMask << (28 - PC_2[i]))) )
        {
            NewKey[0] = NewKey[0] | KeyMask;
        }
        else if ( (PC_2[i] > 28) && (Key[0] & (tmpMask << (56 - PC_2[i]))) )
        {
            NewKey[0] = NewKey[0] | KeyMask;
        }
    }
    Key[0] = NewKey[0];
    Key[1] = NewKey[1];
}
/* PC_1(Permuted choice 1)와 PC_2(Permuted choice 2)를 이용하여 16개의 보조키 생성 */
void KeySchedule(char * Key, DWORD * RoundKey)
{
    int SHIFT = 0;
    DWORD dwKey[2] = { 0,0 };
    DWORD tmp0, tmp1;
    for (int i = 0; i < 4; i++)
    {
        SHIFT = ((3 - i) * 8);
        dwKey[1] ^= ((DWORD)Key[i]) << SHIFT;
        dwKey[0] ^= ((DWORD)Key[i + 4]) << SHIFT;
    }
    /* Parity Bit 제거 후 56Bit로 변환 */
    PermutedChoice1(dwKey);
    /* 16 Round Key 생성 */
    for (int i = 0; i < 16; i++)
    {
        /* 왼쪽 순환 쉬프트 */
        if (LSHIFT[i] == 1)
        {
            tmp0 = ((dwKey[0] & 0x08000000) >> 27);
            tmp0 |= ((dwKey[0] & 0x07FFFFFF) << 1);
            dwKey[0] = tmp0;
            tmp1 = ((dwKey[1] & 0x08000000) >> 27);
            tmp1 |= ((dwKey[1] & 0x07FFFFFF) << 1);
            dwKey[1] = tmp1;
        }
        /* 2Bit 순환 쉬프트 */
        else
        {
            tmp0 = ((dwKey[0] & 0x0c000000) >> 26);
            tmp0 |= ((dwKey[0] & 0x03FFFFFF) << 2);
            dwKey[0] = tmp0;
            tmp1 = ((dwKey[1] & 0x0c000000) >> 26);
            tmp1 |= ((dwKey[1] & 0x03FFFFFF) << 2);
            dwKey[1] = tmp1;
        }
        /* 48Bit 서비스 변환 */
        PermutedChoice2(dwKey);
        /* 매 라운드마다 쉬프트 된 Key값을 대입 */
        RoundKey[2 * i + 1] = dwKey[1];
        RoundKey[2 * i] = dwKey[0];
        dwKey[1] = tmp1;
        dwKey[0] = tmp0;
    }
}
/* En/De cryption 함수 */
void Cryption_EnDe(short mSwitch, DWORD * data, DWORD * RoundKey)
{
    InitialPermutation(data);
    /* En/De Switch */
    switch (mSwitch)
    {
        /* 단일 비트(One bit) Shift Round : 1, 2, 9, 16 */
        /* 이중 비트(Two bits) Shift Round : 0, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15 */
        /* Encryption */ case (1) : { for (int round = 0; round < 16; round++) { Round(mSwitch, data, RoundKey[2 * round + 1], RoundKey[2 * round]); } break; }
        /* Decryption */ case (2) : { for (int round = 15; round >= 0; round--) { Round(mSwitch, data, RoundKey[2 * round + 1], RoundKey[2 * round]); } break; }
    } /* Key-Schedule 보조키 생성하는 Round Rotations을 16번 수행해야 하기 때문에 암/복호화시 16번 수행시킨다.  */
    InverseInitialPermutation(data);
}
