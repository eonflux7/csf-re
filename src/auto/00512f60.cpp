#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

namespace std {
struct _String_base;
}

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, sub_515C20, 0x00515C20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(void*, __fastcall, sub_512C60, 0x00512C60, (int a1, int a2, void* a3));
CSF_FUNC(void*, __fastcall, sub_43EB30, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));

// Builds the quick-save file name ("<slot>quick.sav") for the current profile
// and stores it into the string object passed in.
static std::_String_base* __fastcall CSaveGameManager_SetQuickSaveFileName(CSaveGameManager* self, void* /*edx*/, C_0082d410* a2)
{
    int profileGlobal = GetGlobalValue_8B056C();
    uint8_t* profileBase = reinterpret_cast<uint8_t*>(profileGlobal);
    int activeSlot = sub_515C20(reinterpret_cast<void*>(CSF_AT(uint32_t, profileBase, 0x8)), nullptr);

    char quickSaveName[20];
    _sprintf(quickSaveName, CSF_STR(0x008408A0, "%dquick.sav"), activeSlot);

    sub_512C60(static_cast<int>(reinterpret_cast<uintptr_t>(self)), 0, a2);
    return reinterpret_cast<std::_String_base*>(
        sub_43EB30(a2, 0, quickSaveName, static_cast<unsigned int>(std::strlen(quickSaveName))));
}

HOOK(0x00512F60, CSaveGameManager_SetQuickSaveFileName, "auto");
