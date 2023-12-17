#include <iostream>
#include <cstring>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <stdarg.h>
#include <map>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#define SERVER_PORT 5208 
#define BUF_SIZE 8192
#define MAX_CLNT 8

struct RSA_DATA {
    unsigned char ciphertext[256];
    int ciphertext_len;
    RSA *rsa_keypair;
    std::string msg;
    std::string pri;
    std::string pub;
};

void handle_clnt(int clnt_sock);
void send_msg(RSA_DATA data);
int output(const char *arg,...);
int error_output(const char *arg,...);
void error_handling(const std::string &message);

// 현재 사용자 수
int clnt_cnt = 0;
// 동시성 제어를 위한 뮤텍스
std::mutex mtx; 
// 각 클라이언트의 이름과 소켓을 저장하는 unordered_map
std::unordered_map<std::string, int>clnt_socks;
// map<std::string, RSA_BOX> clnt_data;

int main(int argc,const char **argv,const char **envp) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    // AF_INET: IPv4 사용
    // SOCK_STREAM: 연결 지향 데이터 전송
    // IPPROTO_TCP: TCP 프로토콜 사용
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        error_handling("socket() failed!");
    }
    // 소켓을 지정된 IP 및 포트에 할당
    // serv_addr을 0으로 채웁니다 (sockaddr_in 구조)
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // serv_addr.sin_port=htons(atoi(argv[1]));
    serv_addr.sin_port = htons(SERVER_PORT);

    // bind
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() failed!");
    }
    printf("the server is running on port %d\n", SERVER_PORT);
    // serv_sock을 수신 대기 상태로 전환하고 Client의 요청 대기
    if (listen(serv_sock, MAX_CLNT) == -1) {
        error_handling("listen() error!");
    }

    while(1) {
        // Client 요청 대기
        clnt_addr_size = sizeof(clnt_addr);
        // Client가 없을 때, accept()가 프로그램 실행을 차단합니다.
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1){
            error_handling("accept() failed!");
        }

        // Client 처리
        mtx.lock();
        clnt_cnt++;
        mtx.unlock();

        // 쓰레드 처리
        std::thread th(handle_clnt, clnt_sock);
        th.detach();

        output("Connected client IP: %s \n", inet_ntoa(clnt_addr.sin_addr));
    }
    close(serv_sock);
    return 0;
}

void handle_clnt(int clnt_sock) {
    char buffer[BUF_SIZE];
    int flag = 0;

    // client의 이름 prefix로 지정
    char tell_name[13] = "#new client:";
    while(recv(clnt_sock, buffer, sizeof(buffer), 0) != 0) {
        // 수시받은 데이터 처리
        RSA_DATA data;
        memcpy(&data, buffer, sizeof(data));

        char msg[BUF_SIZE];
        std::strcpy(msg, data.msg.c_str());

        // 채팅방에 처음 입장할 시 brodcast 확인
        if (std::strlen(msg) > std::strlen(tell_name)) {
            // msg에 '#new client:'가 있는지 확인
            char pre_name[13];
            std::strncpy(pre_name, msg, 12);
            pre_name[12] = '\0';
            if (std::strcmp(pre_name, tell_name) == 0) {
                // client 이름 선언
                char name[20];
                std::strcpy(name, msg+12);
                if(clnt_socks.find(name) == clnt_socks.end()) {
                    output("the name of socket %d: %s\n", clnt_sock, name);
                    clnt_socks[name] = clnt_sock;
                } else {
                    // 중복 Client 이름 처리
                    data.msg = std::string(name) + " 사용중인 이름입니다. 다른 이름을 선택해 주세요.";

                    // 구조체를 8비트 단위로 변환합니다.
                    char *buffer = new char[sizeof(data)];
                    memcpy(buffer, &data, sizeof(data));
                    send(clnt_sock, buffer, sizeof(data), 0);
                    // delete[] buffer;
                    mtx.lock();
                    clnt_cnt--;
                    mtx.unlock();
                    flag = 1;
                }
            }
        }

        if(flag == 0) {
            // 클라이언트들에게 메세지 전송
            std::cout << "암호문! " << data.pri << std::endl;
            send_msg(data);
        }
    }
    if(flag == 0) {
        // CLient가 연결을 끊고 clnt_socks에서 제거
        std::string leave_msg;
        std::string name;
        mtx.lock();
        for (auto it = clnt_socks.begin(); it != clnt_socks.end(); ++it ){
            if(it->second == clnt_sock){
                name = it->first;
                clnt_socks.erase(it->first);
            }
        }
        clnt_cnt--;
        mtx.unlock();
        RSA_DATA leave_data;
        leave_data.msg = name + " 가 채팅방을 나갔습니다.";
        // send_msg(leave_data);
        output("%s 가 채팅방을 나갔습니다.\n", name.c_str());
        close(clnt_sock);
    }
    else {
        close(clnt_sock);
    }
}

void send_msg(RSA_DATA data) {
    std::cout << "SEND" << std::endl;
    mtx.lock();
    // 실시간 채팅
    for (auto it = clnt_socks.begin(); it != clnt_socks.end(); it++) {
        // 구조체를 8비트 단위로 변환합니다.
        char *buffer = new char[sizeof(data)];
        memcpy(buffer, &data, sizeof(data));

        send(it->second, buffer, sizeof(data), 0);
        delete[] buffer;
    }
    mtx.unlock();
    std::cout << "SEND END" << std::endl;
}

int output(const char *arg, ...) {
    int res;
    va_list ap;
    va_start(ap, arg);
    res = vfprintf(stdout, arg, ap);
    va_end(ap);
    return res;
}

int error_output(const char *arg, ...) {
    int res;
    va_list ap;
    va_start(ap, arg);
    res = vfprintf(stderr, arg, ap);
    va_end(ap);
    return res;
}

void error_handling(const std::string &message) {
    std::cerr<<message<<std::endl;
    exit(1);
}