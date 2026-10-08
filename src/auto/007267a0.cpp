#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// int __cdecl sub_6910F0(size_t size)
CSF_FUNC(int, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));

// _DWORD *__thiscall sub_737A70(_DWORD *this, int a2)  (ret 4)
CSF_FUNC(int, __fastcall, InitializeResourceWrapper, 0x737A70, (void* self, void* edx_unused, int owner));

// 0x007267A0: allocates a 0x90-byte child object and hands it (together with `this`
// as owner) to the initializer sub_737A70; returns the initializer's result, or 0
// when the allocation fails.
static int __fastcall CreateAndInitChild90(C_008655e8* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(Mem_Alloc(0x90));
    if (child != nullptr)
        return InitializeResourceWrapper(child, nullptr, reinterpret_cast<int>(self));
    return 0;
}

HOOK(0x7267A0, CreateAndInitChild90, "auto");

