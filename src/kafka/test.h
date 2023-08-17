#ifndef __TEST_H__
#define __TEST_H__

#include "KafkaProducer.h"
#include "kafkaConsumer.h"

class Test
{
public:
    Test(std::string brokers, std::string groupId);

    void handlePrivatrMessage(const std::string &userName, const std::string &message);
    void handleGroupMessage(const std::string &groupName, const std::string &message);

    void start();

private:
    KafkaProducer producer_;
    KafkaConsumer consumer_;
};

#endif