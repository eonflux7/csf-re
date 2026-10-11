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

// Degrees-to-radians factor stored as a float constant in .rdata (fmul dword ptr [0x82e898]).
CSF_GLOBAL(float, g_DegToRadFactor, 0x0082E898);

CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SubObjectSetRotation, 0x0041E420, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, SubObjectSetSecondaryAngle, 0x0041E7E0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, SubObjectSetSpeedLimit, 0x0041E820, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, DispatchStateFromField, 0x004DB140, (void* self, void* edx, int a2));

// Updates the facing/rotation sub-object of this task from the resolved object's scalar,
// then dispatches to one of two state slots depending on the alternate-offset flag.
static int __fastcall C_0082f9f8_UpdateFacingAndDispatch(C_0082f9f8* self, void* /*edx*/, float a2, char a3)
{
    // Sub-object record (type S_0041e820) lives at +0x244 of the owner object.
    (void)CSF_VTBL(0x0041E820);

    // Flag: a3 set and the owner's byte at +0x230 is clear.
    bool useAlternateOffset = (a3 != 0) && (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x230) == 0);
    self->pad_0020[48] = useAlternateOffset ? 1 : 0;

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    double scalar = ComputeScalarForResolvedObject(CSF_AT(void*, owner, 0x14), nullptr);
    float rotationRadians = static_cast<float>(scalar * g_DegToRadFactor);

    void* subObject = owner + 0x244;
    SubObjectSetRotation(subObject, nullptr, rotationRadians);
    SubObjectSetSecondaryAngle(owner + 0x244, nullptr, a2);
    SubObjectSetSpeedLimit(owner + 0x244, nullptr, CSF_AT(float, owner, 0x200));

    if (self->pad_0020[48]) {
        return DispatchStateFromField(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(&self->pad_0020[8])));
    }
    return DispatchStateFromField(self, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0014)));
}

HOOK(0x421B30, C_0082f9f8_UpdateFacingAndDispatch, "auto");
