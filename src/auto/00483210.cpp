#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls, bound by address.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Returns the cached object (self+0x84, self+0x88 or self+0x8C) whose provider id matches
// providerId. Returns 0 if none matches. providerId == -1 returns 0 immediately.
static int __fastcall C_0085e7b8_FindCachedObjectForProvider(C_0085e7b8* self, void* /*edx*/, int providerId) {
    if (providerId == -1) {
        return 0;
    }

    int foundObject = 0;

    // Global service object -> provider -> descriptor lookup.
    int serviceHandle = GetValue8B0554();
    void* serviceObj = reinterpret_cast<void*>(static_cast<intptr_t>(serviceHandle));
    void* providerObj = CSF_AT(void*, serviceObj, 0x1fc0);
    int descriptor = ((int (__fastcall*)(void*, void*, int))(CSF_AT(void**, providerObj, 0)[2]))(providerObj, nullptr, providerId);

    // Primary cached slot.
    int primaryCached = CSF_AT(int, self, 0x84);
    if (primaryCached != 0) {
        void* primaryPtr = reinterpret_cast<void*>(static_cast<intptr_t>(primaryCached));
        if (static_cast<int>(reinterpret_cast<intptr_t>(GetCachedObjectProvider(primaryPtr, nullptr))) == providerId) {
            return primaryCached;
        }
    }

    // Secondary slot, taken only when the descriptor has type 0x1b.
    int secondaryCached = CSF_AT(int, self, 0x88);
    void* descriptorPtr = reinterpret_cast<void*>(static_cast<intptr_t>(descriptor));
    if (secondaryCached != 0 && CSF_AT(int, descriptorPtr, 0x74) == 0x1b) {
        return secondaryCached;
    }

    // Tertiary cached slot.
    int tertiaryCached = CSF_AT(int, self, 0x8c);
    if (tertiaryCached != 0) {
        void* tertiaryPtr = reinterpret_cast<void*>(static_cast<intptr_t>(tertiaryCached));
        if (static_cast<int>(reinterpret_cast<intptr_t>(GetCachedObjectProvider(tertiaryPtr, nullptr))) == providerId) {
            foundObject = tertiaryCached;
        }
    }

    return foundObject;
}

HOOK(0x483210, C_0085e7b8_FindCachedObjectForProvider, "auto");
