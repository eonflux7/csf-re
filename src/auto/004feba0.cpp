#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
static CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
static CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470,
                (void* self, void* edx, int a2));
static CSF_FUNC(uint32_t, __fastcall, SceneLoader_SetSectorEnabled, 0x004ED1C0,
                (S_004e8e00* self, void* edx, int a2, int a3));

static int __cdecl SceneLoader_EnableSelectedSectors()
{
    int result = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        return result;

    auto* sectors = CSF_AT(C_0086f128*,
                          reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);
    auto* storage = reinterpret_cast<void*>(sectors->field_0008);
    result = static_cast<int>(reinterpret_cast<uintptr_t>(storage));
    const uint32_t begin = CSF_AT(uint32_t, storage, 0x4);
    uint32_t count = 0;
    if (begin)
    {
        const uint32_t end = CSF_AT(uint32_t, storage, 0x8);
        result = static_cast<int32_t>(end - begin) / 556;
        count = static_cast<uint32_t>(result);
    }

    for (uint32_t index = 0; index < count; ++index)
    {
        auto* record = reinterpret_cast<void*>(
            SceneLoader_GetSectorRecord(sectors, nullptr, static_cast<int>(index)));
        bool selected = false;
        int secondaryIndex = 12;
        int offset = 24;
        do
        {
            for (int column = 0; column < 3; ++column)
            {
                void* table = reinterpret_cast<void*>(GetGlobalValue_558150());
                bool matches = CSF_AT(uint32_t, record, 0) ==
                               CSF_AT(uint32_t, table, offset);
                if (!matches)
                {
                    table = reinterpret_cast<void*>(GetGlobalValue_558150());
                    matches = CSF_AT(uint32_t, record, 0) ==
                              CSF_AT(uint32_t, table, 4 * (secondaryIndex + column));
                }
                if (matches)
                    selected = true;
                offset += 4;
            }
            secondaryIndex += 3;
        } while (offset < 48);

        if (CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) == 2 ||
            CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
        {
            int extraOffset = 384;
            do
            {
                int remaining = 3;
                do
                {
                    void* table = reinterpret_cast<void*>(GetGlobalValue_558150());
                    if (CSF_AT(uint32_t, record, 0) ==
                        CSF_AT(uint32_t, table, extraOffset))
                        selected = true;
                    extraOffset += 4;
                    --remaining;
                } while (remaining);
            } while (extraOffset < 408);
        }

        bool specialMatch = false;
        if (CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) == 3 ||
            CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
        {
            void* table = reinterpret_cast<void*>(GetGlobalValue_558150());
            specialMatch = CSF_AT(uint32_t, record, 0) ==
                           CSF_AT(uint32_t, table, 440);
            if (!specialMatch)
            {
                table = reinterpret_cast<void*>(GetGlobalValue_558150());
                specialMatch = CSF_AT(uint32_t, record, 0) ==
                               CSF_AT(uint32_t, table, 444);
            }
        }

        if (specialMatch || selected)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                sectors->vtbl[0x18 / 4])(sectors, nullptr, static_cast<int>(index));

            auto* loader = reinterpret_cast<S_004e8e00*>(GetValue8B0554());
            SceneLoader_SetSectorEnabled(loader, nullptr, static_cast<int>(index), 1);
        }
        result = static_cast<int>(count);
    }
    return result;
}

HOOK(0x004FEBA0, SceneLoader_EnableSelectedSectors, "auto");
