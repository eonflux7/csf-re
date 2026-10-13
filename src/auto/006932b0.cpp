#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, CoreParser_FindChildByName, 0x00692980, (void* self, void* , char* name));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

using ComponentPoolPointers = std::uint8_t*[256];
CSF_GLOBAL(ComponentPoolPointers, g_ComponentPools, 0x008BAF48);

// Retain the packet's explicitly listed vtable binding.
[[maybe_unused]] static const auto kPropertyNodeVtable = CSF_VTBL(0x00692B80);

static S_00692b80* __fastcall S_00692b80_FindProperty(
    S_00692b80* self, void* /*edx*/, char* name)
{
    if (self->field_000b == 0xFF)
    {
        return reinterpret_cast<S_00692b80*>(
            CoreParser_FindChildByName(
                reinterpret_cast<void*>(self->field_0000), nullptr, name));
    }

    if (CSF_AT(std::uint8_t, self, 0x0A) != 2)
        return nullptr;

    const unsigned int childCount =
        static_cast<unsigned int>(S_00692b80_GetChildCount(self, nullptr));

    for (unsigned int index = 0; index < childCount; ++index)
    {
        S_00692b80* component =
            S_00692b80_GetComponent(self, nullptr, index);
        const std::uint16_t nameIndex =
            CSF_AT(std::uint16_t, component, 0x08);

        if (nameIndex != 0xFFFF)
        {
            std::uint8_t* pool = g_ComponentPools[self->field_000b];
            const char* componentName =
                CSF_AT(std::uint8_t, pool, 0x14)
                    ? nullptr
                    : CSF_AT(const char**, pool, 0x1C)[nameIndex];

            if (name && componentName && !__stricmp(name, componentName))
                return component;
        }
    }

    return nullptr;
}

HOOK(0x006932B0, S_00692b80_FindProperty, "auto");
