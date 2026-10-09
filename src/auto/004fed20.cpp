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

// Callees (bound by address, names and signatures per the packet).
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(bool, __cdecl, sub_4FDA40, 0x004FDA40, (char* a1, char* a2));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void*, __fastcall, sub_525730, 0x00525730, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_726C40, 0x00726C40, (void* self, void* edx));

// Data.
CSF_GLOBAL(const char*, g_psz_PFX_IMPACTO_BALA, 0x008A8BD0);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Walks the 55 impact particle prefix names (dword table, byte stride 4),
// looks each one up in the particle registry and, if the entry is unused
// or its name differs from the default, re-registers it; then runs the
// final refresh.
static int SceneLoader_RefreshImpactParticles()
{
    void* registry = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC));

    for (uint32_t offset = 0; offset < 0xDC; offset += 4)
    {
        const char* prefixName = CSF_AT(const char*, &g_psz_PFX_IMPACTO_BALA, offset);

        // registry->vtbl[3](registry, prefixName) -> entry index or -1
        int entryIndex = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, registry, 0)[3])(registry, nullptr, prefixName);

        if (entryIndex != -1)
        {
            // registry->vtbl[2](registry, entryIndex) -> entry record
            void* entry = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, registry, 0)[2])(registry, nullptr, entryIndex);

            // entry + 4 is the address handed to the refresh call.
            uint8_t* entryPayload = reinterpret_cast<uint8_t*>(entry) + 4;

            bool needsRefresh = (CSF_AT(uint32_t, entry, 4) != 0);
            if (!needsRefresh)
            {
                // Name stored inline (<16 chars) or via pointer at +0x0C.
                char* storedName =
                    (CSF_AT(uint32_t, entry, 0x20) < 0x10u)
                        ? reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(entry) + 0xC)
                        : CSF_AT(char*, entry, 0xC);

                needsRefresh = !sub_4FDA40(storedName, reinterpret_cast<char*>(&g_byte_82E65C));
            }

            if (needsRefresh)
            {
                void* impactTable = reinterpret_cast<void*>(GetGlobalInt());
                void* refresher = reinterpret_cast<void*>(sub_726C40(impactTable, nullptr));

                // refresher->vtbl[4](refresher, entryPayload)
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, refresher, 0)[4])(refresher, nullptr, entryPayload);
            }
        }
    }

    void* finalTarget = reinterpret_cast<void*>(Get_008b0678());
    return static_cast<int>(reinterpret_cast<intptr_t>(sub_525730(finalTarget, nullptr)));
}

HOOK(0x004FED20, SceneLoader_RefreshImpactParticles, "auto");
