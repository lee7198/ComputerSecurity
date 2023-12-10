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

#define SERVER_PORT 5208 
#define BUF_SIZE 1024
#define MAX_CLNT 32   

void handle_clnt(int clnt_sock);
void send_msg(const std::string &msg);
int output(const char *arg,...);
int error_output(const char *arg,...);
void error_handling(const std::string &message);

// 현재 사용자 수
int clnt_cnt = 0;
// 동시성 제어를 위한 뮤텍스
std::mutex mtx; 
// 각 클라이언트의 이름과 소켓을 저장하는 unordered_map
std::unordered_map<std::string, int>clnt_socks;

int main(int argc,const char **argv,const char **envp){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    // AF_INET: IPv4 사용
    // SOCK_STREAM: 연결 지향 데이터 전송
    // IPPROTO_TCP: TCP 프로토콜 사용
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1){
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
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind() failed!");
    }
    printf("the server is running on port %d\n", SERVER_PORT);
    // serv_sock을 수신 대기 상태로 전환하고 Client의 요청 대기
    if (listen(serv_sock, MAX_CLNT) == -1){
        error_handling("listen() error!");
    }

    while(1){   
        // Client 요청 대기
        clnt_addr_size = sizeof(clnt_addr);
        // When no client connects, accept() blocks program execution until a client connects.
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

void handle_clnt(int clnt_sock){
    char msg[BUF_SIZE];
    int flag = 0;

    // Client의 이름 prefix로 지정
    char tell_name[13] = "#new client:";
    while(recv(clnt_sock, msg, sizeof(msg),0) != 0){
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
                if(clnt_socks.find(name) == clnt_socks.end()){
                    output("the name of socket %d: %s\n", clnt_sock, name);
                    clnt_socks[name] = clnt_sock;
                }
                else {
                    // 중복 Client 이름 처리
                    std::string error_msg = std::string(name) + " 사용중인 이름입니다. 다른 이름을 선택해 주세요.";
                    send(clnt_sock, error_msg.c_str(), error_msg.length()+1, 0);
                    mtx.lock();
                    clnt_cnt--;
                    mtx.unlock();
                    flag = 1;
                }
            }
        }

        if(flag == 0)
            send_msg(std::string(msg));
    }
    if(flag == 0){
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
        leave_msg = name + " 가 채팅방을 나갔습니다.";
        send_msg(leave_msg);
        output("%s 가 채팅방을 나갔습니다.\n", name.c_str());
        close(clnt_sock);
    }
    else {
        close(clnt_sock);
    }
}

void send_msg(const std::string &msg){
    mtx.lock();
    // 비공개 채팅 메세지 형식: [send_clnt] @recv_clnt message
    // @를 확인하여 비공개 처리를 합니다.
    std::string pre = "@";
    int first_space = msg.find_first_of(" ");
    if (msg.compare(first_space+1, 1, pre) == 0){
        // 유니캐스팅
        // recv_clnt와 msg 사이의 공백 처리
        int space = msg.find_first_of(" ", first_space+1);
        std::string receive_name = msg.substr(first_space+2, space-first_space-2);
        std::string send_name = msg.substr(1, first_space-2);
        if(clnt_socks.find(receive_name) == clnt_socks.end()) {
            // 비공개 사용자가 존재하지 않는다면
            std::string error_msg = "[error] 사용자가 존재하지 않습니다. " + receive_name;
            send(clnt_socks[send_name], error_msg.c_str(), error_msg.length()+1, 0);
        }
        else {
            std::cout << send_name << " -> " << receive_name << msg << std::endl;
            send(clnt_socks[receive_name], msg.c_str(), msg.length()+1, 0);
            send(clnt_socks[send_name], msg.c_str(), msg.length()+1, 0);
        }
    }
    else {
        // 실시간 채팅
        for (auto it = clnt_socks.begin(); it != clnt_socks.end(); it++) {
            send(it->second, msg.c_str(), msg.length()+1, 0);
        }
    }
    mtx.unlock();
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

void error_handling(const std::string &message){
    std::cerr<<message<<std::endl;
    exit(1);
}