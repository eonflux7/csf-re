#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __stdcall, SceneLightsObjects_ProcessComponent,
         0x004F1870, (void* a1, int a2));

static int __fastcall SceneLightsObjects_ReadObjectProperties(
    S_004f1a60* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    auto* properties = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, reader, 0)[0x80 / 4])(
                reader, nullptr, 0, contextAddress);
    }

    int savedContext = CSF_AT(int, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(int, context, 0x1C) = 0;

    if (!failed)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, reader, 0)[0x44 / 4])(
                    reader, nullptr, properties + 0x3C, contextAddress) == -1;
    }

    bool processComponent = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (processComponent)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLightsObjects_ProcessComponent(properties + 0x08, contextAddress);
    }

    processComponent = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (processComponent)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLightsObjects_ProcessComponent(properties + 0x18, contextAddress);
    }

    processComponent = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (processComponent)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLightsObjects_ProcessComponent(properties + 0x28, contextAddress);
    }

    processComponent = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (processComponent)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, reader, 0)[0x54 / 4])(
                    reader, nullptr, properties + 0x38, contextAddress) == -1;
    }

    CSF_AT(int, context, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* reader = CSF_AT(void*, context, 0x04);
        int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, reader, 0)[0x7C / 4])(
                reader, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004F1A60, SceneLightsObjects_ReadObjectProperties, "auto");
