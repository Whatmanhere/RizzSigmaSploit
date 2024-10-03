#pragma once
#include "../include.hpp"

namespace logFile {
    std::filesystem::path get_latest_log()
    {
        std::vector<std::filesystem::path> roblox_log;
        std::wstring app_data_path;
        wchar_t path[MAX_PATH];

        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
        {
            app_data_path = std::wstring(path);
        }
        else {
            return std::filesystem::path();
        }
        // veryyyy real yoru = paster
        std::wstring roblox_log_path = app_data_path + L"\\Roblox\\logs";

        for (const auto& entry : std::filesystem::directory_iterator(roblox_log_path))
        {
            if (entry.is_regular_file() && entry.path().extension() == L".log" &&
                entry.path().filename().wstring().find(L"Player") != std::wstring::npos)
            {
                roblox_log.push_back(entry.path());
            }
        }

        std::sort(roblox_log.begin(), roblox_log.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
            });

        if (!roblox_log.empty()) {
            return roblox_log[0];
        }
        else {
            return std::filesystem::path();
        }
    }
}