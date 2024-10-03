#include "Execution.hpp"

namespace Rizz Sigma {
	void RunScript(std::uintptr_t L, const std::string& Source) {
		std::string LuaScript = Luau::compile(Source, { 0 }, { 0 }, &BytecodeEncode);

        RBXDataModel DM = RBXDataModel(0x00);
        printf("Place Id %d\n", DM.PlaceId);

		if (LuaScript.at(0) == 0)
			lua_print(3, LuaScript.c_str() + 1);

		std::string CompressedScript = Compress(LuaScript);
		lua_load(L, &CompressedScript, "", 0);
		lua_taskdefer(L);

		lua_pop(L);
	}

    char kBytecodeMagic[] = "RSB1";
    int kBytecodeHashMultiplier = 41;
    int kBytecodeHashSeed = 42;
// power of brainrot
    static std::string Compress(std::string Bytecode) {
        int Size = Bytecode.size();
        int MaxSize = ZSTD_compressBound(Size);

        std::vector<char> Compressed(MaxSize);
        int CompressedSize = ZSTD_compress(&Compressed[0], MaxSize, Bytecode.c_str(), Size, ZSTD_maxCLevel());

        std::string Result = kBytecodeMagic;
        Result.append(reinterpret_cast<char*>(&Size), sizeof(Size));
        Result.append(&Compressed[0], CompressedSize);

        unsigned int Hash = XXH32(&Result[0], Result.size(), kBytecodeHashSeed);

        unsigned char HashBytes[4];
        memcpy(HashBytes, &Hash, sizeof(Hash));

        for (size_t i = 0; i < Result.size(); ++i)
            Result[i] ^= HashBytes[i % 4] + i * kBytecodeHashMultiplier;

        return Result;
    }
}