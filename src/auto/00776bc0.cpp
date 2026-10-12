#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, ControlPointVector_ConstructAtEnd, 0x00775170, (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, ControlPointVector_GrowAndEmplace, 0x00776310, (void* self, void* edx, uint32_t* a2, char* a3, int a4));

// Appends one 60-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_776310).
// Otherwise it constructs the new element at end (sub_775170) and advances end by 60.
static uint32_t* __fastcall ControlPointVector_PushBack(S_00776310* self, void* /*edx*/, uint32_t* a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 60);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 60))
    {
        (void)CSF_VTBL(0x00776310);
        return ControlPointVector_GrowAndEmplace(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                                                 reinterpret_cast<char*>(self->field_0008), reinterpret_cast<int>(a3));
    }

    uint32_t end = self->field_0008;
    uint32_t* result = ControlPointVector_ConstructAtEnd(reinterpret_cast<uint32_t*>(end), 1, a3,
                                                         static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                                                         reinterpret_cast<int>(a3));
    self->field_0008 = end + 60;
    return result;
}

HOOK(0x00776BC0, ControlPointVector_PushBack, "auto");
