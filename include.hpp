#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include <Shlobj.h>
#include <regex>

#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

#include "Luau/include/Compiler.h"
#include "Luau/include/BytecodeBuilder.h"
#include "Luau/include/BytecodeUtils.h"
#include "Luau/include/lua.h"

#include "zstd/include/zstd.h"
#include "zstd/include/xxhash.h"

#include "bytecode/compressor.hpp"
#include "helper.h"
#include "addresses.h"

#include "roblox_engine/getdata.h"
#include "roblox_engine/fileHandler.h"

using namespace std;
// very cooked