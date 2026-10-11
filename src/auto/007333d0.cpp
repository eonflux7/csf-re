#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall C_008670f0_UpdateAnchorPosition(C_008670f0* self, void* /*edx*/)
{
    void* parent = reinterpret_cast<void*>(self->field_0014);
    if (parent)
    {
        const int32_t width = static_cast<int32_t>(self->field_00a0 - self->field_0098);
        const int32_t height = static_cast<int32_t>(self->field_00a4 - self->field_009c);
        const float horizontal = static_cast<float>(width) * g_HalfDurationScale;
        const float parentHeight = static_cast<float>(
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, parent, 0)[0x24 / 4])(parent, nullptr));

        float vertical = static_cast<float>(height);
        if (vertical > parentHeight)
            vertical = (vertical - parentHeight) * g_HalfDurationScale + parentHeight;

        self->field_00f4 = 0;
        self->field_00fc = 1;
        self->field_00ec = horizontal;
        self->field_00f0 = vertical;
    }
    return reinterpret_cast<int>(&self->field_00ec);
}

HOOK(0x007333D0, C_008670f0_UpdateAnchorPosition, "auto");
