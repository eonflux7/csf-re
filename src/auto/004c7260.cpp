#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, BindLinkedSubobject, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __stdcall, CoverAttack_DispatchProviderHandle, 0x004C6350, (int a1));

static int __stdcall CoverAttack_DispatchThroughProvider(C_0085e760* a1, int /*a2*/, int /*a3*/)
{
    int linkedField10 = GetLinkedObjectField10(a1, nullptr);
    BindLinkedSubobject(a1, nullptr, linkedField10 + 4);

    C_0085e7b8* entry = reinterpret_cast<C_0085e7b8*>(
        static_cast<std::intptr_t>(ResolveEntryLookupValue(a1, nullptr)));

    void* provider = GetCachedObjectProvider(entry, nullptr);
    int providerHandle = static_cast<int>(reinterpret_cast<std::intptr_t>(provider));

    GetLinkedObjectField10(a1, nullptr);

    return CoverAttack_DispatchProviderHandle(providerHandle);
}

HOOK(0x4C7260, CoverAttack_DispatchThroughProvider, "auto");
