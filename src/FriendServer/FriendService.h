#ifndef __FRIENDSERVICE_H__
#define __FRIENDSERVICE_H__

#include "Friend.pb.h"
#include "friendmodel.h"
#include "admin.h"

#include <string>
#include <vector>

class FriendService : public FriendProto::FriendServiceRpc
{
public:
    bool AddFriend(std::string adminName, std::string peerName,
                   FriendProto::ResultCode *code);

    bool GetFriend(std::string adminName, FriendProto::ResultCode *code,
                   std::vector<FriendProto::AdminInfo> &friends);

    bool DelFriend(std::string adminName, std::string peerName,
                   FriendProto::ResultCode *code);

    virtual void AddFriend(::google::protobuf::RpcController *controller,
                           const ::FriendProto::AddFriendRequest *request,
                           ::FriendProto::AddFriendResponse *response,
                           ::google::protobuf::Closure *done) override;

    virtual void GetFriend(::google::protobuf::RpcController *controller,
                           const ::FriendProto::GetFriendRequest *request,
                           ::FriendProto::GetFriendResponse *response,
                           ::google::protobuf::Closure *done) override;

    virtual void DelFriend(::google::protobuf::RpcController *controller,
                           const ::FriendProto::DelFriendRequest *request,
                           ::FriendProto::DelFriendResponse *response,
                           ::google::protobuf::Closure *done) override;

private:
    FriendModel friendmodel;
};

#endif