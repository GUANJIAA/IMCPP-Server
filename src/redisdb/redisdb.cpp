#include "redisdb.h"

#include "mprpcapplication.h"

RedisClient *RedisClient::getInstance()
{
    static RedisClient *instance;
    if (instance == nullptr)
    {
        instance = new RedisClient();
    }
    return instance;
}

bool RedisClient::initConnectionPool()
{
    std::string server = MprpcApplication::GetInstance().GetConfig().Load("redisServerIp");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("redisServerPort");
    std::string poolSize = MprpcApplication::GetInstance().GetConfig().Load("redisPoolSize");
    poolSize_ = atoi(poolSize.c_str());
    for (size_t i = 0; i < poolSize_; ++i)
    {
        redisContext *con = redisConnect(server.c_str(), atoi(port.c_str()));
        if (con == nullptr || con->err)
        {
            if (con)
            {
                std::cerr << "Connection error: " << con->errstr << std::endl;
            }
            else
            {
                std::cerr << "Connection error: Can't allocate redis context" << std::endl;
            }
            return false;
        }
        connectionPool_.push(con);
    }
    return true;
}

bool RedisClient::setData(const std::string &command, const std::string &key, const std::string &value)
{
    redisContext *conn = getConnection();
    if (conn == nullptr)
    {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "%s %s %s", command.c_str(), key.c_str(), value.c_str());
    releaseConnection(conn);

    if (reply == nullptr)
    {
        std::cerr << "SET error: " << conn->errstr << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisClient::getData(const std::string &command, const std::string &key, const std::string &field, std::string &value)
{
    redisContext *conn = getConnection();
    if (conn == nullptr)
    {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "%s %s %s", command.c_str(), key.c_str(), field.c_str());
    releaseConnection(conn);

    if (reply == nullptr)
    {
        std::cerr << "GET error: " << conn->errstr << std::endl;
        return false;
    }
    if (reply->type == REDIS_REPLY_STRING)
    {
        value = reply->str;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisClient::getSetData(const std::string &key, std::vector<std::string> &vec)
{
    redisContext *conn = getConnection();
    if (conn == nullptr)
    {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "SMEMBERS %s", key.c_str());
    releaseConnection(conn);

    if (reply == nullptr)
    {
        std::cerr << "SMEMBERS error: " << conn->errstr << std::endl;
        return false;
    }
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        for (size_t i = 0; i < reply->elements; ++i)
        {
            vec.push_back(reply->element[i]->str);
        }
    }
    else
    {
        std::cout << reply->type << std::endl;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisClient::delData(const std::string &command, const std::string &key)
{
    redisContext *conn = getConnection();
    if (conn == nullptr)
    {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "%s %s", command.c_str(), key.c_str());
    releaseConnection(conn);

    if (reply == nullptr)
    {
        std::cerr << "DEL error: " << conn->errstr << std::endl;
        return false;
    }
    std::cout << "DEL " << key << ": " << reply->integer << " keys deleted" << std::endl;
    freeReplyObject(reply);
    return true;
}

void RedisClient::disConnect()
{
    while (!connectionPool_.empty())
    {
        redisContext *con = connectionPool_.front();
        redisFree(con);
        connectionPool_.pop();
    }
}

RedisClient::RedisClient()
    : poolSize_(0) {}

RedisClient::~RedisClient()
{
    disConnect();
}

redisContext *RedisClient::getConnection()
{
    std::unique_lock<std::mutex> lock(poolMutex_);
    if (connectionPool_.empty())
    {
        return nullptr;
    }
    redisContext *conn = connectionPool_.front();
    connectionPool_.pop();
    return conn;
}

void RedisClient::releaseConnection(redisContext *conn)
{
    if (conn != nullptr)
    {
        std::unique_lock<std::mutex> lock(poolMutex_);
        connectionPool_.push(conn);
    }
}