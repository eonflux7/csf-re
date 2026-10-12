#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, ConstructElementAtEnd, 0x006E1680, (int a1, int a2, void* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GrowAndEmplaceElement, 0x006E20E0, (void* self, void* edx, uint32_t* a2, void* a3, void* a4));

// Appends one 60-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_6E20E0).
// Otherwise it constructs the new element at end (sub_6E1680) and advances end by 60.
static uint32_t* __fastcall S_006e20e0_EmplaceBack(S_006e20e0* self, void* /*edx*/, void* a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 60);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 60))
    {
        (void)CSF_VTBL(0x006E20E0);
        return GrowAndEmplaceElement(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                                     reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
                                     a3);
    }

    uint32_t end = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(
        ConstructElementAtEnd(static_cast<int>(end), 1, a3,
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(a3))));
    self->field_0008 = end + 60;
    return result;
}

HOOK(0x006E25C0, S_006e20e0_EmplaceBack, "auto");
