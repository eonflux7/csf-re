#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpAtomicSetGeometry, 0x0058EFE0, (uint32_t* a1, int a2, char a3));
CSF_FUNC(char, __fastcall, AtomicInstance_RunPostSwitchUpdate, 0x006E67C0, (void* self, void* edx));

// Scans the three geometry slots (+0x1c..+0x24) from the highest index down,
// selects the highest non-empty slot, and swaps the atomic's geometry when the
// selection changed. Returns the selected slot's low byte, or -1 if all slots are empty.
static char __fastcall C_00861c08_SelectHighestGeometrySlot(C_00861c08* self, void* /*edx*/, char a2)
{
    int index = 2;
    uint8_t* slot = reinterpret_cast<uint8_t*>(self) + 0x24;

    while (*reinterpret_cast<uint32_t*>(slot) == 0) {
        if (--index < 0) {
            return static_cast<char>(index);
        }
        slot -= 4;
    }

    char result = static_cast<char>(index);
    if (static_cast<uint8_t>(index) != CSF_AT(uint8_t, self, 0x18)) {
        CSF_AT(uint8_t, self, 0x18) = static_cast<uint8_t>(index);

        uint32_t geometry =
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x1c + 4 * index);
        result = static_cast<char>(geometry);

        if (geometry != 0) {
            RpAtomicSetGeometry(reinterpret_cast<uint32_t*>(self->field_000c),
                                static_cast<int>(geometry), 1);
            result = a2;
            if (a2 != 0) {
                result = AtomicInstance_RunPostSwitchUpdate(self, nullptr);
            }
        }
    }
    return result;
}

HOOK(0x006E6920, C_00861c08_SelectHighestGeometrySlot, "auto");
