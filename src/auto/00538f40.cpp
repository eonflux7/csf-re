#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, S_004f21d0_ResetInnerBuffer, 0x00501170, (S_004f21d0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

[[clang::optnone]]
static C_0082d410* __cdecl MenuLoading_GetMissionName(C_0082d410* destination)
{
    auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8B0568());
    if (CompareStringSubstring(
            settings + 0x98, nullptr, 0,
            CSF_AT(unsigned int, settings, 0xAC),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        // The record includes the embedded string at +0x88.
        // The temporary string follows the record at +0xC8.
        alignas(8) std::uint8_t frameBytes[0xE4];
        S_004f21d0& record =
            *reinterpret_cast<S_004f21d0*>(frameBytes);
        C_0082d410& temporaryName =
            *reinterpret_cast<C_0082d410*>(frameBytes + 0xC8);

        S_004f21d0_ResetInnerBuffer(&record, nullptr);
        auto* saveState = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* provider = CSF_AT(void*, saveState, 0x4);

        if (provider)
        {
            const char* name =
                reinterpret_cast<const char*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, provider, 0)[0x30 / 4])(provider, nullptr);

            CSF_AT(std::uint32_t, &temporaryName, 0x18) = 15;
            CSF_AT(std::uint32_t, &temporaryName, 0x14) = 0;
            reinterpret_cast<char*>(&temporaryName.field_0004)[0] = 0;

            AssignStringRange_429B60(
                &temporaryName, nullptr, const_cast<char*>(name),
                static_cast<unsigned int>(std::strlen(name)));

            reinterpret_cast<void(__fastcall*)(void*, void*, S_004f21d0*, C_0082d410*)>(
                CSF_AT(void**, provider, 0)[0x1C / 4])(
                    provider, nullptr, &record, &temporaryName);

            if (CSF_AT(std::uint32_t, &temporaryName, 0x18) >= 0x10)
            {
                if (temporaryName.field_0004)
                    MemDebug_FreeDebugBlock(
                        reinterpret_cast<void*>(temporaryName.field_0004));
            }
        }

        AssignStringSubstring(
            destination, nullptr, &record.pad_006c[28], 0,
            static_cast<std::uint32_t>(-1));
        SaveGame_DestroyRecord(
            reinterpret_cast<std::uint32_t*>(&record), nullptr);
    }
    else
    {
        settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8B0568());
        unsigned int capacity = CSF_AT(unsigned int, settings, 0x94);
        auto* missionString = settings + 0x7C;
        char* text = capacity < 0x10
            ? reinterpret_cast<char*>(missionString + 0x4)
            : CSF_AT(char*, missionString, 0x4);

        AssignStringRange_429B60(
            destination, nullptr, text,
            static_cast<unsigned int>(std::strlen(text)));
    }

    return destination;
}

HOOK(0x00538F40, MenuLoading_GetMissionName, "auto");
