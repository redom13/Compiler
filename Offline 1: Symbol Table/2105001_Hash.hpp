#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

class Hash{
    public:
        static uint64_t sdbm_hash(string key,uint64_t num_buckets){
            uint64_t hash = 0;
            for (char c : key) {
                hash = (c + (hash << 6) + (hash << 16) - hash)% num_buckets;
            }
            return hash;
        }
};

#endif