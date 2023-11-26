
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


using namespace std;

int main() {
    // 创建UDP套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    // 设置发送和接收缓冲区大小
    int sndbuf_size = 1024 * 1024; // 发送缓冲区大小1M
    int rcvbuf_size = 1024 * 1024; // 接收缓冲区大小1M
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&sndbuf_size, sizeof(sndbuf_size));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&rcvbuf_size, sizeof(rcvbuf_size));

    // 绑定本地套接字，用于接收行情数据
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(8000); // 目标端口号设置为8000
    inet_pton(AF_INET, "127.0.0.1", &local_addr.sin_addr); // 目标IP地址设置为本机地址
    if (bind(sockfd, (sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        cerr << "Failed to bind socket." << endl;
        return -1;
    }

    cout << "Waiting for new data..." << endl;
    // 接收行情数据，并输出到控制台上
    int count = 0;
    while (true) {
        char buffer[4096];
        sockaddr_in addr;
        int addr_len = sizeof(addr);
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&addr, (socklen_t*)&addr_len);
        printf("%d", len);
        if (len < 0) {
            cerr << "Failed to receive message." << endl;
            continue;
        }

        // buffer[len] = '\0';
        cout << "Received: " << buffer << ", count: " << count << endl;

        ++count;
    }

    close(sockfd);
    return 0;
}
