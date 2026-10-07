#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Shared helper: prepares/refreshes the object (bound at the address the
// original calls). Called as a __thiscall with no stack arguments.
CSF_FUNC(char, __fastcall, C_00865980_PrepareUpdate, 0x00713E80,
         (C_00865980* self, void* edx));

// Tail jump into vtable slot 0x88 (slot 34) forwarding ecx unchanged.
static __declspec(naked) int __fastcall C_00865980_ForwardUpdate(
    C_00865980* self, void* edx)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 088h]
    }
}

static int __fastcall C_00865980_ConditionalUpdate(
    C_00865980* self, void* /*edx*/)
{
    const int result =
        ((int (__fastcall*)(void*, void*))self->vtbl[8])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    C_00865980_PrepareUpdate(self, nullptr);

    [[clang::musttail]] return C_00865980_ForwardUpdate(self, nullptr);
}

HOOK(0x00728860, C_00865980_ConditionalUpdate, "auto");
