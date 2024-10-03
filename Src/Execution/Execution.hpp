#pragma once
#include "../../Luau/include/Compiler.h"
#include "../../Luau/include/BytecodeBuilder.h"
#include "../../Luau/include/BytecodeUtils.h"
#include "../../Luau/include/lua.h"

#include "../RBX/Offsets.hpp"

#include "../../zstd/include/zstd.h"
#include "../../zstd/include/xxhash.h"
//rizz
namespace RizzSigmaSploit {
    void RunScript(std::uintptr_t L, const std::string& Source);
    static std::string Compress(std::string Bytecode);
}

class BytecodeEncoder : public Luau::BytecodeEncoder {
    inline void encode(uint32_t* data, size_t count) override {
        for (auto i = 0u; i < count;) {
            auto& OpCode = *reinterpret_cast<uint8_t*>(data + i);
            i += Luau::getOpLength(LuauOpcode(OpCode));
            OpCode *= 227;
        }
    }
};

BytecodeEncoder BytecodeEncode = {};