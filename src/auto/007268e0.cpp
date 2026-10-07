#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// int __cdecl sub_6910F0(size_t size)
CSF_FUNC(int, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));

// _DWORD *__thiscall sub_734B90(_DWORD *this, int owner)  (ret 4)
CSF_FUNC(int, __fastcall, InitChildObject_734B90, 0x734B90, (void* self, void* edx_unused, int owner));

// 0x007268E0: allocates a 0x10C-byte child object and hands it (with `this` as the
// owner argument) to the initializer sub_734B90; returns the initializer's result,
// or 0 when the allocation fails.
static int __fastcall CreateAndInitChild10C(C_008655e8* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(Mem_Alloc(0x10C));
    if (child != nullptr)
        return InitChildObject_734B90(child, nullptr, reinterpret_cast<int>(self));
    return 0;
}

HOOK(0x7268E0, CreateAndInitChild10C, "auto");
