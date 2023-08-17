#ifndef __CHATSERVICE_H__
#define __CHATSERVICE_H__

#include "msgmodel.h"
#include "groupmodel.h"
#include "departmodel.h"
#include "redispb.h"
#include "ChatMsg.pb.h"

#include <vector>

class ChatService : public ChatMessageProto::chatMsgRpc
{
public:
    bool AddChatMsg(std::string recvName, std::string sendName,
                    std::string message, std::string isRead,
                    ChatMessageProto::ResultCode *code);

    bool DelChatMsg(int msgId, std::string recvName, std::string sendName,
                    std::string message, ChatMessageProto::ResultCode *code);

    bool QueryChatMsg(std::string recvName, std::string sendName,
                         std::vector<ChatMessageProto::chatMsg> &msg,
                      ChatMessageProto::ResultCode *code);

    bool QueryOfflineMsg(std::string recvName, std::vector<ChatMessageProto::chatMsg> &msg,
                         ChatMessageProto::ResultCode *code);

    bool AddGroupChatMsg(std::string groupName, std::string sendName,
                         std::string message, ChatMessageProto::ResultCode *code);

    bool DelGroupChatMsg(int msgId, std::string groupName, std::string sendName,
                         std::string message, ChatMessageProto::ResultCode *code);

    bool QueryGroupChatMsg(std::string userName, std::vector<ChatMessageProto::groupChatMsg> &groupMsg,
                           ChatMessageProto::ResultCode *code);

    bool AddDepartChatMsg(std::string departName, std::string sendName,
                          std::string message, ChatMessageProto::ResultCode *code);

    bool DelDepartChatMsg(int msgId, std::string departName, std::string sendName,
                          std::string message, ChatMessageProto::ResultCode *code);

    bool QueryDepartChatMsg(std::string userName, std::vector<ChatMessageProto::departChatMsg> &departMsg,
                            ChatMessageProto::ResultCode *code);

    virtual void AddChatMsg(::google::protobuf::RpcController *controller,
                            const ::ChatMessageProto::addMsgRequest *request,
                            ::ChatMessageProto::addMsgResponse *response,
                            ::google::protobuf::Closure *done) override;

    virtual void DelChatMsg(::google::protobuf::RpcController *controller,
                            const ::ChatMessageProto::delMsgRequest *request,
                            ::ChatMessageProto::delMsgResponse *response,
                            ::google::protobuf::Closure *done) override;

    virtual void QueryChatMsg(::google::protobuf::RpcController *controller,
                              const ::ChatMessageProto::queryMsgRequest *request,
                              ::ChatMessageProto::queryMsgResponse *response,
                              ::google::protobuf::Closure *done) override;

    virtual void QueryOfflineMsg(::google::protobuf::RpcController *controller,
                                 const ::ChatMessageProto::queryOfflineMsgRequest *request,
                                 ::ChatMessageProto::queryOfflineMsgResponse *response,
                                 ::google::protobuf::Closure *done) override;

    virtual void AddGroupChatMsg(::google::protobuf::RpcController *controller,
                                 const ::ChatMessageProto::addGroupMsgRequest *request,
                                 ::ChatMessageProto::addGroupMsgResponse *response,
                                 ::google::protobuf::Closure *done) override;

    virtual void DelGroupChatMsg(::google::protobuf::RpcController *controller,
                                 const ::ChatMessageProto::delGroupMsgRequest *request,
                                 ::ChatMessageProto::delGroupMsgResponse *response,
                                 ::google::protobuf::Closure *done) override;

    virtual void QueryGroupChatMsg(::google::protobuf::RpcController *controller,
                                   const ::ChatMessageProto::queryGroupMsgRequest *request,
                                   ::ChatMessageProto::queryGroupMsgResponse *response,
                                   ::google::protobuf::Closure *done) override;

    virtual void AddDepartChatMsg(::google::protobuf::RpcController *controller,
                                  const ::ChatMessageProto::addDepartMsgRequest *request,
                                  ::ChatMessageProto::addDepartMsgResponse *response,
                                  ::google::protobuf::Closure *done) override;

    virtual void DelDepartChatMsg(::google::protobuf::RpcController *controller,
                                  const ::ChatMessageProto::delDepartMsgRequest *request,
                                  ::ChatMessageProto::delDepartMsgResponse *response,
                                  ::google::protobuf::Closure *done) override;

    virtual void QueryDepartChatMsg(::google::protobuf::RpcController *controller,
                                    const ::ChatMessageProto::queryDepartMsgRequest *request,
                                    ::ChatMessageProto::queryDepartMsgResponse *response,
                                    ::google::protobuf::Closure *done) override;

private:
    ChatMsgModel chatmsgmodel;
    GroupModel groupmodel;
    DepartModel departmodel;
    //    OfflineMsgModel offlinemsgmodel;
};

#endif