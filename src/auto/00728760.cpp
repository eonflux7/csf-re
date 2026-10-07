#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008656d0_PrepareUpdate, 0x00713E80,
         (C_008656d0* self, void* edx));

static __declspec(naked) int __fastcall C_008656d0_ForwardUpdate(
    C_008656d0* self, void* edx)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 084h]
    }
}

static int __fastcall C_008656d0_ConditionalUpdate(
    C_008656d0* self, void* /*edx*/)
{
    const int result =
        ((int (__fastcall*)(void*, void*))self->vtbl[8])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    C_008656d0_PrepareUpdate(self, nullptr);

    [[clang::musttail]] return C_008656d0_ForwardUpdate(self, nullptr);
}

HOOK(0x00728760, C_008656d0_ConditionalUpdate, "auto");
