#ifndef des_h
#define des_h

#include <stdio.h>
#include <iostream>
#include <string.h>

/*
* AND (Logical Conjunction) : 모든 논리 변수가 1일 때만 1이 되고, 어느 한 변수라도 0이면 그 결과는 0이다.
* OR (Logical Disjunction) : 두 명제의 논리합은 문장의 의미가 통하든 통하지 않든  나  중 어느 하나가 참이면 참인 명제이다.
* XOR (Exclusive OR) : 수리 논리학에서 주어진 2개의 명제 가운데 1개만 참일 경우를 판단하는 논리 연산이다.
*/
/* Typedef  */
typedef unsigned long DWORD;
/* Initial Permutation Map */
static const unsigned char IP[64] =
{
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
}; /* 순열 맵을 통해서 평문을 다시 재배열시 사용한다. */
/* Inverse Initial Permutation Map */
static const unsigned char IIP[64] =
{
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
}; /* 역 순열 맵을 통해서 배열 된 맵을 다시 재배열시 사용한다. */
/* Permutation (P-box시 사용) */
static const unsigned char P[32] =
{
    16,  7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26,  5, 18, 31, 10,
    2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
}; /* 32 Bit Block을 P 순열 맵으로 배열시 사용한다.  */
/* Substitution (S-box시 사용) */
static const unsigned char SBox_DES[8][4][16] =
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
}; /* ExpansionPermutation 함수에서 확장 된 Right(RightHalf) 48Bit로 바뀐 수열은 각각 6비트씩 8토막으로 나뉜 다음, S-box 치환을 거칠 때 사용한다. */
/* Permuted choice 1 */
static const unsigned char PC_1[64] =
{
    /* Left Half */
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    /* Right Half */
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};
/* Left와 Right Half부분을 입력 된 Key 값을 보여진 테이블 값을 적용하며 64Bit 중
   8BIt인 (8, 16, 24, 32, 40, 48, 56, 64)는 Parity Bits로 사용한다.
*/
/* Permuted choice 2 */
static const unsigned char PC_2[48] =
{
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32,
}; /* Parity Bit가 제거 된 56Bit Key를 매 라운드마다 48Bit 보조키로 선택 시 사용한다. */
/* LSHIFT(Rotations in the key-schedule) */
static const unsigned char LSHIFT[] =
{
    1,1,2,2,
    2,2,2,2,
    1,2,2,2,
    2,2,2,1
}; /* Permuted Choice 2 수행하기 전 56Bit를 28Bit 씩 2개로 나누어 좌우측이동을 시행 시 사용한다.  */
/* 입력 Bit를 IP(Initial Permutation)를 하여 Bit 순서 변경 */

/* init */
void InitialPermutation(DWORD * Data);
/* 변경 된 Bit를 IIP(Inverse Initial Permutation)를 하여 Bit 변경 */
void ExpansionPermutation(DWORD RightHalf, DWORD * eWord);
/* 치환 함수 (PermutationFunction) : P(Permutation)을 이용하여 치환 */
DWORD PermutationFunction(DWORD dWord);
/* S-Box를 이용하여 6Bit 블록을 4Bit 블록으로 치환   */
DWORD SubstitutionChoice(DWORD * eWord);
/* RoundKey를 통하여 암호키 K로 부터 보조키열을 차래대로 개입  */
void Round(short mSwitch, DWORD * dWord, DWORD RoundKey1, DWORD RoundKey0);
/* PC_1 (Permuted choice 1)을 이용하여 64Bit Key를 (8, 16, 24, 32, 48, 56, 64) 위치를 삭제하여 56 Bit Key 생성 */
void PermutedChoice1(DWORD * Key);
/* PC_2 (Permuted choice 2)를 이용하여 56Bit Key를 매 라운드마다 좌우측 순환시켜 48Bit 서브키 생성 */
void PermutedChoice2(DWORD *Key);
/* PC_1(Permuted choice 1)와 PC_2(Permuted choice 2)를 이용하여 16개의 보조키 생성 */
void KeySchedule(char * Key, DWORD * RoundKey);
/* En/De cryption 함수 */
void Cryption_EnDe(short mSwitch, DWORD * data, DWORD * RoundKey);

#endif /* des_h */
