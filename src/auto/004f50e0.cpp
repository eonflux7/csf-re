#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Forwards a single argument to virtual slot 19 (byte offset 0x4C) of the
// object stored at this+4, passing 0 as the trailing argument. Returns the
// slot's result unchanged.
static int __fastcall CharacterModel_ForwardToChildSlot19(void* self, void* /*edx*/, int a2)
{
    void** child = CSF_AT(void**, self, 0x04);
    void* childVtbl = CSF_AT(void*, child, 0);
    return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, childVtbl, 0)[0x4C / 4])(
        child, nullptr, a2, 0);
}

HOOK(0x004F50E0, CharacterModel_ForwardToChildSlot19, "auto");
