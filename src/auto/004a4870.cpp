#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A4840, 0x004A4840, (void* self, void* edx, int a2, int a3, int a4));

static int __fastcall C_00834598_GetStateCode(
    C_00834598* self, void* /*edx*/, int key)
{
    void* related = reinterpret_cast<void*>(self->field_0008);

    if (key == 8 && self->field_002c)
    {
        if (self->field_0034 == 1)
        {
            const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
            if (state == 0)
                return 25;
            if (state == 1)
                return 63;
            return -1;
        }

        if (self->field_002c)
        {
            if (self->field_0034 == 2)
            {
                const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
                if (state == 0)
                    return 28;
                if (state == 1)
                    return 66;
                return -1;
            }

            if (self->field_002c)
            {
                if (self->field_0034 == 3)
                {
                    const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
                    if (state == 0)
                        return 48;
                    if (state == 1)
                        return 67;
                    return -1;
                }

                if (self->field_002c && self->field_0034 == 4)
                {
                    const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
                    if (state == 0)
                        return 52;
                    if (state == 1)
                        return 67;
                    return -1;
                }
            }
        }
    }

    if (static_cast<uint8_t>(sub_4A4840(self, nullptr, key, 23, 8)) != 0)
    {
        const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
        if (state == 0)
            return 48;
        if (state == 1)
            return 67;
        return -1;
    }

    if (static_cast<uint8_t>(sub_4A4840(self, nullptr, key, 5, 8)) != 0)
    {
        const int alternate = CSF_AT(int, related, 0x1E0);
        const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
        if (state == 0)
            return alternate ? 50 : 26;
        if (state == 1)
            return 64;
    }
    else if (static_cast<uint8_t>(sub_4A4840(self, nullptr, key, 6, 8)) != 0)
    {
        const int alternate = CSF_AT(int, related, 0x1E0);
        const uint32_t state = CSF_AT(uint32_t, related, 0x1C4);
        if (state == 0)
            return alternate ? 51 : 27;
        if (state == 1)
            return 65;
    }

    return -1;
}

HOOK(0x004A4870, C_00834598_GetStateCode, "auto");
