#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CMenuProfiles_TailDispatchReset(
    CMenuProfiles*, void*) {
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x40]
    }
}

static int __fastcall CMenuProfiles_ResetState(
    CMenuProfiles* self, void* edx) {
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = UINT32_MAX;
    CSF_AT(std::uint32_t, self, 0x30) = UINT32_MAX;

    [[clang::musttail]] return CMenuProfiles_TailDispatchReset(self, edx);
}

HOOK(0x0053B910, CMenuProfiles_ResetState, "auto");
