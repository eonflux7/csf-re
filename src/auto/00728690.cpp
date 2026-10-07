#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall C_00865870_SetOwnedObject(
    C_00865870* self, void* /*edx*/, void* replacement)
{
    void* previous = reinterpret_cast<void*>(self->field_0030);
    if (replacement == previous)
        return;

    if (previous)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_0030 = 0;
    }

    self->field_0030 = reinterpret_cast<std::uint32_t>(replacement);
    if (replacement)
    {
        std::uint32_t* source =
            reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, replacement, 0)[12])(replacement, nullptr);

        self->field_0034 = source[0];
        self->field_0038 = source[1];
        self->field_003c = source[2];
        self->field_0040 = source[3];
    }
}

HOOK(0x00728690, C_00865870_SetOwnedObject, "auto");
