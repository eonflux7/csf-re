#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall sub_6932B0(ecx = this, [esp+4] = String1) — looks the key string up inside the object.
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x6932B0,
         (std::uint8_t* self, void* /*edx*/, char* key));

// Pointer to the key string constant used for this lookup.
CSF_GLOBAL(char*, g_LookupKeyString, 0x8A88B0);

static std::uint8_t* __stdcall LookupByKeyString(std::uint8_t* object)
{
    if (!object)
        return nullptr;

    return FindProperty(object, nullptr, g_LookupKeyString);
}

HOOK(0x72CA40, LookupByKeyString, "auto");
