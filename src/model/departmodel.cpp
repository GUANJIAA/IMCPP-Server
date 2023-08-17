#include "departmodel.h"

#include "mysqldb.h"
#include <iostream>

bool DepartModel::createDepart(Depart &depart)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into alldepart(`departname`,`departdesc`) values('%s','%s')",
            depart.getName().c_str(), depart.getDesc().c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        depart.setId(mysql_insert_id(mysql->getConnection()));
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool DepartModel::addDepart(std::string departName, std::string userName, std::string userRole)
{
    bool result = false;

    result = quitDepart(userName);

    char sql[1024] = {0};

    sprintf(sql, "insert into `departuser`(`departName`,`userName`,`userRole`) values('%s','%s','%s')",
            departName.c_str(), userName.c_str(), userRole.c_str());

    //std::cout<<sql<<std::endl;

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();

    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool DepartModel::delDepart(std::string departName, std::string userName)
{
    char sql[1024] = {0};

    sprintf(sql, "delete from `alldepart` where `departName` = '%s'",
            departName.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }

    sprintf(sql, "delete from `departuser` where `departName` = '%s'",
            departName.c_str());

    if (mysql->update(sql))
    {
        result = true;
    }

    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool DepartModel::quitDepart(std::string userName)
{
    char sql[1024] = {0};

    sprintf(sql, "delete from `departuser` where `username` = '%s'",
            userName.c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }

    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

Depart DepartModel::queryDepart(std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.departname,a.departdesc from \
            alldepart a inner join departuser b on a.departname = b.departname \
            where b.username = '%s'",
            userName.c_str());

    Depart depart;

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    {

        MYSQL_RES *res = mysql->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                depart.setId(atoi(row[0]));
                depart.setName(row[1]);
                depart.setDesc(row[2]);
            }
            mysql_free_result(res);
        }
    }

    {
        sprintf(sql, "select a.id,a.name,a.status,a.email,a.phone,b.userrole from Admin a\
                inner join departuser b on a.name=b.username where b.departname = '%s'",
                depart.getName().c_str());

        MYSQL_RES *res = mysql->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                DepartUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setStatus(row[2]);
                user.setEmail(row[3]);
                user.setPhone(row[4]);
                user.setRole(row[5]);
                depart.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return depart;
}

std::vector<std::string> DepartModel::queryDepartUsers(std::string departName, std::string userName)
{
    char sql[1024] = {0};
    sprintf(sql, "select username from departuser where departname = '%s' and username != '%s'",
            departName.c_str(), userName.c_str());

    std::vector<std::string> nameVec;
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            nameVec.push_back(row[0]);
        }
        mysql_free_result(res);
    }
    return nameVec;
}
