#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds to the original game functions called by this routine.
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(char, __fastcall, CharTask_SetFireReady, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Starts the firing sequence on the sub-object resolved from this->field_0008 + 116.
static int __fastcall AttackTask_BeginFiringSequence(S_004223f0* self, void* /*edx*/)
{
    C_0082f9f8* subObject = static_cast<C_0082f9f8*>(
        ResolveSubObject(self, nullptr, static_cast<int>(self->field_0008 + 116)));

    // Owner chain: subObject->field_0008 -> +0x14 -> +0x204 is the char task controller.
    void* owner = reinterpret_cast<void*>(subObject->field_0008);
    void* ownerParent = CSF_AT(void*, owner, 0x14);
    void* charTask = CSF_AT(void*, ownerParent, 0x204);

    TaskCharAction_48CFE0(charTask, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(subObject)), 1, static_cast<char>(-100));

    // Re-read the chain after the call, as the original does.
    owner = reinterpret_cast<void*>(subObject->field_0008);
    ownerParent = CSF_AT(void*, owner, 0x14);
    charTask = CSF_AT(void*, ownerParent, 0x204);
    CharTask_SetFireReady(charTask, nullptr, 1);

    // Virtual slot 14 on the object at ownerParent->+0x48.
    owner = reinterpret_cast<void*>(subObject->field_0008);
    ownerParent = CSF_AT(void*, owner, 0x14);
    void* notifier = CSF_AT(void*, ownerParent, 0x48);
    uint32_t ownerRef = subObject->field_0008;
    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, notifier, 0)[14])(
        notifier, nullptr, ownerRef);

    // Address of the embedded sub-object at +0x78 is passed as the movement argument.
    int movementArg = static_cast<int>(reinterpret_cast<uintptr_t>(
        reinterpret_cast<uint8_t*>(subObject) + 0x78));
    return CharTask_StartMovement(subObject, nullptr, movementArg);
}

HOOK(0x47B560, AttackTask_BeginFiringSequence, "auto");
