#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, sub_46D770, 0x0046D770, (int a1, int a2, void* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, sub_46E250, 0x0046E250, (void* self, void* edx, uint32_t* a2, void* a3, void* a4));

// Appends one 28-byte element to the buffer (begin = +4, end = +8, capacity = +0C).
// If there is no storage or the buffer is full, it delegates to the grow/reallocate path (sub_46E250).
// Otherwise it constructs the new element at end (sub_46D770) and advances end by 28.
static void* __fastcall S_0046e250_EmplaceBack(S_0046e250* self, void* /*edx*/, void* a3)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) / 28);

    if (begin == 0 || index >= static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) / 28))
    {
        (void)CSF_VTBL(0x0046E250);
        return sub_46E250(self, nullptr, reinterpret_cast<uint32_t*>(&a3),
                          reinterpret_cast<void*>(self->field_0008), a3);
    }

    uint32_t end = self->field_0008;
    void* result = sub_46D770(static_cast<int>(end), 1,
                              a3,
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)));
    self->field_0008 = end + 28;
    return result;
}

HOOK(0x0046E380, S_0046e250_EmplaceBack, "auto");
