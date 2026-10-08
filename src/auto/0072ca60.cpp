#include <cstdint>
#include "globals.h"
#include "hook.h"

// _BYTE *__thiscall sub_6932B0(unsigned __int8 *this, char *String1)  (ret 4)
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x6932B0,
         (std::uint8_t* self, void* /*edx*/, char* key));

// Global pointer to the lookup key string (loaded as dword ptr [0x008A88B4]).
CSF_GLOBAL(char*, g_LookupKeyString, 0x8A88B4);

// __stdcall, ret 4: null-safe wrapper that looks an object up by the global key string.
static std::uint8_t* __stdcall LookupByGlobalKeyString(std::uint8_t* object)
{
    if (!object)
        return nullptr;

    return FindProperty(object, nullptr, g_LookupKeyString);
}

HOOK(0x72CA60, LookupByGlobalKeyString, "auto");
