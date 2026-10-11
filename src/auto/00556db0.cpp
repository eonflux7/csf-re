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

CSF_FUNC(void*, __fastcall, sub_555E40, 0x00555E40, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, sub_749270, 0x00749270, (void* self, void* edx, char* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Drains the spawn-point list hanging off self->field_0c88. When the list head's
// first entry is empty, the head object is released through its vtable slot 0.
// Otherwise each entry is rebuilt via sub_749270/sub_555E40 until its counter reaches zero.
static void __fastcall SpawnPoints_ClearPendingList(S_00555e40* self, void* /*edx*/) {
    uint32_t listRoot = self->field_0c88;
    if (listRoot == 0) {
        return;
    }

    void* rootObj = reinterpret_cast<void*>(static_cast<uintptr_t>(listRoot));
    int firstEntry = CSF_AT(int, rootObj, 0x4);

    if (firstEntry == 0) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, rootObj, 0)[0])(rootObj, nullptr, 1);
        self->field_0c88 = 0;
        return;
    }

    uint8_t* spawnEntry =
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstEntry)) - 0x440;

    if (CSF_AT(uint32_t, spawnEntry, 0x464) > 0) {
        unsigned char a2[0x40];
        do {
            sub_749270(spawnEntry, nullptr, reinterpret_cast<char*>(a2));
            sub_555E40(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));

            if (CSF_AT(uint32_t, a2, 0x20) >= 8) {
                void* block = CSF_AT(void*, a2, 0x0C);
                if (block != nullptr) {
                    Mem_FreeDebug(block);
                }
            }

            CSF_AT(uint32_t, a2, 0x20) = 7;
            CSF_AT(int, a2, 0x1C) = 0;
            CSF_AT(uint16_t, a2, 0x0C) = 0;
        } while (CSF_AT(uint32_t, spawnEntry, 0x464) > 0);
    }
}

HOOK(0x00556DB0, SpawnPoints_ClearPendingList, "auto");
