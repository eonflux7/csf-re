#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AppendBytesToString_43EB30, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __fastcall, sub_515C20, 0x00515C20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Builds the "Games\<slot>\" path suffix on the given path string.
static int __stdcall CSaveGameManager_BuildGamesPath(C_0082d410* pathBuffer)
{
    alignas(8) char slotText[8];

    AssignStringRange_429B60(pathBuffer, nullptr, const_cast<char*>(CSF_STR(0x840888, "Games")), 5);

    uintptr_t globalValue = static_cast<uintptr_t>(GetGlobalValue_8B056C());
    uint32_t settingsPtr = CSF_AT(uint32_t, reinterpret_cast<void*>(globalValue), 0x8);
    int slotNumber = sub_515C20(reinterpret_cast<void*>(static_cast<uintptr_t>(settingsPtr)), nullptr);

    _sprintf(slotText, reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0083090C)), slotNumber);
    AppendBytesToString_43EB30(pathBuffer, 0, slotText, static_cast<unsigned int>(std::strlen(slotText)));
    return static_cast<int>(reinterpret_cast<intptr_t>(StringAppendFill(pathBuffer, nullptr, 1, '/')));
}

HOOK(0x00512C60, CSaveGameManager_BuildGamesPath, "auto");
