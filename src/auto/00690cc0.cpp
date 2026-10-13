#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x6472C0: __fastcall (this, edx, uint32_t* a2), ret 4.
CSF_FUNC(int, __fastcall, MemDebug_ProcessRecordKey, 0x006472C0, (void* self, void* edx, uint32_t* a2));

// Original: __thiscall, ret 8. The callee's `this` is the address of the
// first stack argument (the char a2 slot). The record key is built from
// this+0x10 / this+0x14 and passed by pointer.
static int __fastcall MemDebug_AllocDebug(void* self, void* /*edx*/, char a2, int a3)
{
    uint32_t key[2];
    uint32_t firstWord = CSF_AT(uint32_t, self, 0x10);
    key[1] = CSF_AT(uint32_t, self, 0x14);
    key[0] = firstWord;

    // Original passes the stack-argument address as ecx and &key as edx and the stack arg.
    return MemDebug_ProcessRecordKey(reinterpret_cast<void*>(&a2), key, key);
}

HOOK(0x690CC0, MemDebug_AllocDebug, "auto");
