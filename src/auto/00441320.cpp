#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ConstructElementAtEnd, 0x0043F4A0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GrowAndEmplaceElement, 0x00440E70, (void* self, void* edx, uint32_t* a2, int a3, int a4));

// Appends one 56-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_440E70).
// Otherwise it constructs the new element at end (sub_43F4A0) and advances end by 56.
static uint32_t* __fastcall S_00440e70_EmplaceBack(S_00440e70* self, void* /*edx*/, int a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 56);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 56))
    {
        (void)CSF_VTBL(0x00440E70);
        return GrowAndEmplaceElement(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                                     static_cast<int>(self->field_0008), a3);
    }

    uint32_t end = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(
        ConstructElementAtEnd(static_cast<int>(end), 1, a3,
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), a3));
    self->field_0008 = end + 56;
    return result;
}

HOOK(0x00441320, S_00440e70_EmplaceBack, "auto");
