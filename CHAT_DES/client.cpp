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
#include <vector>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
using namespace std;

#define SERVER_PORT 5208
#define BUF_SIZE 8192
#define IP "127.0.0.1"

struct RSA_DATA {
    unsigned char ciphertext[256];
    int ciphertext_len;
    RSA *rsa_keypair;
    string msg;
    string pri;
    string pub;
};

// socket
void send_msg(int sock);
void recv_msg(int sock);
int output(const char *arg,...);
int error_output(const char *arg,...);
void error_handling(const string &message);
RSA_DATA encrpt(RSA *key, string msg);
void decrypt(RSA_DATA *data);
string get_pubkey(RSA *key);
string get_prikey(RSA *key);
void key_gen();

// socket
string name = "RSA_CHAT";
string msg;

// RSA
BIGNUM *e;
RSA *keypair;
string pub_key;
string pri_key;

int main(int argc,const char **argv,const char **envp){
    // RSA Key 생성
    key_gen();

    int sock;
    // sockaddr_in serv_addr{};
    struct sockaddr_in serv_addr;

    if(argc != 2){
        error_output("Usage : %s <Name> \n",argv[0]);
        exit(1);
    }

    // client name
    name = "["+string(argv[1])+"]";

    // sock=socket(PF_INET, SOCK_STREAM, 0);
    // 소켓 생성, TCP 프로토콜 사용
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1){
        error_handling("socket() failed!");
    }

    // 소켓과 지정된 IP, 포트 바인딩
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    // 서버 연결
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() failed!");
    }
    // serverd
    RSA_DATA data;
    string my_name = "#new client: " + string(argv[1]);
    cout << data.msg << endl;
    data.msg = my_name;

    // 구조체를 8비트 단위로 변환합니다.
    char *buffer = new char[sizeof(RSA_DATA)];
    memcpy(buffer, &data, sizeof(data));

    send(sock, buffer, sizeof(data), 0);

    delete[] buffer;
    
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
        if(msg == "Quit"|| msg == "quit"){
            close(sock);
            exit(0);
        }

        string name_msg = name + " " + string(msg);
        const char* plaintext = msg.c_str();

        // unsigned char ciphertext[256];
        // 암호화
        RSA_DATA data = encrpt(keypair, name_msg);

        // 구조체를 8비트 단위로 변환합니다.
        char *buffer = new char[sizeof(data)];
        memcpy(buffer, &data, sizeof(data));
        
        send(sock, buffer, sizeof(data), 0);

        delete[] buffer;
        RSA_free(keypair);
        BN_free(e);
    }
}

void recv_msg(int sock){
    while(1) {
        char buffer[BUF_SIZE];
        // err 처리
        if(recv(sock, buffer, sizeof(buffer), 0) == -1) 
            exit(-1);

        // 수시받은 데이터 처리
        RSA_DATA data;
        memcpy(&data, buffer, sizeof(data));

        // if(RSA_check_key(data.rsa_keypair) < 1) {
        //     printf("RSA 키페어 검증 오류\n");
        //     exit(0);
        // }

        // if(data.msg.find("#new client:") == string::npos) {
        //     cout << name << " : " << data.msg << endl;
        //     if(data.ciphertext_len > 0)
        //         decrypt(&data);
        // } else {
        //     // 초기 입장
        // }
        cout << data.msg << endl;
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

RSA_DATA encrpt(RSA *key, string msg) {
    unsigned char ciphertext[256];

    cout << "start encrpt" << endl;

    // 암호화
    int ciphertext_len = RSA_public_encrypt(msg.length()+1, (const unsigned char*)msg.c_str(), ciphertext, key, RSA_PKCS1_OAEP_PADDING);

    if(ciphertext_len == -1) {
        printf("RSA 암호화 오류\n");
        exit(1);
    }

    RSA_DATA data;
    memcpy(&data.ciphertext, &ciphertext, sizeof(unsigned char)*256);
    memcpy(&data.ciphertext_len, &ciphertext_len, sizeof(int));
    memcpy(&data.rsa_keypair, &key, sizeof(key));
    // data.rsa_keypair = key;
    data.pub = pub_key;
    data.pri = pri_key;
    data.msg = msg;

    cout << "암호문: " << data.ciphertext << endl;

    return data;
}

void decrypt(RSA_DATA *data) {
    unsigned char decryptedtext[256];
    
    // cout << "start decrypt" << endl;

    // // 복호화
    // int decryptedtext_len = RSA_private_decrypt(data->ciphertext_len-1, data->ciphertext, decryptedtext, data->rsa_keypair, RSA_PKCS1_OAEP_PADDING);

    // cout << decryptedtext_len << endl;

    // if(decryptedtext_len == -1) {
    //     printf("RSA 복호화 오류\n");
    //     exit(1);
    // }
    

    // // 평문 출력
    // printf("복호화 결과: %s\n", decryptedtext);


    // /////

    BIO *rpri =BIO_new_mem_buf(data->pri.c_str(),-1);
    BIO_write(rpri, data->pri.c_str(),(int)data->pri.length());

    RSA *rsa_prikey = NULL;
    if(!PEM_read_bio_RSAPrivateKey(rpri, &rsa_prikey, NULL, NULL)){
        printf("PEM_read_bio_RSAPrivateKey error\n");
        exit(-1);
    }

    int decrypt_len = -1;
    if((decrypt_len = RSA_private_decrypt(data->ciphertext_len, (unsigned char*)data->ciphertext, decryptedtext, rsa_prikey, RSA_PKCS1_OAEP_PADDING)) == -1) {
        printf("RSA_public_encrypt error!!\n");
        exit(-1);
    }
}

string get_pubkey(RSA *key) {
    const unsigned char *p;
    BIO *pub_bio = BIO_new(BIO_s_mem());
    size_t pub_len = BIO_pending(pub_bio);
    pub_key[pub_key.length()] = '\n';

    if(PEM_write_bio_RSA_PUBKEY(pub_bio, key) != 1 ) 
        throw "PEM_write_bio_RSAPublicKey fail";

    int pub_size = BIO_get_mem_data(pub_bio, &p);
    if( pub_size <= 0 ) 
        throw "Public size is zero";
    pub_size++; // include null

    char *pub_buffer = new char[pub_size];
    int pub_read_size = BIO_read(pub_bio, pub_buffer, pub_size-1);
    if( pub_read_size != pub_size-1 ) 
        throw "Public read fail";
    
    delete[] pub_buffer;

    return string(pub_buffer);
}

string get_prikey(RSA *key) {
    const unsigned char *p;
    BIO *pri_bio = BIO_new(BIO_s_mem());
    size_t pri_len = BIO_pending(pri_bio);
    pri_key[pri_key.length()] = '\n';

    if(PEM_write_bio_RSAPrivateKey(pri_bio, key, NULL, NULL, 0, NULL, NULL) != 1) 
        throw "PEM_write_bio_RSAPrivateKey fail";

    int pri_size = BIO_get_mem_data(pri_bio, &p);
    if( pri_size <= 0 ) 
        throw "Private size is zero";
    pri_size++; // include null

    char *pri_buffer = new char[pri_size];
    int pri_read_size = BIO_read(pri_bio, pri_buffer, pri_size-1);
    if( pri_read_size != pri_size-1 ) 
        throw "Private read fail";

    delete[] pri_buffer;

    return string(pri_buffer);
}

void key_gen() {
    cout << "KEY GEN" << endl;

    keypair = RSA_new();
    e = BN_new();
    BN_set_word(e, RSA_F4);

    // RSA 키페어 생성
    if(RSA_generate_key_ex(keypair, 2048, e, NULL) != 1) {
        printf("RSA 키페어 생성 오류\n");
        exit(1);
    }

    pub_key = get_pubkey(keypair);
    pri_key = get_prikey(keypair);
}