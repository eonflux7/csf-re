#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TAutonomo_PreUpdate, 0x004D73E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* edx, float a2, char a3));

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083C548);

// Resolves the controller object reached from the result of CharTask_StartMovement.
static void* GetMovementController(int started)
{
    uint8_t* a = reinterpret_cast<uint8_t*>(started);
    uint8_t* b = CSF_AT(uint8_t*, a, 8);
    uint8_t* c = CSF_AT(uint8_t*, b, 0x14);
    return CSF_AT(void*, c, 0x30);
}

static int __fastcall C_0082f9f8_Update(C_0082f9f8* self, void* /*edx*/)
{
    TAutonomo_PreUpdate(self, nullptr);

    uint8_t* owner = CSF_AT(uint8_t*, self, 8);

    if (CSF_AT(uint8_t, owner, 0x314))
    {
        char flag = CSF_AT(char, owner, 0x32d);
        void* target = owner + 0x31c;
        int started = CharTask_StartMovement(self, nullptr, (int)(intptr_t)&self->field_0014);
        void* ctrl = GetMovementController(started);
        void** vt = CSF_AT(void**, ctrl, 0);
        return ((int(__fastcall*)(void*, void*, void*, char))vt[0xAC / 4])(ctrl, nullptr, target, flag);
    }

    float* pos = GetObjectPositionVector(CSF_AT(void*, owner, 0x14), nullptr);
    // x87 keeps differences, squares and sum in extended precision: use double.
    double dx = (double)CSF_AT(float, self, 0x68) - (double)pos[0];
    double dy = (double)CSF_AT(float, self, 0x6C) - (double)pos[1];
    double dz = (double)CSF_AT(float, self, 0x70) - (double)pos[2];
    double dist = std::sqrt(dz * dz + dy * dy + dx * dx);

    if (dist <= (double)g_MessageBoxAnimationScale)
    {
        float angle = CSF_AT(float, self, 0x74) * g_DegreesToRadians;
        int started = CharTask_StartMovement(self, nullptr, (int)(intptr_t)(reinterpret_cast<uint8_t*>(self) + 0x28));
        C_0082f9f8* ctrl = reinterpret_cast<C_0082f9f8*>(GetMovementController(started));
        return C_0082f9f8_Start(ctrl, nullptr, angle, 1);
    }
    else
    {
        int started = CharTask_StartMovement(self, nullptr, (int)(intptr_t)&self->field_0014);
        void* ctrl = GetMovementController(started);
        void** vt = CSF_AT(void**, ctrl, 0);
        return ((int(__fastcall*)(void*, void*, void*, int))vt[0xAC / 4])(
            ctrl, nullptr, reinterpret_cast<uint8_t*>(self) + 0x68, 0);
    }
}

HOOK(0x004D85D0, C_0082f9f8_Update, "auto");
