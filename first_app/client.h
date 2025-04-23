#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class DataParser {
public:
    void convert_to_KB(std::string &str);
};

class DataSender : public DataParser {
private:
    std::string buffer_;
    std::mutex mu_;
    std::condition_variable cond_;
    bool isReady_ = false;
    int sockfd;
    struct sockaddr_in address;

    void launchSocket();

public:
    void run_Stream1();
    void run_Stream2();
};