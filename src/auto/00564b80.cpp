#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static __declspec(naked) int __fastcall MenuAccounts_CompleteResetAndDispatch(
    void* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        mov dword ptr [ecx + 0x34], 0xffffffff
        mov byte ptr [ecx + 0x30], 1
        jmp dword ptr [eax + 0x40]
    }
}

static int __fastcall MenuAccounts_ResetState(void* self, void* edx)
{
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint8_t, self, 0x2D) = 0;
    CSF_AT(uint8_t, self, 0x2E) = 0;
    CSF_AT(uint8_t, self, 0x2F) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint8_t, self, 0x48) = 0;

    [[clang::musttail]] return MenuAccounts_CompleteResetAndDispatch(self, edx);
}

HOOK(0x00564B80, MenuAccounts_ResetState, "auto");
