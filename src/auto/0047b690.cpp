#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address, per the packet's "bind as" lines.
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharSignalOwnerCode, 0x0048C9D0, (void* self, void* edx, char a2));

// Calls the base prepare step, then if the pending flag at +0x14 is set, clears it and
// signals the owner's context with code -10. Returns the flag value read after prepare.
static char __fastcall TCharTask_ConsumePendingFlag(TCharTask* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    char result = CSF_AT(char, self, 0x14);
    if (result)
    {
        CSF_AT(char, self, 0x14) = 0;

        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        uint32_t ownerContext = CSF_AT(uint32_t, owner, 0x14);
        void* context = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerContext));
        uint32_t target = CSF_AT(uint32_t, context, 0x204);
        void* targetObj = reinterpret_cast<void*>(static_cast<uintptr_t>(target));

        return CharSignalOwnerCode(targetObj, nullptr, -10);
    }
    return result;
}

HOOK(0x0047B690, TCharTask_ConsumePendingFlag, "auto");
