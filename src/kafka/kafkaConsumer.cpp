#include "kafkaConsumer.h"

#include <stdexcept>
#include <iostream>
#include <librdkafka/rdkafka.h>
#include <librdkafka/rdkafkacpp.h>

class KafkaConsumer::KafkaConsumerImpl
{
public:
    KafkaConsumerImpl(std::string brokers, std::string groupId)
    {
        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        std::string errstr;

        if (conf->set("boostrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
        {
            delete conf;
            throw std::runtime_error("Failed to set broker list: " + errstr);
        }

        if (conf->set("group.id", groupId, errstr) != RdKafka::Conf::CONF_OK)
        {
            delete conf;
            throw std::runtime_error("Failed to set group ID: " + errstr);
        }

        if (conf->set("enable.auto.commit", "true", errstr) != RdKafka::Conf::CONF_OK ||
            conf->set("auto.commit.interval.ms", "1000", errstr) != RdKafka::Conf::CONF_OK)
        {
            delete conf;
            throw std::runtime_error("Failed to set 'enable.auto.commit' configuration: " + errstr);
        }

        consumer_ = RdKafka::Consumer::create(conf, errstr);
        if (!consumer_)
        {
            delete conf;
            throw std::runtime_error("Failed to create Kafka consumer: " + errstr);
        }

        delete conf;
    }

    ~KafkaConsumerImpl()
    {
        if (consumer_)
        {
            // consumer_->close();
            delete consumer_;
            consumer_ = nullptr;
        }
    }

    void consume(const std::string &topic, std::function<void(const std::string &, const std::string &)> callback)
    {
        RdKafka::Topic *topic_ptr = RdKafka::Topic::create(consumer_, topic, nullptr, errstr_);
        if (!topic_ptr)
        {
            throw std::runtime_error("Failed to create Kafka topic: " + errstr_);
        }

        RdKafka::ErrorCode resp = consumer_->start(topic_ptr, 0, RdKafka::Topic::OFFSET_END);
        if (resp != RdKafka::ERR_NO_ERROR)
        {
            throw std::runtime_error("Failed to start consuming: " + RdKafka::err2str(resp));
        }

        while (true)
        {
            RdKafka::Message *msg = consumer_->consume(topic_ptr, 0, 1000);
            if (msg->err() == RdKafka::ERR_NO_ERROR)
            {
                if (msg->len() > 0)
                {
                    std::string key = msg->key() ? static_cast<const char *>(msg->key()->data()) : "";
                    std::string message = std::string(static_cast<const char *>(msg->payload()), msg->len());
                    callback(message, key);
                }
            }
            else
            {
                std::cerr << "Error while consuming message: " << RdKafka::err2str(msg->err()) << std::endl;
            }
            delete msg;
        }
    }

private:
    RdKafka::Consumer *consumer_;
    std::string errstr_;
};

KafkaConsumer::KafkaConsumer(std::string brokers, std::string groupId)
    : impl_(new KafkaConsumerImpl(brokers, groupId)) {}

void KafkaConsumer::consume(const std::string &topic,
                            std::function<void(const std::string &, const std::string &)> callback)
{
    impl_->consume(topic, callback);
}