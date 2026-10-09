#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSubsystem, 0x00652010, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_RestorePreviousLog, 0x006981B0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetLogState, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_RefreshSubsystem, 0x006CADD0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall S_00500350_FinishPendingScene(
    S_00500350* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x20) != 1)
    {
        int global = GetGlobalInt();
        int subsystem = SceneLoader_GetSubsystem(
            reinterpret_cast<void*>(global), nullptr);
        return SceneLoader_RefreshSubsystem(
            reinterpret_cast<void*>(subsystem), nullptr);
    }

    int pending = CSF_AT(int, self, 0x100);
    if (pending != 0)
        return pending;

    std::uint32_t index = 0;
    if (CSF_AT(std::uint32_t, self, 0x44) > 0)
    {
        auto* records = CSF_AT(std::uint8_t*, self, 0x3C);
        const std::uint32_t sceneId = CSF_AT(std::uint32_t, self, 0x14);
        auto* cursor = records + 4;

        for (;;)
        {
            if (CSF_AT(std::uint32_t, cursor, 0x34) == sceneId &&
                CSF_AT(int, cursor, 0) == 2)
            {
                CSF_AT(std::uint8_t*, self, 0x100) =
                    records + 0x44 * index;
                break;
            }

            ++index;
            cursor += 0x44;
            if (index >= CSF_AT(std::uint32_t, self, 0x44))
                break;
        }
    }

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr);

    CSF_AT(int, self, 0x20) = 0;

    int global = GetGlobalInt();
    int subsystem = SceneLoader_GetSubsystem(
        reinterpret_cast<void*>(global), nullptr);
    SceneLoader_RefreshSubsystem(
        reinterpret_cast<void*>(subsystem), nullptr);

    auto* selected = CSF_AT(std::uint8_t*, self, 0x100);
    std::uint32_t sectorIndex = CSF_AT(std::uint32_t, selected, 0x38);
    auto* sectors = reinterpret_cast<std::uint8_t*>(self->field_0030);

    if (CSF_AT(std::uint8_t, sectors + 0x58 * sectorIndex, 0x1C) != 0)
    {
        ((void(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, self, 0)[0x38 / 4])(
                self, nullptr, sectorIndex);
    }

    CSF_AT(int, self, 0x100) = 0;
    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;

    Nullsub_506AC0();
    SceneLoader_SetLogState(0);

    int globalValue = GetGlobalValue_8BC1E0();
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(globalValue), 0xA5C) != 0)
    {
        SceneLoader_RestorePreviousLog();
        // The forwarded callee is void; its incidental EAX is not a defined result.
        return 0;
    }
    return globalValue;
}

HOOK(0x004FF760, S_00500350_FinishPendingScene, "auto");
