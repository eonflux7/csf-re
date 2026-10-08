#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x506BD0 - returns a value from global 0x8B056C (used here as a pointer)
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

static int __fastcall CMenuAccounts_UpdateField48FromGlobal(void* self, void* /*edx*/)
{
    int globalValue = GetGlobalValue_8B056C();
    uint8_t* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalValue)));
    void* sourceObject = *reinterpret_cast<void**>(base + 0x14);
    void** vtbl = *reinterpret_cast<void***>(sourceObject);
    int result = (reinterpret_cast<int (__fastcall*)(void*, void*)>(vtbl[14]))(sourceObject, nullptr);
    CSF_AT(uint8_t, self, 0x48) = static_cast<uint8_t>(result);
    return result;
}

HOOK(0x564CD0, CMenuAccounts_UpdateField48FromGlobal, "auto");
