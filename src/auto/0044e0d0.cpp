#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_0041e990_ReleaseAllEntries, 0x0041E990, (S_0041e990* self, void*));

static int __fastcall C_00831230_Destroy(C_00831230* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00831230);

    uint32_t firstBlock = CSF_AT(uint32_t, self, 0x0c);
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    if (firstBlock) {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(firstBlock)));
        CSF_AT(uint32_t, self, 0x0c) = 0;
    }

    S_0041e990_ReleaseAllEntries(CSF_AT(S_0041e990*, self, 0x1c), nullptr);

    S_0041e990* entry = CSF_AT(S_0041e990*, self, 0x1c);
    if (entry) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
        CSF_AT(S_0041e990*, self, 0x1c) = nullptr;
    }

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x0c));
    if (result) {
        result = Mem_Free(CSF_AT(void*, self, 0x0c));
        CSF_AT(uint32_t, self, 0x0c) = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0044E0D0, C_00831230_Destroy, "auto");
