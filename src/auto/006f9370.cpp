#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ResetScreenEffect, 0x007090F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));

static char __fastcall C_00862d48_Initialize(
    C_00862d48* self, void* /*edx*/, int configuration)
{
    ResetScreenEffect(self, nullptr);
    const char result = InitializeScreenEffect(self, nullptr, configuration);

    void** vtable = CSF_AT(void**, self, 0);
    CSF_AT(std::uint8_t, self, 0x30) = 0;
    reinterpret_cast<std::uint8_t*>(&self->field_0060)[0] = 0;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(vtable[0x2C / 4])(
        self, nullptr);
    self->field_0008 = 1;

    if (!result)
        ResetScreenEffect(self, nullptr);

    return result;
}

HOOK(0x006F9370, C_00862d48_Initialize, "auto");
