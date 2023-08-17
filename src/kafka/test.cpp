#include "test.h"

#include <iostream>
#include <unordered_map>

Test::Test(std::string brokers, std::string groupId)
    : producer_(brokers), consumer_(brokers, groupId) {}

void Test::handlePrivatrMessage(const std::string &userName, const std::string &message)
{
    std::cout << "Received private message for user " << userName << ":"
              << message << std::endl;
}

void Test::handleGroupMessage(const std::string &groupName, const std::string &message)
{
    std::cout << "Received group message for group " << groupName << ":"
              << message << std::endl;
}

void Test::start()
{
    consumer_.consume("user_topic", [&](const std::string &message, const std::string &key)
                      {
        if(!key.empty())
        {
            handlePrivatrMessage(key,message);
        } });

    consumer_.consume("group_topic", [&](const std::string &message, const std::string &key)
                      {
        if(!key.empty())
        {
            handleGroupMessage(key,message);
        } });
}