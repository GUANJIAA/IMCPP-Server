#ifndef __LOGINSERVICE_H__
#define __LOGINSERVICE_H__

#include "Login.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "adminModel.h"

#include <iostream>
#include <string>

class LoginService : public LoginProto::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd,
               LoginProto::ResultCode *code);

    bool Logout(std::string name, LoginProto::ResultCode *code);

    bool Register(std::string name, std::string pwd,
                  std::string email, std::string phone,
                  LoginProto::ResultCode *code);

    bool Retrieve(std::string name, std::string password,
                  std::string email, std::string phone,
                  LoginProto::ResultCode *code);

    virtual void Login(::google::protobuf::RpcController *controller,
                       const ::LoginProto::LoginRequest *request,
                       ::LoginProto::LoginResponse *response,
                       ::google::protobuf::Closure *done) override;

    virtual void Logout(::google::protobuf::RpcController *controller,
                        const ::LoginProto::LogoutRequest *request,
                        ::LoginProto::LogoutResponse *response,
                        ::google::protobuf::Closure *done) override;

    virtual void Register(::google::protobuf::RpcController *controller,
                          const ::LoginProto::RegisterRequest *request,
                          ::LoginProto::RegisterResponse *response,
                          ::google::protobuf::Closure *done) override;

    virtual void Retrieve(::google::protobuf::RpcController *controller,
                          const ::LoginProto::RetrieveRequest *request,
                          ::LoginProto::RetrieveResponse *response,
                          ::google::protobuf::Closure *done) override;

private:
    AdminModel adminmodel;
};

#endif