#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static char __fastcall C_008637d8_Destroy(C_008637d8* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x008637D8);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x008637BC));

    char result = static_cast<char>(self->field_0024);
    if (result) {
        result = static_cast<char>(
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0020)));
    }

    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0070C3B0, C_008637d8_Destroy, "auto");
