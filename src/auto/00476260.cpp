#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ParticleManager_ProcessReferencedParticle, 0x0051F700, (void* self, void*));
CSF_FUNC(int, __fastcall, ProcessWeaponContext, 0x006C8800, (void* self, void* edx));

static int __fastcall C_008316d8_ProcessReferencedObjects(
    C_008316d8* self, void* /*edx*/)
{
    void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
    if (entry)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entry, 0)[0x34 / 4])(entry, nullptr, 0);
    }

    int result = static_cast<int>(self->vGetPageCountForContexts());
    if (result)
    {
        void* context =
            reinterpret_cast<void*>(self->vGetPageCountForContexts());
        (void)CSF_VTBL(0x006C8800);
        result = ProcessWeaponContext(context, nullptr);
    }

    if (self->field_0030)
        result = ParticleManager_ProcessReferencedParticle(
            reinterpret_cast<void*>(self->field_0030), nullptr);

    void* referencedObject = reinterpret_cast<void*>(self->field_0038);
    if (referencedObject)
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, referencedObject, 0)[0x2C / 4])(
                referencedObject, nullptr, 0);

    return result;
}

HOOK(0x00476260, C_008316d8_ProcessReferencedObjects, "auto");
