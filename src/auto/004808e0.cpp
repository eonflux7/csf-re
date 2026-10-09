#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the three-dword source args into a local buffer, then forwards that
// buffer through the action owner's object chain (vtable slots 0x28, 0x18, 0x10
// on the first chain, then 0x28, 0x18, 0x54 on the second).
static void __fastcall CCharWeaponFsm_ForwardTargetToCurrentAction(CCharWeaponFsm* self, void* /*edx*/, int* sourceArgs)
{
    if (self->field_000c == 9 || self->field_0024 == 0xFFFFFFFFu)
        return;

    int argBuffer[3];
    argBuffer[0] = sourceArgs[0];
    argBuffer[1] = sourceArgs[1];
    argBuffer[2] = sourceArgs[2];

    // First chain: owner -> level1 (slot 0x28) -> level2 (slot 0x18) -> level3 (slot 0x10)
    C_00833908* actionOwner = reinterpret_cast<C_00833908*>(self->field_0008);
    int level1Value = actionOwner->vGetField016c();
    void* level1 = reinterpret_cast<void*>(static_cast<uintptr_t>(level1Value));
    void* level2 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, level1, 0)[6])(level1, nullptr);
    void* level3 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, level2, 0)[4])(level2, nullptr);
    argBuffer[1] = CSF_AT(int, level3, 4);

    // Second chain: reload owner -> level1 (slot 0x28) -> level2 (slot 0x18) -> submit (slot 0x54)
    actionOwner = reinterpret_cast<C_00833908*>(self->field_0008);
    int submitTarget1Value = actionOwner->vGetField016c();
    void* submitTarget1 = reinterpret_cast<void*>(static_cast<uintptr_t>(submitTarget1Value));
    void* submitTarget2 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, submitTarget1, 0)[6])(submitTarget1, nullptr);
    ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, submitTarget2, 0)[21])(submitTarget2, nullptr, argBuffer);
}

HOOK(0x4808E0, CCharWeaponFsm_ForwardTargetToCurrentAction, "auto");
