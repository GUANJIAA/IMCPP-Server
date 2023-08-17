#include "groupmodel.h"

#include "mysqldb.h"

bool GroupModel::createGroup(Group &group)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup(`groupname`,`groupdesc`) values('%s','%s')",
            group.getName().c_str(), group.getDesc().c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        group.setId(mysql_insert_id(mysql->getConnection()));
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool GroupModel::addGroup(std::string groupName, std::string userName, std::string userRole)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into `groupuser`(`groupname`,`username`,`userrole`) values('%s','%s','%s')",
            groupName.c_str(), userName.c_str(), userRole.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool GroupModel::quitGroup(std::string groupName, std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from `groupuser` where `userName` = '%s' and `groupName` = '%s'",
            userName.c_str(), groupName.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

std::vector<Group> GroupModel::queryGroup(std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.groupname,a.groupdesc from \
            allgroup a inner join groupuser b on a.groupname = b.groupname \
            where b.username = '%s'",
            userName.c_str());

    std::vector<Group> groupVec;

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            Group group;
            group.setId(atoi(row[0]));
            group.setName(row[1]);
            group.setDesc(row[2]);
            groupVec.push_back(group);
        }
        mysql_free_result(res);
    }

    for (Group &group : groupVec)
    {
        sprintf(sql, "select a.id,a.name,a.status,a.email,a.phone,b.userrole from Admin a\
                inner join groupuser b on a.name=b.username where b.groupname = '%s'",
                group.getName().c_str());

        MYSQL_RES *res = mysql->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setStatus(row[2]);
                user.setEmail(row[3]);
                user.setPhone(row[4]);
                user.setRole(row[5]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return groupVec;
}

Group GroupModel::queryGroupUsers(std::string groupName, std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "select username from groupuser where groupname = '%s' and username != '%s'",
            groupName.c_str(), userName.c_str());

    Group group;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            GroupUser user;
            user.setId(atoi(row[0]));
            user.setName(row[1]);
            user.setStatus(row[2]);
            user.setEmail(row[3]);
            user.setPhone(row[4]);
            user.setRole(row[5]);
            group.getUsers().push_back(user);
        }
        mysql_free_result(res);
    }
    return group;
}
