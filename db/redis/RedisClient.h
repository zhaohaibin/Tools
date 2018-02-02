//
// Created by zhaohaibin on 1/30/18.
//

#ifndef REDISTEST_REDISCLIENT_H
#define REDISTEST_REDISCLIENT_H

#include <string>
#include <hiredis/hiredis.h>

using namespace std;

enum REDIS_STATUS
{
    REDIS_STATUS_ERROR,
    REDIS_STATUS_OK,
    REDIS_STATUS_TIMEOUT
};

class RedisClient
{
public:
    RedisClient();
    ~RedisClient();

    bool Connect(const string& host, int port);
    string GetErrorMessage();

    REDIS_STATUS SwitchDB(int dbNumber);

    REDIS_STATUS Lpush(const string& list, const string& value);

    REDIS_STATUS Brpop(const string& list, string& value, int timeOut = 1);

private:
    void FreeRedisContext();

private:
    redisContext* m_pRedisContext;
    string m_errorMsg;
};


#endif //REDISTEST_REDISCLIENT_H
