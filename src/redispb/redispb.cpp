#include "redispb.h"

#include <iostream>
#include <thread>

RedisPb::RedisPb()
    : _publish_context(nullptr), _subsrcibe_context(nullptr) {}

RedisPb::~RedisPb()
{
    if (_publish_context != nullptr)
    {
        redisFree(_publish_context);
    }
    if (_subsrcibe_context != nullptr)
    {
        redisFree(_subsrcibe_context);
    }
}

bool RedisPb::publishConnect()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("redisPBServerIp");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("redisPBServerPort");

    _publish_context = redisConnect(ip.c_str(), atoi(port.c_str()));
    if (nullptr == _publish_context)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    std::cout << "connect redis-server success!" << std::endl;

    return true;
}

bool RedisPb::subscribeConnect()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("redisPBServerIp");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("redisPBServerPort");
    _subsrcibe_context = redisConnect(ip.c_str(), atoi(port.c_str()));
    if (nullptr == _subsrcibe_context)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    // 在单独的线程中，监听通道上的事件，有消息就给业务层上报
    std::thread t([&]()
                  { observer_channel_message(); });
    t.detach();

    std::cout << "connect redis-server success!" << std::endl;

    return true;
}

bool RedisPb::ischannel(std::string channel)
{
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBSUB NUMSUB %s", channel.c_str());

    if (reply->element[1]->integer != 0)
    {
        return true;
    }
    return false;
}

bool RedisPb::publish(std::string channel, std::string message)
{
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %s %s", channel.c_str(), message.c_str());
    if (nullptr == reply)
    {
        std::cerr << "publish command failed!" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisPb::subscribe(std::string channel)
{
    if (REDIS_ERR == redisAppendCommand(this->_subsrcibe_context, "SUBSCRIBE %s", channel.c_str()))
    {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }
    // redisBufferWrite可以循环发送缓冲区，知道缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subsrcibe_context, &done))
        {
            std::cerr << "subscribe command failed!" << std::endl;
            return false;
        }
    }

    return true;
}

bool RedisPb::unsubscribe(std::string channel)
{
    if (REDIS_ERR == redisAppendCommand(this->_subsrcibe_context, "UNSUBSCRIBE %s", channel.c_str()))
    {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }
    // redisBufferWrite可以循环发送缓冲区，知道缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subsrcibe_context, &done))
        {
            std::cerr << "subscribe command failed!" << std::endl;
            return false;
        }
    }

    return true;
}

void RedisPb::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->_subsrcibe_context, (void **)&reply))
    {
        // 订阅收到的消息是一个带三元素的数组
        if (reply != nullptr && reply->element[1] != nullptr && reply->element[2]->str != nullptr)
        {
            // 给业务层上报通道上发生的消息
            _notify_message_handler(reply->element[1]->str, reply->element[2]->str);
            // std::cout << reply->element[1]->str << "-" << reply->element[2]->str << std::endl;
        }
        freeReplyObject(reply);
    }
    std::cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << std::endl;
}
void RedisPb::init_notify_handler(std::function<void(std::string, std::string)> fn)
{
    this->_notify_message_handler = fn;
}