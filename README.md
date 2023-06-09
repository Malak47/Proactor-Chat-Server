# Proactor Chat Server

This application implements a **multithreading chat server** using the **proactor design pattern**.

The **server** creates a new thread for each incoming message to handle sending messages to all other users in the chat.
After sending a message to all chat users, the new thread will terminate.

The **server** supports an **unlimited number of clients** by dynamically storing file descriptors and handlers.

---

## About the Code

This project consists of two main components: the `Proactor` and the `main server application`.

1. **Proactor:** The Proactor class is a mechanism that receives a file descriptor (fd) from a client and a function to
   perform when the fd is active (has data to read). The Proactor supports a large number of fds and spawns a new thread
   for each message received.

2. **Server Application:** This is where the Proactor is used. The server application creates a socket to listen for
   incoming client connections. Each time a connection is accepted, it is added to the Proactor with a handler function.
   This handler function is responsible for receiving data from the client and broadcasting it to all other clients.

---

## How to Compile

You can compile the code using the provided makefile. Use the `make all` command to compile the `server application` and
the `Proactor library`, and `make clean` command to remove the compiled files.

```shell
make all
```

---

## How to Run

To run the `server`:

- Use the command `./st_proactor` to run the server with a default port 9034.

   ```shell
   $ ./st_proactor
   Listening on port: [9034] ...
   ```

- Use the command `./st_proactor <port>` to run the server with a specify port.
   ```shell
   $ ./st_proactor 8080
   Listening on port: [8080] ...
   ```

To connect as a `client`:

- use `telnet <hostname> <port>` from another terminal window.

  If you are running the client on the same machine as the server, you
  can use `localhost` as the hostname.

   ```shell
   $ telnet localhost 8080
   Trying ::1...
   Trying 127.0.0.1...
   Connected to localhost.
   Escape character is '^]'.
   ```

---

## ScreenShot

### Default Port:

![default port.png](Screenshots%2Fdefault%20port.png)

### Specific Port:

![given port.png](Screenshots%2Fgiven%20port.png)
