#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(uint32_t*, __cdecl, ShiftTimedEntriesLeft, 0x006F9FE0,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static int __fastcall S_00452280_UpdateTimedEntries(
    S_00452280* self, void* /*edx*/, int elapsedBits)
{
    float elapsed;
    std::memcpy(&elapsed, &elapsedBits, sizeof(elapsed));

    int count = 0;
    if (self->field_0100)
        count = static_cast<int32_t>(
            self->field_0104 - self->field_0100) / 20;

    int result = count - 1;
    if (result < 0)
        return result;

    int offset = 20 * result;
    int entriesRemaining = result + 1;
    do
    {
        auto* entry =
            reinterpret_cast<uint8_t*>(self->field_0100) + offset;
        float remainingTime = CSF_AT(float, entry, 0) - elapsed;
        CSF_AT(float, entry, 0) = remainingTime;

        if (remainingTime < g_fZero)
        {
            entry = reinterpret_cast<uint8_t*>(self->field_0100) + offset;
            ShiftTimedEntriesLeft(
                reinterpret_cast<uint32_t*>(entry + 20),
                reinterpret_cast<uint32_t*>(self->field_0104),
                reinterpret_cast<uint32_t*>(entry),
                elapsedBits);
            self->field_0104 -= 20;
        }

        offset -= 20;
        result = --entriesRemaining;
    } while (entriesRemaining);

    return result;
}

HOOK(0x00453A80, S_00452280_UpdateTimedEntries, "auto");
