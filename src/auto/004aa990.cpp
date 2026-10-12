#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (by address)
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));

// Calls GetLinkedObjectValue(a2) and returns the owner pointer (value - 4), or null when the link is 0.
static uint8_t* ResolveLinkedOwner(int a2) {
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr);
    if (linked == 0) {
        return nullptr;
    }
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linked) - 4);
}

static int __fastcall AiCore_ActivateUnassignedCoverState(void* self, void* /*edx*/, int a2, char a3) {
    CSF_AT(char, self, 0x4) = a3;

    // First link lookup: its owner's +0x2c object is the `this` for the stance call.
    uint8_t* stanceOwner = ResolveLinkedOwner(a2);
    void* stanceObj = CSF_AT(void*, stanceOwner, 0x2c);
    CoverAttack_SetStance_492720(stanceObj, nullptr, 1);

    // Second lookup: object at +0x2c gets virtual slot 35 (offset 0x8c) called.
    uint8_t* vtOwner = ResolveLinkedOwner(a2);
    void* vtObj = CSF_AT(void*, vtOwner, 0x2c);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, vtObj, 0)[35])(
        vtObj, nullptr, 0, 0, -1, 0);

    // Third lookup: v6 (task owner pointer).
    uint8_t* owner6 = ResolveLinkedOwner(a2);
    int v6 = static_cast<int>(reinterpret_cast<uintptr_t>(owner6));

    // Fourth lookup: object at +0x2c, then the char-task pointer at +0x204.
    uint8_t* owner8 = ResolveLinkedOwner(a2);
    void* taskObj = CSF_AT(void*, owner8, 0x2c);
    void* charTask = CSF_AT(void*, taskObj, 0x204);

    return TaskCharAction_48CFE0(charTask, nullptr, v6, 5, 1);
}

HOOK(0x004AA990, AiCore_ActivateUnassignedCoverState, "auto");
