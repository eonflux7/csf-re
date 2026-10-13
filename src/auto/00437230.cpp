#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));

// a2 is a pointer-sized layout entry (the sprite-entry record read by the HUD layout code).
// a3 is the sprite component index into the CGuiTacticalMap sprite table.
static int __fastcall CGuiTacticalMap_ResolveSpriteValue(CGuiTacticalMap* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* layoutEntry = reinterpret_cast<uint8_t*>(a2);

    if (CSF_AT(uint8_t, layoutEntry, 0x5c))
    {
        // Sanitario entry: resolve through the HUD global with key 7.
        CHud* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
        return CHud_LookupValueByKey(hud, nullptr, 7);
    }

    if (CSF_AT(int, layoutEntry, 0x38) < 0)
    {
        return reinterpret_cast<int*>(self->m_sprites)[a3];
    }

    int variant = CSF_AT(int, layoutEntry, 0x3c);
    if (variant == -1)
    {
        return reinterpret_cast<int*>(self->m_sprites)[a3 + 1];
    }
    if (variant == 0)
    {
        return reinterpret_cast<int*>(self->m_sprites)[a3 + 2];
    }
    if (variant == 1)
    {
        return reinterpret_cast<int*>(self->m_sprites)[a3 + 3];
    }
    return 0;
}

HOOK(0x00437230, CGuiTacticalMap_ResolveSpriteValue, "auto");
