#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x6932B0: __thiscall lookup helper taking a key string (rets 4).
CSF_FUNC(std::uint8_t*, __fastcall, FindByKeyString, 0x6932B0,
         (std::uint8_t* self, void* /*edx*/, char* key));

// Key string used by this variant of the lookup.
CSF_GLOBAL(char*, g_LookupKeyString, 0x8A88B8);

static std::uint8_t* __stdcall FindElementByKeyString(std::uint8_t* object)
{
    if (!object)
        return nullptr;

    return FindByKeyString(object, nullptr, g_LookupKeyString);
}

HOOK(0x72CA80, FindElementByKeyString, "auto");
