#ifndef __USERSERVER_H__
#define __USERSERVER_H__

#include "UserMsg.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "adminModel.h"

#include <string>

class UserMsgService : public UserMsgProto::UserMsgServiceRpc
{
public:
    Admin queryUserMsg(std::string name, UserMsgProto::ResultCode *code);

    bool updateUserMsg(std::string name, std::string pwd,
                       std::string email, std::string phone,
                       std::string desc,std::string departName,
                       UserMsgProto::ResultCode *code);

    virtual void QueryUserMsg(::google::protobuf::RpcController *controller,
                              const ::UserMsgProto::QueryUserMsgRequest *request,
                              ::UserMsgProto::QueryUserMsgResponse *response,
                              ::google::protobuf::Closure *done) override;

    virtual void UpdateUserMsg(::google::protobuf::RpcController *controller,
                               const ::UserMsgProto::UpdateUserMsgRequest *request,
                               ::UserMsgProto::UpdateUserMsgResponse *response,
                               ::google::protobuf::Closure *done) override;

private:
    AdminModel adminmodel;
};

#endif