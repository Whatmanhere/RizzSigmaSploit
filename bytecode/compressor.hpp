#pragma once
#include "../include.hpp"

namespace sigma
{ 
    char kBytecodeMagic[] = "RSB1";
    int kBytecodeHashMultiplier = 41;
    int kBytecodeHashSeed = 42;

    static std::string compress_bytecode(std::string bytecode)
    {
        int dataSize = bytecode.size();
        int maxSize = ZSTD_compressBound(dataSize);

        std::vector<char> compressed(maxSize);
        int compressedSize = ZSTD_compress(&compressed[0], maxSize, bytecode.c_str(), dataSize, ZSTD_maxCLevel());

        std::string result = kBytecodeMagic;
        result.append(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        result.append(&compressed[0], compressedSize);

        unsigned int hash = XXH32(&result[0], result.size(), kBytecodeHashSeed);

        unsigned char hashbytes[4];
        memcpy(hashbytes, &hash, sizeof(hash));

        for (size_t i = 0; i < result.size(); ++i)
            result[i] ^= hashbytes[i % 4] + i * kBytecodeHashMultiplier;
// me when yoru shuts the fuck up
        return result;
    }
    /*
        static __forceinline std::string decompress(std::string data)
        {
            const char bytecode_magic[] = "RSB1";

            uint8_t hash_bytes[4];
            memcpy(hash_bytes, &data[0], 4);

            for (auto i = 0u; i < 4; ++i)
            {
                hash_bytes[i] ^= bytecode_magic[i];
                hash_bytes[i] -= i * 41;
            }

            for (size_t i = 0; i < data.size(); ++i)
                data[i] ^= hash_bytes[i % 4] + i * 41;

            XXH64(&data[0], data.size(), 42);

            uint32_t data_size;
            memcpy(&data_size, &data[4], 4);

            std::vector<uint8_t> skibidisize(data_size);
            ZSTD_decompress(&skibidisize[0], data_size, &data[8], data.size() - 8);

            return std::string((char*)(&skibidisize[0]), data_size);
        }
        */
}