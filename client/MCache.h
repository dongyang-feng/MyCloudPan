/*
 * @Author: fdy
 * @Date: 2023-06-19 17:06:00
 * @LastEditTime: 2023-06-19 21:17:49
 * @FilePath: /MyCloudPan/MyCloudPan/client/MCache.h
 * @Description: 对memcached的封装，使用MCache类的实例对内存缓存进行管理
 *
 */
#ifndef _MCACHED_H
#define _MCACHED_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <libmemcached/memcached.h>

using namespace std;

class MCache
{
public:
    MCache(const char *ip, int port)
    {
        memc = memcached_create(NULL);                               // 只是创建了一个客户端实例，并未与具体的 Memcached 服务器建立连接
        servers = memcached_server_list_append(NULL, ip, port, &rc); // 用于将一个服务器地址（host:port）添加到 Memcached 服务器列表中
        rc = memcached_server_push(memc, servers);                   // 与服务器建立连接
        memcached_server_free(servers);                              // 释放 memcached_server_st 结构体实例所占用的内存空  为什么要式房贷哦服务器实例啊
    }

    ~MCache()
    {
        memcached_free(memc);
    }
    bool insertValue(const char *key, const char *value, int timeout)
    {
        rc = memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)(timeout), (uint32_t)(0));
        if (rc == MEMCACHED_SUCCESS)
        {
            cout << "Server key:" << key << " data: " << value << endl;
        }
        else
        {
            return false;
        }
        return true;
    }
    char *getValue(const char *key)
    {
        if (NULL == key)
        {
            cout << "key in empty." << endl;
            return NULL;
        }
        time_t expiration = 3;
        size_t value_length;
        uint32_t flags;
        char *value = NULL;
        value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
        if (rc == MEMCACHED_SUCCESS)
        {
            cout << "Get key: " << key << "  data:" << value << endl;
            return value;
        }
        return NULL;
    }
    bool deleteKey(const char *key)
    {
        rc = memcached_delete(memc, key, strlen(key), time_t(0));
        if (rc == MEMCACHED_SUCCESS)
        {
            cout << "Delete key: " << key << endl;
            return true;
        }
        return false;
    }

private:
    memcached_st *memc;
    memcached_return rc;
    memcached_server_st *servers;
};
#endif

// int main()
// {
//     memcached_st *memc;
//     memcached_return rc;
//     memcached_server_st *servers;
//     char value[8191];

//     // connection mutil server
//     memc = memcached_create(memc);
//     servers = memcached_server_list_append(NULL, "localhost", 11211, &rc);
//     rc = memcached_server_push(memc, servers);
//     memcached_server_free(servers);

//     // save mutil data
//     size_t i;
//     const char *keys[] = {"key1", "key2", "key3"};
//     size_t key_length[] = {4, 4, 4};
//     char *values[] = {"This is a first value", "This is a second value", "This is a third value"};
//     size_t val_length[] = {21, 22, 21};
//     for (int i = 0; i < 3; i++)
//     {
//         rc = memcached_set(memc, keys[i], key_length[i], values[i], val_length[i], (time_t)180, (uint32_t)0);
//         if (rc == MEMCACHED_SUCCESS)
//         {
//             printf("Save key:%s data:\"%s\" success.\n", keys[i], values[i]);
//         }
//     }
//     // Fetch multi data
//     char return_key[MEMCACHED_MAX_KEY];
//     size_t return_key_length;
//     char *return_value;
//     size_t return_value_length;
//     uint32_t flags;
//     rc = memcached_mget(memc, keys, key_length, 3);
//     while ((return_value = memcached_fetch(memc, return_key,&return_key_length, &return_value_length, &flags, &rc))) {
//         if (rc == MEMCACHED_SUCCESS) {
//             printf("Fetch key:%s data:%s\n", return_key, return_value);
//         }
//     }
//     //Delete multi data
//     for (i=0; i <3; i++) {
//         rc = memcached_set(memc, keys[i], key_length[i],values[i], val_length[i], (time_t)180, (uint32_t)0);
//         rc = memcached_delete(memc, keys[i],key_length[i], (time_t)0);
//         if (rc == MEMCACHED_SUCCESS) {
//             printf("Delete %s success\n", keys[i], values[i]);
//         }
//     }
//     //free
//     memcached_free(memc);
//     return 0;
// }
