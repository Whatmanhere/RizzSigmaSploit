#include "include.hpp"

extern "C" __declspec(dllexport) LRESULT CALLBACK JustSomeCallingFunctionThatIsReallyLong(int code, WPARAM wParam, LPARAM lParam) {
    return CallNextHookEx(NULL, code, wParam, lParam);
}

class bytecode_encoder_t : public Luau::BytecodeEncoder
{
    inline void encode(uint32_t* data, size_t count) override
    {
        for (auto i = 0u; i < count;)
        {
            auto& opcode = *reinterpret_cast<uint8_t*>(data + i);

            i += Luau::getOpLength(LuauOpcode(opcode));

            opcode *= 227;
        }
    }
};
bytecode_encoder_t bytecode_encoder = {};

string read_file_content(const filesystem::path& file_path)
{
    ifstream file(file_path);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

void Execute(const std::string& Source, uintptr_t Lua_State) {
    std::string luaScript = Luau::compile(Source, { 0 }, { 0 }, &bytecode_encoder);
    if (luaScript.at(0) == 0)
        Execute(("error(" + luaScript.substr(1) + ")").c_str(), Lua_State);
    std::string compressed = sigma::compress_bytecode(luaScript);
    rbx_luavmload(Lua_State, &compressed, "RizzSigmasploit", 0);
    rbx_taskdefer(Lua_State);
    *(uintptr_t*)(Lua_State + 0x10) -= 1 * TopOffset;
}


uintptr_t hexStringToUintPtr(std::string hexStr) {
    uintptr_t address = 0;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> address;
    return address;
}

void executeScript(uintptr_t Lua_State, const std::string& tempfile) {
    static auto lastModifiedTime = std::filesystem::file_time_type{};
    std::error_code ec;

    auto currentModifiedTime = std::filesystem::last_write_time(tempfile, ec);
    if (ec || currentModifiedTime == lastModifiedTime) return;

    lastModifiedTime = currentModifiedTime;

    std::ifstream inFile(tempfile);
    if (!inFile) return;

    std::string command((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    if (command.empty()) return;

    std::cout << command << std::endl;

    Execute(command.c_str(), Lua_State);

    std::ofstream outFile(tempfile, std::ofstream::trunc);
}

void writePlaceIdToFile(const std::string& filePath, int64_t placeId) {
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << placeId;
        outFile.close();
    }
    else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
}

std::string uintptrToString(uintptr_t value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

DWORD WINAPI EntryPoint(LPVOID lpParam) {
    filesystem::path latest_log_path = logFile::get_latest_log();
    string log_content = read_file_content(latest_log_path);
    regex view_address_regex(R"(SurfaceController\[_:1\]::initialize view\(([\dA-Fa-f]+)\))");
    smatch match;
    char* localniggerdata = getenv("LOCALAPPDATA");
    std::string tempfile = std::string(localniggerdata) + "\\RizzSigmasploit.txt";
    std::filesystem::file_time_type lastModifiedTime;

    if (regex_search(log_content, match, view_address_regex)) {
        if (match.size() > 1) {
            string view_address = match[1];
// real....
            while (true) {
                std::vector<uint32_t> identity_capabilities = { 0, 3, 0, 11, 3, 1, 11, 63, 63, 12 };
                uintptr_t Data_Model = getData::DataModel(hexStringToUintPtr(view_address));
                uintptr_t Script_Context = getData::ScriptContext(Data_Model);
                //rbx_print(1, uintptrToString(getData::GameLoaded(Data_Model)).c_str());
                if (getData::InGame(Data_Model) == true) {
                    uintptr_t Global_State = getData::GlobalState(Script_Context);
                    uintptr_t Lua_State = getData::LuaState(Global_State);
                    uintptr_t User_Data = getData::UserData(Lua_State);
                    int64_t userdata = *(int64_t*)(Lua_State + 0x78);
                    uintptr_t identity = *reinterpret_cast<uintptr_t*>(userdata + 0x30);
                    uintptr_t enc_capabilities = identity_capabilities[identity] ^ 0x3FFFF00;
                    *reinterpret_cast<std::int64_t*>(userdata + 0x30) = 8;
                    *reinterpret_cast<std::int64_t*>(userdata + PermissionOffset) = enc_capabilities;
                    Execute("printidentity()", Lua_State);
                    Execute("for i=1, 100 do print('Testing') end", Lua_State);
                    executeScript(Lua_State, tempfile);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    return false;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved) {
    DisableThreadLibraryCalls(hModule);

    if (dwCallReason == DLL_PROCESS_ATTACH)
        std::thread(EntryPoint, nullptr).detach();

    return TRUE;
}