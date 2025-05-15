#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

class Hash{
    public:
        // static uint64_t sdbm_hash(string key,uint64_t num_buckets){
        //     uint64_t hash = 0;
        //     for (char c : key) {
        //         hash = c + (hash << 6) + (hash << 16) - hash;
        //     }
        //     return hash;
        // }

        unsigned int sdbm_hash(const char *p) {
            unsigned int hash = 0;
            auto *str = (unsigned char *) p;
            int c{};
            while ((c = *str++)) {
            hash = c + (hash << 6) + (hash << 16) - hash;
            }
            return hash;
           }

        // https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c
        static uint64_t fnv1a_hash(string key, uint64_t num_buckets) {
            const uint64_t FNV_offset_basis = 14695981039346656037ULL;
            const uint64_t FNV_prime = 1099511627776ULL;
            uint64_t hash = FNV_offset_basis;
    
            for (char c : key) {
                hash ^= static_cast<uint64_t>(static_cast<unsigned char>(c)); // XOR with byte value
                hash *= FNV_prime; // Multiply by prime
            }
            return hash % num_buckets;
        }
    
        // https://gist.github.com/ykworx/7b8118ce95bd712003bcdc9eaa87e51f
        static uint64_t jenkins_hash(string key, uint64_t num_buckets) {
            uint64_t hash = 0;
            for (char c : key) {
                hash += static_cast<uint64_t>(static_cast<unsigned char>(c));
                hash += (hash << 10);
                hash ^= (hash >> 6);
            }
            hash += (hash << 3);
            hash ^= (hash >> 11);
            hash += (hash << 15);
            return hash % num_buckets;
        }
};

#endif