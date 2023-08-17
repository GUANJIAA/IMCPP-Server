#include "Login.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

#include <iostream>
#include <json/json.h>
#include <functional>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

enum LoginMsgId
{
    LOGIN_MSG = 1,
    LOGOUT_MSG,
    REGISTER_MSG,
    RETRIEVE_MSG
};

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
        tcpcon->shutdown();
    }
}

void Server::onMessage(const muduo::net::TcpConnectionPtr &tcpcon, muduo::net::Buffer *buffer)
{
    std::cout << buffer;
    std::string buf = buffer->retrieveAllAsString();
    Json::Reader reader;
    Json::Value data;
    reader.parse(buf, data);
    if (data["msgid"] == LOGIN_MSG)
    {
        LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
        LoginProto::LoginRequest request;
        request.set_name(data["name"].asString());
        request.set_pwd(data["pwd"].asString());
        LoginProto::LoginResponse response;
        stub.Login(nullptr, &request, &response, nullptr);
        Json::Value result;
        result["msgid"] = LOGIN_MSG;
        result["errcode"] = response.result().errcode();
        result["errmsg"] = response.result().errmsg();
        result["ip"] = response.msgserverip();
        result["port"] = response.msgserverport();
        std::string str = Json::FastWriter().write(result);
        str+="@@@";
        tcpcon->send(str);
    }
    else if(data["msgid"] == LOGOUT_MSG)
    {
        LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
        LoginProto::LogoutRequest request;
        request.set_name(data["name"].asString());
        LoginProto::LogoutResponse response;
        stub.Logout(nullptr, &request, &response, nullptr);
        Json::Value result;
        result["msgid"] = LOGOUT_MSG;
        result["errcode"] = response.result().errcode();
        result["errmsg"] = response.result().errmsg();
        std::string str = Json::FastWriter().write(result);
        str+="@@@";
        tcpcon->send(str);
    }
    else if (data["msgid"] == REGISTER_MSG)
    {
        LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
        LoginProto::RegisterRequest request;
        request.set_name(data["name"].asString());
        request.set_pwd(data["pwd"].asString());
        request.set_email(data["email"].asString());
        request.set_phone(data["phone"].asString());
        LoginProto::RegisterResponse response;
        stub.Register(nullptr, &request, &response, nullptr);
        Json::Value result;
        result["msgid"] = REGISTER_MSG;
        result["errcode"] = response.result().errcode();
        result["errmsg"] = response.result().errmsg();
        std::string str = Json::FastWriter().write(result);
        str+="@@@";
        tcpcon->send(str);
    }
    else if (data["msgid"] == RETRIEVE_MSG)
    {
        LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
        LoginProto::RetrieveRequest request;
        request.set_name(data["name"].asString());
        request.set_pwd(data["pwd"].asString());
        request.set_email(data["email"].asString());
        request.set_phone(data["phone"].asString());
        LoginProto::RetrieveResponse response;
        stub.Retrieve(nullptr, &request, &response, nullptr);
        Json::Value result;
        result["msgid"] = RETRIEVE_MSG;
        result["errcode"] = response.result().errcode();
        result["errmsg"] = response.result().errmsg();
        std::string str = Json::FastWriter().write(result);
        str+="@@@";
        tcpcon->send(str);
    }
}

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("loginServerIp");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("loginServerPort");

    std::cout << ip << ":" << port << std::endl;

    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr(ip, atoi(port.c_str()));
    std::string name = "LoginServer:" + ip + ":" + port;
    Server server(&loop, addr, name);

    server.start();
    loop.loop();
    return 0;
}