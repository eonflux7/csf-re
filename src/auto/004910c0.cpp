#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00833908_PrepareUpdate, 0x00490720,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00833908_FinishComponentUpdate, 0x0048FEF0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_UpdateInternalState, 0x0048EFC0,
         (void* self, void* edx, float a2));

static char __fastcall C_00833908_UpdateComponents(
    C_00833908* self, void* /*edx*/, float deltaTime)
{
    C_00833908_PrepareUpdate(self, nullptr);

    char result = 0;
    void* component = reinterpret_cast<void*>(self->field_0048);
    if (component)
    {
        result = static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, component, 0)[0x2C / 4])(
                    component, nullptr, deltaTime));
    }

    if ((self->field_0170 & 0x10) == 0)
        return result;

    result = static_cast<char>(self->field_0174);
    if (!result)
        return result;

    component = reinterpret_cast<void*>(self->field_0034);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x90 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_0038);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x88 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_0030);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x90 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_003c);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x98 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_0040);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x84 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_0044);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x84 / 4])(
                component, nullptr, deltaTime);
    }

    component = reinterpret_cast<void*>(self->field_004c);
    if (component)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, component, 0)[0x30 / 4])(
                component, nullptr, deltaTime);
    }

    C_00833908_FinishComponentUpdate(self, nullptr);
    return static_cast<char>(
        C_00833908_UpdateInternalState(self, nullptr, deltaTime));
}

HOOK(0x004910C0, C_00833908_UpdateComponents, "auto");

