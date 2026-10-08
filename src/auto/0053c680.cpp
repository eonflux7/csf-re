#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static __declspec(naked) int __fastcall CMenuSoundOptions_DispatchFinalInitialization(
    CMenuSoundOptions*, void*)
{
    __asm {
        mov eax, edx
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x80]
    }
}

static int __fastcall CMenuSoundOptions_ResetAndInitialize(
    CMenuSoundOptions* self, void* /*edx*/)
{
    self->field_0014 = 0;
    self->field_0054 = 0;
    self->field_0018 = 0;
    self->field_0048 = 0;
    self->field_0058 = 0;

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x40 / 4])(self, nullptr);

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr);

    const int previousResult = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x7C / 4])(self, nullptr);

    // Transport the preceding EAX value through EDX; the thunk restores it.
    [[clang::musttail]] return CMenuSoundOptions_DispatchFinalInitialization(
        self, reinterpret_cast<void*>(static_cast<std::uintptr_t>(previousResult)));
}

HOOK(0x0053C680, CMenuSoundOptions_ResetAndInitialize, "auto");
