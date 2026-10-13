#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0, (void* self, void* edx, uint32_t count, uint32_t flag));

// Reserves capacity for the range [a3, a4) of the wide string, then writes the
// cursor (pointer to the element at the start of that range) into *a2 and returns a2.
static int* __fastcall S_004f21d0_ReserveRangeAndGetCursor(S_004f21d0* self, void* /*edx*/, int* a2, int a3, unsigned int a4)
{
    // Address of the embedded SSO buffer (self + 4).
    uint16_t* sso_buffer = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(self) + 4);

    // Current data pointer: inline buffer when capacity < 8, otherwise the heap pointer stored at +4.
    uint16_t* data = (self->field_0018 < 8) ? sso_buffer : CSF_AT(uint16_t*, self, 0x04);

    // Element index of a3 relative to the data start (signed, as in the original).
    int start_index = 0;
    if (a3 != 0)
        start_index = (a3 - reinterpret_cast<int>(data)) >> 1;

    // Element count of the range [a3, a4) (unsigned subtraction, then signed shift).
    unsigned int range_count = a4;
    if (a4 != 0)
        range_count = static_cast<unsigned int>(static_cast<int>(a4 - a3) >> 1);

    ReserveSsoOrHeapCapacity(self, nullptr, static_cast<uint32_t>(start_index), range_count);

    // Re-read the storage mode and data pointer after the call.
    uint16_t* cursor;
    if (self->field_0018 < 8)
        cursor = sso_buffer + start_index;
    else
        cursor = CSF_AT(uint16_t*, self, 0x04) + start_index;

    *a2 = static_cast<int>(reinterpret_cast<uintptr_t>(cursor));
    return a2;
}

HOOK(0x004F21D0, S_004f21d0_ReserveRangeAndGetCursor, "auto");
