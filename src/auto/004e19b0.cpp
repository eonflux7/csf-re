#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));

static void __fastcall C_0083e5e8_ApplyScaledProgress(C_0083e5e8* self, void* /*edx*/, int* a2)
{
    int linkedMax = self->vGetLinkedValueDC();
    double scaledStep = (double)*a2 * (double)g_ProgressScale;
    double increment = (double)linkedMax * scaledStep;
    self->field_0008 += (uint32_t)(int64_t)increment;

    if ((int)self->field_0008 > (int)self->vGetLinkedValueDC())
    {
        self->field_0008 = (uint32_t)self->vGetLinkedValueDC();
    }

    C_0085ec18_NullSub(reinterpret_cast<int>(a2));
}

HOOK(0x004E19B0, C_0083e5e8_ApplyScaledProgress, "auto");
