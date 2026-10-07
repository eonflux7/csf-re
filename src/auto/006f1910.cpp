#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto C_00860a30_SetFlag0C =
    reinterpret_cast<void (__fastcall*)(C_00860a30*, void*, int)>(0x006C9FE0);

static void __fastcall C_00862490_SetFlag28AndNotify(
    C_00862490* self, void* /*edx*/, char setBaseFlag)
{
    self->field_0028 = 1;

    if (setBaseFlag != 0) {
        C_00860a30_SetFlag0C(self, nullptr, 1);
    } else {
        void* recipient = reinterpret_cast<void*>(self->field_0040);
        void* context = reinterpret_cast<void*>(self->field_0018);
        std::uint32_t argument = CSF_AT(std::uint32_t, context, 0x64);
        void** vtable = CSF_AT(void**, recipient, 0);

        reinterpret_cast<void (__fastcall*)(void*, void*, int, std::uint32_t)>(
            vtable[3])(recipient, nullptr, 0, argument);
    }
}

HOOK(0x006F1910, C_00862490_SetFlag28AndNotify, "auto");
