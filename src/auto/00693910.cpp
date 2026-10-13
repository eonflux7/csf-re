#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006932B0, __thiscall (this in ecx, keyword on stack), ret 4.
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

// Original: 0x00692C40, __fastcall-style method, no stack args.
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));

// Looks up a named property and parses its integer value; returns 0 when the property is absent.
static int __fastcall S_00692b80_ReadIntegerProperty(S_00692b80* self, void* /*edx*/, char* keyword)
{
    std::uint8_t* property = FindProperty(reinterpret_cast<std::uint8_t*>(self), nullptr, keyword);
    if (property)
    {
        return S_00692b80_ParseIntegerComponent(reinterpret_cast<S_00692b80*>(property), nullptr);
    }
    return 0;
}

HOOK(0x00693910, S_00692b80_ReadIntegerProperty, "auto");
