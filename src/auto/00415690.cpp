#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082dfd0_PrepareUpdate, 0x00411A30,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0082dfd0_FinishUpdate, 0x00411B10,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082dfd0_BuildDirectionalUpdate, 0x00412B10,
         (void* self, void* edx, void* a2, float* a3, float a4));
CSF_FUNC(int, __fastcall, C_0082dfd0_ApplyDirectionalUpdate, 0x00415220,
         (void* self, void* edx, float a2, int* a3, int a4));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

static void __fastcall C_0082dfd0_Update(
    C_0082dfd0* self, void* /*edx*/, int argument)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    self->field_008a = 0;
    self->field_0094 = 0.0f;

    if (CSF_AT(uint32_t, owner, 0x50) == 0 &&
        (CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) != 0)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x34 / 4])(self, nullptr);
    }

    if (!self->field_0089)
    {
        owner = reinterpret_cast<void*>(self->field_0004);
        bool performDirectionalUpdate = CSF_AT(uint32_t, owner, 0x50) != 0;
        if (!performDirectionalUpdate)
        {
            const uint32_t flags = CSF_AT(uint32_t, owner, 0x2E0);
            performDirectionalUpdate =
                (flags & 0x10000) == 0 && (flags & 0x2000) == 0;
        }

        if (performDirectionalUpdate)
        {
            float step;
            std::memcpy(&step, &argument, sizeof(step));
            C_0082dfd0_PrepareUpdate(self, nullptr, step);

            float direction[3];
            direction[0] = -g_AxisY;
            direction[1] = -g_AxisY_Y;
            direction[2] = -g_AxisY_Z;
            int result[3] = {};

            C_0082dfd0_BuildDirectionalUpdate(
                self, nullptr, result, direction, step);
            C_0082dfd0_ApplyDirectionalUpdate(
                self, nullptr, step, reinterpret_cast<int*>(direction),
                reinterpret_cast<int>(result));
        }
    }

    C_0082dfd0_FinishUpdate(self, nullptr, argument);
}

HOOK(0x00415690, C_0082dfd0_Update, "auto");
