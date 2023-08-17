#include "MsgServer.h"

#include "UserMsg.pb.h"
#include "Group.pb.h"
#include "Depart.pb.h"
#include "Friend.pb.h"
#include "ChatMsg.pb.h"
#include "Login.pb.h"

#include "mprpcchannel.h"

MsgServer *MsgServer::instance()
{
    static MsgServer server;
    return &server;
}

MsgServer::MsgServer()
{
    _msgHandlerMap.insert({NEW_CONNECTION, bind(&MsgServer::newConnection, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_ADMININFO, bind(&MsgServer::queryAdminInfo, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({UPDATE_ADMININFO, bind(&MsgServer::updateAdminInfo, this, std::placeholders::_1, std::placeholders::_2)});

    _msgHandlerMap.insert({ONE_CHAT_MSG, bind(&MsgServer::oneChat, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({DEL_CHAT_MSG, bind(&MsgServer::delChatMsg, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_CHAT_MSG, bind(&MsgServer::queryChatMsg, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_OFFLINE_CHAT_MSG, bind(&MsgServer::queryOfflineChatMsg, this, std::placeholders::_1, std::placeholders::_2)});

    _msgHandlerMap.insert({ADD_FRIEND_MSG, bind(&MsgServer::addFriend, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({DEL_FRIEND_MSG, bind(&MsgServer::delFriend, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_FRIEND_MSG, bind(&MsgServer::queryFriend, this, std::placeholders::_1, std::placeholders::_2)});

    _msgHandlerMap.insert({GROUP_CHAT_MSG, bind(&MsgServer::groupChat, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_GROUPCHAT_MSG, bind(&MsgServer::queryGroupChat, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({CREATE_GROUP_MSG, bind(&MsgServer::createGroup, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, bind(&MsgServer::addGroup, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUIT_GROUP_MSG, bind(&MsgServer::quitGroup, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_GROUP_MSG, bind(&MsgServer::queryGroup, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_GROUP_USERS_MSG, bind(&MsgServer::queryGroupUser, this, std::placeholders::_1, std::placeholders::_2)});

    _msgHandlerMap.insert({DEPART_CHAT_MSG, bind(&MsgServer::departChat, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_DEPARTCHAT_MSG, bind(&MsgServer::queryDepartChat, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({CREATE_DEPART_MSG, bind(&MsgServer::createDepart, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({DEL_DEPART_MSG, bind(&MsgServer::delDepart, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({ADD_DEPART_MSG, bind(&MsgServer::addDepart, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUIT_DEPART_MSG, bind(&MsgServer::quitDepart, this, std::placeholders::_1, std::placeholders::_2)});
    _msgHandlerMap.insert({QUERY_DEPART_USERS_MSG, bind(&MsgServer::queryDepart, this, std::placeholders::_1, std::placeholders::_2)});
    // _msgHandlerMap.insert({QUIT_DEPART_MSG, bind(&MsgServer::quitDepart, this, std::placeholders::_1, std::placeholders::_2)});

    if (_redis.subscribeConnect() && _redis.publishConnect())
    {
        _redis.init_notify_handler(bind(&MsgServer::handleRedisSubscribeMessage,
                                        this, std::placeholders::_1, std::placeholders::_2));
    }
}

void MsgServer::newConnection(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    {
        std::lock_guard<std::mutex> lock(_connMutex);
        _userConnMap.insert({userName, tcpconn});
    }
    tcpconn->setTcpNoDelay(true);
    queryAdminInfo(tcpconn, data);
    queryFriend(tcpconn, data);
    queryGroup(tcpconn, data);
    queryDepart(tcpconn, data);

    data["recvName"] = userName;
    // queryOfflineChatMsg(tcpconn, data);
    _redis.subscribe(userName);
}

void MsgServer::queryAdminInfo(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();

    UserMsgProto::UserMsgServiceRpc_Stub stub(new MprpcChannel());
    UserMsgProto::QueryUserMsgRequest request;
    request.set_name(userName);
    UserMsgProto::QueryUserMsgResponse response;
    stub.QueryUserMsg(nullptr, &request, &response, nullptr);

    Json::Value result;
    result["msgid"] = QUERY_ADMININFO;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    result["userName"] = response.usermsg().name();
    result["userPwd"] = response.usermsg().pwd();
    result["userEmail"] = response.usermsg().email();
    result["userPhone"] = response.usermsg().phone();
    result["userDesc"] = response.usermsg().desc();
    result["userDepartName"] = response.usermsg().departname();

    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::updateAdminInfo(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    std::string userPwd = data["userPwd"].asString();
    std::string userEmail = data["userEmail"].asString();
    std::string userPhone = data["userPhone"].asString();
    std::string userDesc = data["userDesc"].asString();
    std::string userDepartName = data["userDepartName"].asString();

    UserMsgProto::UserMsgServiceRpc_Stub stub(new MprpcChannel());
    UserMsgProto::UpdateUserMsgRequest request;
    UserMsgProto::UserInfo *userInfo = request.mutable_usermsg();
    userInfo->set_name(userName);
    userInfo->set_pwd(userPwd);
    userInfo->set_email(userEmail);
    userInfo->set_phone(userPhone);
    userInfo->set_desc(userDesc);
    userInfo->set_departname(userDepartName);
    UserMsgProto::UpdateUserMsgResponse response;
    stub.UpdateUserMsg(nullptr, &request, &response, nullptr);

    Json::Value result;
    result["msgid"] = UPDATE_ADMININFO;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    result["userName"] = userName;
    result["userPwd"] = userPwd;
    result["userEmail"] = userEmail;
    result["userPhone"] = userPhone;
    result["userDesc"] = userDesc;
    result["userDepartName"] = userDepartName;

    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::oneChat(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string recvName = data["recvName"].asString();

    // std::cout << std::endl;
    // std::cout << "------------------" << std::endl;
    // std::cout << ":" << _redis.ischannel(recvName) << std::endl;

    {
        std::lock_guard<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(recvName);
        std::string str = Json::FastWriter().write(data);
        if (it != _userConnMap.end())
        {
            str += "@@@";
            it->second->send(str);
            data["isRead"] = "true";
        }
        else if (_redis.ischannel(recvName))
        {
            str += "@@@";
            _redis.publish(recvName, str);
            data["isRead"] = "true";
        }
    }

    std::string sendName = data["sendName"].asString();
    std::string message = data["message"].asString();
    std::string isRead = data["isRead"].asString();
    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::addMsgRequest request;
    request.mutable_msg()->set_recvname(recvName);
    request.mutable_msg()->set_sendname(sendName);
    request.mutable_msg()->set_message(message);
    request.mutable_msg()->set_isread(isRead);
    ChatMessageProto::addMsgResponse response;
    stub.AddChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = ONE_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::delChatMsg(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    int msgId = data["msgId"].asInt();
    std::string recvName = data["recvName"].asString();
    std::string sendName = data["sendName"].asString();
    std::string message = data["message"].asString();

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::delMsgRequest request;
    request.mutable_msg()->set_msgid(msgId);
    request.mutable_msg()->set_recvname(recvName);
    request.mutable_msg()->set_sendname(sendName);
    request.mutable_msg()->set_message(message);
    ChatMessageProto::delMsgResponse response;
    stub.DelChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = DEL_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryChatMsg(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string recvName = data["recvName"].asString();
    std::string sendName = data["sendName"].asString();

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::queryMsgRequest request;
    request.set_recvname(recvName);
    request.set_sendname(sendName);
    ChatMessageProto::queryMsgResponse response;
    stub.QueryChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    int i = 0;
    for (auto &val : response.msg())
    {
        Json::Value chatmsg;
        chatmsg["msgId"] = val.msgid();
        chatmsg["recvName"] = val.recvname();
        chatmsg["sendName"] = val.sendname();
        chatmsg["message"] = val.message();
        result["chatmsgs"].append(chatmsg);
        // i++;
        // if (i == 2)
        // {
        //     std::string str = Json::FastWriter().write(result);
        //     str += "@@@";
        //     tcpconn->send(str);
        //     i = 0;
        //     result["chatmsgs"].clear();
        // }
    }
    // if (i != 0)
    // {
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
    // }
}

void MsgServer::queryOfflineChatMsg(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string recvName = data["recvName"].asString();

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::queryOfflineMsgRequest request;
    request.set_recvname(recvName);
    ChatMessageProto::queryOfflineMsgResponse response;
    stub.QueryOfflineMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_OFFLINE_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for (auto &val : response.msg())
    {
        Json::Value chatmsg;
        chatmsg["msgId"] = val.msgid();
        chatmsg["recvName"] = val.recvname();
        chatmsg["sendName"] = val.sendname();
        chatmsg["message"] = val.message();
        result["chatmsgs"].append(chatmsg);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::addFriend(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    std::string peerName = data["peerName"].asString();

    FriendProto::FriendServiceRpc_Stub stub(new MprpcChannel());
    FriendProto::AddFriendRequest request;
    request.set_adminname(userName);
    request.set_peername(peerName);
    FriendProto::AddFriendResponse response;
    stub.AddFriend(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = ADD_FRIEND_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::delFriend(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    std::string peerName = data["peerName"].asString();

    FriendProto::FriendServiceRpc_Stub stub(new MprpcChannel());
    FriendProto::DelFriendRequest request;
    request.set_adminname(userName);
    request.set_peername(peerName);
    FriendProto::DelFriendResponse response;
    stub.DelFriend(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = DEL_FRIEND_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryFriend(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();

    FriendProto::FriendServiceRpc_Stub stub(new MprpcChannel());
    FriendProto::GetFriendRequest request;
    request.set_adminname(userName);
    FriendProto::GetFriendResponse response;
    stub.GetFriend(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_FRIEND_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for (auto &val : response.data())
    {
        Json::Value temp;
        temp["friendName"] = val.name();
        temp["friendStatus"] = val.status();
        temp["friendEmail"] = val.email();
        temp["friendPhone"] = val.phone();
        temp["friendDesc"] = val.desc();
        temp["friendDepartName"] = val.departname();
        result["Friend"].append(temp);
    }
    std::string str = Json::FastWriter().write(result);
    // std::cout<<str<<std::endl;
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::groupChat(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string groupName = data["groupName"].asString();
    std::string sendName = data["sendName"].asString();
    std::string message = data["message"].asString();
    for (auto &val : data["recvName"])
    {
        Json::Value temp;
        temp["msgid"] = GROUP_CHAT_MSG;
        temp["groupName"] = groupName;
        temp["sendName"] = sendName;
        temp["message"] = message;
        temp["recvName"] = val.asString();

        std::lock_guard<std::mutex> lock(_connMutex);
        std::string userName = val.asString();
        auto it = _userConnMap.find(userName);
        std::string str = Json::FastWriter().write(temp);
        if (it != _userConnMap.end())
        {
            str += "@@@";
            it->second->send(str);
        }
        else if (_redis.ischannel(val.asString()))
        {
            _redis.publish(val.asString(), str);
        }
    }

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::addGroupMsgRequest request;
    request.mutable_groupmsg()->set_groupname(groupName);
    request.mutable_groupmsg()->set_sendname(sendName);
    request.mutable_groupmsg()->set_message(message);
    ChatMessageProto::addGroupMsgResponse response;
    stub.AddGroupChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = GROUP_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryGroupChat(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::queryGroupMsgRequest request;
    request.set_username(userName);
    ChatMessageProto::queryGroupMsgResponse response;
    stub.QueryGroupChatMsg(nullptr, &request, &response, nullptr);

    Json::Value result;
    result["msgid"] = QUERY_GROUPCHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for(auto &val:response.groupmsg())
    {
        Json::Value temp;
        temp["msgId"]=val.msgid();
        temp["groupName"]=val.groupname();
        temp["sendName"]=val.sendname();
        temp["message"]=val.message();
        result["chatmsgs"].append(temp);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::createGroup(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string groupName = data["groupName"].asString();
    std::string groupDesc = data["groupDesc"].asString();

    GroupProto::GroupServiceRpc_Stub stub(new MprpcChannel());
    GroupProto::CreateGroupRequest request;
    GroupProto::GroupInfo *group = request.mutable_group();
    group->set_groupname(groupName);
    group->set_groupdesc(groupDesc);
    GroupProto::CreateGroupResponse response;
    stub.CreateGroup(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = CREATE_GROUP_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    result["groupName"] = groupName;
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::addGroup(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string groupName = data["groupName"].asString();
    std::string userName = data["userName"].asString();
    std::string userRole = data["userRole"].asString();

    GroupProto::GroupServiceRpc_Stub stub(new MprpcChannel());
    GroupProto::AddGroupRequest request;
    request.set_groupname(groupName);
    request.set_username(userName);
    request.set_userrole(userRole);
    GroupProto::AddGroupResponse response;
    stub.AddGroup(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = ADD_GROUP_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::quitGroup(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string groupName = data["groupName"].asString();
    std::string userName = data["userName"].asString();

    GroupProto::GroupServiceRpc_Stub stub(new MprpcChannel());
    GroupProto::QuitGroupRequest request;
    request.set_groupname(groupName);
    request.set_username(userName);
    GroupProto::QuitGroupResponse response;
    stub.QuitGroup(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUIT_GROUP_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryGroup(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();

    GroupProto::GroupServiceRpc_Stub stub(new MprpcChannel());
    GroupProto::QueryGroupRequest request;
    request.set_username(userName);
    GroupProto::QueryGroupResponse response;
    stub.QueryGroup(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_GROUP_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for (auto &val : response.groups())
    {
        Json::Value group;
        group["groupName"] = val.groupname();
        group["groupDesc"] = val.groupdesc();
        for (auto &user : val.groupusers())
        {
            Json::Value userInfo;
            userInfo["userName"] = user.username();
            userInfo["userEmail"] = user.useremail();
            userInfo["userPhone"] = user.userphone();
            userInfo["userRole"] = user.userrole();
            group["users"].append(userInfo);
        }
        result["groups"].append(group);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryGroupUser(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    std::string groupName = data["groupName"].asString();

    GroupProto::GroupServiceRpc_Stub stub(new MprpcChannel());
    GroupProto::QueryGroupUsersRequest request;
    request.set_username(userName);
    request.set_groupname(groupName);
    GroupProto::QueryGroupUsersResponse response;
    stub.QueryGroupUsers(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_GROUP_USERS_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for (auto &user : response.groupuser())
    {
        Json::Value userInfo;
        userInfo["userName"] = user.username();
        userInfo["userEmail"] = user.useremail();
        userInfo["userPhone"] = user.userphone();
        userInfo["userRole"] = user.userrole();
        result["users"].append(userInfo);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::departChat(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string departName = data["departName"].asString();
    std::string sendName = data["sendName"].asString();
    std::string message = data["message"].asString();
    for (auto &val : data["recvName"])
    {
        Json::Value temp;
        temp["msgid"] = DEPART_CHAT_MSG;
        temp["departName"] = departName;
        temp["sendName"] = sendName;
        temp["message"] = message;
        temp["recvName"] = val.asString();

        std::lock_guard<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(val.asString());
        std::string str = Json::FastWriter().write(temp);
        if (it != _userConnMap.end())
        {
            str += "@@@";
            it->second->send(str);
        }
        else if (_redis.ischannel(val.asString()))
        {
            _redis.publish(val.asString(), str);
        }
    }
    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::addDepartMsgRequest request;
    request.mutable_departmsg()->set_departname(departName);
    request.mutable_departmsg()->set_sendname(sendName);
    request.mutable_departmsg()->set_message(message);
    ChatMessageProto::addDepartMsgResponse response;
    stub.AddDepartChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = DEPART_CHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryDepartChat(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();

    ChatMessageProto::chatMsgRpc_Stub stub(new MprpcChannel());
    ChatMessageProto::queryDepartMsgRequest request;
    request.set_username(userName);
    ChatMessageProto::queryDepartMsgResponse response;
    stub.QueryDepartChatMsg(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_DEPARTCHAT_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    for (auto &val : response.departmsg())
    {
        Json::Value temp;
        temp["msgId"] = val.msgid();
        temp["departName"] = val.departname();
        temp["sendName"] = val.sendname();
        temp["message"] = val.message();
        result["chatmsgs"].append(temp);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::createDepart(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string departName = data["departName"].asString();
    std::string departDesc = data["departDesc"].asString();

    DepartProto::DepartServiceRpc_Stub stub(new MprpcChannel());
    DepartProto::CreateDepartRequest request;
    DepartProto::DepartInfo *depart = request.mutable_depart();
    depart->set_departname(departName);
    depart->set_departdesc(departDesc);
    DepartProto::CreateDepartResponse response;
    stub.CreateDepart(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = CREATE_DEPART_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::delDepart(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string departName = data["departName"].asString();
    std::string userName = data["userName"].asString();

    DepartProto::DepartServiceRpc_Stub stub(new MprpcChannel());
    DepartProto::DelDepartRequest request;
    request.set_departname(departName);
    request.set_username(userName);
    DepartProto::DelDepartResponse response;
    stub.DelDepart(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = DEL_DEPART_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::addDepart(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string departName = data["departName"].asString();
    std::string userName = data["userName"].asString();
    std::string userRole = data["userRole"].asString();

    DepartProto::DepartServiceRpc_Stub stub(new MprpcChannel());
    DepartProto::AddDepartRequest request;
    request.set_departname(departName);
    request.set_username(userName);
    request.set_userrole(userRole);
    DepartProto::AddDepartResponse response;
    stub.AddDepart(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = ADD_DEPART_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    result["departName"] = departName;
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::quitDepart(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string departName = data["departName"].asString();
    std::string userName = data["userName"].asString();

    DepartProto::DepartServiceRpc_Stub stub(new MprpcChannel());
    DepartProto::QuitDepartRequest request;
    request.set_departname(departName);
    request.set_username(userName);
    DepartProto::QuitDepartResponse response;
    stub.QuitDepart(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUIT_DEPART_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::queryDepart(const muduo::net::TcpConnectionPtr &tcpconn, Json::Value &data)
{
    std::string userName = data["userName"].asString();
    std::string departName = data["departName"].asString();

    DepartProto::DepartServiceRpc_Stub stub(new MprpcChannel());
    DepartProto::QueryDepartRequest request;
    request.set_username(userName);
    DepartProto::QueryDepartResponse response;
    stub.QueryDepart(nullptr, &request, &response, nullptr);
    Json::Value result;
    result["msgid"] = QUERY_DEPART_USERS_MSG;
    result["errcode"] = response.result().errcode();
    result["errmsg"] = response.result().errmsg();

    result["departName"] = response.departs().departname();
    result["departDesc"] = response.departs().departdesc();
    for (auto &user : response.departs().departusers())
    {
        Json::Value temp;
        temp["userName"] = user.username();
        temp["userEmail"] = user.useremail();
        temp["userPhone"] = user.userphone();
        temp["userRole"] = user.userrole();
        result["users"].append(temp);
    }
    std::string str = Json::FastWriter().write(result);
    str += "@@@";
    tcpconn->send(str);
}

void MsgServer::clientCloseException(const muduo::net::TcpConnectionPtr &conn)
{
    std::string userName;
    {
        std::lock_guard<std::mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); it++)
        {
            if (it->second == conn)
            {
                userName = it->first;
                _userConnMap.erase(it);
                break;
            }
        }
    }

    if (userName != "")
    {
        _redis.unsubscribe(userName);
        LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
        LoginProto::LogoutRequest request;
        request.set_name(userName);
        LoginProto::LogoutResponse response;
        stub.Logout(nullptr, &request, &response, nullptr);
    }
}

void MsgServer::allReset()
{

    {
        std::lock_guard<std::mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); it++)
        {
            std::string userName;
            userName = it->first;
            _userConnMap.erase(it);
            _redis.unsubscribe(userName);
            LoginProto::UserServiceRpc_Stub stub(new MprpcChannel());
            LoginProto::LogoutRequest request;
            request.set_name(userName);
            LoginProto::LogoutResponse response;
            stub.Logout(nullptr, &request, &response, nullptr);
        }
    }
}

MsgHandler MsgServer::getHandler(int msgid)
{
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end())
    {
        return [=](const muduo::net::TcpConnectionPtr &conn, Json::Value &data)
        {
            std::cout << "msgid: " << msgid << "can not find handler!";
        };
    }
    else
    {
        return _msgHandlerMap[msgid];
    }
}

void MsgServer::handleRedisSubscribeMessage(std::string name, std::string msg)
{
    // std::cout << "----------------------------------" << std::endl;
    // std::cout << name << "-" << msg << std::endl;
    std::lock_guard<std::mutex> lock(_connMutex);
    auto it = _userConnMap.find(name);
    if (it != _userConnMap.end())
    {
        it->second->send(msg);
        return;
    }
}