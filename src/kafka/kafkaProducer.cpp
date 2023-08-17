#include "KafkaProducer.h"

#include <iostream>
#include <stdexcept>
#include <librdkafka/rdkafkacpp.h>
#include <librdkafka/rdkafka.h>

class KafkaProducer::KafkaProducerImpl
{
public:
    KafkaProducerImpl(std::string brokers)
    {
        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        std::string errstr;

        if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
        {
            delete conf;
            throw std::runtime_error("Failed to set broker list: " + errstr);
        }

        if (conf->set("acks", "all", errstr) != RdKafka::Conf::CONF_OK)
        {
            delete conf;
            throw std::runtime_error("Failed to set 'acks' configuration: " + errstr);
        }

        producer_ = RdKafka::Producer::create(conf, errstr);
        if (!producer_)
        {
            delete conf;
            throw std::runtime_error("Failed to create Kafka producer: " + errstr);
        }
    }

    ~KafkaProducerImpl()
    {
        if (producer_)
        {
            producer_->flush(5000);
            delete producer_;
            producer_ = nullptr;
        }
    }

    bool send(const std::string &topic, const std::string &key, const std::string &message)
    {
        std::string resp;
        RdKafka::Topic *topic_ptr = RdKafka::Topic::create(producer_, topic, nullptr, resp);
        if (!topic_ptr)
        {
            std::cerr << "Failed to create Kafka topic: " << resp << std::endl;
            return false;
        }

        RdKafka::Headers *headers = RdKafka::Headers::create();
        if (key.size() > 0)
        {
            headers->add("key", key.data(), key.size());
        }

        RdKafka::ErrorCode send_resp = producer_->produce(
            topic_ptr, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY,
            const_cast<char *>(message.c_str()), message.size(), nullptr, 0, 0, headers);

        producer_->poll(0);

        return send_resp == RdKafka::ERR_NO_ERROR;
    }

private:
    RdKafka::Producer *producer_;
};

KafkaProducer::KafkaProducer(std::string brokers)
    : impl_(new KafkaProducerImpl(brokers)) {}

bool KafkaProducer::sendToTopic(const std::string &topic, const std::string &key, const std::string &message)
{
    std::string topic = "user_topic";
    return impl_->send(topic, key, message);
}
