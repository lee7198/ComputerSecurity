#include<iostream>
#include<math.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

long int n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i;
string msg;
// 두 소수
long int p = 4019;
long int q = 977;


int gcd(long int pr) {
    int i;
    j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

// 공개키와 개인키를 생성
long int cd(long int x) {
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

// enc key 생성
void ce() {
    int k;
    k = 0;
    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        flag = gcd(i);
        if (flag == 1 && i != p && i != q)
        {
            e[k] = i;
            flag = cd(e[k]);
            if (flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if (k == 99)
                break;
        }
    }
}

string encrypt(string msg) {
    long int pt, ct, key = e[0], k, len;
    string result;
    i = 0;
    len = msg.length();
    
    while (i != len)
    {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    for (i = 0; en[i] != -1; i++)
        result += en[i];

    return result;
}

string decrypt(long int *en, long int n, long int *d) {
    long int pt, ct, key = d[0], k;
    string result;
    i = 0;
    while (en[i] != -1)
    {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;
    for (i = 0; m[i] != -1; i++)
        result += m[i];

    return result;
}

int main() {
    cout << "\nENTER MESSAGE\n";
    // fflush(stdin);
    // cin >> msg;
    msg = "hello world";

    for (i = 0; msg[i] != '\0'; i++)
        m[i] = msg[i];
    
    // clac
    n = p * q;
    t = (p - 1) * (q - 1);
    ce();

    cout << "\nPOSSIBLE VALUES OF e AND d ARE\n";
    for (i = 0; i < j - 1; i++)
        cout << e[i] << "\t" << d[i] << endl;

    cout << "\nTHE ENCRYPTED MESSAGE" << endl;
    cout << encrypt(msg) << endl;

    cout << "\nTHE DECRYPTED MESSAGE" << endl;
    cout << decrypt(en, n, d) << endl;

    return 0;
}
