//
// Created by zhaohaibin on 1/30/18.
//

#include <string.h>
#include "RedisClient.h"

REDIS_STATUS SucessCheck(redisContext* pContex, string& msg)
{
    msg.clear();
    if( 0!= pContex->err)
    {
        if(REDIS_ERR_IO == pContex->err)
        {
            char error[32];
            sprintf(error, "IO ERROR : %d", errno);
            msg.append(error);
        }else
        {
            msg.append(pContex->errstr);
        }
        return REDIS_STATUS_ERROR;
    }
    return REDIS_STATUS_OK;
}

REDIS_STATUS SuccessCheck(redisReply* pReply, string& msg)
{
    if(pReply == NULL)
        return REDIS_STATUS_ERROR;
    if(REDIS_REPLY_ERROR ==pReply->type)
    {
        msg.append(pReply->str,pReply->len);
        freeReplyObject(pReply);
        return REDIS_STATUS_ERROR;
    }
    if(REDIS_REPLY_NIL == pReply->type)
    {
        freeReplyObject(pReply);
        return REDIS_STATUS_TIMEOUT;
    }
    return REDIS_STATUS_OK;
}

RedisClient::RedisClient()
: m_pRedisContext(NULL)
{

}

RedisClient::~RedisClient()
{
    FreeRedisContext();
}

bool RedisClient::Connect(const string& host, int port)
{
    m_pRedisContext = redisConnect(host.c_str(), port);
    if(SucessCheck(m_pRedisContext, m_errorMsg) == false)
    {
        FreeRedisContext();
        return false;
    }
    return true;
}

string RedisClient::GetErrorMessage()
{
    return  m_errorMsg;
}

REDIS_STATUS RedisClient::SwitchDB(int dbNumber)
{
    char cmd[32];
    sprintf(cmd, "SELECT %d", dbNumber);
    redisReply* reply = (redisReply*)redisCommand(m_pRedisContext, cmd);
    REDIS_STATUS re = SuccessCheck(reply, m_errorMsg);
    return re;
}

REDIS_STATUS RedisClient::Lpush(const string& list, const string& value)
{
    redisReply* pReply = (redisReply*)redisCommand(m_pRedisContext, "%s %s %s", "LPUSH", list.c_str(), value.c_str());
    REDIS_STATUS re = SuccessCheck(pReply, m_errorMsg);
    if(re == REDIS_STATUS_OK)
        freeReplyObject(pReply);
    return re;
}

REDIS_STATUS RedisClient::Brpop(const string& list, string& value, int timeOut/* = 1*/)
{

    redisReply* pReply = (redisReply*)redisCommand(m_pRedisContext, "%s %s %d", "BRPOP", list.c_str(),timeOut);
    REDIS_STATUS ret = SuccessCheck(pReply, m_errorMsg);
    if(REDIS_STATUS_OK == ret)
    {
        if(REDIS_REPLY_ARRAY == pReply->type)
        {
            if(pReply->elements == 2)
                value.append(pReply->element[1]->str, pReply->element[1]->len);
            freeReplyObject(pReply);
        }
    }
    return ret;
}

void RedisClient::FreeRedisContext()
{
    if(m_pRedisContext != NULL)
        redisFree(m_pRedisContext);
}