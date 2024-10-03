#pragma once
#include <cstdint>
#include <string>
#include <Windows.h>

#define Rebase(x) (x + reinterpret_cast<std::uintptr_t>(GetModuleHandleA(0)))

namespace RizzSigmaSploit {
//erm
	std::uintptr_t TopOffset = 0x10;
	std::uintptr_t PermissionOffset = 0x48;

#define lua_pop(L) *(std::uintptr_t*)(L + 0x10) -= 1 * TopOffset

	using taskdefer_rbx = uintptr_t(__cdecl*)(uintptr_t L);
	taskdefer_rbx lua_taskdefer = reinterpret_cast<taskdefer_rbx>(Rebase(0xE4A780));

	using luavmload_rbx = uintptr_t(__fastcall*)(uintptr_t L, std::string* Source, const char* Chunk, int Env);
	luavmload_rbx lua_load = reinterpret_cast<luavmload_rbx>(Rebase(0xA91C50));

	using getglobalstate_rbx = uintptr_t(__cdecl*)(uintptr_t ScriptContext, uintptr_t* Indentity, uintptr_t* Script);
	getglobalstate_rbx rbx_getglobalstate = reinterpret_cast<getglobalstate_rbx>(Rebase(0xCDE2C0));

	using decryptluastate_rbx = uintptr_t(__cdecl*)(uintptr_t L);
	decryptluastate_rbx rbx_decryptluastate = reinterpret_cast<decryptluastate_rbx>(Rebase(0xA8F690));

	using print_rbx = uintptr_t(__cdecl*)(int L, const char* Source);
	print_rbx lua_print = reinterpret_cast<print_rbx>(Rebase(0x11510F0));

	class RBXDataModel {
		public:
		RBXDataModel(std::uintptr_t ViewAddress) {
			this->ViewAddress = ViewAddress;
		}
		std::uintptr_t DataModel = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(this->ViewAddress + 0x118) + 0x198);
		std::uintptr_t ScriptContext = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(this->DataModel + 0x50) + 0x0) + 0x3D0);
		std::uintptr_t PlaceId = *reinterpret_cast<std::uintptr_t*>(this->DataModel + 0x160);

		private:
		std::uintptr_t ViewAddress;
	};
	class RBXLuaState {
		public:
		RBXLuaState(std::uintptr_t ScriptContext) {
			this->ScriptContext = ScriptContext;
		}
		std::uintptr_t GlobalState = rbx_getglobalstate(this->ScriptContext + 0x110, &this->Identity, this->Script);
		std::uintptr_t LuaState = rbx_decryptluastate(this->GlobalState + 0x88);
		std::uintptr_t UserData = this->LuaState + 0x78;

		private:
		std::uintptr_t ScriptContext;
		std::uintptr_t Identity = { 8 };
		std::uintptr_t Script[2] = { 0,0 };
	};
}