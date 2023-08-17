#include "adminModel.h"
#include "logger.h"

#include <iostream>

bool AdminModel::insert(Admin &admin)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into `Admin`(`name`,`pwd`,`email`,`phone`) \
            values('%s','%s','%s','%s')",
            admin.getName().c_str(), admin.getPassword().c_str(), 
            admin.getEmail().c_str(), admin.getPhone().c_str());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        admin.setId(mysql_insert_id(mysql->getConnection()));
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

Admin AdminModel::query(std::string name)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from `Admin` where `name` = '%s'", name.c_str());
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    MYSQL_RES *res = mysql->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row != nullptr)
        {
            Admin admin;
            admin.setId((uint8_t)atoi(row[0]));
            admin.setName(row[1]);
            admin.setPassword(row[2]);
            admin.setStatus(row[3]);
            admin.setEmail(row[4]);
            admin.setPhone(row[5]);
            admin.setDesc(row[6]);
            admin.setDepartName(row[7]);

            if (res != nullptr)
            {
                mysql_free_result(res);
            }
            connection_pool::GetInstance()->ReleaseConnection(mysql);
            return admin;
        }
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return Admin();
}

bool AdminModel::update(Admin &admin)
{
    char sql[1024] = {0};

    sprintf(sql, "UPDATE `Admin` SET `pwd` = '%s',`phone` = '%s',\
            `email` = '%s',`desc` = '%s',`depart` = '%s'\
             WHERE `name` = '%s'",
            admin.getPassword().c_str(), admin.getPhone().c_str(),
            admin.getEmail().c_str(), admin.getDesc().c_str(),
            admin.getDepartName().c_str(),admin.getName().c_str());
    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool AdminModel::updateState(Admin &admin)
{
    char sql[1024] = {0};
    sprintf(sql, "update `Admin` set `status` = '%s' where `id` = %d",
            admin.getStatus().c_str(), admin.getId());

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}

bool AdminModel::resetState()
{
    char sql[1024] = {0};
    sprintf(sql, "update `Admin` set `status` = 'offline' where `status` = 'online'");

    MySQL *mysql = connection_pool::GetInstance()->GetConnection();
    bool result = false;
    if (mysql->update(sql))
    {
        result = true;
    }
    connection_pool::GetInstance()->ReleaseConnection(mysql);
    return result;
}