#include "client.h"

int main() {
    DataSender task;
    while (true) {
        try {
            std::thread thread_one(&DataSender::run_Stream1, std::ref(task));
            std::thread thread_two(&DataSender::run_Stream2, std::ref(task));

            thread_one.join();
            thread_two.join();
        } catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    return 0;
}
