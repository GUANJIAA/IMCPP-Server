#include "OfflineMsgService.h"

#include <vector>

bool OfflineMsgService::AddOfflineMsg(std::string recvName, std::string sendName,
                                      std::string message, OfflineMessageProto::ResultCode *code)
{
    bool result = offlinemsgmodel.addOfflineMsg(recvName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddOfflineMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddOfflineMsg failed");
    }
    return result;
}

bool OfflineMsgService::DelOfflineMsg(int msgId, std::string recvName, std::string sendName,
                                      std::string message, OfflineMessageProto::ResultCode *code)
{
    bool result = offlinemsgmodel.delOfflineMsg(msgId, recvName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddOfflineMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddOfflineMsg failed");
    }
    return result;
}

bool OfflineMsgService::QueryOfflineMsg(std::string recvName,
                                        std::vector<OfflineMessageProto::msg> &Msg,
                                        OfflineMessageProto::ResultCode *code)
{
    std::vector<Msg> msgVec = offlinemsgmodel.queryOfflineMsg(recvName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryOfflineMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            OfflineMessageProto::msg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_recvname(val.getRecvName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            Msg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryOfflineMsg successful");
    }
    return result;
}

bool OfflineMsgService::AddOfflineGroupMsg(std::string groupName, std::string sendName,
                                           std::string message, OfflineMessageProto::ResultCode *code)
{
    bool result = offlinemsgmodel.addOfflineGroupMsg(groupName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddOfflineMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddOfflineMsg failed");
    }
    return result;
}

bool OfflineMsgService::DelOfflineGroupMsg(int msgId, std::string groupName, std::string sendName,
                                           std::string message, OfflineMessageProto::ResultCode *code)
{

    bool result = offlinemsgmodel.delOfflineGroupMsg(msgId, groupName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddOfflineMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddOfflineMsg failed");
    }
    return result;
}

bool OfflineMsgService::QueryOfflineGroupMsg(std::string userName,
                                             std::vector<OfflineMessageProto::groupMsg> &groupMsg,
                                             OfflineMessageProto::ResultCode *code)
{
    std::vector<GroupMsg> msgVec = offlinemsgmodel.queryOfflineGroupMsg(userName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryOfflineGroupMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            OfflineMessageProto::groupMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_groupname(val.getGroupName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            groupMsg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryOfflineGroupMsg successful");
    }
    return result;
}

bool OfflineMsgService::AddOfflineDepartMsg(std::string departName, std::string sendName,
                                            std::string message, OfflineMessageProto::ResultCode *code)
{
    bool result = offlinemsgmodel.addOfflineDepartMsg(departName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddOfflineDepartMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddOfflineDepartMsg failed");
    }
    return result;
}

bool OfflineMsgService::DelOfflineDepartMsg(int msgId, std::string departName, std::string sendName,
                                            std::string message, OfflineMessageProto::ResultCode *code)
{
    bool result = offlinemsgmodel.delOfflineDepartMsg(msgId, departName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("DelOfflineDepartMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("DelOfflineDepartMsg failed");
    }
    return result;
}

bool OfflineMsgService::QueryOfflineDepartMsg(std::string userName,
                                              std::vector<OfflineMessageProto::departMsg> &departMsg,
                                              OfflineMessageProto::ResultCode *code)
{
    std::vector<DepartMsg> msgVec = offlinemsgmodel.queryOfflineDepartMsg(userName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryOfflineDepartMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            OfflineMessageProto::departMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_departname(val.getDepartName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            departMsg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryOfflineDepartMsg successful");
    }
    return result;
}

void OfflineMsgService::AddOfflineMsg(::google::protobuf::RpcController *controller,
                                      const ::OfflineMessageProto::addOfflineMsgRequest *request,
                                      ::OfflineMessageProto::addOfflineMsgResponse *response,
                                      ::google::protobuf::Closure *done)
{
    std::string recvName = request->offlinemsg().recvname();
    std::string sendName = request->offlinemsg().sendname();
    std::string message = request->offlinemsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddOfflineMsg(recvName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void OfflineMsgService::DelOfflineMsg(::google::protobuf::RpcController *controller,
                                      const ::OfflineMessageProto::delOfflineMsgRequest *request,
                                      ::OfflineMessageProto::delOfflineMsgResponse *response,
                                      ::google::protobuf::Closure *done)
{
    int msgId = request->offlinemsg().msgid();
    std::string recvName = request->offlinemsg().recvname();
    std::string sendName = request->offlinemsg().sendname();
    std::string message = request->offlinemsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelOfflineMsg(msgId, recvName, sendName, message, code);
    response->set_success(result);
    done->Run();
}

void OfflineMsgService::QueryOfflineMsg(::google::protobuf::RpcController *controller,
                                        const ::OfflineMessageProto::queryOfflineMsgRequest *request,
                                        ::OfflineMessageProto::queryOfflineMsgResponse *response,
                                        ::google::protobuf::Closure *done)
{
    std::string recvName = request->recvname();
    OfflineMessageProto::ResultCode *code = response->mutable_result();
    std::vector<OfflineMessageProto::msg> msgVec;
    bool result = QueryOfflineMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        OfflineMessageProto::msg *temp = response->add_offlinemsg();
        temp->set_msgid(val.msgid());
        temp->set_recvname(val.recvname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}

void OfflineMsgService::AddOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                           const ::OfflineMessageProto::addOfflineGroupMsgRequest *request,
                                           ::OfflineMessageProto::addOfflineGroupMsgResponse *response,
                                           ::google::protobuf::Closure *done)
{
    std::string groupName = request->offlinegroupmsg().groupname();
    std::string sendName = request->offlinegroupmsg().sendname();
    std::string message = request->offlinegroupmsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddOfflineGroupMsg(groupName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void OfflineMsgService::DelOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                           const ::OfflineMessageProto::delOfflineGroupMsgRequest *request,
                                           ::OfflineMessageProto::delOfflineGroupMsgResponse *response,
                                           ::google::protobuf::Closure *done)
{
    int msgId = request->offlinegroupmsg().msgid();
    std::string groupName = request->offlinegroupmsg().groupname();
    std::string sendName = request->offlinegroupmsg().sendname();
    std::string message = request->offlinegroupmsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelOfflineGroupMsg(msgId, groupName, sendName, message, code);
    response->set_success(result);
    done->Run();
}

void OfflineMsgService::QueryOfflineGroupMsg(::google::protobuf::RpcController *controller,
                                             const ::OfflineMessageProto::queryOfflineGroupMsgRequest *request,
                                             ::OfflineMessageProto::queryOfflineGroupMsgResponse *response,
                                             ::google::protobuf::Closure *done)
{
    std::string recvName = request->username();
    OfflineMessageProto::ResultCode *code = response->mutable_result();
    std::vector<OfflineMessageProto::groupMsg> msgVec;
    bool result = QueryOfflineGroupMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        OfflineMessageProto::groupMsg *temp = response->add_offlinegroupmsg();
        temp->set_msgid(val.msgid());
        temp->set_groupname(val.groupname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}

void OfflineMsgService::AddOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                            const ::OfflineMessageProto::addOfflineDepartMsgRequest *request,
                                            ::OfflineMessageProto::addOfflineDepartMsgResponse *response,
                                            ::google::protobuf::Closure *done)
{
    std::string departName = request->offlinedepartmsg().departname();
    std::string sendName = request->offlinedepartmsg().sendname();
    std::string message = request->offlinedepartmsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddOfflineDepartMsg(departName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void OfflineMsgService::DelOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                            const ::OfflineMessageProto::delOfflineDepartMsgRequest *request,
                                            ::OfflineMessageProto::delOfflineDepartMsgResponse *response,
                                            ::google::protobuf::Closure *done)
{
    int msgid = request->offlinedepartmsg().msgid();
    std::string departName = request->offlinedepartmsg().departname();
    std::string sendName = request->offlinedepartmsg().sendname();
    std::string message = request->offlinedepartmsg().message();

    OfflineMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelOfflineDepartMsg(msgid, departName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void OfflineMsgService::QueryOfflineDepartMsg(::google::protobuf::RpcController *controller,
                                              const ::OfflineMessageProto::queryOfflineDepartMsgRequest *request,
                                              ::OfflineMessageProto::queryOfflineDepartMsgResponse *response,
                                              ::google::protobuf::Closure *done)
{
    std::string recvName = request->username();
    OfflineMessageProto::ResultCode *code = response->mutable_result();
    std::vector<OfflineMessageProto::departMsg> msgVec;
    bool result = QueryOfflineDepartMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        OfflineMessageProto::departMsg *temp = response->add_offlinedepartmsg();
        temp->set_msgid(val.msgid());
        temp->set_departname(val.departname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}