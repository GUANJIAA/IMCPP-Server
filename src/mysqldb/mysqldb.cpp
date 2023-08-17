#include "mysqldb.h"

#include "mprpcapplication.h"
#include "logger.h"

#include <string>

MySQL::MySQL()
{
    m_conn = mysql_init(nullptr);
    if (m_conn == nullptr)
    {
        LOG_ERROR("mysql init fail");
    }
}

MySQL::~MySQL()
{
    if (m_conn != nullptr)
    {
        mysql_close(m_conn);
    }
}

bool MySQL::connect()
{
    std::string server = MprpcApplication::GetInstance().GetConfig().Load("mysqlserverIp");
    std::string user = MprpcApplication::GetInstance().GetConfig().Load("mysqldbAccount");
    std::string password = MprpcApplication::GetInstance().GetConfig().Load("mysqldbPwd");
    std::string dbname = MprpcApplication::GetInstance().GetConfig().Load("mysqldbName");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("mysqlserverPort");

    MYSQL *p = mysql_real_connect(m_conn, server.c_str(), user.c_str(), password.c_str(),
                                  dbname.c_str(), atoi(port.c_str()), nullptr, 0);
    if (p != nullptr)
    {
        mysql_query(m_conn, "set names gbk");
        LOG_INFO("connect mysql success");
        return true;
    }
    else
    {
        LOG_ERROR("connect mysql fail!");
        return false;
    }
}

bool MySQL::update(std::string sql)
{
    if (mysql_query(m_conn, sql.c_str()))
    {
        LOG_INFO("update fail!");
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::query(std::string sql)
{
    if (mysql_query(m_conn, sql.c_str()))
    {
        LOG_INFO("query fail!");
        return nullptr;
    }
    return mysql_use_result(m_conn);
}

MYSQL *MySQL::getConnection()
{
    return m_conn;
}

void connection_pool::init()
{
    int maxConn = atoi(MprpcApplication::GetInstance().GetConfig().Load("MysqldbPoolSize").c_str());
    for (int i = 0; i < maxConn; ++i)
    {
        MySQL *con(new MySQL);
        if (!con->connect())
        {
            continue;
        }
        connList.push_back(con);
        ++m_FreeConn;
    }
    sem_init(&m_sem, 0, m_FreeConn);
    pthread_mutex_init(&m_mutex, NULL);
}

// 当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MySQL *connection_pool::GetConnection()
{
    MySQL *con = NULL;
    if (connList.size() == 0)
    {
        return NULL;
    }
    sem_wait(&m_sem);

    pthread_mutex_lock(&m_mutex);
    con = connList.front();
    connList.pop_front();
    --m_FreeConn;
    ++m_CurConn;
    pthread_mutex_unlock(&m_mutex);

    return con;
}

bool connection_pool::ReleaseConnection(MySQL *con)
{
    if (con == NULL)
    {
        return false;
    }

    pthread_mutex_lock(&m_mutex);
    connList.push_back(con);
    ++m_FreeConn;
    --m_CurConn;
    pthread_mutex_unlock(&m_mutex);

    sem_post(&m_sem);
    return true;
}

void connection_pool::DestroyPool()
{
    pthread_mutex_lock(&m_mutex);
    if (connList.size() > 0)
    {
        for (auto it = connList.begin(); it != connList.end(); it++)
        {
            MySQL *con = *it;
            mysql_close(con->getConnection());
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear();
    }
    pthread_mutex_unlock(&m_mutex);
}