#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, TaskObj_GetImpactSpeed, 0x0048E510, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, TaskObj_CanStartAnim, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, Task_InitFromVector, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_0082f9f8_StartImpactAnim(C_0082f9f8* self, void* /*edx*/, float scale)
{
    int result;

    CSF_AT(uint8_t, self, 0x2B8) = 0;
    self->field_0018 &= 0xFFFFFFF9;

    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (owner)
    {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr);
        if (result == 7)
            return (char)result;
    }

    result = (unsigned char)TaskObj_CanStartAnim(reinterpret_cast<void*>(self->field_0014), nullptr, (int)self, 2, 1);
    if ((char)result == 0)
        return (char)result;

    CSF_AT(int, self, 0x1FC) = 1;

    uint32_t* pos = reinterpret_cast<uint32_t*>(GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr));
    CSF_AT(uint32_t, self, 0x20C) = pos[0];
    CSF_AT(uint32_t, self, 0x210) = pos[1];
    CSF_AT(uint32_t, self, 0x214) = pos[2];
    CSF_AT(uint32_t, self, 0x218) = pos[0];
    CSF_AT(uint32_t, self, 0x21C) = pos[1];
    CSF_AT(uint32_t, self, 0x220) = pos[2];

    int animId = 5;
    if (scale < g_fZero)
        animId = 6;

    void* target = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(int, self, 0x238) = animId;
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, target, 0)[0x8C / 4])(target, nullptr, animId, 0, -1, 0);

    void* t2 = reinterpret_cast<void*>(self->field_0014);
    void* o1 = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, t2, 0)[0x28 / 4])(t2, nullptr);
    void* o2 = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, o1, 0)[0x18 / 4])(o1, nullptr);

    std::uint32_t buf[2];
    float** res = ((float**(__fastcall*)(void*, void*, std::uint32_t*))CSF_AT(void**, o2, 0)[0x8C / 4])(o2, nullptr, buf);
    float* dir = *res;
    float negX = -dir[0];
    float negY = -dir[1];
    float negZ = -dir[2];

    if (reinterpret_cast<uint8_t*>(buf)[4])
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(buf[0]));

    float dx = negX * scale;
    float dy = negY * scale;
    float dz = negZ * scale;

    float* pos2 = reinterpret_cast<float*>(GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr));
    float px = dx + pos2[0];
    float py = dy + pos2[1];
    float pz = dz + pos2[2];
    CSF_AT(float, self, 0x224) = px;
    CSF_AT(float, self, 0x228) = py;
    CSF_AT(float, self, 0x22C) = pz;

    int animNow = CSF_AT(int, self, 0x238);
    CSF_AT(float, self, 0x200) = (float)TaskObj_GetImpactSpeed(reinterpret_cast<void*>(self->field_0014), nullptr, animNow);
    CSF_AT(uint8_t, self, 0x230) = 0;

    result = Task_InitFromVector(self, nullptr, (int)((uint8_t*)self + 0x74));
    self->field_0018 |= 2u;
    return (char)result;
}

HOOK(0x004208E0, C_0082f9f8_StartImpactAnim, "auto");
