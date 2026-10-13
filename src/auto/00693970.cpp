#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void* edx));

static int __fastcall ReadIntegerAttribute(S_00692b80* self, void* /*edx*/, char* attributeName)
{
    std::uint8_t* property = FindProperty(reinterpret_cast<std::uint8_t*>(self), nullptr, attributeName);
    int result = 0;
    if (property != nullptr) {
        return S_00692b80_ParseIntegerComponent(reinterpret_cast<S_00692b80*>(property), nullptr);
    }
    return result;
}

HOOK(0x693970, ReadIntegerAttribute, "auto");
