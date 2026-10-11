#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __cdecl, sub_6C0EE0, 0x006C0EE0, (int a1, int a2));
CSF_FUNC(char*, __cdecl, sub_6E81A0, 0x006E81A0, (int a1));
CSF_FUNC(uint32_t*, __fastcall, sub_6F76C0, 0x006F76C0, (void* self, void* edx, int a2, int a3));

// Looks for an existing entry in the table whose +4 key matches a2 (via sub_6C0EE0).
// If found with a non-zero +0x10 value, that value is used; otherwise a fallback
// value comes from sub_6E81A0. Then a 0x24-byte object is allocated and initialized.
static int __fastcall C_00860bb8_CreateEntryForObject(C_00860bb8* self, void* /*edx*/, int a2)
{
    uint32_t entryValue = 0;
    uint8_t* matchedEntry = nullptr;
    bool found = false;

    uint32_t index = 0;
    if (self->field_005c != 0) {
        for (;;) {
            uint32_t* table = reinterpret_cast<uint32_t*>(self->field_0054);
            uint8_t* entry = reinterpret_cast<uint8_t*>(table[index]);
            if (sub_6C0EE0(a2, static_cast<int>(CSF_AT(uint32_t, entry, 4)))) {
                matchedEntry = entry;
                found = true;
                break;
            }
            ++index;
            if (index >= self->field_005c) {
                break;
            }
        }
    }

    if (found) {
        entryValue = CSF_AT(uint32_t, matchedEntry, 0x10);
    }
    if (!found || entryValue == 0) {
        entryValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sub_6E81A0(a2)));
    }

    void* block = Mem_Alloc(0x24);
    if (block) {
        return reinterpret_cast<int>(sub_6F76C0(block, nullptr, a2, static_cast<int>(entryValue)));
    }
    return 0;
}

HOOK(0x006CB8C0, C_00860bb8_CreateEntryForObject, "auto");
