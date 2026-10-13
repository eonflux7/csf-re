#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int32_t, g_8BB408, 0x008BB408);
CSF_GLOBAL(int32_t, g_8BB40C, 0x008BB40C);

CSF_FUNC(uint8_t*, __fastcall, S_00471340_InitDefaults, 0x0069B220, (S_00471340* self, void*));

// Resets the tokenizer's shared globals, installs the class vtable, initializes
// the four embedded S_00471340 sub-objects, and clears the state fields.
// Returns the object itself.
static uint8_t* __fastcall C_0085eac0_Construct(C_0085eac0* self, void* /*edx*/)
{
    g_8BB408 = 0;
    g_8BB40C = 0;

    self->vtbl = CSF_VTBL(0x0085EAC0);

    S_00471340_InitDefaults(reinterpret_cast<S_00471340*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    S_00471340_InitDefaults(reinterpret_cast<S_00471340*>(reinterpret_cast<uint8_t*>(self) + 0x4C), nullptr);
    S_00471340_InitDefaults(reinterpret_cast<S_00471340*>(reinterpret_cast<uint8_t*>(self) + 0x90), nullptr);
    S_00471340_InitDefaults(reinterpret_cast<S_00471340*>(reinterpret_cast<uint8_t*>(self) + 0xD4), nullptr);

    self->field_0004 = 0;
    self->field_0118 = 0;

    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x0069DD50, C_0085eac0_Construct, "auto");
