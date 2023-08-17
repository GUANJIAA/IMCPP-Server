#ifndef __KAFKAPRODUCER_H__
#define __KAFKAPRODUCER_H__

#include <string>

class KafkaProducer
{
public:
    KafkaProducer(std::string brokers);

    bool sendToTopic(const std::string &topic, const std::string &key, const std::string &message);

private:
    class KafkaProducerImpl;
    KafkaProducerImpl *impl_;
};

#endif