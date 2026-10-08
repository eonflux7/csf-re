#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
                (std::uint8_t* self, void* edx, char* name));
static CSF_FUNC(int, __fastcall, GetComponent, 0x00692BD0,
                (std::uint8_t* self, void* edx, unsigned int index));
static CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0,
                (unsigned int value));

static CSF_GLOBAL(char*, g_FourComponentPropertyName, 0x008A85FC);

static char __cdecl ReadFourComponentProperty(
    float* components, std::uint8_t* properties)
{
    if (!properties)
        return 0;

    std::uint8_t* values =
        FindProperty(properties, nullptr, g_FourComponentPropertyName);
    if (!values)
        return 0;

    int value = GetComponent(values, nullptr, 0);
    components[0] =
        static_cast<float>(ReadComponentFloat(static_cast<unsigned int>(value)));

    value = GetComponent(values, nullptr, 1);
    components[1] =
        static_cast<float>(ReadComponentFloat(static_cast<unsigned int>(value)));

    value = GetComponent(values, nullptr, 2);
    components[2] =
        static_cast<float>(ReadComponentFloat(static_cast<unsigned int>(value)));

    value = GetComponent(values, nullptr, 3);
    components[3] =
        static_cast<float>(ReadComponentFloat(static_cast<unsigned int>(value)));

    return 1;
}

HOOK(0x0071DF00, ReadFourComponentProperty, "auto");
