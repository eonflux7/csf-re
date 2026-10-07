#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970,
         (std::uint8_t* self, void* edx, char* attributeName));

CSF_GLOBAL(char*, g_Component3Attribute, 0x008A8630);
CSF_GLOBAL(char*, g_Component0Attribute, 0x008A8634);
CSF_GLOBAL(char*, g_Component1Attribute, 0x008A8638);
CSF_GLOBAL(char*, g_Component2Attribute, 0x008A863C);

static int __fastcall GetObjectAndSetComponents(
    void* self, void* /*edx*/, std::uint8_t* attributes)
{
    if (!attributes)
        return 0;

    void* provider = CSF_AT(void*, self, 0x04);
    const int objectAddress =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[18])(provider, nullptr);

    float components[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    components[3] = static_cast<float>(
        ReadIntegerAttribute(attributes, nullptr, g_Component3Attribute));
    components[0] = static_cast<float>(
        ReadIntegerAttribute(attributes, nullptr, g_Component0Attribute));
    components[1] = static_cast<float>(
        ReadIntegerAttribute(attributes, nullptr, g_Component1Attribute));
    components[2] = static_cast<float>(
        ReadIntegerAttribute(attributes, nullptr, g_Component2Attribute));

    void* object = reinterpret_cast<void*>(objectAddress);
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, object, 0)[11])(object, nullptr, components);

    return objectAddress;
}

HOOK(0x0071E2C0, GetObjectAndSetComponents, "auto");
