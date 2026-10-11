#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A4F10, 0x004A4F10, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_4A5C40, 0x004A5C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4A3020, 0x004A3020, (void* self, void* edx, int a2, int a3, int a4, int a5));

// Looks up the table index for a2; if found, forwards to the virtual slot 0x54
// with the table entry's value. Otherwise runs the fallback path.
static int __fastcall C_00834598_DispatchByLookup(C_00834598* self, void* /*edx*/,
                                                  int a2, int a3, int a4, int a5)
{
    int index = sub_4A4F10(self, nullptr, a2);
    if (index != -1)
    {
        uint8_t* table = reinterpret_cast<uint8_t*>(self->field_007c);
        int entryValue = CSF_AT(int, table + 8 * index, 4);
        return self->vSlot54(entryValue, a3, a4, a5);
    }

    sub_4A5C40(self, nullptr);
    return sub_4A3020(self, nullptr, a2, a3, a4, a5);
}

HOOK(0x004A5CC0, C_00834598_DispatchByLookup, "auto");
