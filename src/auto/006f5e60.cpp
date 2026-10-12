#include <cstdint>
#include "globals.h"
#include <cstring>
#include <cmath>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(int, __cdecl, RwCameraCreate, 0x006290B0, ());
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(char, __fastcall, S_006e48a0_UpdateCamera, 0x006F5CD0, (void* self, void* edx));

static int __fastcall S_006e48a0_InitializeCamera(
    S_006e48a0* self, void* /*edx*/, int owner, const uint32_t* parameters)
{
    self->field_0000 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862830));
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0.0f;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0034) = 0;

    self->field_0004 = owner;
    self->field_0008 = NAN;
    self->field_003c = 1;
    self->field_0038 = 1128792064;

    self->field_000c = parameters[1];
    self->field_0010 = parameters[8];
    self->field_0014 = parameters[2];
    self->field_0018 = parameters[3];
    std::memcpy(&self->field_001c, &parameters[4], sizeof(float));
    self->field_0020 = parameters[5];
    self->field_0024 = parameters[6];
    self->field_0028 = parameters[7];
    self->field_002c = parameters[0];

    self->field_0030 = RwCameraCreate();
    void* frame = RwFrameCreate();
    _rwObjectHasFrameSetFrame(
        static_cast<int>(self->field_0030), reinterpret_cast<int>(frame));
    S_006e48a0_UpdateCamera(self, nullptr);

    return reinterpret_cast<int>(self);
}

HOOK(0x006F5E60, S_006e48a0_InitializeCamera, "auto");
