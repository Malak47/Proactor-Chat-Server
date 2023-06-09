//
// Created by malaklinux on 6/9/23.
//

#ifndef PROACTOR_CHAT_SERVER_ST_PROACTOR_HPP
#define PROACTOR_CHAT_SERVER_ST_PROACTOR_HPP

#include <thread>
#include <map>
#include <vector>
#include <poll.h>
#include <functional>
#include <stdexcept>
#include <system_error>
#include <memory>


using namespace std;

typedef function<void(int)> handler_t;

class st_proactor {
private:
    std::thread *proactor_thread;
    bool active;
    std::map<int, handler_t> handlers;

public:
    st_proactor();

    ~st_proactor();

    static shared_ptr<st_proactor> createProactor();

    int runProactor();

    int cancelProactor();

    int addFD2Proactor(int fd, handler_t handler);

    int removeHandler(int fd);

    void waitFor();
};

#endif //PROACTOR_CHAT_SERVER_ST_PROACTOR_HPP
