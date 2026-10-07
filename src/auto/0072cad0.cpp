#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall sub_6932B0(unsigned __int8 *this, char *String1)  (ret 4)
CSF_FUNC(std::uint8_t*, __fastcall, FindByKeyString, 0x6932B0,
         (std::uint8_t* self, void* /*edx*/, char* key));

// Key string pointer used as the search key by this wrapper.
CSF_GLOBAL(char*, g_SearchKeyString, 0x8A88A4);

static std::uint8_t* __stdcall LookupObjectByKeyString(std::uint8_t* object)
{
    if (!object)
        return nullptr;

    return FindByKeyString(object, nullptr, g_SearchKeyString);
}

HOOK(0x72CAD0, LookupObjectByKeyString, "auto");
