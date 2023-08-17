#ifndef __OFFLINEMSGSERVICE_H__
#define __OFFLINEMSGSERVICE_H__

#include "msgmodel.h"
#include "offlinemessage.pb.h"

class OfflineMsgService : public OfflineMessageProto::offlineMsgRpc
{
public:
    bool AddOfflineMsg(std::string recvName, std::string sendName,
                       std::string message, OfflineMessageProto::ResultCode *code);

    bool DelOfflineMsg(int msgId, std::string recvName, std::string sendName,
                       std::string message, OfflineMessageProto::ResultCode *code);

    bool QueryOfflineMsg(std::string recvName,
                         std::vector<OfflineMessageProto::msg> &msg,
                         OfflineMessageProto::ResultCode *code);

    bool AddOfflineGroupMsg(std::string groupName, std::string sendName,
                            std::string message, OfflineMessageProto::ResultCode *code);

    bool DelOfflineGroupMsg(int msgId, std::string groupName, std::string sendName,
                            std::string message, OfflineMessageProto::ResultCode *code);

    bool QueryOfflineGroupMsg(std::string userName,
                              std::vector<OfflineMessageProto::groupMsg> &groupMsg,
                              OfflineMessageProto::ResultCode *code);

    bool AddOfflineDepartMsg(std::string departName, std::string sendName,
                             std::string message, OfflineMessageProto::ResultCode *code);

    bool DelOfflineDepartMsg(int msgId, std::string departName, std::string sendName,
                             std::string message, OfflineMessageProto::ResultCode *code);

    bool QueryOfflineDepartMsg(std::string userName,
                               std::vector<OfflineMessageProto::departMsg> &departMsg,
                               OfflineMessageProto::ResultCode *code);

    virtual void AddOfflineMsg(::google::protobuf::RpcController *controller,
                               const ::OfflineMessageProto::addOfflineMsgRequest *request,
                               ::OfflineMessageProto::addOfflineMsgResponse *response,
                               ::google::protobuf::Closure *done) override;

    virtual void DelOfflineMsg(::google::protobuf::RpcController *controller,
                               const ::OfflineMessageProto::delOfflineMsgRequest *request,
                               ::OfflineMessageProto::delOfflineMsgResponse *response,
                               ::google::protobuf::Closure *done) override;

    virtual void QueryOfflineMsg(::google::protobuf::RpcController *controller,
                                 const ::OfflineMessageProto::queryOfflineMsgRequest *request,
                                 ::OfflineMessageProto::queryOfflineMsgResponse *response,
                                 ::google::protobuf::Closure *done) override;

    virtual void AddOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                    const ::OfflineMessageProto::addOfflineGroupMsgRequest *request,
                                    ::OfflineMessageProto::addOfflineGroupMsgResponse *response,
                                    ::google::protobuf::Closure *done) override;

    virtual void DelOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                    const ::OfflineMessageProto::delOfflineGroupMsgRequest *request,
                                    ::OfflineMessageProto::delOfflineGroupMsgResponse *response,
                                    ::google::protobuf::Closure *done) override;

    virtual void QueryOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                      const ::OfflineMessageProto::queryOfflineGroupMsgRequest *request,
                                      ::OfflineMessageProto::queryOfflineGroupMsgResponse *response,
                                      ::google::protobuf::Closure *done) override;

    virtual void AddOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                     const ::OfflineMessageProto::addOfflineDepartMsgRequest *request,
                                     ::OfflineMessageProto::addOfflineDepartMsgResponse *response,
                                     ::google::protobuf::Closure *done) override;

    virtual void DelOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                     const ::OfflineMessageProto::delOfflineDepartMsgRequest *request,
                                     ::OfflineMessageProto::delOfflineDepartMsgResponse *response,
                                     ::google::protobuf::Closure *done) override;

    virtual void QueryOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                       const ::OfflineMessageProto::queryOfflineDepartMsgRequest *request,
                                       ::OfflineMessageProto::queryOfflineDepartMsgResponse *response,
                                       ::google::protobuf::Closure *done) override;

private:
    OfflineMsgModel offlinemsgmodel;
};

#endif