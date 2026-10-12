#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplyCoverAttackState, 0x004B6F70, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static int __stdcall C_0085e760_ApplyLinkedCoverAttackState(
    C_0085e760* self, int /*unused2*/, int /*unused3*/)
{
    const int state = ResolveEntryLookupValue(self, nullptr);
    const int linkedObject = GetLinkedObjectField10(self, nullptr);
    return ApplyCoverAttackState(
        reinterpret_cast<void*>(linkedObject), nullptr,
        reinterpret_cast<void*>(state));
}

HOOK(0x004B7010, C_0085e760_ApplyLinkedCoverAttackState, "auto");
