#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_421CA0, 0x00421CA0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void*, uint32_t* output));

static void __fastcall TFncGirar_StepTowardsTarget(TFncGirar* self, void* /*edx*/, float arg0)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);

    // Original stack block of 0x1C bytes: direction output (3 floats),
    // delta vector (3 floats), squared distance (1 float).
    float block[7];
    float* dir = &block[0];
    float* delta = &block[3];

    delta[0] = CSF_AT(float, owner, 0x224) - CSF_AT(float, owner, 0x218);
    delta[1] = CSF_AT(float, owner, 0x228) - CSF_AT(float, owner, 0x21C);
    delta[2] = CSF_AT(float, owner, 0x22C) - CSF_AT(float, owner, 0x220);

    S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(delta), nullptr,
                                reinterpret_cast<uint32_t*>(dir));

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    float step = arg0 * CSF_AT(float, owner, 0x200);
    block[6] = delta[2] * delta[2] + delta[1] * delta[1] + delta[0] * delta[0];

    float mx = dir[0] * step;
    float my = dir[1] * step;
    float mz = dir[2] * step;
    CSF_AT(float, owner, 0x218) = mx + CSF_AT(float, owner, 0x218);
    CSF_AT(float, owner, 0x21C) = my + CSF_AT(float, owner, 0x21C);
    CSF_AT(float, owner, 0x220) = mz + CSF_AT(float, owner, 0x220);

    uint8_t* obj = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t pos[3];
    pos[0] = CSF_AT(uint32_t, obj, 0x218);
    pos[1] = CSF_AT(uint32_t, obj, 0x21C);
    pos[2] = CSF_AT(uint32_t, obj, 0x220);

    uint32_t sub14 = CSF_AT(uint32_t, obj, 0x14);
    uint32_t target = CSF_AT(uint32_t, sub14, 0x4C);
    void** vtbl = CSF_AT(void**, target, 0);
    ((void(__fastcall*)(void*, void*, void*))vtbl[0x50 / 4])(reinterpret_cast<void*>(target), nullptr, pos);

    sub_421CA0(obj, nullptr);

    if (step * step >= block[6])
        sub_4DB1D0(self, nullptr, 0);
}

HOOK(0x00422800, TFncGirar_StepTowardsTarget, "auto");
