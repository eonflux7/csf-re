#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ApplyContextValue, 0x006C86E0,
         (void* self, void* edx, int a2));

static int __fastcall C_008316d8_ApplyEntryAndContextValue(
    C_008316d8* self, void* /*edx*/, int value)
{
    int entry = self->vResolveEntryLookupValue();
    if (entry)
    {
        void* object = reinterpret_cast<void*>(entry);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr, value);
    }

    uint32_t context = self->vGetPageCountForContexts();
    if (!context)
        return 0;

    context = self->vGetPageCountForContexts();
    return ApplyContextValue(reinterpret_cast<void*>(context), nullptr, value);
}

HOOK(0x00475C30, C_008316d8_ApplyEntryAndContextValue, "auto");
