#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (S_00692b80* self, void* , char* attributeName));
CSF_FUNC(int, __fastcall, S_00693740_ReleaseBuffers, 0x00694380, (S_00693740* self, void*));
CSF_FUNC(char, __fastcall, S_00693740_LoadFile, 0x00694770, (S_00693740* self, void* , int filename));
CSF_FUNC(char, __fastcall, sub_783090, 0x00783090, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_7832C0, 0x007832C0, (void* self, void* edx, void* a2));

CSF_GLOBAL(const char*, g_pszKey_VERSION_8A9BF8, 0x008A9BF8);

// Loads a versioned file into a temporary loader object, applies it when the
// VERSION attribute is 0 or 1 (version 1 also runs the extra apply step), and
// always releases the loader buffers before returning.
static char __fastcall S_00782f90_LoadVersionedFile(S_00782f90* self, void* /*edx*/, int a2)
{
    // Stack-resident loader object (room 0x3C bytes).
    alignas(8) uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    S_00692b10_InitializeFile(&file, nullptr);

    if (S_00693740_LoadFile(reinterpret_cast<S_00693740*>(&file), nullptr, a2))
    {
        S_00692b80* a2a = reinterpret_cast<S_00692b80*>(file.field_0000);
        int version = ReadIntegerAttribute(a2a, nullptr,
                                           const_cast<char*>(g_pszKey_VERSION_8A9BF8));
        if (version <= 1)
        {
            char result = 0;
            sub_783090(self, nullptr);
            if (version == 1)
                result = sub_7832C0(self, nullptr, a2a);
            S_00693740_ReleaseBuffers(reinterpret_cast<S_00693740*>(&file), nullptr);
            return result;
        }
    }

    S_00693740_ReleaseBuffers(reinterpret_cast<S_00693740*>(&file), nullptr);
    return 0;
}

HOOK(0x00783340, S_00782f90_LoadVersionedFile, "auto");
