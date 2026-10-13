#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, S_0048f590_EnableObjectAndLinkedFlag, 0x0048F590, (S_0048f590* self, void*));

[[maybe_unused]] static const void* const s_EnableObjectVtbl = CSF_VTBL(0x0048F590);

static char __fastcall S_0048f590_NotifyEntryHandlesThenEnable(S_0048f590* self, void* /*edx*/)
{
    uint32_t entryIndex = 0;
    if (self->field_019c != 0)
    {
        uint32_t entryOffset = 0;
        do
        {
            uint32_t entryAddr = self->field_0194 + entryOffset;
            int entryState = CSF_AT(int, reinterpret_cast<uint8_t*>(entryAddr), 8);
            if (entryState >= 0)
            {
                void* resolved = reinterpret_cast<void*>(
                    CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(entryAddr)));
                if (resolved != nullptr)
                {
                    ((void(__fastcall*)(void*, void*))CSF_AT(void**, resolved, 0)[47])(resolved, nullptr);
                }
            }
            ++entryIndex;
            entryOffset += 28;
        } while (entryIndex < self->field_019c);
    }
    return S_0048f590_EnableObjectAndLinkedFlag(self, nullptr);
}

HOOK(0x0049ECB0, S_0048f590_NotifyEntryHandlesThenEnable, "auto");
