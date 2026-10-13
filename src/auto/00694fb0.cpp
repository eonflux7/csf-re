#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, sub_694D50, 0x00694D50, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_694DC0, 0x00694DC0, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, sub_694EA0, 0x00694EA0, (void* self, void* edx, char* a2));

// Writes a formatted entry: optional separator/prefix, optional tagged value, then the
// final text (via sub_694DC0 or sub_694EA0 depending on the per-slot flag byte).
static unsigned int __fastcall S_00694cd0_WriteEntry(S_00694cd0* self, void* /*edx*/,
                                                     const char* valueText, const char* tagText) {
    // Leading separator: " " when the slot flag is set, otherwise the inline buffer at +0x14.
    if (CSF_AT(uint8_t, self, self->field_0004 * 8u + 0x11cu)) {
        sub_694DC0(self, nullptr, const_cast<char*>(CSF_STR(0x83F284, " ")));
    } else {
        sub_694DC0(self, nullptr, reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x14));
    }

    if (tagText) {
        sub_694DC0(self, nullptr, const_cast<char*>(tagText));
        sub_694DC0(self, nullptr, const_cast<char*>(CSF_STR(0x83F284, " ")));
        sub_694D50(self, nullptr, static_cast<int>(self->field_0004));
    }

    // Final write: re-read the flag, since the callees above may change state.
    if (CSF_AT(uint8_t, self, self->field_0004 * 8u + 0x11cu)) {
        return sub_694DC0(self, nullptr, const_cast<char*>(valueText));
    }
    return static_cast<unsigned int>(sub_694EA0(self, nullptr, const_cast<char*>(valueText)));
}

HOOK(0x00694FB0, S_00694cd0_WriteEntry, "auto");
