#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindByKeyString, 0x6932B0,
         (std::uint8_t* self, void* /*edx*/, char* key));

CSF_GLOBAL(char*, g_LookupKeyString, 0x8A88A8);

static std::uint8_t* __stdcall LookupByKeyString(std::uint8_t* object)
{
    if (!object)
        return nullptr;

    return FindByKeyString(object, nullptr, g_LookupKeyString);
}

HOOK(0x72CAF0, LookupByKeyString, "auto");
