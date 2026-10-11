#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reset helpers bound by address (signatures per packet).
CSF_FUNC(int, __fastcall, sub_4DB110, 0x004DB110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_41EF10, 0x0041EF10, (void* self, void* edx));

// Clears two navigation counters in the object referenced by this->field_0008,
// then tail-forwards to sub_41EF10 with the sub-object at +0x29C as `this`.
static int __fastcall TNavegando_ResetNavigationState(TNavegando* self, void* /*edx*/)
{
    sub_4DB110(self, nullptr);

    uint8_t* navData = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(int, navData, 0x23C) = 0;
    CSF_AT(int, navData, 0x240) = 0;

    return sub_41EF10(navData + 0x29C, nullptr);
}

HOOK(0x420F20, TNavegando_ResetNavigationState, "auto");
