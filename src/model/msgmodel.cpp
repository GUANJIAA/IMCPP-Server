#include "msgmodel.h"

#include "mysqldb.h"

#include<iostream>

bool ChatMsgModel::addChatMsg(std::string recvName, std::string sendName,
                              std::string message, std::string isRead)
{
    // std::cout<<recvName<<":"<<sendName<<":"<<message<<":"<<isRead<<std::endl;

    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO `chatmessage`(`recvname`,`sendname`,`message`,`isread`) \
            VALUES ('%s','%s','%s','%s')",
            recvName.c_str(), sendName.c_str(), message.c_str(), isRead.c_str());
    std::cout<<sql<<std::endl;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool ChatMsgModel::delChatMsg(int msgId, std::string recvName,
                              std::string sendName, std::string message)
{
    char sql[1024] = {0};
    sprintf(sql, "DELETE FROM `chatmessage` WHERE `msgid` = %d AND\
            `recvname` = '%s' AND `sendname` = '%s' AND `message` = '%s'",
            msgId, recvName.c_str(), sendName.c_str(), message.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

std::vector<Msg> ChatMsgModel::queryChatMsg(std::string recvName,std::string sendName)
{
    char sql[1024] = {0};
    sprintf(sql, "SELECT * FROM `chatmessage` WHERE (`recvName` = '%s' \
            AND `sendName` = '%s') OR (`recvName` = '%s' AND `sendName` = '%s')",
            recvName.c_str(),sendName.c_str(),sendName.c_str(),recvName.c_str());

    std::vector<Msg> MsgVec;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            Msg temp;
            temp.setMsgId(atoi(row[0]));
            temp.setRecvName(row[1]);
            temp.setSendName(row[2]);
            temp.setMessage(row[3]);
            MsgVec.push_back(temp);
        }
        mysql_free_result(res);
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return MsgVec;
}

std::vector<Msg> ChatMsgModel::queryOfflineMsg(std::string recvName)
{
    char sql[1024] = {0};
    sprintf(sql, "SELECT * FROM `chatmessage` WHERE `recvname` = '%s' AND `isread` = 'false'",
            recvName.c_str());

    std::vector<Msg> offlineMsgVec;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            Msg temp;
            temp.setMsgId(atoi(row[0]));
            temp.setRecvName(row[1]);
            temp.setSendName(row[2]);
            temp.setMessage(row[3]);
            offlineMsgVec.push_back(temp);
        }
        mysql_free_result(res);
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return offlineMsgVec;
}

bool ChatMsgModel::addGroupChatMsg(std::string groupName, std::string sendName,
                                   std::string message)
{
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO `chatgroupmessage`(`groupname`,`sendname`,`message`) \
            values ('%s','%s','%s')",
            groupName.c_str(), sendName.c_str(), message.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool ChatMsgModel::delGroupChatMsg(int msgId, std::string groupName,
                                   std::string sendName, std::string message)
{
    char sql[1024] = {0};
    sprintf(sql, "DELETE FROM `chatgroupmessage` WHERE `msgid` = %d \
            AND `groupname` = '%s' AND `sendname` = '%s' AND `message` = '%s'",
            msgId, groupName.c_str(), sendName.c_str(), message.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}
std::vector<GroupMsg> ChatMsgModel::queryGroupChatMsg(std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "SELECT c.msgid,c.groupname,c.sendname,c.message FROM `chatgroupmessage` c\
            INNER JOIN `groupuser` g ON c.groupname = g.groupname WHERE g.username = '%s'",
            userName.c_str());

    std::vector<GroupMsg> chatGroupMsgVec;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            GroupMsg temp;
            temp.setMsgId(atoi(row[0]));
            temp.setGroupName(row[1]);
            temp.setSendName(row[2]);
            temp.setMessage(row[3]);
            chatGroupMsgVec.push_back(temp);
        }
        mysql_free_result(res);
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return chatGroupMsgVec;
}

bool ChatMsgModel::addDepartChatMsg(std::string departName, std::string sendName,
                                    std::string message)
{
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO `chatdepartmessage`(`departname`,`sendname`,`message`) \
            VALUES ('%s','%s','%s')",
            departName.c_str(), sendName.c_str(), message.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool ChatMsgModel::delDepartChatMsg(int msgId, std::string departName,
                                    std::string sendName, std::string message)
{
    char sql[1024] = {0};
    sprintf(sql, "DELETE FROM `chatdepartmessage` WHERE `msgid` = %d\
            AND `departname` = '%s' AND `sendname` = '%s' AND `message` = '%s'",
            msgId, departName.c_str(), sendName.c_str(), message.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

std::vector<DepartMsg> ChatMsgModel::queryDepartChatMsg(std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "SELECT c.msgid,c.departname,c.sendname,c.message FROM `chatdepartmessage` c\
            INNER JOIN `departuser` d ON c.departname = d.departname WHERE d.username = '%s'",
            userName.c_str());

    std::vector<DepartMsg> DepartMsgVec;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            DepartMsg temp;
            temp.setMsgId(atoi(row[0]));
            temp.setDepartName(row[1]);
            temp.setSendName(row[2]);
            temp.setMessage(row[3]);
            DepartMsgVec.push_back(temp);
        }
        mysql_free_result(res);
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return DepartMsgVec;
}

// bool OfflineMsgModel::addOfflineMsg(std::string recvName, std::string sendName,
//                                     std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "insert into `chatmessage`(`recvname`,`sendname`,`message`) values ('%s','%s','%s')",
//             recvName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// bool OfflineMsgModel::delOfflineMsg(int msgId, std::string recvName,
//                                     std::string sendName, std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "DELETE FROM `chatmessage` WHERE `msgid` = %d \
//             `recvname` = '%s' AND `sendname` = '%s' AND `message` = '%s' AND `isread` = 'false'",
//             msgId, recvName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// std::vector<Msg> OfflineMsgModel::queryOfflineMsg(std::string recvName)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "SELECT * FROM `chatmessage` WHERE `recvname` = '%s'",
//             recvName.c_str());

//     std::vector<Msg> offlineMsgVec;
//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     MYSQL_RES *res = mysql->query(sql);
//     if (res != nullptr)
//     {
//         MYSQL_ROW row;
//         while ((row = mysql_fetch_row(res)) != nullptr)
//         {
//             Msg temp;
//             temp.setMsgId(atoi(row[0]));
//             temp.setRecvName(row[1]);
//             temp.setSendName(row[2]);
//             temp.setMessage(row[3]);
//             offlineMsgVec.push_back(temp);
//         }
//         mysql_free_result(res);
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return offlineMsgVec;
// }

// bool OfflineMsgModel::addOfflineGroupMsg(std::string groupName, std::string sendName,
//                                          std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "insert into `chatgroupmessage`(`groupname`,`sendname`,`message`) values ('%s','%s','%s')",
//             groupName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// bool OfflineMsgModel::delOfflineGroupMsg(int msgId, std::string groupName,
//                                          std::string sendName, std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "DELETE FROM `chatgroupmessage` WHERE `msgid` = %d \
//             AND `groupname` = '%s' AND `sendname` = '%s' AND `message` = '%s'",
//             msgId, groupName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// std::vector<GroupMsg> OfflineMsgModel::queryOfflineGroupMsg(std::string userName)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "select o.msgid,o.groupname,o.sendname,o.message from `chatgroupmessage` o\
//             inner join `groupuser` a on o.groupname = a.groupname where a.username = '%s'",
//             userName.c_str());

//     std::vector<GroupMsg> offlineGroupMsgVec;
//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     MYSQL_RES *res = mysql->query(sql);
//     if (res != nullptr)
//     {
//         MYSQL_ROW row;
//         while ((row = mysql_fetch_row(res)) != nullptr)
//         {
//             GroupMsg temp;
//             temp.setMsgId(atoi(row[0]));
//             temp.setGroupName(row[1]);
//             temp.setSendName(row[2]);
//             temp.setMessage(row[3]);
//             offlineGroupMsgVec.push_back(temp);
//         }
//         mysql_free_result(res);
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return offlineGroupMsgVec;
// }

// bool OfflineMsgModel::addOfflineDepartMsg(std::string departName, std::string sendName,
//                                           std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "insert into `chatdepartmessage`(`departname`,`sendname`,`message`) values ('%s','%s','%s')",
//             departName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// bool OfflineMsgModel::delOfflineDepartMsg(int msgId, std::string departName,
//                                           std::string sendName, std::string message)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "DELETE FROM `chatdepartmessage` WHERE `msgid` = %d \
//             AND `departname` = '%s' AND `sendname` = '%s' AND `message` = '%s'",
//             msgId, departName.c_str(), sendName.c_str(), message.c_str());

//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     bool result = false;
//     if (mysql->update(sql))
//     {
//         result = true;
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return result;
// }

// std::vector<DepartMsg> OfflineMsgModel::queryOfflineDepartMsg(std::string userName)
// {
//     char sql[1024] = {0};
//     sprintf(sql, "select o.msgid,o.departname,o.sendname,o.message from `chatdepartmessage` o\
//             inner join `departuser` a on o.departname = a.departname where a.username = '%s'",
//             userName.c_str());

//     std::vector<DepartMsg> offlineDepartMsgVec;
//     MySQL *mysql = connection_pool::GetInstance()->GetConnection();
//     MYSQL_RES *res = mysql->query(sql);
//     if (res != nullptr)
//     {
//         MYSQL_ROW row;
//         while ((row = mysql_fetch_row(res)) != nullptr)
//         {
//             DepartMsg temp;
//             temp.setMsgId(atoi(row[0]));
//             temp.setDepartName(row[1]);
//             temp.setSendName(row[2]);
//             temp.setMessage(row[3]);
//             offlineDepartMsgVec.push_back(temp);
//         }
//         mysql_free_result(res);
//     }
//     connection_pool::GetInstance()->ReleaseConnection(mysql);
//     return offlineDepartMsgVec;
// }