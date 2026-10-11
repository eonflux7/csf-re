#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall Member08_ReleaseStorage(void* self, void* /*edx*/)
{
    int* first = CSF_AT(int*, self, 0x124);
    if (first)
    {
        MissionScript_DestroyElementRange(first, CSF_AT(int*, self, 0x128));
        void* block = CSF_AT(void*, self, 0x124);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x124) = 0;
    CSF_AT(uint32_t, self, 0x128) = 0;
    CSF_AT(uint32_t, self, 0x12C) = 0;

    first = CSF_AT(int*, self, 0x98);
    if (first)
    {
        MissionScript_DestroyElementRange(first, CSF_AT(int*, self, 0x9C));
        void* block = CSF_AT(void*, self, 0x98);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;

    int result = 0;
    void* block = CSF_AT(void*, self, 0x4);
    if (block)
        result = Mem_FreeDebug(block);

    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x00410DA0, Member08_ReleaseStorage, "auto");
