#ifndef __REDISDB_H__
#define __REDISDB_H__

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <hiredis/hiredis.h>

class RedisClient
{
public:
    static RedisClient *getInstance();

    bool initConnectionPool();

    bool setData(const std::string &command, const std::string &key, const std::string &value);

    bool getData(const std::string &command, const std::string &key, const std::string &field, std::string &value);

    bool getSetData(const std::string &key, std::vector<std::string>& vec);

    bool delData(const std::string &command, const std::string &key);

    void disConnect();

private:
    RedisClient();

    ~RedisClient();

    redisContext *getConnection();

    void releaseConnection(redisContext *conn);

    size_t poolSize_;
    std::queue<redisContext *> connectionPool_;
    std::mutex poolMutex_;
};

#endif