#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(uint8_t*, __fastcall, S_00692b80_ReadWideAttributeText, 0x00692E40, (S_00692b80* self, void* , int64_t outputArguments));

static uint8_t* __fastcall S_00692b80_ReadWideText(S_00692b80* self, void* /*edx*/, char* attributeName, int64_t outputArguments)
{
    S_00692b80* property = S_00692b80_FindProperty(self, nullptr, attributeName);
    if (property != nullptr)
        return S_00692b80_ReadWideAttributeText(property, nullptr, outputArguments);

    uint32_t outputPointer = static_cast<uint32_t>(outputArguments);
    *reinterpret_cast<uint16_t*>(outputPointer) = 0;
    return reinterpret_cast<uint8_t*>(outputPointer);
}

HOOK(0x006939F0, S_00692b80_ReadWideText, "auto");
