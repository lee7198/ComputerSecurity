#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sstream>
using namespace std;

#define BUF_SIZE 1024
#define SERVER_PORT 5208
#define IP "127.0.0.1"

struct MSG_DATA
{
    char* message;
    int n;
    int* e;
};


// RSA
int n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i;
string msg_rsa;
// 두 소수
int p = 4019;
int q = 977;

// socket
void send_msg(int sock);
void recv_msg(int sock);
int output(const char *arg,...);
int error_output(const char *arg,...);
void error_handling(const string &message);
// RSA
int gcd(int pr);
int cd(int x);
void ce();
string encrypt(string msg_);
string decrypt(int *en, int n, int e);

string name = "RSA_CHAT";
string msg;

int main(int argc,const char **argv,const char **envp){
    int sock;
    // sockaddr_in serv_addr{};
    struct sockaddr_in serv_addr;

    if (argc != 2){
        error_output("Usage : %s <Name> \n",argv[0]);
        exit(1);
    }

    // client name
    name = "["+string(argv[1])+"]";

    // sock=socket(PF_INET, SOCK_STREAM, 0);
    // 소켓 생성, TCP 프로토콜 사용
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1){
        error_handling("socket() failed!");
    }

    // 소켓과 지정된 IP, 포트 바인딩
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    // 서버 연결
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() failed!");
    }
    // serverd
    string my_name = "#new client:" + string(argv[1]);
    send(sock, my_name.c_str(), my_name.length() + 1, 0);
    
    // 메시지를 보내고 받는 스레드 생성
    thread snd(send_msg, sock);
    thread rcv(recv_msg, sock);
    
    snd.join();
    rcv.join();
    
    close(sock);

    return 0;
}

void send_msg(int sock){
    while(1){
        getline(cin, msg);
        if (msg == "Quit"|| msg == "quit"){
            close(sock);
            exit(0);
        }

        // RSA
        for (i = 0; msg[i] != '\0'; i++)
            m[i] = msg[i];

        // 소수 찾기
        n = p * q;
        t = (p - 1) * (q - 1);
        // key 생성
        ce();


        MSG_DATA msg_data;

        // [name] massage 형식
        string name_msg = name + " " + encrypt(msg);

        ostringstream oss;
        oss << name_msg << "&" << n << "[";
        for (int k: e) {
            oss << k << ",";
        }
        oss << "]";

        send(sock, oss.str().c_str(), oss.str().length() + 1, 0);
    }
}

void recv_msg(int sock){
    char name_msg[BUF_SIZE + name.length() + 1];
    while (1){
        int str_len = recv(sock, name_msg, BUF_SIZE+name.length() + 1, 0);
        if (str_len == -1){
            exit(-1);
        }
        cout<<string(name_msg)<<endl;
    }
}

int output(const char *arg, ...){
    int res;
    va_list ap;
    va_start(ap, arg);
    res = vfprintf(stdout, arg, ap);
    va_end(ap);
    return res;
}

int error_output(const char *arg, ...){
    int res;
    va_list ap;
    va_start(ap, arg);
    res = vfprintf(stderr, arg, ap);
    va_end(ap);
    return res;
}

void error_handling(const string &message){
    cerr<<message<<endl;
    exit(1);
}

// RSA
int gcd(int pr) {
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
int cd(int x) {
    int k = 1;
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
    int pt, ct, key = e[0], k, len;
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

string decrypt(int *en, int n, int *d) {
    int pt, ct, key = d[0], k;
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