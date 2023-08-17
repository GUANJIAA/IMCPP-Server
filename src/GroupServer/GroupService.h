#ifndef __GROUPSERVICE_H__
#define __GROUPSERVICE_H__

#include "groupmodel.h"
#include "group.h"
#include "Group.pb.h"

class GroupService : public GroupProto::GroupServiceRpc
{
public:
    bool CreateGroup(Group &group, GroupProto::ResultCode *code);

    bool AddGroup(std::string groupName, std::string userName,
                  std::string userRole, GroupProto::ResultCode *code);

    bool QuitGroup(std::string groupName, std::string userName,
                   GroupProto::ResultCode *code);

    bool QueryGroup(std::string userName,
                    std::vector<GroupProto::GroupInfo> &Groups,
                    GroupProto::ResultCode *code);

    bool QueryGroupUsers(std::string groupName,
                         std::string userName,
                         Group &group,
                         GroupProto::ResultCode *code);

    virtual void CreateGroup(::google::protobuf::RpcController *controller,
                             const ::GroupProto::CreateGroupRequest *request,
                             ::GroupProto::CreateGroupResponse *response,
                             ::google::protobuf::Closure *done) override;

    virtual void AddGroup(::google::protobuf::RpcController *controller,
                          const ::GroupProto::AddGroupRequest *request,
                          ::GroupProto::AddGroupResponse *response,
                          ::google::protobuf::Closure *done) override;

    virtual void QuitGroup(::google::protobuf::RpcController *controller,
                           const ::GroupProto::QuitGroupRequest *request,
                           ::GroupProto::QuitGroupResponse *response,
                           ::google::protobuf::Closure *done) override;

    virtual void QueryGroup(::google::protobuf::RpcController *controller,
                            const ::GroupProto::QueryGroupRequest *request,
                            ::GroupProto::QueryGroupResponse *response,
                            ::google::protobuf::Closure *done) override;

    virtual void QueryGroupUsers(::google::protobuf::RpcController *controller,
                                 const ::GroupProto::QueryGroupUsersRequest *request,
                                 ::GroupProto::QueryGroupUsersResponse *response,
                                 ::google::protobuf::Closure *done) override;

private:
    GroupModel groupmodel;
};

#endif