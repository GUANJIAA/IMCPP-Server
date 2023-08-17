#ifndef __MSGMODEL_H__
#define __MSGMODEL_H__

#include "ChatMsg.pb.h"

#include <vector>
#include <string>

class Msg
{
public:
    void setMsgId(int id) { msgid = id; }
    void setRecvName(const std::string &name) { recvName = name; }
    void setSendName(const std::string &name) { sendName = name; }
    void setMessage(const std::string &msg) { message = msg; }

    int getMsgId() { return msgid; }
    std::string getRecvName() { return recvName; }
    std::string getSendName() { return sendName; }
    std::string getMessage() { return message; }

private:
    int msgid;
    std::string recvName;
    std::string sendName;
    std::string message;
};

class GroupMsg
{
public:
    void setMsgId(int id) { msgid = id; }
    void setGroupName(const std::string &name) { groupName = name; }
    void setSendName(const std::string &name) { sendName = name; }
    void setMessage(const std::string &msg) { message = msg; }

    int getMsgId() { return msgid; }
    std::string getGroupName() { return groupName; }
    std::string getSendName() { return sendName; }
    std::string getMessage() { return message; }

private:
    int msgid;
    std::string groupName;
    std::string sendName;
    std::string message;
};

class DepartMsg
{
public:
    void setMsgId(int id) { msgid = id; }
    void setDepartName(const std::string &name) { departName = name; }
    void setSendName(const std::string &name) { sendName = name; }
    void setMessage(const std::string &msg) { message = msg; }

    int getMsgId() { return msgid; }
    std::string getDepartName() { return departName; }
    std::string getSendName() { return sendName; }
    std::string getMessage() { return message; }

private:
    int msgid;
    std::string departName;
    std::string sendName;
    std::string message;
};

class ChatMsgModel : public ChatMessageProto::chatMsgRpc
{
public:
    bool addChatMsg(std::string recvName, std::string sendName,
                    std::string message, std::string isRead);
    bool delChatMsg(int msgId, std::string recvName,
                    std::string sendName, std::string message);
    std::vector<Msg> queryChatMsg(std::string recvName,std::string sendName);
    std::vector<Msg> queryOfflineMsg(std::string recvName);

    bool addGroupChatMsg(std::string groupName, std::string sendName,
                         std::string message);
    bool delGroupChatMsg(int msgId, std::string groupName,
                         std::string sendName, std::string message);
    std::vector<GroupMsg> queryGroupChatMsg(std::string userName);

    bool addDepartChatMsg(std::string departName, std::string sendName,
                          std::string message);
    bool delDepartChatMsg(int msgId, std::string departName,
                          std::string sendName, std::string message);
    std::vector<DepartMsg> queryDepartChatMsg(std::string userName);
};

// class OfflineMsgModel : public OfflineMessageProto::offlineMsgRpc
// {
// public:
//     bool addOfflineMsg(std::string recvName, std::string sendName,
//                        std::string message);
//     bool delOfflineMsg(int msgId, std::string recvName,
//                        std::string sendName, std::string message);
//     std::vector<Msg> queryOfflineMsg(std::string recvName);

//     bool addOfflineGroupMsg(std::string groupName, std::string sendName,
//                             std::string message);
//     bool delOfflineGroupMsg(int msgId, std::string groupName,
//                             std::string sendName, std::string message);
//     std::vector<GroupMsg> queryOfflineGroupMsg(std::string userName);

//     bool addOfflineDepartMsg(std::string departName, std::string sendName,
//                              std::string message);
//     bool delOfflineDepartMsg(int msgId, std::string departName,
//                              std::string sendName, std::string message);
//     std::vector<DepartMsg> queryOfflineDepartMsg(std::string userName);

// private:
// };

#endif