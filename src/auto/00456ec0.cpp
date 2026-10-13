#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the first entry in the portrait list whose name equals `name`, or 0.
// Each entry: +0x0C holds the name inline when its length (+0x20) is below 0x10,
// otherwise +0x0C holds a pointer to the name.
static int __fastcall HudPortraits_FindEntryByName(S_00456ec0* self, void* /*edx*/, const char* name)
{
    const uint32_t count = self->field_0018;
    uint32_t* entries = CSF_AT(uint32_t*, self, 0x10);

    for (uint32_t i = 0; i < count; ++i) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(entries[i]);

        const char* entryName;
        if (CSF_AT(uint32_t, entry, 0x20) < 0x10u) {
            entryName = reinterpret_cast<const char*>(entry + 0x0C);
        } else {
            entryName = CSF_AT(const char*, entry, 0x0C);
        }

        // Byte-wise equality up to and including the terminator.
        const unsigned char* a = reinterpret_cast<const unsigned char*>(name);
        const unsigned char* b = reinterpret_cast<const unsigned char*>(entryName);
        bool equal = true;
        for (;;) {
            if (*a != *b) {
                equal = false;
                break;
            }
            if (*a == 0) {
                break;
            }
            ++a;
            ++b;
        }

        if (equal) {
            return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
        }
    }
    return 0;
}

HOOK(0x456EC0, HudPortraits_FindEntryByName, "auto");
