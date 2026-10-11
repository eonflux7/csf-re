#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall S_007337a0_SumMatchingScaledValues(
    S_007337a0* self, void* /*edx*/, int matchId)
{
    double total = g_fZero;
    uint32_t count = self->field_0060;
    uint32_t index = 0;
    bool found = false;

    if (static_cast<int32_t>(count) >= 4)
    {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_0058);
        uint8_t* first = entries + 0x10;
        uint8_t* second = entries + 0x40;

        do
        {
            if (CSF_AT(int32_t, first, 0x1C) == matchId)
            {
                found = true;
                total += static_cast<double>(
                    CSF_AT(int32_t, CSF_AT(void*, first, 0), 0x28))
                    * CSF_AT(float, self, 0xC0);
            }
            else if (found)
            {
                return total;
            }

            if (CSF_AT(int32_t, second, 0x1C) == matchId)
            {
                found = true;
                total += static_cast<double>(
                    CSF_AT(int32_t, CSF_AT(void*, second, 0), 0x28))
                    * CSF_AT(float, self, 0xC0);
            }
            else if (found)
            {
                return total;
            }

            if (CSF_AT(int32_t, second, 0x4C) == matchId)
            {
                found = true;
                total += static_cast<double>(
                    CSF_AT(int32_t, CSF_AT(void*, second, 0x30), 0x28))
                    * CSF_AT(float, self, 0xC0);
            }
            else if (found)
            {
                return total;
            }

            if (CSF_AT(int32_t, second, 0x7C) == matchId)
            {
                found = true;
                total += static_cast<double>(
                    CSF_AT(int32_t, CSF_AT(void*, second, 0x60), 0x28))
                    * CSF_AT(float, self, 0xC0);
            }
            else if (found)
            {
                return total;
            }

            first += 0xC0;
            count = self->field_0060;
            index += 4;
            second += 0xC0;
        } while (index < count - 3);
    }

    if (index < count)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            self->field_0058 + 48 * index + 16);

        do
        {
            if (CSF_AT(int32_t, entry, 0x1C) == matchId)
            {
                found = true;
                total += static_cast<double>(
                    CSF_AT(int32_t, CSF_AT(void*, entry, 0), 0x28))
                    * CSF_AT(float, self, 0xC0);
            }
            else if (found)
            {
                return total;
            }

            ++index;
            entry += 0x30;
        } while (index < count);
    }

    return total;
}

HOOK(0x00733830, S_007337a0_SumMatchingScaledValues, "auto");
