#pragma once
#include "include.hpp"

uintptr_t DLLbase = (uintptr_t)GetModuleHandleA(NULL);

using taskdefer_rbx = uintptr_t(__cdecl*)(uintptr_t rl);
taskdefer_rbx rbx_taskdefer = reinterpret_cast<taskdefer_rbx>(DLLbase + 0xE5F4B0);

using luavmload_rbx = uintptr_t(__fastcall*)(uintptr_t rl, std::string* source, const char* chunk, int env);
luavmload_rbx rbx_luavmload = reinterpret_cast<luavmload_rbx>(DLLbase + 0xA9C0A0);

using getglobalstate_rbx = uintptr_t(__cdecl*)(uintptr_t sc, uintptr_t* indentity, uintptr_t* script);
getglobalstate_rbx rbx_getglobalstate = reinterpret_cast<getglobalstate_rbx>(DLLbase + 0xCE4170);

using decryptluastate_rbx = uintptr_t(__cdecl*)(uintptr_t encrypted);
decryptluastate_rbx rbx_decryptluastate = reinterpret_cast<decryptluastate_rbx>(DLLbase + 0xA99AE0);

using print_rbx = uintptr_t(__cdecl*)(int rl, const char* source);
print_rbx rbx_print = reinterpret_cast<print_rbx>(DLLbase + 0x11510F0);

uintptr_t TopOffset = helper::valueToHex(16);
uintptr_t PermissionOffset = helper::valueToHex(72);