#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_73B580, 0x0073B580, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_73B5C0, 0x0073B5C0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_73B600, 0x0073B600, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_NearZeroThreshold, 0x00867480);

// Referenced by the packet's vtable list (address 0x0073B580 appears as a jump target at 0x0073BAA9).
[[maybe_unused]] static const auto kReferencedVtbl = CSF_VTBL(0x0073B580);

static void __fastcall S0073b580_UpdateDefault(S_0073b580* self, void* /*edx*/, float a2)
{
    sub_73B580(self, nullptr, a2);
}

static void __fastcall S0073b580_UpdateModeA(S_0073b580* self, void* /*edx*/, float a2)
{
    sub_73B5C0(self, nullptr, a2);
}

static void __fastcall S0073b580_UpdateModeB(S_0073b580* self, void* /*edx*/, float a2)
{
    sub_73B600(self, nullptr, a2);
}

static void __fastcall S0073b580_DispatchByMode(S_0073b580* self, void* /*edx*/, float a2)
{
    if (self->field_0004 == 0) {
        return;
    }
    if (!(fabs(-self->field_0014) > g_NearZeroThreshold)) {
        return;
    }

    uint32_t mode = self->field_0024;
    if (mode == 0) {
        S0073b580_UpdateDefault(self, nullptr, a2);
    } else if (mode == 1) {
        S0073b580_UpdateModeA(self, nullptr, a2);
    } else if (mode == 2) {
        S0073b580_UpdateModeB(self, nullptr, a2);
    }
}

HOOK(0x0073BA70, S0073b580_DispatchByMode, "auto");
