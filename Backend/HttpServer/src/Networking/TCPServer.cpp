#include "TCPServer.h"

#include <vector>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "ServerExceptions.h"

namespace Willow{
    //Constructors / Destructors
    TCPServer::TCPServer()
        : m_IpAddress("localhost"), m_Port(0)
    {
        //By default the server is running
        this->m_IsRunning = true;
    }

    TCPServer::TCPServer(const char* ipAddress, int port)
        : m_IpAddress(ipAddress), m_Port(port)
    {
        //By default the server is running
        this->m_IsRunning = true;
    }

    TCPServer::~TCPServer()
    {
        //Clean the data used by the server

        //Close the server socket
        close(this->m_Socket);

        //Close the epoll descriptor
        close(this->m_EpollDescriptor);
    }

    //Protected functions
    void TCPServer::SendToClient(uint64_t clientSocket, std::string message){
        send(clientSocket, message.data(), message.size(), 0);
    }

    //Setters
    void TCPServer::SetPort(unsigned int port){
        if(port > 65000)
            throw ServerException("Cannot set the port bigger than 65000", 1);

        this->m_Port = port;
    }

    //Close the server
    void TCPServer::Close() {
        this->m_IsRunning = false;
        std::cout << "Server is stopping!\n";
    }

    //Public functions
    int TCPServer::init(){
        //Initializes the server to listen for connections

        //Address structure for the server
        struct sockaddr_in serv_addr;
        
        //Socket creation
        this->m_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if(this->m_Socket < 0){
            std::cout << "Cannot open socket!\n";
            return -1;
        }

        //Initialize the address structure with 0s
        memset(&serv_addr, 0x00, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(this->m_Port);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        //Bind the server address to the server socket
        if(bind(this->m_Socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
            return -1;
        }

        //Listen for maximum of 10 connections
        int res = listen(this->m_Socket, 10);
        if(res < 0)
            return -1;

        //Create the epoll file descriptor
        this->m_EpollDescriptor = epoll_create(20);

        //Create the epoll event for the server socket
        struct epoll_event ev;
        memset(&ev, 0x00, sizeof(ev));

        ev.data.fd = this->m_Socket;
        ev.events = EPOLLIN;
        
        //Add the server socket to the epoll file descriptor
        res = epoll_ctl(this->m_EpollDescriptor, EPOLL_CTL_ADD, this->m_Socket, &ev);
        if(res < 0)
            return -1;

        //Create the poll event for the STDIN
        struct epoll_event evStdin;
        memset(&evStdin, 0x00, sizeof(evStdin));

        evStdin.data.fd = STDIN_FILENO;
        evStdin.events = EPOLLIN;

        //Add the file descriptor for stdin to the epoll file descriptor
        res = epoll_ctl(this->m_EpollDescriptor, EPOLL_CTL_ADD, STDIN_FILENO, &evStdin);
        if(res < 0)
            return -1;

        return 0;
    }

    int TCPServer::run(){
        //Run the server
        std::cout << "Starting server!" << std::endl;
        struct epoll_event ret_evs[20];

        while (this->m_IsRunning)
        {
            //Return event structure
            memset(ret_evs, 0x00, sizeof(ret_evs));            

            //Wait for read notification
            int nfds = epoll_wait(this->m_EpollDescriptor, ret_evs, 20, -1);

            for(int i =0; i < nfds; i++) {
                if((static_cast<size_t>(ret_evs[i].data.fd) == this->m_Socket) && (ret_evs[i].events & EPOLLIN) != 0){
                    //Handle a new connection
                    
                    //Add the client to the list of sockets in the epoll file descriptor
                    struct epoll_event client_ev;
                    memset(&client_ev, 0x00, sizeof(client_ev));

                    //Client details for connection
                    struct sockaddr_in client_info;
                    memset(&client_info, 0x00, sizeof(client_info));
                    socklen_t* p = new socklen_t;
                    *p = INET_ADDRSTRLEN;

                    //Create a client socket
                    int client_socket = accept(this->m_Socket, (struct sockaddr*)&client_info, p);
                    if(client_socket < 0) {
                        std::cout << "Cannot create client socket";
                        continue;
                    }

                    client_ev.data.fd = client_socket;
                    client_ev.events = EPOLLIN;

                    epoll_ctl(this->m_EpollDescriptor, EPOLL_CTL_ADD, client_socket, &client_ev);

                    OnClientConnected(client_socket);
                }
                else if((ret_evs[i].data.fd == STDIN_FILENO) && (ret_evs[i].events & EPOLLIN) != 0){
                    //Handle input from STDIN
                    char* buf = new char[64];
                    size_t bytesIn = read(ret_evs[i].data.fd, buf, 64);
                    
                    if(bytesIn > 1) {
                        std::string command = buf;

                        OnCommandReceived(command);
                    }
                }
                else {
                    //Handle message from connected client
                    const unsigned int MAX_BUF_LENGTH = 32768;
                    std::vector<unsigned char> buf(MAX_BUF_LENGTH);

                    int bytesIn = recv(ret_evs[i].data.fd, &buf[0], MAX_BUF_LENGTH, 0);
                    std::cout << "BytesIn: " << bytesIn << std::endl;

                    //Handle client disconnection
                    if(bytesIn <= 0){
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            continue;

                        OnClientDisconnected(ret_evs[i].data.fd);
                        close(ret_evs[i].data.fd);

                        epoll_ctl(this->m_EpollDescriptor, EPOLL_CTL_DEL, ret_evs[i].data.fd, nullptr);  
                    }
                    else {
                        std::string msg;
                        msg.append(buf.cbegin(), buf.cend());

                        OnMessageReceived(ret_evs[i].data.fd, msg);
                    }
                }
            }
        }

        //Clear the data
        //Close the server socket
        close(this->m_Socket);

        //Close the epoll descriptor
        close(this->m_EpollDescriptor);

        return 0;
    }

    uint64_t TCPServer::ConnectToHttpServer(const char* ipAddress, unsigned int port){
        //Connect to another http server

        //Create the client socket
        int clientSock = -1;
        struct sockaddr_in serverAddr;

        clientSock = socket(AF_INET, SOCK_STREAM, 0);
        if(clientSock == -1)
            throw ServerException("Couldn't create the socket for connection!", 9);

        memset(&serverAddr, 0x00, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
        serverAddr.sin_port = htons(port);

        int res = connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if(res != 0)
            throw ServerException("Couldn't connect to server!", 10);
        
        //Add the client socket to the epoll descriptor
        struct epoll_event ev;
        ev.data.fd = clientSock;
        ev.events = EPOLLIN;
        epoll_ctl(this->m_EpollDescriptor, EPOLL_CTL_ADD, clientSock, &ev);
    
        //Return the socket
        return clientSock;
    }
}