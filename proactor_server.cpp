//
// Created by Lara Abu Hamad on 21/05/2023.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "st_proactor.hpp"
#include <unistd.h>  // for close()
#include <set>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    // Default port
    int port = 9034;

    // If the user provided a port, use it
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    // create a socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listener, (struct sockaddr *) &addr, sizeof(addr));
    listen(listener, 10);

    // create a proactor
    auto proactor = st_proactor::createProactor();
    cout << "Listening on port: [" << port << "] ..." << endl;

    // Define a container to hold all clients.
    set<int> clients;

    // define a handler
    handler_t handler = [&proactor, &clients](int fd) {
        char buf[1024];
        int bytesReceived = recv(fd, buf, sizeof(buf) - 1, 0);
        if (bytesReceived > 0) {
            buf[bytesReceived] = '\0';  // Ensure null-terminated
            cout << "[Client " << fd << "] message: " << buf;

            // Prepare the message to be forwarded
            string forwardMsg = "[Client " + to_string(fd) + "] message: " + string(buf);

            // Forward the message to all clients
            for (int clientFd: clients) {
                if (clientFd != fd) {  // Don't send the message to the sender
                    send(clientFd, forwardMsg.c_str(), forwardMsg.length(), 0);
                }
            }
        } else if (bytesReceived == 0) {
            cout << "Client " << fd << " disconnected." << endl;
            clients.erase(fd);  // Remove the client from our list
            proactor->removeHandler(fd);  // Remove the handler from the proactor
            close(fd);  // Close the client socket
        } else {
            cerr << "Error occurred during recv from client " << fd << endl;
        }
    };


    // define a listener handler to accept new connections
    handler_t listenerHandler = [&proactor, &handler, &clients](int listener) {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(listener, (struct sockaddr *) &clientAddr, &addrLen);
        if (clientFd >= 0) {
            clients.insert(clientFd);  // Add the new client to our list
            proactor->addFD2Proactor(clientFd, handler);
        }
    };

    // add the listener to the proactor with the listener handler
    proactor->addFD2Proactor(listener, listenerHandler);

    // run the proactor
    proactor->runProactor();

    // wait for the proactor
    proactor->waitFor();

    return 0;
}
