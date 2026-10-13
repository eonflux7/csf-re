#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: game heap debug free, __cdecl, caller pops (ret 0 in callee).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases one owned child object (virtual deleting destructor, flag 1), clears its slot,
// and returns the callee's eax result. Each child is released through its own vtbl[0].
static int __fastcall C_0083f130_ReleaseOwnedObjects(C_0083f130* self, void* /*edx*/)
{
    int result = 0;

    // Child at +0x20
    void* child020 = reinterpret_cast<void*>(self->field_0020);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F130);
    if (child020)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child020, 0)[0])(child020, nullptr, 1);
        self->field_0020 = 0;
    }

    // Child at +0x38
    void* child038 = reinterpret_cast<void*>(self->field_0038);
    if (child038)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child038, 0)[0])(child038, nullptr, 1);
        self->field_0038 = 0;
    }

    // Child at +0x40
    void* child040 = reinterpret_cast<void*>(self->field_0040);
    if (child040)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child040, 0)[0])(child040, nullptr, 1);
        self->field_0040 = 0;
    }

    // Child at +0x3C
    void* child03C = reinterpret_cast<void*>(self->field_003c);
    if (child03C)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child03C, 0)[0])(child03C, nullptr, 1);
        self->field_003c = 0;
    }

    // Child at +0x44
    void* child044 = reinterpret_cast<void*>(self->field_0044);
    if (child044)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child044, 0)[0])(child044, nullptr, 1);
        self->field_0044 = 0;
    }

    // Heap buffer at +0x08, only when the inline capacity (+0x1C) is >= 16 (heap-allocated).
    if (self->field_001c >= 0x10)
    {
        result = static_cast<int>(self->field_0008);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0018 = 0;
    self->field_001c = 15;
    *reinterpret_cast<uint8_t*>(&self->field_0008) = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x4F46E0, C_0083f130_ReleaseOwnedObjects, "auto");
