#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Fills the 44-byte record slot at `destination` from `argument` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int*, __cdecl, sub_776F60, 0x00776F60,
         (int a1, int a2, void* a3, int a4, int a5));

// Grows the 44-byte record array and inserts one record (__fastcall, ret 12).
CSF_FUNC(uint32_t*, __fastcall, sub_778920, 0x00778920,
         (void* self, void* edx, uint32_t* a2, void* a3, void* a4));

// Appends one 44-byte record to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to sub_778920.
static int* __fastcall S_00778920_AppendRecord44(
    S_00778920* self, void* /*edx*/, void* argument)
{
    const uint32_t begin = self->field_0004;
    uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 44);
    }

    if (begin == 0 ||
        size >= static_cast<uint32_t>(
            static_cast<std::int32_t>(self->field_000c - begin) / 44))
    {
        return reinterpret_cast<int*>(
            sub_778920(self, nullptr, reinterpret_cast<uint32_t*>(&argument),
                       reinterpret_cast<void*>(self->field_0008), argument));
    }

    const uint32_t recordEnd = self->field_0008;
    int* const result = sub_776F60(
        static_cast<int>(recordEnd), 1, argument,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(argument)));

    self->field_0008 = recordEnd + 44u;
    return result;
}

HOOK(0x00778A90, S_00778920_AppendRecord44, "auto");
