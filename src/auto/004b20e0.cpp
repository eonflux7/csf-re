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

// Bound callees (addresses from the packet)
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_SelectUsableTarget, 0x004AEF70, (S_004aef70* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

// Resolves the object linked to `self` (GetLinkedObjectValue minus the 4-byte header), or nullptr.
static uint8_t* ResolveLinkedBase(void* self) {
    int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linked) {
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked - 4));
    }
    return nullptr;
}

static int __stdcall AiAlertStates_ResolveLinkedTargetState(int a1, int a2) {
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    uint8_t* base = ResolveLinkedBase(self);
    void* ownerObj = CSF_AT(void*, base, 0x2C);
    void* member = CSF_AT(void*, ownerObj, 0x44);

    // Virtual slot 0x90 / 4 = 36 on the member's vtable; returns bool (tested via al).
    bool isReady = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, member, 0)[36])(member, nullptr);
    int result = isReady ? 1 : 0;

    if (isReady) {
        (void)CSF_VTBL(0x004AEF70);

        uint8_t* usableBase = ResolveLinkedBase(self);
        S_004aef70* usable = reinterpret_cast<S_004aef70*>(usableBase);
        S_004aef70_SelectUsableTarget(usable, nullptr);

        uint8_t* targetBase = ResolveLinkedBase(self);
        void* targetObj = CSF_AT(void*, targetBase, 0x2C);

        // Virtual slot 0x8C / 4 = 35: (obj, 0, 0, -1, 0)
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, targetObj, 0)[35])(targetObj, nullptr, 0, 0, -1, 0);

        return Dispatch_NoLinkedTarget(self, nullptr, 0);
    }

    return result;
}

HOOK(0x4B20E0, AiAlertStates_ResolveLinkedTargetState, "auto");
