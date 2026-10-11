#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Fast-call thunks for the two virtual slots used by the original.
// Slot 0x68 (index 26) on the object stored at self+0x10: no arguments, returns a pointer.
// Slot 0x40 (index 16) on that returned object: one stack argument (0), returns a byte bool.
static bool __fastcall MpSpawnPoints_IsSpawnChangeAllowed(void* self, void* /*edx*/)
{
    // Sentinel -1 at +0x14 means the change is always allowed.
    if (CSF_AT(int, self, 0x14) == -1)
        return true;

    // Flag byte at +0x18 blocks the change.
    if (CSF_AT(uint8_t, self, 0x18) != 0)
        return false;

    // First virtual call: owner = [self+0x10]; v3 = owner->vtbl[0x68 / 4]().
    void* owner = CSF_AT(void*, self, 0x10);
    auto getChild = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0x68 / 4]);
    void* child = getChild(owner, nullptr);

    // The original reloads [self+0x10] after the call and tests it for null.
    if (CSF_AT(void*, self, 0x10) == nullptr)
        return false;

    // Second virtual call: child->vtbl[0x40 / 4](0) with the value in ecx.
    auto probe = reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, child, 0)[0x40 / 4]);
    return probe(child, nullptr, 0);
}

HOOK(0x004F5740, MpSpawnPoints_IsSpawnChangeAllowed, "auto");
