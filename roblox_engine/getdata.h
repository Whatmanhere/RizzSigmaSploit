#pragma once
#include "../include.hpp"

uintptr_t exploitidentity = {8};
uintptr_t exploitscript[] = {0,0};
//???
namespace getData {
    uintptr_t DataModel(uintptr_t view_address) {
        return *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(view_address + 0x118) + 0x198);
    }
    uintptr_t ScriptContext(uintptr_t DataModel)
    {
        return *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(DataModel + 0x50) + 0x0) + 0x3D0);
    }
    uintptr_t GlobalState(uintptr_t scriptcontext){
        return rbx_getglobalstate(scriptcontext + 0x110, &exploitidentity, exploitscript);
    }
    uintptr_t LuaState(uintptr_t global_state) {
        return rbx_decryptluastate(global_state + 0x88);
    }
    uintptr_t UserData(uintptr_t LuaState) {
        return LuaState + 0x78;
    }
    uintptr_t InGame(uintptr_t DataModel) {
        if (*reinterpret_cast<uintptr_t*>(DataModel + 0x160) != 0 && *reinterpret_cast<uintptr_t*>(DataModel + 0x168) != 0) {
            return true;
        }
        return false;
    }
}