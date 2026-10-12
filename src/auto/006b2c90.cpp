#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Byte-wise equality with the same early-out the original uses: stop at the first
// differing byte or at the common terminating NUL.
static bool CStringEqualsExact(const char* a, const char* b) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(a);
    const unsigned char* q = reinterpret_cast<const unsigned char*>(b);
    while (*p == *q) {
        if (*p == 0) {
            return true;
        }
        ++p;
        ++q;
    }
    return false;
}

// Returns the index of the first element whose virtual name (vtable slot 5, +0x14)
// equals `name`, or -1 if none matches.
static int __fastcall C_00860288_FindIndexByName(C_00860288* self, void* /*edx*/, const char* name) {
    if (self->field_0024 == 0) {
        return -1;
    }

    int index = 0;
    do {
        // The element array and its count are re-read every iteration, as in the original.
        void* element = reinterpret_cast<void**>(self->field_0020)[index];
        if (element != nullptr) {
            using NameGetterFn = const char* (__fastcall*)(void* self, void* edx);
            NameGetterFn getName = reinterpret_cast<NameGetterFn>(CSF_AT(void**, element, 0)[0x14 / 4]);
            const char* elementName = getName(element, nullptr);
            if (elementName != nullptr && name != nullptr && CStringEqualsExact(elementName, name)) {
                return index;
            }
        }
        ++index;
    } while (static_cast<uint32_t>(index) < static_cast<uint32_t>(self->field_0024));

    return -1;
}

HOOK(0x006B2C90, C_00860288_FindIndexByName, "auto");
