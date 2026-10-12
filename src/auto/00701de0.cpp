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

CSF_FUNC(char, __fastcall, ParticleEmitter_HasValidWorldBinding, 0x00701D40, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GfxObjectInstances_TouchCachedEntry, 0x007013B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectInstances_ReleaseEntryObject, 0x006C4330, (void* self, void* edx));

// Walks the emitter's owned instance slots. For each slot whose owner is this emitter,
// refreshes its stamp, touches its cache entry if linked, and releases the attached object,
// summing the object's +0x0C value into the return.
static int __fastcall ParticleEmitter_ReleaseOwnedInstances(S_00701d40* self, void* /*edx*/, S_007013b0* a2)
{
    int releasedTotal = 0;
    const uint32_t ownedCount = self->field_0014;

    if (ownedCount != 0)
    {
        (void)CSF_VTBL(0x00701D40);
        if (ParticleEmitter_HasValidWorldBinding(self, nullptr, a2))
        {
            for (uint32_t slot = 0; slot < ownedCount; ++slot)
            {
                const int entryIndex = reinterpret_cast<int*>(self->field_000c)[slot];
                if (entryIndex == -1)
                    continue;

                const uint32_t instanceBase = a2->field_0008;
                const int byteOffset = 16 * entryIndex;
                const uint32_t entryAddr = instanceBase + static_cast<uint32_t>(byteOffset);
                S_00701d40* owner = *reinterpret_cast<S_00701d40**>(entryAddr);
                uint32_t* entry = reinterpret_cast<uint32_t*>(entryAddr);

                if (owner != self)
                    continue;

                const bool wasUnlinked = (entry[1] == 0xFFFFFFFFu);
                const uint32_t attachedObject = entry[3];
                entry[2] = static_cast<uint32_t>(a2->field_0034);

                if (!wasUnlinked)
                {
                    (void)CSF_VTBL(0x007013B0);
                    GfxObjectInstances_TouchCachedEntry(a2, nullptr, static_cast<int>(entryAddr));
                }

                if (attachedObject != 0)
                {
                    releasedTotal += reinterpret_cast<int*>(attachedObject)[3];
                    GfxObjectInstances_ReleaseEntryObject(reinterpret_cast<void*>(attachedObject), nullptr);
                }
            }
        }
    }

    return releasedTotal;
}

HOOK(0x00701DE0, ParticleEmitter_ReleaseOwnedInstances, "auto");
