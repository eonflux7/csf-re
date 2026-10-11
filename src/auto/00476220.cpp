#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ParticleManager_ProcessStoredEntry, 0x0051F6D0, (void* self, void*));

static int __fastcall C_008316d8_ProcessResolvedEntry(
    C_008316d8* self, void* /*edx*/, int /*unused*/)
{
    int result = self->vResolveEntryLookupValue();
    void* entry = reinterpret_cast<void*>(result);
    if (entry)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entry, 0)[0x34 / 4])(entry, nullptr, 1);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, 1);
    }

    if (self->field_0030)
        ParticleManager_ProcessStoredEntry(
            reinterpret_cast<void*>(self->field_0030), nullptr);

    return result;
}

HOOK(0x00476220, C_008316d8_ProcessResolvedEntry, "auto");
