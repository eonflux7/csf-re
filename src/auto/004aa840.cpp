#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(char*, __fastcall, GetLinkedStateName, 0x004AA7D0, (void* self, void* edx, void* a2));

static const char* __stdcall C_0085e760_GetLinkedStateName(
    C_0085e760* self, int /*unused2*/, int /*unused3*/)
{
    const int entryState = ResolveEntryLookupValue(self, nullptr);
    const int linkedObject = GetLinkedObjectField10(self, nullptr);
    return GetLinkedStateName(
        reinterpret_cast<void*>(static_cast<std::intptr_t>(linkedObject)), nullptr,
        reinterpret_cast<void*>(static_cast<std::intptr_t>(entryState)));
}

HOOK(0x004AA840, C_0085e760_GetLinkedStateName, "auto");
