#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, CopyRecordArray, 0x00448B60, (uint32_t* self, void*, const uint32_t* source));

static uint32_t* __cdecl Assign(uint32_t* first, uint32_t* last, const uint32_t* source) {
    for (; first != last; first += 5) {
        *first = *source;
        CopyRecordArray(first + 1, nullptr, source + 1);
    }
    return last;
}
HOOK(0x00449550, Assign, "stl");
