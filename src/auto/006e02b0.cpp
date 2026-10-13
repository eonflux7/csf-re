#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ApplySkinRegionSelection, 0x006C57A0, (C_0085e7b8** self, void* /*edx*/, C_0086754c* a2));
CSF_FUNC(int, __fastcall, CreateSkinRegionByKind, 0x006E0230, (void* self, void* edx, int a2));

// Looks up a candidate skin region for the request in a2 (a2[0] = kind, a2[6] = name length, a2[18] = index)
// and, when one is created, initializes it and applies it to the global skin region selection.
static char* __fastcall C_00861988_CreateSkinRegionIfMatch(C_00861988* self, void* /*edx*/, int* a2)
{
    if (a2[18] > (int)self->field_0060)
        return nullptr;

    if (a2[0] != 5)
    {
        int nameMatches = CompareStringSubstring(
            reinterpret_cast<uint8_t*>(a2) + 4,
            nullptr,
            0,
            static_cast<unsigned int>(a2[6]),
            reinterpret_cast<const char*>(&g_byte_82E65C),
            0);
        if (!nameMatches)
            return nullptr;
    }

    C_0086754c* region = reinterpret_cast<C_0086754c*>(CreateSkinRegionByKind(self, nullptr, a2[0]));
    if (region)
    {
        ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, region, 0)[6])(region, nullptr, a2);
        ((int (__fastcall*)(void*, void*))CSF_AT(void**, region, 0)[8])(region, nullptr);
        C_0085e7b8_ApplySkinRegionSelection(
            reinterpret_cast<C_0085e7b8**>(GetGlobalValue_6C5760()),
            nullptr,
            region);
    }
    return reinterpret_cast<char*>(region);
}

HOOK(0x006E02B0, C_00861988_CreateSkinRegionIfMatch, "auto");
