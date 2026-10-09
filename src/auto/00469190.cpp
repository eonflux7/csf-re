#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Entry_ResolveOwner, 0x0045AA50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_0045d610_UnlinkEntry, 0x004690A0, (void* self, void* edx, int a2));

// Removes every child entry of this list whose record is unnamed (first byte of its name is 0)
// and that either has no resolvable owner or whose owner id matches the filter record's id.
// Each removed child is flagged, unlinked, then released through its virtual destructor slot.
static uint32_t __fastcall S_0045d610_PurgeMatchingChildren(S_0045d610* self, void* /*edx*/, int filterRecord)
{
    uint32_t result = self->field_0030;
    for (uint32_t i = 0; i < result; ++i)
    {
        int* childSlots = reinterpret_cast<int*>(self->field_0028);
        int childAddr = childSlots[i];
        if (childAddr != 0)
        {
            void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childAddr));
            uint8_t* childName = CSF_AT(uint8_t*, child, 0x10);
            if (*childName == 0)
            {
                bool unowned = Entry_ResolveOwner(child, nullptr) == 0;
                if (unowned || !unowned)
                {
                    if (unowned)
                    {
                        goto remove_child;
                    }
                    int owner = Entry_ResolveOwner(child, nullptr);
                    uint32_t ownerId = CSF_AT(uint32_t,
                        reinterpret_cast<void*>(static_cast<uintptr_t>(owner)), 0x20);
                    uint32_t filterId = CSF_AT(uint32_t,
                        reinterpret_cast<void*>(static_cast<uintptr_t>(filterRecord)), 4);
                    if (ownerId != filterId)
                    {
                        result = self->field_0030;
                        continue;
                    }
                }
remove_child:
                CSF_AT(uint8_t, child, 0x80) = 0;
                S_0045d610_UnlinkEntry(self, nullptr, childAddr);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
                reinterpret_cast<int*>(self->field_0028)[i] = 0;
            }
        }
        result = self->field_0030;
    }
    return result;
}

HOOK(0x00469190, S_0045d610_PurgeMatchingChildren, "auto");
