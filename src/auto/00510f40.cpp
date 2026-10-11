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

CSF_FUNC(int, __fastcall, DestroyOwnedSubobject, 0x00510FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitOwnedSubobject, 0x00510F90, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_0086f128_ResetAndAllocateOwnedSubobject(C_0086f128* self, void* /*edx*/)
{
    void* existingBlock = CSF_AT(void*, self, 0x08);
    if (existingBlock) {
        DestroyOwnedSubobject(existingBlock, nullptr);
        Mem_Free(existingBlock);
        CSF_AT(void*, self, 0x08) = nullptr;
    }

    void* newBlock = Mem_Alloc(16);
    if (newBlock) {
        int result = InitOwnedSubobject(newBlock, nullptr, 0);
        CSF_AT(void*, self, 0x08) = newBlock;
        return result;
    }

    CSF_AT(void*, self, 0x08) = nullptr;
    return 0;
}

HOOK(0x00510F40, C_0086f128_ResetAndAllocateOwnedSubobject, "auto");
