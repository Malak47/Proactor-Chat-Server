//
// Created by malaklinux on 6/9/23.
//

#include "st_proactor.hpp"

st_proactor::st_proactor() : proactor_thread(nullptr), active(false) {}

st_proactor::~st_proactor() {
    cancelProactor();
}

 shared_ptr<st_proactor> st_proactor::createProactor() {
    shared_ptr<st_proactor> proactor = make_shared<st_proactor>();
    return proactor;
}


int st_proactor::runProactor() {
    if (active) {
        return -1;  // error, already running
    }

    active = true;
    proactor_thread = new thread([this]() {
        while (active) {
            vector<pollfd> fds;
            for (const auto &handler: handlers) {
                fds.push_back({handler.first, POLLIN, 0});
            }
            int num_events = poll(fds.data(), fds.size(), 1000); // timeout is 1000ms
            if (num_events < 0) {
                throw system_error(errno, generic_category(), "poll failed");
            }
            if (num_events > 0) {
                for (auto &fd: fds) {
                    if (fd.revents & POLLIN) {
                        thread handler_thread(handlers[fd.fd], fd.fd);
                        handler_thread.detach();
                    }
                }
            }
        }
    });

    return 0;
}

int st_proactor::cancelProactor() {
    if (!active) {
        return -1;  // error, not running
    }

    active = false;
    if (proactor_thread && proactor_thread->joinable()) {
        proactor_thread->join();
        delete proactor_thread;
        proactor_thread = nullptr;
    }
    return 0;
}

int st_proactor::addFD2Proactor(int fd, handler_t handler) {
    if (handlers.count(fd)) {
        return -1;  // error, fd already exists
    }

    handlers[fd] = handler;
    return 0;
}

int st_proactor::removeHandler(int fd) {
    if (!handlers.count(fd)) {
        return -1;  // error, fd does not exist
    }

    handlers.erase(fd);
    return 0;
}

void st_proactor::waitFor() {
    if (proactor_thread && proactor_thread->joinable()) {
        proactor_thread->join();
    }
}


