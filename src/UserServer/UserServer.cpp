#include "UserServer.h"
#include "redisdb.h"

#include <json/json.h>

Admin UserMsgService::queryUserMsg(std::string name, UserMsgProto::ResultCode *code)
{
    Admin userInfo;
    RedisClient *redisClient = RedisClient::getInstance();
    std::string command = "HGET";
    std::string key = "admin";
    std::string field = name;
    std::string result;
    if (redisClient->getData(command, key, field, result))
    {
        std::cout << result << std::endl;
        userInfo.setName(name);
        Json::Reader reader;
        Json::Value data;
        reader.parse(result, data);
        userInfo.setStatus(data["status"].asString());
        userInfo.setPassword(data["pwd"].asString());
        userInfo.setEmail(data["email"].asString());
        userInfo.setPhone(data["phone"].asString());
        userInfo.setDesc(data["desc"].asString());
        userInfo.setDepartName(data["depart"].asString());
    }
    else
    {
        userInfo = adminmodel.query(name);
    }
    if (userInfo.getId() < 0)
    {
        code->set_errcode(1);
        code->set_errmsg("queryUserMsg failed");
    }
    else
    {
        code->set_errcode(0);
        code->set_errmsg("queryUserMsg successed");
    }
    return userInfo;
}

bool UserMsgService::updateUserMsg(std::string name, std::string pwd,
                                   std::string email, std::string phone,
                                   std::string desc, std::string departName,
                                   UserMsgProto::ResultCode *code)
{
    Admin userInfo;
    userInfo.setName(name);
    userInfo.setPassword(pwd);
    userInfo.setEmail(email);
    userInfo.setPhone(phone);
    userInfo.setDesc(desc);
    userInfo.setDepartName(departName);
    bool result = adminmodel.update(userInfo);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("updateUserMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("updateUserMsg failed");
    }
    return result;
}

void UserMsgService::QueryUserMsg(::google::protobuf::RpcController *controller,
                                  const ::UserMsgProto::QueryUserMsgRequest *request,
                                  ::UserMsgProto::QueryUserMsgResponse *response,
                                  ::google::protobuf::Closure *done)
{
    std::string name = request->name();
    UserMsgProto::ResultCode *code = response->mutable_result();
    Admin userInfo = queryUserMsg(name, code);
    response->mutable_usermsg()->set_id(userInfo.getId());
    response->mutable_usermsg()->set_name(userInfo.getName());
    response->mutable_usermsg()->set_pwd(userInfo.getPassword());
    response->mutable_usermsg()->set_email(userInfo.getEmail());
    response->mutable_usermsg()->set_phone(userInfo.getPhone());
    response->mutable_usermsg()->set_desc(userInfo.getDesc());
    response->mutable_usermsg()->set_departname(userInfo.getDepartName());
    done->Run();
}

void UserMsgService::UpdateUserMsg(::google::protobuf::RpcController *controller,
                                   const ::UserMsgProto::UpdateUserMsgRequest *request,
                                   ::UserMsgProto::UpdateUserMsgResponse *response,
                                   ::google::protobuf::Closure *done)
{
    std::string name = request->usermsg().name();
    std::string pwd = request->usermsg().pwd();
    std::string email = request->usermsg().email();
    std::string phone = request->usermsg().phone();
    std::string desc = request->usermsg().desc();
    std::string departName = request->usermsg().departname();
    UserMsgProto::ResultCode *code = response->mutable_result();
    updateUserMsg(name, pwd, email, phone, desc, departName, code);
    done->Run();
}