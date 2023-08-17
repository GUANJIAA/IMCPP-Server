#include "DepartService.h"
#include "redisdb.h"

#include <iostream>
#include <json/json.h>

bool DepartService::CreateDepart(Depart &depart, DepartProto::ResultCode *code)
{
    bool result = departmodel.createDepart(depart);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("CreateDepart successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("CreateDepart failed");
    }
    return result;
}

bool DepartService::DelDepart(std::string departName, std::string userName, DepartProto::ResultCode *code)
{
    bool result = departmodel.delDepart(departName, userName);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("DelDepart successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("DelDepart failed");
    }
    return result;
}

bool DepartService::AddDepart(std::string departName,
                              std::string userName,
                              std::string userRole,
                              DepartProto::ResultCode *code)
{
    bool result = departmodel.addDepart(departName, userName, userRole);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddDepart successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddDepart failed");
    }
    return result;
}

bool DepartService::QuitDepart(std::string departName, std::string userName, DepartProto::ResultCode *code)
{
    bool result = departmodel.quitDepart(userName);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddDepart successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddDepart failed");
    }
    return result;
}

bool DepartService::QueryDepart(std::string userName,
                                DepartProto::DepartInfo &Departs,
                                DepartProto::ResultCode *code)
{
    Depart departVec;
    RedisClient *redisClient = RedisClient::getInstance();
    std::string command = "HGET";
    std::string key = "admin";
    std::string userInfo;
    // std::cout << userName << std::endl;
    if (redisClient->getData(command, key, userName, userInfo))
    {
        // std::cout << userInfo << std::endl;
        Json::Reader reader;
        Json::Value data;
        reader.parse(userInfo, data);
        std::string departName = data["depart"].asString();
        std::vector<std::string> vec;
        std::string departInfo;
        if (redisClient->getData(command, "alldepart", departName, departInfo))
        {
            // std::cout << departInfo << std::endl;
            Json::Value temp;
            reader.parse(departInfo, temp);
            departVec.setName(temp["departName"].asString());
            departVec.setDesc(temp["departDesc"].asString());
            departVec.setId(atoi(temp["id"].asString().c_str()));
        }
        if (redisClient->getSetData("departUsers_" + departName, vec))
        {
            for (auto &val : vec)
            {
                std::string str;
                Json::Value jsonVal;
                reader.parse(val, jsonVal);
                DepartUser departUser;
                departUser.setRole(jsonVal["userRole"].asString());
                // std::cout << val << std::endl;
                if (redisClient->getData("HGET", "admin", jsonVal["userName"].asString(), str))
                {
                    // std::cout << str << std::endl;
                    Json::Value temp;
                    reader.parse(str, temp);

                    departUser.setName(temp["name"].asString());
                    departUser.setEmail(temp["email"].asString());
                    departUser.setPhone(temp["phone"].asString());
                    departUser.setDesc(temp["desc"].asString());
                    departVec.getUsers().push_back(departUser);
                }
            }
        }
    }
    else
    {
        departVec = departmodel.queryDepart(userName);
    }
    // Depart departVec = departmodel.queryDepart(userName);
    bool result = false;
    if (departVec.getName() == "")
    {
        code->set_errcode(1);
        code->set_errmsg("QueryDepart failed");
    }
    else
    {
        Departs.set_departid(departVec.getId());
        Departs.set_departname(departVec.getName());
        Departs.set_departdesc(departVec.getDesc());
        for (auto &uval : departVec.getUsers())
        {
            DepartProto::DepartUser *info = Departs.add_departusers();
            info->set_username(uval.getName());
            info->set_useremail(uval.getEmail());
            info->set_userphone(uval.getPhone());
            info->set_userrole(uval.getRole());
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryDepart successful");
    }
    return result;
}

bool DepartService::QueryDepartUsers(std::string departName,
                                     std::string userName,
                                     std::vector<std::string> &departusers,
                                     DepartProto::ResultCode *code)
{
    // RedisClient *redisClient = RedisClient::getInstance();

    // if (redisClient->getSetData("departUsers_" + departName, departusers))
    // {
    //     for (auto &val : departusers)
    //     {
    //         std::cout << val << std::endl;
    //     }
    // }
    // else
    // {
    //     departusers = departmodel.queryDepartUsers(departName, userName);
    // }
    departusers = departmodel.queryDepartUsers(departName, userName);
    if (departusers.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryDepartUsers failed");
        return false;
    }
    else
    {
        code->set_errcode(0);
        code->set_errmsg("QueryDepartUsers successed");
        return true;
    }
}

void DepartService::CreateDepart(::google::protobuf::RpcController *controller,
                                 const ::DepartProto::CreateDepartRequest *request,
                                 ::DepartProto::CreateDepartResponse *response,
                                 ::google::protobuf::Closure *done)
{
    DepartProto::DepartInfo departInfo = request->depart();
    Depart depart;
    depart.setName(departInfo.departname());
    depart.setDesc(departInfo.departdesc());

    DepartProto::ResultCode *code = response->mutable_result();
    bool result = CreateDepart(depart, code);

    response->set_success(result);
    done->Run();
}

void DepartService::DelDepart(::google::protobuf::RpcController *controller,
                              const ::DepartProto::DelDepartRequest *request,
                              ::DepartProto::DelDepartResponse *response,
                              ::google::protobuf::Closure *done)
{
    std::string departName = request->departname();
    std::string adminName = request->username();

    DepartProto::ResultCode *code = response->mutable_result();
    bool result = DelDepart(departName, adminName, code);

    response->set_success(result);
    done->Run();
}

void DepartService::AddDepart(::google::protobuf::RpcController *controller,
                              const ::DepartProto::AddDepartRequest *request,
                              ::DepartProto::AddDepartResponse *response,
                              ::google::protobuf::Closure *done)
{
    std::string departName = request->departname();
    std::string adminName = request->username();
    std::string role = request->userrole();

    DepartProto::ResultCode *code = response->mutable_result();
    bool result = AddDepart(departName, adminName, role, code);

    response->set_success(result);
    done->Run();
}

void DepartService::QuitDepart(::google::protobuf::RpcController *controller,
                               const ::DepartProto::QuitDepartRequest *request,
                               ::DepartProto::QuitDepartResponse *response,
                               ::google::protobuf::Closure *done)
{
    std::string departName = request->departname();
    std::string adminName = request->username();

    DepartProto::ResultCode *code = response->mutable_result();
    bool result = QuitDepart(departName, adminName, code);

    response->set_success(result);
    done->Run();
}

void DepartService::QueryDepart(::google::protobuf::RpcController *controller,
                                const ::DepartProto::QueryDepartRequest *request,
                                ::DepartProto::QueryDepartResponse *response,
                                ::google::protobuf::Closure *done)
{
    std::string adminName = request->username();

    DepartProto::ResultCode *code = response->mutable_result();
    DepartProto::DepartInfo departs;
    bool result = QueryDepart(adminName, departs, code);

    response->mutable_departs()->set_departid(departs.departid());
    response->mutable_departs()->set_departname(departs.departname());
    response->mutable_departs()->set_departdesc(departs.departdesc());
    for (auto &user : departs.departusers())
    {
        DepartProto::DepartUser *tempuser = response->mutable_departs()->add_departusers();
        tempuser->set_username(user.username());
        tempuser->set_useremail(user.useremail());
        tempuser->set_userphone(user.userphone());
        tempuser->set_userrole(user.userrole());
    }

    response->set_success(result);

    done->Run();
}

void DepartService::QueryDepartUsers(::google::protobuf::RpcController *controller,
                                     const ::DepartProto::QueryDepartUsersRequest *request,
                                     ::DepartProto::QueryDepartUsersResponse *response,
                                     ::google::protobuf::Closure *done)
{
    std::string adminName = request->username();
    std::string departName = request->departname();
    DepartProto::ResultCode *code = response->mutable_result();
    std::vector<std::string> departusers;
    bool result = QueryDepartUsers(departName, adminName, departusers, code);
    for (auto &val : departusers)
    {
        std::string *temp = response->add_departusername();
        temp->swap(val);
    }
    response->set_success(result);

    done->Run();
}