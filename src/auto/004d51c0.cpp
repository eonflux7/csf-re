#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_DegreesToRadians, 0x0083BCF0);

CSF_FUNC(int, __fastcall, ForwardRequestWithoutMovement_4DB380, 0x004DB380, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* edx, int movement));
CSF_FUNC(int, __fastcall, MovementStarted_4D4F70, 0x004D4F70, (void* self, void* edx));

static int __fastcall C_0083c198_BeginMovementFromRequest(C_0083c198* self, void* /*edx*/, int a2)
{
    if ((self->field_03c4 & 0x80000000u) == 0)
        return ForwardRequestWithoutMovement_4DB380(self, nullptr, a2);

    uint8_t* request = reinterpret_cast<uint8_t*>(a2);
    self->field_03c0 = CSF_AT(uint32_t, request, 4);

    int v4 = CSF_AT(int, request, 8);
    self->field_03c4 = static_cast<uint32_t>(v4);

    if (v4 >= 0)
    {
        self->field_03e4 = CSF_AT(uint8_t, request, 12);

        C_008342e8* resolver = reinterpret_cast<C_008342e8*>(self->field_0014);
        float headingRadians = static_cast<float>(C_008342e8_ComputeResolvedObjectScalar(resolver, nullptr)) * g_DegreesToRadians;
        *reinterpret_cast<float*>(&self->field_03e8) = headingRadians;

        int movementArg = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x340);
        int startResult = C_0082f9f8_StartMovement_4DB140(reinterpret_cast<C_0082f9f8*>(self), nullptr, movementArg);

        return MovementStarted_4D4F70(reinterpret_cast<void*>(startResult), nullptr);
    }

    return a2;
}

HOOK(0x4D51C0, C_0083c198_BeginMovementFromRequest, "auto");
