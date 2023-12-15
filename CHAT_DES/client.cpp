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
#include <vector>
using namespace std;

#define BUF_SIZE 4096
#define SERVER_PORT 5208
#define IP "127.0.0.1"

// socket
void send_msg(int sock);
void recv_msg(int sock);
int output(const char *arg,...);
int error_output(const char *arg,...);
void error_handling(const string &message);
// RSA
int gcd(long int pr);
int cd(int long x);
void ce(long int t, long int flag);
string encrypt(string msg_);
string decrypt(vector<long int> en, long int n, vector<long int> d);
// convert
vector<long int> split(string str, char Delimiter);

struct RSA_BOX {
    long int en[100];
    long int n;
    long int d[100];
    string msg;
};

// RSA
long int n, t, flag, e[100], d[100], temp[100], m[100], en[100];
// 두 소수
long int p = 4019;
long int q = 977;

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
    RSA_BOX data;
    string my_name = "#new client:" + string(argv[1]);
    cout << data.msg << endl;
    // strcpy(data.msg, my_name.c_str());
    data.msg = my_name;

    // 구조체를 8비트 단위로 변환합니다.
    char *buffer = new char[sizeof(data)];
    memcpy(buffer, &data, sizeof(data));

    send(sock, buffer, sizeof(data), 0);
    
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
        for (int i = 0; msg[i] != '\0'; i++)
            m[i] = msg[i];

        // 소수 찾기
        n = p * q;
        t = (p - 1) * (q - 1);
        // key 생성
        ce(t, flag);
        // 암호화
        encrypt(msg);


        // [name] massage 형식
        string name_msg = name + " " + string(msg);

        RSA_BOX data;
        memcpy(&data.n, &n, sizeof(long));
        memcpy(&data.en, &en, sizeof(long)*100);
        memcpy(&data.d, &d, sizeof(long)*100);
        // strcpy(data.msg, name_msg);
        data.msg = name_msg;

        // 구조체를 8비트 단위로 변환합니다.
        char *buffer = new char[sizeof(data)];
        memcpy(buffer, &data, sizeof(data));

        ostringstream oss;
        oss << name_msg << "{";
        for (long int k: en)
            oss << k << ",";
        oss << "}";

        oss << "&" << n << "[";
        for (long int k: d)
            oss << k << ",";
        oss << "]";

        // send(sock, oss.str().c_str(), oss.str().length() + 1, 0);
        send(sock, buffer, sizeof(data), 0);
    }
}

void recv_msg(int sock){
    char buffer[BUF_SIZE];
    char name_msg[BUF_SIZE + name.length() + 1];
    while (1){
        int str_len = recv(sock, buffer, sizeof(buffer), 0);
        if (str_len == -1) exit(-1);
        
        string name_msg_ = string(name_msg);
        RSA_BOX data;
        // 수시받은 데이터 처리
        memcpy(&data, buffer, sizeof(data));

        cout << "DATA.N: " << data.n << endl;
        cout << "DATA.MSG: " << data.msg << endl;

        if(name_msg_.find("&") != string::npos) {
            // decrypt에 필요한 정보들
            int first_space = name_msg_.find_first_of(" ");
            int en_idx  = name_msg_.find_last_of("{");
            int key_idx  = name_msg_.find_last_of("&");
            int d_idx  = name_msg_.find_last_of("[");

            string sender = name_msg_.substr(0, first_space);
            string cypher_str = name_msg_.substr(en_idx+1, key_idx-en_idx-3);
            long int key = stoi(name_msg_.substr(key_idx+1, d_idx-key_idx-1));
            string d_array_str = name_msg_.substr(d_idx+1, name_msg_.length()-d_idx-3);

            vector<long int> d_array = split(d_array_str, ',');
            vector<long int> en_array = split(cypher_str, ',');
            cout << "d size: " << d_array.size() << endl;
            cout << "en size: " << en_array.size() << endl;
            cout << key << endl;
            // decription a ciphertext
            cout << sender << " " << decrypt(en_array, key, d_array) << endl;
        } else {
            cout << string(name_msg) << endl;
        }

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
int gcd(long int pr) {
    int i;
    int j = sqrt(pr);
    for (i = 2; i <= j; i++) {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

// 공개키와 개인키를 생성
int cd(long int x) {
    long int k = 1;
    while (1) {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

// enc key 생성
void ce(long int t, long int flag) {
    int k;
    k = 0;
    for (int i = 2; i < t; i++) {
        if (t % i == 0)
            continue;
        flag = gcd(i);
        if (flag == 1 && i != p && i != q) {
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
    int i = 0;
    len = msg.length();
    
    while (i != len) {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for (int j = 0; j < key; j++) {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    for (int i = 0; en[i] != -1; i++)
        result += en[i];

    return result;
}

string decrypt(vector<long int> en, long int n, vector<long int> d) {
    long int pt, ct, key = d[0], k;
    string result;
    int i = 0;
    while (en[i] != -1) {
        ct = temp[i];
        k = 1;
        for (int j = 0; j < key; j++) {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;
    for (int i = 0; m[i] != -1; i++)
        result += m[i];

    return result;
}

vector<long int> split(string str, char Delimiter) {
    istringstream iss(str); 
    string buffer;
    vector<long int> result;
    
    while (getline(iss, buffer, Delimiter)) {    
        long int num = stoi(buffer);
        result.push_back(num);
    }

    return result;
}