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

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CharWeaponFsm_InsertHandle, 0x00488C10,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(unsigned int, __fastcall, CharWeaponFsm_AppendHandle, 0x00488FF0,
         (void* self, void* edx, uint32_t* a3));

static void __fastcall CharWeaponFsm_InsertActionByPriority(
    void* self, void* /*edx*/, C_00862490* action)
{
    if (!action)
        return;

    if (CSF_AT(int32_t,
               reinterpret_cast<void*>(Handle_GetValuePointer(action, nullptr)),
               0x84) == -1)
    {
        CharWeaponFsm_AppendHandle(
            static_cast<uint8_t*>(self) + 0x74, nullptr,
            reinterpret_cast<uint32_t*>(&action));
        return;
    }

    const uint32_t begin = CSF_AT(uint32_t, self, 0x78);
    const int32_t count = begin
        ? (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x7C) - begin) >> 2)
        : 0;

    for (int32_t index = 0; index < count; ++index)
    {
        auto* entries = CSF_AT(C_00862490**, self, 0x78);
        const int32_t existingPriority = CSF_AT(
            int32_t,
            reinterpret_cast<void*>(
                Handle_GetValuePointer(entries[index], nullptr)),
            0x84);

        const int32_t actionPriority = CSF_AT(
            int32_t,
            reinterpret_cast<void*>(
                Handle_GetValuePointer(action, nullptr)),
            0x84);

        if (actionPriority < existingPriority)
        {
            CharWeaponFsm_InsertHandle(
                static_cast<uint8_t*>(self) + 0x74, nullptr,
                CSF_AT(char*, self, 0x78) + 4 * index, 1,
                reinterpret_cast<uint32_t*>(&action));

            const int32_t currentIndex = CSF_AT(int32_t, self, 0x90);
            if (currentIndex != -1 && currentIndex >= index)
            {
                CSF_AT(int32_t, self, 0x90) = static_cast<int32_t>(
                    static_cast<uint32_t>(currentIndex) + 1);
            }
            return;
        }
    }

    CharWeaponFsm_AppendHandle(
        static_cast<uint8_t*>(self) + 0x74, nullptr,
        reinterpret_cast<uint32_t*>(&action));
}

HOOK(0x00489390, CharWeaponFsm_InsertActionByPriority, "auto");
