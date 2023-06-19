/*
 * @Author: fdy
 * @Date: 2023-06-19 21:17:20
 * @LastEditTime: 2023-06-19 22:03:18
 * @FilePath: /MyCloudPan/MyCloudPan/client/test.cpp
 * @Description: 
 * 
 */
#include "MCache.h"

int main()
{
    memcached_st *memc;
    memcached_return rc;
    memcached_server_st *servers;
    char value[8191];

    // connection mutil server
    memc = memcached_create(memc);
    servers = memcached_server_list_append(NULL, "localhost", 11211, &rc);
    rc = memcached_server_push(memc, servers);
    memcached_server_free(servers);
    // cout<<"创建成功"<<endl;

    // save mutil data
    size_t i;
    const char *keys[] = {"key1", "key2", "key3"};
    size_t key_length[] = {4, 4, 4};
    const char *values[] = {"This is a first value", "This is a second value", "This is a third value"};
    size_t val_length[] = {21, 22, 21};
    for (int i = 0; i < 3; i++)
    {
        rc = memcached_set(memc, keys[i], key_length[i], values[i], val_length[i], (time_t)180, (uint32_t)0);
        if (rc == MEMCACHED_SUCCESS)
        {
            printf("Save key:%s data:\"%s\" success.\n", keys[i], values[i]);
        }
        else{
            cout<<rc<<endl;
        }
    }
    // Fetch multi data
    char return_key[MEMCACHED_MAX_KEY];
    size_t return_key_length;
    char *return_value;
    size_t return_value_length;
    uint32_t flags;
    rc = memcached_mget(memc, keys, key_length, 3);
    while ((return_value = memcached_fetch(memc, return_key,&return_key_length, &return_value_length, &flags, &rc))) {
        if (rc == MEMCACHED_SUCCESS) {
            printf("Fetch key:%s data:%s\n", return_key, return_value);
        }
    }
    //Delete multi data
    for (i=0; i <3; i++) {
        rc = memcached_set(memc, keys[i], key_length[i],values[i], val_length[i], (time_t)180, (uint32_t)0);
        rc = memcached_delete(memc, keys[i],key_length[i], (time_t)0);
        if (rc == MEMCACHED_SUCCESS) {
            printf("Delete %s success\n", keys[i]);
        }
    }
    //free
    memcached_free(memc);
    return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <unistd.h>
// #include <string>
// #include <iostream>
// #include <libmemcached/memcached.h>//C 接口
// #include <libmemcached/util.h>  //工具，包括pool功能
// #include<libmemcached/memcached.hpp> //将c接口封装成的c++接口，
// using namespace std;

// int main(int argc, char *argv[])
// {
//   memcached_st *memc;
//   memcached_return rc;
//   memcached_server_st *servers;

//   memc = memcached_create(NULL);

//   servers = memcached_server_list_append_with_weight(NULL, (char*)"localhost", 11211,  50  , &rc);
//   servers = memcached_server_list_append_with_weight(servers, (char*)"localhost", 11212 , 50 , &rc);

//   rc = memcached_server_push(memc, servers);

//   memcached_server_free(servers);

//   memcached_behavior_set(memc,MEMCACHED_BEHAVIOR_DISTRIBUTION,MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA);

//   memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS, true) ;//set失败，则快隔离将其标记为dead
//   memcached_behavior_set(memc , MEMCACHED_BEHAVIOR_DEAD_TIMEOUT , 5);//慢恢复，每隔多少s试探一次即可。

//   int time_sl = 0;
//   int times = 0;

//   while(times++ < 100000)
//   {
//       //save data
//       const char  *keys[]= {"key1", "key2", "key3","key4" ,"key5","key6","key7"};
//       const  size_t key_length[]= {4, 4, 4, 4 ,4 ,4 ,4 };
//       const char *values[] = {"key1Value", "key2Value", "key3Value","key4Value" , "key5Value" , "key6Value" , "key7Value"};
//       size_t val_length[]= {sizeof(values[0]), sizeof(values[1]), sizeof(values[2]), sizeof(values[3]),sizeof(values[4]),sizeof(values[5]),sizeof(values[6])};
//       for (int i = 0 ; i < 7; i++)
//       {
//           rc = memcached_set(memc, keys[i], key_length[i], values[i], val_length[i], (time_t)180,(uint32_t)0);
//           printf("key: %s  rc:%s\n", keys[i], memcached_strerror(memc, rc));   // 输出状态

//           char* result = memcached_get(memc, keys[i], key_length[i], NULL, NULL, &rc);
//           if (rc == MEMCACHED_SUCCESS)
//           {
//               cout << "Get value:" << result << " sucessful!" << endl;
//           }else
//               cout << memcached_strerror(memc, rc) << endl;
//       }
//       printf("time: %d\n", time_sl++);
//       sleep(1);

//   }
//       memcached_free(memc);
//       return 0;
// }
