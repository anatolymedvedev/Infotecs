#include "server.h"

int main() {
    Server server;
    server.launchSocket();
    
    while (true) {
        try {
            server.RunReviewer();
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
        }
    }
    

    return 0;
}