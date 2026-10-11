#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_008670f0_UpdateAfterPositionQuery, 0x00733310,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_GetFallbackPosition, 0x007333D0,
         (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall C_008670f0_GetPosition(
    C_008670f0* self, void* /*edx*/, float* position)
{
    const int32_t height =
        static_cast<int32_t>(self->field_00a4 - self->field_009c);

    void* child = reinterpret_cast<void*>(self->field_0014);
    ((double(__fastcall*)(void*, void*, int))
        CSF_AT(void**, child, 0)[0x1C / 4])(child, nullptr, 1);

    const uint32_t count = self->field_0080;
    if (count)
    {
        const auto* lastEntry = reinterpret_cast<const uint8_t*>(
            self->field_0078 + 28 * count - 28);
        const void* entryObject = CSF_AT(void*, lastEntry, 0);

        position[0] =
            static_cast<float>(CSF_AT(int32_t, entryObject, 0x28)) *
                self->field_00c0 +
            CSF_AT(float, lastEntry, 0x04) -
            static_cast<float>(static_cast<int32_t>(self->field_0098));

        child = reinterpret_cast<void*>(self->field_0014);
        const double childHeight =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr);

        const float x = position[0];
        position[1] = static_cast<float>(
            (static_cast<double>(height) - childHeight) *
                g_HalfDurationScale +
            childHeight + position[1]);

        self->field_00ec = x;
        self->field_00f0 = position[1];
    }
    else
    {
        const float* fallback = reinterpret_cast<const float*>(
            C_008670f0_GetFallbackPosition(self, nullptr));
        position[0] = fallback[0];
        position[1] = fallback[1];
    }

    C_008670f0_UpdateAfterPositionQuery(self, nullptr);
}

HOOK(0x00733460, C_008670f0_GetPosition, "auto");
