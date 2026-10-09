#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Grows the element array of the sub-object (reserve/grow); takes one stack arg.
CSF_FUNC(char, __fastcall, sub_768E80, 0x00768E80, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_769180, 0x00769180, (void* self, void* edx, uint32_t* a2, int a3));

// Appends a 12-byte element to the array sub-object (data, capacity, count, growBy).
static void BindingList_PushElement(uint8_t* list, const uint32_t* src)
{
    if (CSF_AT(uint32_t, list, 8) >= CSF_AT(uint32_t, list, 4))
        sub_768E80(list, nullptr, (int)CSF_AT(uint32_t, list, 0xC));
    uint32_t index = CSF_AT(uint32_t, list, 8);
    uint32_t base = CSF_AT(uint32_t, list, 0);
    CSF_AT(uint32_t, list, 8) = index + 1;
    uint32_t* dst = reinterpret_cast<uint32_t*>(base + 12 * index);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

static S_00768e80* __fastcall InputBindings_Init(S_00768e80* self, void* /*edx*/,
                                                 uint32_t arg0, uint32_t a3, uint32_t a4, uint32_t a5,
                                                 int a6, int a7,
                                                 uint32_t* a8, uint32_t* a9, uint32_t* a10,
                                                 uint32_t* a2, uint32_t* a12)
{
    uint8_t* list = reinterpret_cast<uint8_t*>(self) + 0x10;

    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 10;
    self->field_0000 = arg0;
    self->field_0004 = a3;
    self->field_0008 = a4;
    self->field_000c = a5;

    if (!*a8)
        return self;

    uint32_t needed;
    if (!*a9) {
        needed = 1;
    } else if (!*a10) {
        needed = 2;
    } else if (!*a2) {
        needed = 3;
    } else {
        needed = (*a12 != 0) + 4;
    }

    uint32_t capacity = CSF_AT(uint32_t, list, 4);
    if (needed > capacity)
        sub_768E80(list, nullptr, (int)(needed - capacity));

    if (!*a8)
        return self;

    BindingList_PushElement(list, a8);

    if (*a9) {
        BindingList_PushElement(list, a9);
        if (*a10) {
            BindingList_PushElement(list, a10);
            if (*a2) {
                sub_769180(list, nullptr, a2, 0);
                if (*a12)
                    sub_769180(list, nullptr, a12, 0);
            }
        }
    }
    return self;
}

HOOK(0x007693C0, InputBindings_Init, "auto");
