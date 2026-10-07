#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// int __cdecl sub_6910F0(size_t size)
CSF_FUNC(int, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));

// _DWORD *__thiscall sub_738130(_DWORD *this, int owner)  (ret 4)
CSF_FUNC(int, __fastcall, InitChildObject34, 0x738130, (void* self, void* edx_unused, int owner));

// 0x007268A0: allocates a 0x34-byte child object and hands it (together with `this`
// as owner) to the initializer sub_738130; returns the initializer's result, or 0
// when the allocation fails.
static int __fastcall CreateAndInitChild34(C_008655e8* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(Mem_Alloc(0x34));
    if (child != nullptr)
        return InitChildObject34(child, nullptr, reinterpret_cast<int>(self));
    return 0;
}

HOOK(0x7268A0, CreateAndInitChild34, "auto");
