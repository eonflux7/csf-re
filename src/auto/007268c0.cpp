#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// int __cdecl sub_6910F0(size_t size)
CSF_FUNC(int, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));

// _DWORD *__thiscall sub_738610(_DWORD *this, int arg)  (ret 4)
CSF_FUNC(int, __fastcall, InitChildObject38, 0x738610, (void* self, void* edx_unused, int arg));

// 0x007268C0: allocates a 0x38-byte child object and hands it (together with `this`
// as the argument) to the initializer sub_738610; returns the initializer's result,
// or 0 when the allocation fails.
static int __fastcall CreateAndInitChild38(C_008655e8* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(Mem_Alloc(0x38));
    if (child != nullptr)
        return InitChildObject38(child, nullptr, reinterpret_cast<int>(self));
    return 0;
}

HOOK(0x7268C0, CreateAndInitChild38, "auto");
