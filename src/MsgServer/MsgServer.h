#ifndef __MSGSERVER_H__
#define __MSGSERVER_H__

#include "redispb.h"
#include "Login.pb.h"
#include "Friend.pb.h"
#include "Group.pb.h"
#include "Depart.pb.h"
#include "ChatMsg.pb.h"
#include "offlinemessage.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

#include <string>
#include <mutex>
#include <unordered_map>
#include <json/json.h>
#include <muduo/net/TcpConnection.h>

enum EnMsgType
{
    NEW_CONNECTION = 5,     // 新连接建立
    QUERY_ADMININFO,        // 查询用户详细信息
    UPDATE_ADMININFO,       // 更新用户详细信息
    ONE_CHAT_MSG,           // 私聊消息
    DEL_CHAT_MSG,           // 删除聊天消息
    QUERY_CHAT_MSG,         // 查询聊天消息
    QUERY_OFFLINE_CHAT_MSG, // 查询离线聊天消息
    ADD_FRIEND_MSG,         // 添加好友
    DEL_FRIEND_MSG,         // 删除好友
    QUERY_FRIEND_MSG,       // 查询好友列表
    GROUP_CHAT_MSG,         // 群组聊天消息
    QUERY_GROUPCHAT_MSG,    // 查看群组聊天消息
    CREATE_GROUP_MSG,       // 创建群组
    ADD_GROUP_MSG,          // 添加群组
    QUERY_GROUP_MSG,        // 查询所有群组及所有群友
    QUERY_GROUP_USERS_MSG,  // 查询所在群所有群友
    DEL_GROUP_MSG,          // 删除群组
    QUIT_GROUP_MSG,         // 退出群组
    DEPART_CHAT_MSG,        // 部门聊天消息
    CREATE_DEPART_MSG,      // 创建部门
    ADD_DEPART_MSG,         // 加入部门
    QUERY_DEPART_USERS_MSG, // 查询所在部门的所有同事
    DEL_DEPART_MSG,         // 删除部门
    QUIT_DEPART_MSG,        // 退出部门
    QUERY_DEPARTCHAT_MSG,   // 查询部门聊天信息
};

using MsgHandler = std::function<void(const muduo::net::TcpConnectionPtr &, Json::Value &)>;

class MsgServer
{
public:
    static MsgServer *instance();

    void newConnection(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryAdminInfo(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void updateAdminInfo(const muduo::net::TcpConnectionPtr &, Json::Value &);

    void oneChat(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void delChatMsg(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryChatMsg(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryOfflineChatMsg(const muduo::net::TcpConnectionPtr &, Json::Value &);

    void addFriend(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void delFriend(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryFriend(const muduo::net::TcpConnectionPtr &, Json::Value &);

    void groupChat(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryGroupChat(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void createGroup(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void addGroup(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void quitGroup(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryGroup(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryGroupUser(const muduo::net::TcpConnectionPtr &, Json::Value &);

    void departChat(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryDepartChat(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void createDepart(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void delDepart(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void addDepart(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void quitDepart(const muduo::net::TcpConnectionPtr &, Json::Value &);
    void queryDepart(const muduo::net::TcpConnectionPtr &, Json::Value &);

    void clientCloseException(const muduo::net::TcpConnectionPtr &);
    void allReset();

    MsgHandler getHandler(int msgid);

    void handleRedisSubscribeMessage(std::string, std::string);

private:
    MsgServer();

    std::unordered_map<int, MsgHandler> _msgHandlerMap;

    std::unordered_map<std::string, muduo::net::TcpConnectionPtr> _userConnMap;

    std::mutex _connMutex;

    RedisPb _redis;
};

#endif