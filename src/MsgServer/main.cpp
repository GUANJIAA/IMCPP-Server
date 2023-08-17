#include "MsgServer.h"

#include <signal.h>
#include <json/json.h>
#include <functional>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

class Server
{
public:
    Server(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr, const std::string &nameArg);
    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr &tcpcon);
    void onMessage(const muduo::net::TcpConnectionPtr &tcpcon, muduo::net::Buffer *buffer);

    muduo::net::TcpServer _server;
    muduo::net::EventLoop *_loop;
};

Server::Server(muduo::net::EventLoop *loop,
               const muduo::net::InetAddress &listenAddr,
               const std::string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(std::bind(&Server::onConnection, this, std::placeholders::_1));

    _server.setMessageCallback(std::bind(&Server::onMessage, this, std::placeholders::_1,
                                         std::placeholders::_2));

    _server.setThreadNum(4);
}

void Server::start()
{
    _server.start();
}

void Server::onConnection(const muduo::net::TcpConnectionPtr &tcpcon)
{
    if (!tcpcon->connected())
    {
        MsgServer::instance()->clientCloseException(tcpcon);
        tcpcon->shutdown();
    }
}

void Server::onMessage(const muduo::net::TcpConnectionPtr &tcpcon, muduo::net::Buffer *buffer)
{
    std::string buf = buffer->retrieveAllAsString();
    Json::Reader reader;
    Json::Value data;
    reader.parse(buf, data);
    auto msgHandler = MsgServer::instance()->getHandler(data["msgid"].asInt());
    msgHandler(tcpcon, data);
}

void resetHandler(int)
{
    MsgServer::instance()->allReset();
    exit(0);
}

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("msgServerip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("msgServerport");

    signal(SIGINT,resetHandler);

    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr(ip, atoi(port.c_str()));
    std::string name = "MsgServer:" + ip + ":" + port;
    Server server(&loop, addr, name);

    server.start();
    loop.loop();
    return 0;
}