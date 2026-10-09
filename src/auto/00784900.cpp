#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Weapon definition record passed by value (552 bytes).
struct WeaponDefRecord {
    uint32_t data[138];
};

CSF_FUNC(int, __fastcall, WeaponDefs_DestroyEntry, 0x005112A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void**, __fastcall, WeaponDefs_EraseRange, 0x00784440, (void* self, void* edx, void** a4, char* arg4, int arg8));
CSF_FUNC(int, __fastcall, WeaponDefs_FillInsert, 0x00784590, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

static int __fastcall WeaponDefs_Resize(void* self, void* /*edx*/, int count, WeaponDefRecord value)
{
    char* first = CSF_AT(char*, self, 4);
    unsigned int size;
    if (first) {
        size = (unsigned int)((CSF_AT(int, self, 8) - (int)first) / 552);
    } else {
        size = 0;
    }

    if (size >= (unsigned int)count) {
        if (first) {
            char* last = CSF_AT(char*, self, 8);
            if ((unsigned int)count < (unsigned int)(((int)last - (int)first) / 552)) {
                WeaponDefs_EraseRange(self, nullptr, (void**)&count, first + 552 * count, (int)last);
            }
        }
    } else {
        unsigned int cur;
        if (first) {
            cur = (unsigned int)((CSF_AT(int, self, 8) - (int)first) / 552);
        } else {
            cur = 0;
        }
        WeaponDefs_FillInsert(self, nullptr, CSF_AT(char*, self, 8), (unsigned int)count - cur, value.data);
    }

    // Destroy the by-value record (string member at +0x1F8, len +0x208, cap +0x20C).
    if (CSF_AT(uint32_t, &value, 0x20C) >= 0x10 && CSF_AT(void*, &value, 0x1F8)) {
        Mem_FreeDebug(CSF_AT(void*, &value, 0x1F8));
    }
    CSF_AT(uint32_t, &value, 0x20C) = 15;
    CSF_AT(uint32_t, &value, 0x208) = 0;
    CSF_AT(uint8_t, &value, 0x1F8) = 0;
    return WeaponDefs_DestroyEntry((uint8_t*)&value + 4, nullptr);
}

HOOK(0x00784900, WeaponDefs_Resize, "auto");
