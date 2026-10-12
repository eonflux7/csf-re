#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, ConstructElementAtEnd, 0x0040B4C0, (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GrowAndEmplaceElement, 0x0040C070, (void* self, void* edx, uint32_t* a2, char* a3, uint32_t* a4));

// Appends one 28-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_40C070).
// Otherwise it constructs the new element at end (sub_40B4C0) and advances end by 28.
static uint32_t* __fastcall S_0040c070_EmplaceBack(S_0040c070* self, void* /*edx*/, uint32_t* a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 28);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 28))
    {
        (void)CSF_VTBL(0x0040C070);
        return GrowAndEmplaceElement(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                                     reinterpret_cast<char*>(self->field_0008), a3);
    }

    uint32_t end = self->field_0008;
    uint32_t* result = ConstructElementAtEnd(reinterpret_cast<uint32_t*>(end), 1, a3,
                                             static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                                             reinterpret_cast<int>(a3));
    self->field_0008 = end + 28;
    return result;
}

HOOK(0x0040C2A0, S_0040c070_EmplaceBack, "auto");
