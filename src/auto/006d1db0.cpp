#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GfxLights_AddLightToWorld, 0x006D0E70, (void* a1, int a2));
CSF_FUNC(int, __cdecl, GfxLights_RemoveLightFromWorld, 0x006D0EB0, (void* a1));
CSF_FUNC(char, __fastcall, C_00860e68_AttachLight, 0x006D1D30, (void* self, void* edx, void* a2));

[[maybe_unused]] static const auto kArrayVtableReference = CSF_VTBL(0x0041E5F0);

static char __fastcall C_00860e68_RegisterLight(
    C_00860e68* self, void* /*edx*/, uint32_t* light)
{
    const int lightType =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, light, 0)[0x18 / 4])(light, nullptr);
    const bool isTypeZero = lightType == 0;
    const bool isSpecialLight =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, light, 0)[0x1C / 4])(light, nullptr) == 3;

    S_0041e5f0* lightList = nullptr;
    if (lightType == 1)
        lightList = reinterpret_cast<S_0041e5f0*>(&self->field_0034);
    else if (lightType == 2)
        lightList = reinterpret_cast<S_0041e5f0*>(&self->field_0044);

    if (lightList)
    {
        if (lightList->field_0008 >= lightList->field_0004)
        {
            S_0041e5f0_GrowArrayCapacity(
                lightList, nullptr, static_cast<int>(lightList->field_000c));
        }

        const uint32_t index = lightList->field_0008;
        auto* entry =
            reinterpret_cast<uint32_t*>(lightList->field_0000) + index;
        lightList->field_0008 = index + 1;
        *entry = reinterpret_cast<uint32_t>(light);
    }

    if (isSpecialLight)
    {
        if (self->field_005c >= self->field_0058)
        {
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(&self->field_0054),
                nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x60)));
        }

        const uint32_t index = self->field_005c;
        auto* entry = reinterpret_cast<uint32_t*>(self->field_0054) + index;
        self->field_005c = index + 1;
        *entry = reinterpret_cast<uint32_t>(light);
    }

    GfxLights_RemoveLightFromWorld(light);

    char result = static_cast<char>(isTypeZero);
    if (!isTypeZero)
    {
        if (self->field_009c)
        {
            result = static_cast<char>(GfxLights_AddLightToWorld(
                light, static_cast<int>(self->field_009c)));
        }
        else
        {
            result = C_00860e68_AttachLight(self, nullptr, light);
        }
    }

    self->field_0084 = 0;
    return result;
}

HOOK(0x006D1DB0, C_00860e68_RegisterLight, "auto");
