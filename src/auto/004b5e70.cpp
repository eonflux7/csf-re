#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Returns the linked object's base (value - 4), or nullptr when no linked object exists.
static uint8_t* ResolveLinkedBase(int a1) {
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    if (linked == 0)
        return nullptr;
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked) - 4);
}

static bool __stdcall FncSearch_CheckSearchTargetResolved(int a1, int a2) {
    uint8_t* base = ResolveLinkedBase(a1);
    if (CSF_AT(int32_t, base, 0x608) == -1 && CSF_AT(int32_t, base, 0x60C) == -1)
        return true;

    bool result = false;
    void* taskObj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 0x34));
    if (taskObj != nullptr) {
        uint8_t* base2 = ResolveLinkedBase(a1);
        int32_t selector = CSF_AT(int32_t, base2, 0x630);
        void* stateArg = (selector < 0) ? static_cast<void*>(base2 + 0x610)
                                        : static_cast<void*>(base2 + 0x628);
        // vtable slot 0xDC / 4 = 55
        bool done = ((bool(__fastcall*)(void*, void*, void*))CSF_AT(void**, taskObj, 0)[55])(
            taskObj, nullptr, stateArg);
        if (done)
            return true;
    }
    return result;
}

HOOK(0x004B5E70, FncSearch_CheckSearchTargetResolved, "auto");
