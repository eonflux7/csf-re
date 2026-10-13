#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CoreParser_CopyAttributeText, 0x00692D40, (uint8_t* self, void* , char* buffer, int capacity));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));

static char __fastcall S_00692b80_ReadText(
    S_00692b80* self, void* /*edx*/, char* name, char* buffer, int capacity)
{
    S_00692b80* property = S_00692b80_FindProperty(self, nullptr, name);
    if (property)
        return CoreParser_CopyAttributeText(
            reinterpret_cast<uint8_t*>(property), nullptr, buffer, capacity);

    *buffer = 0;
    return static_cast<char>(reinterpret_cast<std::uintptr_t>(buffer));
}

HOOK(0x006939C0, S_00692b80_ReadText, "auto");
