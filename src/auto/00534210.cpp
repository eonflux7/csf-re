#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AppendStringSubstring, 0x0043EA30, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __cdecl, CoreFiles_SelectArchive, 0x00698960, (char* filename));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall C_0082d410_PrepareLanguageSelection(
    C_0082d410* self, void* /*edx*/)
{
    const uint32_t first = CSF_AT(uint32_t, self, 0x14);
    if (first &&
        static_cast<int32_t>(CSF_AT(uint32_t, self, 0x18) - first) / 28 == 1)
    {
        void* archiveName = reinterpret_cast<uint8_t*>(self) + 0x3C;
        AssignStringRange_429B60(
            archiveName, nullptr,
            const_cast<char*>(CSF_STR(0x00841C74, "Lang/")), 5);
        AppendStringSubstring(
            archiveName, nullptr,
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x14)),
            0, static_cast<unsigned int>(-1));
        Profiles_AppendStringRange(
            archiveName, 0,
            const_cast<char*>(CSF_STR(0x00830FEC, ".pak")), 4);
    }
    else
    {
        const uint32_t currentFirst = CSF_AT(uint32_t, self, 0x14);
        if (currentFirst &&
            static_cast<int32_t>(
                CSF_AT(uint32_t, self, 0x18) - currentFirst) / 28 != 0)
        {
            return 1;
        }

        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x3C,
            nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    }

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint8_t, globalValue, 0xA5C))
    {
        char* archive = reinterpret_cast<char*>(GetGlobalIntPtr());
        CoreFiles_SelectArchive(archive);
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, globalState, 0)[0x24 / 4])(globalState, nullptr);
    return 0;
}

HOOK(0x00534210, C_0082d410_PrepareLanguageSelection, "auto");
