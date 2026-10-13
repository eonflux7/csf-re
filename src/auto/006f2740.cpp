#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00862598_SetChildObject, 0x006CA030, (C_00862598* self, void* , uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, S_006e20e0_RegisterEmitter, 0x006E27D0, (S_006e20e0* self, void* , std::uint32_t* definition));
CSF_FUNC(int, __fastcall, S_006e2150_FindOrCreateParticleState, 0x006E2980, (S_006e2150* self, void* , std::uint32_t* parameters));

static uint32_t* __fastcall C_00862598_ResolveEmitterHandles(C_00862598* self, void* /*edx*/, uint32_t* a2)
{
    if (self->field_0024 != 0xFFFFFFFFu)
    {
        uint32_t definitionValue = self->field_0018;
        S_006e20e0* emitterOwner = (S_006e20e0*)GetValue_8BB738();
        a2[9] = (uint32_t)S_006e20e0_RegisterEmitter(emitterOwner, nullptr,
                                                    reinterpret_cast<uint32_t*>(definitionValue));
    }

    if (self->field_0028 != 0xFFFFFFFFu)
    {
        uint32_t parametersValue = self->field_0018;
        S_006e2150* stateOwner = (S_006e2150*)GetValue_8BB738();
        a2[10] = (uint32_t)S_006e2150_FindOrCreateParticleState(stateOwner, nullptr,
                                                               reinterpret_cast<uint32_t*>(parametersValue));
    }

    a2[15] = self->field_003c;
    a2[16] = self->field_0040;
    return C_00862598_SetChildObject(self, nullptr, a2);
}

HOOK(0x006F2740, C_00862598_ResolveEmitterHandles, "auto");
