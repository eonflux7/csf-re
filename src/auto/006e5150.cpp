#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resolves the chained value referenced by this->field_0008 (keyed by this object's
// address, depth 3) and returns it as a float widened to double.
CSF_FUNC(int*, __fastcall, C_0085e7b8_ResolveChainedValue, 0x006C48C0,
         (C_0085e7b8* self, void* /*edx*/, int* a2, int a3));

static double __fastcall C_00861bd8_GetResolvedFloatValue(C_00861bd8* self, void* /*edx*/)
{
    int thisKey = reinterpret_cast<int>(self);
    C_0085e7b8* chainRoot = reinterpret_cast<C_0085e7b8*>(self->field_0008);
    int* resolved = C_0085e7b8_ResolveChainedValue(chainRoot, nullptr, &thisKey, 3);
    float value = *reinterpret_cast<float*>(resolved);
    return static_cast<double>(value);
}

HOOK(0x6E5150, C_00861bd8_GetResolvedFloatValue, "auto");
