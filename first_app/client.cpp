#include "client.h"

void DataParser::convert_to_KB(std::string &str) {
    for (size_t i{0}; i < str.size(); ++i) {
        if (std::isdigit(str.at(i)) && !(str.at(i) % 2)) {
            str.erase(i, 1);
            str.insert(i, "KB");
        }
    }
}

void DataSender::launchSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(1337);
}

void DataSender::run_Stream1() {
    std::string str;
    std::cout << "Enter string: " << std::endl;
    std::getline(std::cin, str);

    auto isDigit = [](const auto& i) {return std::isdigit(i);};
    while ((str.size() > 64) || (!std::all_of(str.begin(), str.end(), isDigit))) {
        std::cout << "The entered string is incorrect. The string must consist only of numbers and be of size <= 64.\nEnter the string again: \n";
        std::getline(std::cin, str);
    } 
    
    std::sort(str.begin(), str.end(), std::greater<char>());
    convert_to_KB(str);
    std::unique_lock<std::mutex> ul(mu_);
    buffer_ = str;
    isReady_ = true;
    cond_.notify_one();
}

void DataSender::run_Stream2() {
    std::unique_lock<std::mutex> ul(mu_);
    cond_.wait(ul, [this]() { return isReady_; });
    isReady_ = false;
    unsigned sum = 0;
    std::cout << buffer_ << std::endl;
    for (auto const &i : buffer_) {
        if (std::isdigit(i)) {
            sum += i - '0';
        }
    }
    buffer_ = "";
    std::cout << "Sum: " << sum << std::endl;
    ul.unlock();
    launchSocket();
    int res = sendto(sockfd, &sum, sizeof(int),
            MSG_CONFIRM, (const struct sockaddr *) &address,
            sizeof(address));
    
    if (res == -1) {
        perror("sendto failed with error\n");
        exit(EXIT_FAILURE);
    }
}