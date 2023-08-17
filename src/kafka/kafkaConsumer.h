#ifndef __KAFKACONSUMER_H__
#define __KAFKACONSUMER_H__

#include <string>
#include <functional>

class KafkaConsumer
{
public:
    KafkaConsumer(std::string brokers, std::string groupId);

    void consume(const std::string &topic, std::function<void(const std::string &, const std::string &)> callback);

private:
    class KafkaConsumerImpl;
    KafkaConsumerImpl *impl_;
};

#endif