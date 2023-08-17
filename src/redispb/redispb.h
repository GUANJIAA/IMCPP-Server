#ifndef __REDISPB_H__
#define __REDISPB_H__

#include "mprpcapplication.h"

#include <thread>
#include <functional>
#include <hiredis/hiredis.h>

class RedisPb
{
public:
    RedisPb();
    ~RedisPb();

    bool publishConnect();
    bool subscribeConnect();

    bool ischannel(std::string channel);

    bool publish(std::string channel, std::string message);
    bool subscribe(std::string channel);
    bool unsubscribe(std::string channel);

    void observer_channel_message();
    void init_notify_handler(std::function<void(std::string, std::string)> fn);

private:
    redisContext *_publish_context;
    redisContext *_subsrcibe_context;

    std::function<void(std::string, std::string)> _notify_message_handler;
};

#endif