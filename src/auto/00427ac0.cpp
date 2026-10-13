#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));

static int __fastcall HudObjectives_CreateScreenElement(
    void* self, void* /*edx*/, S_00692b80* properties,
    S_00692b80* propertyName, const char* fallbackName)
{
    alignas(8) std::uint8_t nameBytes[0x100] = {};
    char* name = reinterpret_cast<char*>(nameBytes);
    char* keyword = reinterpret_cast<char*>(propertyName);

    if (properties && S_00692b80_FindProperty(properties, nullptr, keyword))
    {
        MenuExtras_ReadText(
            reinterpret_cast<std::uint8_t*>(properties),
            nullptr, keyword, name, 256);
    }
    else
    {
        std::strcpy(name, fallbackName);
    }

    void* manager = CSF_AT(void*, self, 0x4);
    void* resource =
        reinterpret_cast<void* (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, manager, 0)[76 / 4])(
                manager, nullptr, name);

    if (!resource)
    {
        CSF_AT(std::uint8_t, self, 0x8) = 0;
        return 0;
    }

    manager = CSF_AT(void*, self, 0x4);
    void* element =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, manager, 0)[84 / 4])(
                manager, nullptr, resource);

    ReleaseObjectReference(resource, nullptr);

    if (!element)
    {
        CSF_AT(std::uint8_t, self, 0x8) = 0;
        return 0;
    }

    alignas(8) std::uint8_t screenSizeBytes[0x8] = {};
    float* screenSize = reinterpret_cast<float*>(screenSizeBytes);
    Hud_WriteNormalizedScreenSize(screenSize);

    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, element, 0)[152 / 4])(
            element, nullptr, screenSize);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, element, 0)[104 / 4])(
            element, nullptr, 1);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, element, 0)[108 / 4])(
            element, nullptr, 1);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(element));
}

HOOK(0x00427AC0, HudObjectives_CreateScreenElement, "auto");
