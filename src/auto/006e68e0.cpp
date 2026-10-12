#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpAtomicSetGeometry, 0x0058EFE0, (uint32_t* a1, int a2, char a3));
CSF_FUNC(char, __fastcall, sub_6E67C0, 0x006E67C0, (void* self, void* edx));

static char __fastcall ApplyStateChangeEffects(C_00861c08* self, void* /*edx*/)
{
    return sub_6E67C0(self, nullptr);
}

// Sets the state index stored in field_0018. When the index changes and the
// atomic geometry slot for that index is non-null, the geometry is applied to
// the atomic; if a3 is set, the state change effects are also applied.
static char __fastcall C_00861c08_SetStateFlags(C_00861c08* self, void* /*edx*/, uint8_t a2, char a3)
{
    char result = static_cast<char>(a2);

    if (a2 != static_cast<uint8_t>(self->field_0018))
    {
        self->field_0018 = a2;

        // Geometry pointer stored per state index (pad_001a[4 * a2 + 2] == offset 0x1C + 4 * a2).
        uint32_t geometry = CSF_AT(uint32_t, self, 0x1c + 4 * a2);
        if (geometry != 0)
        {
            RpAtomicSetGeometry(reinterpret_cast<uint32_t*>(self->field_000c),
                                static_cast<int>(geometry), 1);

            result = a3;
            if (a3)
                result = ApplyStateChangeEffects(self, nullptr);
        }
    }
    return result;
}

HOOK(0x006E68E0, C_00861c08_SetStateFlags, "auto");
