#include "Redis.h"

Redis::Redis(string host, int port, int db)
{
    struct timeval timeout = {2, 0};    //2s的超时时间
    //redisContext是Redis操作对象
    pRedisContext = (redisContext*)redisConnectWithTimeout(host.c_str(), port, timeout);
    if ( (NULL == pRedisContext) || (pRedisContext->err) )
    {
        if (pRedisContext)
        {
            std::cout << "connect error:" << pRedisContext->errstr << std::endl;
        }
        else
        {
            std::cout << "connect error: can't allocate redis context." << std::endl;
        }
        exit(-1);
    }
    char _db[2];
    int l = sprintf(_db, "%d", db);
    // string select  = "select " + string(_db);
    string selectDB = string(_db);
    // string res = execCmd(select);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "SELECT %s", Lib::stoc(selectDB));
    _freeReply();
}

Redis::~Redis()
{
    delete pRedisContext;
    delete pRedisReply;
}

void Redis::pub(string key, string data)
{
    // string cmd = "publish " + key + " " + data;
    // execCmd(cmd);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "PUBLISH %s %s", Lib::stoc(key), Lib::stoc(data));
    _freeReply();
}

string Redis::pop(string key)
{
    // string cmd = "rpop " + key;
    // return execCmd(cmd);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "RPOP %s", Lib::stoc(key));
    string res = _getResultAsString();
    _freeReply();
    return res;
}

// void Redis::push(string key, string data)
// {
//     string cmd = "lpush " + key + " " + data;
//     execCmd(cmd);
// }

void Redis::push(string key, string data)
{
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "LPUSH %s %s", Lib::stoc(key), Lib::stoc(data));
    _freeReply();
}

void Redis::set(string key, string data)
{
    // string cmd = "set " + key + " " + data;
    // execCmd(cmd);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "SET %s %s", Lib::stoc(key), Lib::stoc(data));
    _freeReply();
}

void Redis::setnx(string key, string data)
{
    // string cmd = "setnx " + key + " " + data;
    // execCmd(cmd);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "SETNX %s %s", Lib::stoc(key), Lib::stoc(data));
    _freeReply();
}

long long Redis::incr(string key)
{
    // string cmd = "incr " + key;
    // return execCmd(cmd, true);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "INCR %s", Lib::stoc(key));
    long long res = _getResultAsInt();
    _freeReply();
    return res;
}

string Redis::get(string key)
{
    // string cmd = "get " + key;
    // return execCmd(cmd);
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "GET %s", Lib::stoc(key));
    string res = _getResultAsString();
    _freeReply();
    return res;
}

void Redis::asService(ACTIONCALLBACK callback, string channel)
{
    _callback = callback;
    _channel = channel;
}

void Redis::run()
{
    pRedisReply = (redisReply*)redisCommand(pRedisContext, "SUBSCRIBE %s", Lib::stoc(_channel));
    freeReplyObject(pRedisReply);
    while(true) {
        int code = redisGetReply(pRedisContext, (void**)&pRedisReply);
        if (pRedisReply->elements >= 3) {
            string data = string(pRedisReply->element[2]->str);
            if(!_callback(data)) break;
        }
        freeReplyObject(pRedisReply);
        if (REDIS_OK != code) break;
    }
}


// string Redis::execCmd(string cmd, bool returnInt)
// {
//     //redisReply是Redis命令回复对象 redis返回的信息保存在redisReply对象中
//     pRedisReply = (redisReply*)redisCommand(pRedisContext, cmd.c_str());  //执行INFO命令
//     string res = "";
//     if (returnInt) {
//         char s[10];
//         sprintf(s, "%d", pRedisReply->integer);
//         res = string(s);
//     } else if (pRedisReply->len > 0)
//         res = pRedisReply->str;
//     //当多条Redis命令使用同一个redisReply对象时
//     //每一次执行完Redis命令后需要清空redisReply 以免对下一次的Redis操作造成影响
//     freeReplyObject(pRedisReply);
//     return res;
// }

string Redis::_getResultAsString()
{
    string res = "";
    if (pRedisReply->len > 0)
        res = pRedisReply->str;
    return res;
}

long long Redis::_getResultAsInt()
{
    return pRedisReply->integer;
}

void Redis::_freeReply()
{
    freeReplyObject(pRedisReply);
}

void Redis::initRds(string id, int num, string host, int port, int db)
{
    _idx[id] = 0;
    _idxNum[id] = num;
    std::vector<Redis *> rdsList(num);
    for (int i = 0; i < num; ++i)
    {
        rdsList[i] = new Redis(host, port, db);
    }
    _objPool[id] = rdsList;
}

Redis * Redis::getRds(string id)
{
    _idx[id]++;
    if (_idx[id] == _idxNum[id]) _idx[id] = 0;
    return _objPool[id][_idx[id]];
}

std::map<string, std::vector<Redis *> > Redis::_objPool = std::map<string, std::vector<Redis *> >() ;
std::map<string, int> Redis::_idx = std::map<string, int>();
std::map<string, int> Redis::_idxNum = std::map<string, int>();
