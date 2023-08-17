#include "ChatService.h"

#include "redisdb.h"

#include <json/json.h>
#include <iostream>

bool ChatService::AddChatMsg(std::string recvName, std::string sendName,
                             std::string message, std::string isRead,
                             ChatMessageProto::ResultCode *code)
{
    bool result = chatmsgmodel.addChatMsg(recvName, sendName, message, isRead);
    bool query = false;
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddChatMsg failed");
    }
    return result;
}

bool ChatService::DelChatMsg(int msgId, std::string recvName, std::string sendName,
                             std::string message, ChatMessageProto::ResultCode *code)
{
    bool result = chatmsgmodel.delChatMsg(msgId, recvName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("DelChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("DelChatMsg failed");
    }
    return result;
}

bool ChatService::QueryChatMsg(std::string recvName, std::string sendName,
                               std::vector<ChatMessageProto::chatMsg> &msg,
                               ChatMessageProto::ResultCode *code)
{
    std::vector<Msg> msgVec;
    RedisClient *redisClient = RedisClient::getInstance();
    std::vector<std::string> vec;
    std::string key = "chatMsg:" + recvName;
    // std::cout << key << std::endl;
    if (redisClient->getSetData(key, vec))
    {
        for (auto &val : vec)
        {
            Json::Reader reader;
            Json::Value data;
            reader.parse(val, data);
            if ((recvName == data["recvName"].asString() && sendName == data["sendName"].asString()) ||
                (recvName == data["sendName"].asString() && sendName == data["recvName"].asString()))
            {
                Msg temp;
                temp.setMsgId(atoi(data["msgid"].asString().c_str()));
                temp.setRecvName(data["recvName"].asString());
                temp.setSendName(data["sendName"].asString());
                temp.setMessage(data["message"].asString());
                msgVec.push_back(temp);
            }
        }
    }
    else
    {
        msgVec = chatmsgmodel.queryChatMsg(recvName, sendName);
    }
    // std::vector<Msg> msgVec = chatmsgmodel.queryChatMsg(recvName, sendName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryChatMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            ChatMessageProto::chatMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_recvname(val.getRecvName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            msg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryChatMsg successful");
    }
    return result;
}

bool ChatService::QueryOfflineMsg(std::string recvName, std::vector<ChatMessageProto::chatMsg> &msg,
                                  ChatMessageProto::ResultCode *code)
{
    std::vector<Msg> msgVec = chatmsgmodel.queryOfflineMsg(recvName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryChatMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            ChatMessageProto::chatMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_recvname(val.getRecvName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            msg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryChatMsg successful");
    }
    return result;
}

bool ChatService::AddGroupChatMsg(std::string groupName, std::string sendName,
                                  std::string message, ChatMessageProto::ResultCode *code)
{

    bool result = chatmsgmodel.addGroupChatMsg(groupName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddGroupChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddGroupChatMsg failed");
    }
    return result;
}

bool ChatService::DelGroupChatMsg(int msgId, std::string groupName, std::string sendName,
                                  std::string message, ChatMessageProto::ResultCode *code)
{
    bool result = chatmsgmodel.delGroupChatMsg(msgId, groupName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("DelGroupChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("DelGroupChatMsg failed");
    }
    return result;
}

bool ChatService::QueryGroupChatMsg(std::string userName, std::vector<ChatMessageProto::groupChatMsg> &groupMsg,
                                    ChatMessageProto::ResultCode *code)
{
    // std::vector<GroupMsg> msgVec;
    // RedisClient *redisClient = RedisClient::getInstance();
    // std::vector<std::string> vec;
    // std::string key = userName;
    // if (redisClient->getSetData("groups_" + userName, vec))
    // {
    //     for (auto &val : vec)
    //     {
    //         // std::cout << val << std::endl;
    //         std::vector<std::string> groupMsg;
    //         if (redisClient->getSetData("groupChatMsg:" + val, groupMsg))
    //         {
    //             for (auto &temp : groupMsg)
    //             {
    //                 Json::Reader reader;
    //                 Json::Value data;
    //                 reader.parse(val, data);
    //                 GroupMsg msg;
    //                 msg.setGroupName(data["groupname"].asString());
    //                 msg.setMsgId(atoi(data["msgid"].asString().c_str()));
    //                 msg.setSendName(data["sendname"].asString());
    //                 msg.setMessage(data["message"].asString());
    //                 msgVec.push_back(msg);
    //             }
    //         }
    //     }
    // }
    // else
    // {
        std::vector<GroupMsg> msgVec = chatmsgmodel.queryGroupChatMsg(userName);
    // }
    // std::vector<GroupMsg> msgVec = chatmsgmodel.queryGroupChatMsg(userName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryGroupChatMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            ChatMessageProto::groupChatMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_groupname(val.getGroupName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            groupMsg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryGroupChatMsg successful");
    }
    return result;
}

bool ChatService::AddDepartChatMsg(std::string departName, std::string sendName,
                                   std::string message, ChatMessageProto::ResultCode *code)
{
    bool result = chatmsgmodel.addDepartChatMsg(departName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("AddDepartChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("AddDepartChatMsg failed");
    }
    return result;
}

bool ChatService::DelDepartChatMsg(int msgId, std::string departName, std::string sendName,
                                   std::string message, ChatMessageProto::ResultCode *code)
{
    bool result = chatmsgmodel.delDepartChatMsg(msgId, departName, sendName, message);
    if (result)
    {
        code->set_errcode(0);
        code->set_errmsg("DelDepartChatMsg successful");
    }
    else
    {
        code->set_errcode(1);
        code->set_errmsg("DelDepartChatMsg failed");
    }
    return result;
}

bool ChatService::QueryDepartChatMsg(std::string userName, std::vector<ChatMessageProto::departChatMsg> &departMsg,
                                     ChatMessageProto::ResultCode *code)
{
    std::vector<DepartMsg> msgVec;
    RedisClient *redisClient = RedisClient::getInstance();
    std::string command = "HGET";
    std::string key = "admin";
    std::string userInfo;
    if (redisClient->getData(command, key, userName, userInfo))
    {
        Json::Reader reader;
        Json::Value data;
        reader.parse(userInfo, data);
        std::string departName = data["depart"].asString();
        std::vector<std::string> vec;
        if (redisClient->getSetData("depatChatMsg:" + departName, vec))
        {
            for (auto &val : vec)
            {
                reader.parse(val, data);
                DepartMsg msg;
                msg.setMsgId(atoi(data["msgid"].asString().c_str()));
                msg.setSendName(data["sendName"].asString());
                msg.setDepartName(data["departname"].asString());
                msg.setMessage(data["message"].asString());
                msgVec.push_back(msg);
            }
        }
    }
    else
    {
        msgVec = chatmsgmodel.queryDepartChatMsg(userName);
    }
    // std::vector<DepartMsg> msgVec = chatmsgmodel.queryDepartChatMsg(userName);
    bool result = false;
    if (msgVec.empty())
    {
        code->set_errcode(1);
        code->set_errmsg("QueryDepartChatMsg failed");
    }
    else
    {
        for (auto &val : msgVec)
        {
            ChatMessageProto::departChatMsg temp;
            temp.set_msgid(val.getMsgId());
            temp.set_departname(val.getDepartName());
            temp.set_sendname(val.getSendName());
            temp.set_message(val.getMessage());
            departMsg.push_back(temp);
        }
        result = true;
        code->set_errcode(0);
        code->set_errmsg("QueryDepartChatMsg successful");
    }
    return result;
}

void ChatService::AddChatMsg(::google::protobuf::RpcController *controller,
                             const ::ChatMessageProto::addMsgRequest *request,
                             ::ChatMessageProto::addMsgResponse *response,
                             ::google::protobuf::Closure *done)
{

    std::string recvName = request->msg().recvname();
    std::string sendName = request->msg().sendname();
    std::string message = request->msg().message();
    std::string isRead = request->msg().isread();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddChatMsg(recvName, sendName, message, isRead, code);

    response->set_success(result);
    done->Run();
}

void ChatService::DelChatMsg(::google::protobuf::RpcController *controller,
                             const ::ChatMessageProto::delMsgRequest *request,
                             ::ChatMessageProto::delMsgResponse *response,
                             ::google::protobuf::Closure *done)
{

    int msgId = request->msg().msgid();
    std::string recvName = request->msg().recvname();
    std::string sendName = request->msg().sendname();
    std::string message = request->msg().message();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelChatMsg(msgId, recvName, sendName, message, code);
    response->set_success(result);
    done->Run();
}

void ChatService::QueryChatMsg(::google::protobuf::RpcController *controller,
                               const ::ChatMessageProto::queryMsgRequest *request,
                               ::ChatMessageProto::queryMsgResponse *response,
                               ::google::protobuf::Closure *done)
{
    std::string recvName = request->recvname();
    std::string sendName = request->sendname();
    ChatMessageProto::ResultCode *code = response->mutable_result();
    std::vector<ChatMessageProto::chatMsg> msgVec;
    bool result = QueryChatMsg(recvName, sendName, msgVec, code);
    for (auto &val : msgVec)
    {
        ChatMessageProto::chatMsg *temp = response->add_msg();
        temp->set_msgid(val.msgid());
        temp->set_recvname(val.recvname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}

void ChatService::QueryOfflineMsg(::google::protobuf::RpcController *controller,
                                  const ::ChatMessageProto::queryOfflineMsgRequest *request,
                                  ::ChatMessageProto::queryOfflineMsgResponse *response,
                                  ::google::protobuf::Closure *done)
{
    std::string recvName = request->recvname();
    ChatMessageProto::ResultCode *code = response->mutable_result();
    std::vector<ChatMessageProto::chatMsg> msgVec;
    bool result = QueryOfflineMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        ChatMessageProto::chatMsg *temp = response->add_msg();
        temp->set_msgid(val.msgid());
        temp->set_recvname(val.recvname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}

void ChatService::AddGroupChatMsg(::google::protobuf::RpcController *controller,
                                  const ::ChatMessageProto::addGroupMsgRequest *request,
                                  ::ChatMessageProto::addGroupMsgResponse *response,
                                  ::google::protobuf::Closure *done)
{

    std::string groupName = request->groupmsg().groupname();
    std::string sendName = request->groupmsg().sendname();
    std::string message = request->groupmsg().message();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddGroupChatMsg(groupName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void ChatService::DelGroupChatMsg(::google::protobuf::RpcController *controller,
                                  const ::ChatMessageProto::delGroupMsgRequest *request,
                                  ::ChatMessageProto::delGroupMsgResponse *response,
                                  ::google::protobuf::Closure *done)
{
    int msgId = request->groupmsg().msgid();
    std::string groupName = request->groupmsg().groupname();
    std::string sendName = request->groupmsg().sendname();
    std::string message = request->groupmsg().message();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelGroupChatMsg(msgId, groupName, sendName, message, code);
    response->set_success(result);
    done->Run();
}

void ChatService::QueryGroupChatMsg(::google::protobuf::RpcController *controller,
                                    const ::ChatMessageProto::queryGroupMsgRequest *request,
                                    ::ChatMessageProto::queryGroupMsgResponse *response,
                                    ::google::protobuf::Closure *done)
{
    std::string recvName = request->username();
    ChatMessageProto::ResultCode *code = response->mutable_result();
    std::vector<ChatMessageProto::groupChatMsg> msgVec;
    bool result = QueryGroupChatMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        ChatMessageProto::groupChatMsg *temp = response->add_groupmsg();
        temp->set_msgid(val.msgid());
        temp->set_groupname(val.groupname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}

void ChatService::AddDepartChatMsg(::google::protobuf::RpcController *controller,
                                   const ::ChatMessageProto::addDepartMsgRequest *request,
                                   ::ChatMessageProto::addDepartMsgResponse *response,
                                   ::google::protobuf::Closure *done)
{

    std::string departName = request->departmsg().departname();
    std::string sendName = request->departmsg().sendname();
    std::string message = request->departmsg().message();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = AddDepartChatMsg(departName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void ChatService::DelDepartChatMsg(::google::protobuf::RpcController *controller,
                                   const ::ChatMessageProto::delDepartMsgRequest *request,
                                   ::ChatMessageProto::delDepartMsgResponse *response,
                                   ::google::protobuf::Closure *done)
{
    int msgid = request->departmsg().msgid();
    std::string departName = request->departmsg().departname();
    std::string sendName = request->departmsg().sendname();
    std::string message = request->departmsg().message();

    ChatMessageProto::ResultCode *code = response->mutable_result();
    bool result = DelDepartChatMsg(msgid, departName, sendName, message, code);

    response->set_success(result);
    done->Run();
}

void ChatService::QueryDepartChatMsg(::google::protobuf::RpcController *controller,
                                     const ::ChatMessageProto::queryDepartMsgRequest *request,
                                     ::ChatMessageProto::queryDepartMsgResponse *response,
                                     ::google::protobuf::Closure *done)
{
    std::string recvName = request->username();
    ChatMessageProto::ResultCode *code = response->mutable_result();
    std::vector<ChatMessageProto::departChatMsg> msgVec;
    bool result = QueryDepartChatMsg(recvName, msgVec, code);
    for (auto &val : msgVec)
    {
        ChatMessageProto::departChatMsg *temp = response->add_departmsg();
        temp->set_msgid(val.msgid());
        temp->set_departname(val.departname());
        temp->set_sendname(val.sendname());
        temp->set_message(val.message());
    }
    response->set_success(result);

    done->Run();
}