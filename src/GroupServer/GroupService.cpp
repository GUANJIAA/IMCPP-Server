#include "GroupService.h"

#include "redisdb.h"

#include <iostream>
#include <json/json.h>

bool GroupService::CreateGroup(Group &group, GroupProto::ResultCode *code)
{
    bool result = groupmodel.createGroup(group);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("CreateGroup successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("CreateGroup failed");
    }
    return result;
}

bool GroupService::AddGroup(std::string groupName,
                            std::string userName,
                            std::string userRole,
                            GroupProto::ResultCode *code)
{
    bool result = groupmodel.addGroup(groupName, userName, userRole);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddGroup successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddGroup failed");
    }
    return result;
}

bool GroupService::QuitGroup(std::string groupName, std::string userName,
                             GroupProto::ResultCode *code)
{
        bool result = groupmodel.quitGroup(groupName, userName);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("QuitGroup successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("QuitGroup failed");
    }
    return result;
}

bool GroupService::QueryGroup(std::string userName,
                              std::vector<GroupProto::GroupInfo> &Groups,
                              GroupProto::ResultCode *code)
{
    std::vector<Group> groupVec;
    bool flag = false;
    RedisClient *redisClient = RedisClient::getInstance();

    std::string key = "groups_" + userName;
    std::vector<std::string> groupsName;

    if (redisClient->getSetData(key, groupsName))
    {
        for (auto &groupName : groupsName)
        {
            Group group;
            // std::cout << groupName << std::endl;
            std::string groupInfo;
            if (redisClient->getData("HGET", "allgroup", groupName, groupInfo))
            {
                Json::Reader reader;
                Json::Value data;
                reader.parse(groupInfo, data);
                group.setId(atoi(data["id"].asString().c_str()));
                group.setName(data["groupName"].asString());
                group.setDesc(data["groupDesc"].asString());
            }
            else
            {
                flag = true;
                break;
            }
            std::vector<std::string> groupUsersName;
            if (redisClient->getSetData("groupUsers_" + groupName, groupUsersName))
            {
                for (auto &userName : groupUsersName)
                {
                    // std::cout << userName << std::endl;
                    GroupUser user;
                    Json::Reader reader;
                    Json::Value data;
                    reader.parse(userName, data);
                    user.setName(data["userName"].asString());
                    user.setRole(data["userRole"].asString());
                    std::string userInfo;
                    if (redisClient->getData("HGET", "admin", data["userName"].asString(), userInfo))
                    {
                        Json::Value temp;
                        reader.parse(userInfo, temp);
                        user.setEmail(temp["email"].asString());
                        user.setPhone(temp["phone"].asString());
                    }
                    else
                    {
                        flag = true;
                        break;
                    }
                    group.getUsers().push_back(user);
                }
            }
            else
            {
                flag = true;
                break;
            }
            groupVec.push_back(group);
        }
    }
    if (flag)
    {
        groupVec = groupmodel.queryGroup(userName);
    }
    // std::vector<Group> groupVec = groupmodel.queryGroup(userName);
    bool result = false;
    if (groupVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryGroup failed");
    }
    else
    {
        for (auto &gval : groupVec)
        {
            GroupProto::GroupInfo temp;
            temp.set_groupid(gval.getId());
            temp.set_groupname(gval.getName());
            temp.set_groupdesc(gval.getDesc());
            for (auto &uval : gval.getUsers())
            {
                GroupProto::GroupUser *info = temp.add_groupusers();
                // std::cout << "-" << uval.getName()
                //           << "-" << uval.getEmail()
                //           << "-" << uval.getPhone()
                //           << "-" << uval.getRole() << std::endl;
                info->set_username(uval.getName());
                info->set_useremail(uval.getEmail());
                info->set_userphone(uval.getPhone());
                info->set_userrole(uval.getRole());
            }
            Groups.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryGroup successful");
    }
    return result;
}

bool GroupService::QueryGroupUsers(std::string groupName,
                                   std::string userName,
                                   Group &group,
                                   GroupProto::ResultCode *code)
{
    RedisClient *redisClient = RedisClient::getInstance();
    bool flag = false;
    std::vector<std::string> groupUserName;
    std::string groupInfo;
    if (redisClient->getData("HGET", "allgroup", groupName, groupInfo))
    {
        std::cout << groupInfo << std::endl;
        Json::Reader reader;
        Json::Value data;
        reader.parse(groupInfo, data);
        group.setId(atoi(data["id"].asString().c_str()));
        group.setName(data["userName"].asString());
        group.setDesc(data["desc"].asString());
    }
    else
    {
        flag = true;
    }
    if (redisClient->getSetData("groupUsers_" + groupName, groupUserName))
    {
        for (auto &val : groupUserName)
        {
            // std::cout << val << std::endl;
            // std::cout << "----------" << std::endl;
            GroupUser userInfo;
            Json::Reader reader;
            Json::Value data;
            reader.parse(val, data);
            userInfo.setName(data["userName"].asString());
            userInfo.setRole(data["userRole"].asString());
            std::string temp;
            // std::cout << "test----------" << std::endl;
            if (redisClient->getData("HGET", "admin", data["userName"].asString(), temp))
            {
                std::cout << temp << std::endl;
                Json::Value usertemp;
                reader.parse(temp, usertemp);
                userInfo.setEmail(usertemp["email"].asString());
                userInfo.setPhone(usertemp["phone"].asString());
            }
            group.getUsers().push_back(userInfo);
        }
    }
    if (flag)
    {
        group = groupmodel.queryGroupUsers(groupName, userName);
    }
    // group = groupmodel.queryGroupUsers(groupName, userName);
    if (group.getUsers().empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryGroupUsers failed");
        return false;
    }
    else
    {
        code->set_errcode(0);
        code->set_errmsg("QueryGroupUsers successed");
        return true;
    }
}

void GroupService::CreateGroup(::google::protobuf::RpcController *controller,
                               const ::GroupProto::CreateGroupRequest *request,
                               ::GroupProto::CreateGroupResponse *response,
                               ::google::protobuf::Closure *done)
{
    GroupProto::GroupInfo groupInfo = request->group();
    Group group;
    group.setName(groupInfo.groupname());
    group.setDesc(groupInfo.groupdesc());

    GroupProto::ResultCode *code = response->mutable_result();
    bool result = CreateGroup(group, code);

    response->set_success(result);
    done->Run();
}

void GroupService::AddGroup(::google::protobuf::RpcController *controller,
                            const ::GroupProto::AddGroupRequest *request,
                            ::GroupProto::AddGroupResponse *response,
                            ::google::protobuf::Closure *done)
{
    std::string groupName = request->groupname();
    std::string adminName = request->username();
    std::string role = request->userrole();

    GroupProto::ResultCode *code = response->mutable_result();
    bool result = AddGroup(groupName, adminName, role, code);

    response->set_success(result);
    done->Run();
}

void GroupService::QuitGroup(::google::protobuf::RpcController *controller,
                             const ::GroupProto::QuitGroupRequest *request,
                             ::GroupProto::QuitGroupResponse *response,
                             ::google::protobuf::Closure *done)
{
    std::string groupName = request->groupname();
    std::string userName = request->username();

    GroupProto::ResultCode *code = response->mutable_result();
    bool result = QuitGroup(groupName, userName, code);

    response->set_success(result);
    done->Run();
}

void GroupService::QueryGroup(::google::protobuf::RpcController *controller,
                              const ::GroupProto::QueryGroupRequest *request,
                              ::GroupProto::QueryGroupResponse *response,
                              ::google::protobuf::Closure *done)
{
    std::string adminName = request->username();

    GroupProto::ResultCode *code = response->mutable_result();
    std::vector<GroupProto::GroupInfo> groups;
    bool result = QueryGroup(adminName, groups, code);

    for (auto &val : groups)
    {
        GroupProto::GroupInfo *temp = response->add_groups();
        temp->set_groupid(val.groupid());
        temp->set_groupname(val.groupname());
        temp->set_groupdesc(val.groupdesc());
        for (auto &user : val.groupusers())
        {
            GroupProto::GroupUser *tempuser = temp->add_groupusers();
            tempuser->set_username(user.username());
            tempuser->set_useremail(user.useremail());
            tempuser->set_userphone(user.userphone());
            tempuser->set_userrole(user.userrole());
        }
    }

    response->set_success(result);

    done->Run();
}

void GroupService::QueryGroupUsers(::google::protobuf::RpcController *controller,
                                   const ::GroupProto::QueryGroupUsersRequest *request,
                                   ::GroupProto::QueryGroupUsersResponse *response,
                                   ::google::protobuf::Closure *done)
{
    std::string adminName = request->username();
    std::string groupName = request->groupname();
    GroupProto::ResultCode *code = response->mutable_result();
    Group groupusers;
    bool result = QueryGroupUsers(groupName, adminName, groupusers, code);
    for (auto &user : groupusers.getUsers())
    {
        GroupProto::GroupUser *temp = response->add_groupuser();
        temp->set_username(user.getName());
        temp->set_useremail(user.getEmail());
        temp->set_userphone(user.getPhone());
        temp->set_userrole(user.getRole());
    }
    response->set_success(result);

    done->Run();
}