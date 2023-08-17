#include "FriendService.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "mysqldb.h"
#include "redisdb.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    connection_pool::GetInstance()->init();
    RedisClient::getInstance()->initConnectionPool();

    RpcProvider provider;
    provider.NotifyService(new FriendService());

    provider.Run();

    return 0;
}