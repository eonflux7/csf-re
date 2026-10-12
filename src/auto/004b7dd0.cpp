#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiAttackTasks_StartLinkedObject(int object, float duration)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(object), nullptr);
    void* owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);

    alignas(8) uint8_t targetHandleBytes[0xC];
    CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, targetHandleBytes, 0x4) = -1;
    CSF_AT(int, targetHandleBytes, 0x8) = -1;

    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, CSF_AT(void*, owner, 0x2C), 0x204),
        nullptr,
        reinterpret_cast<int>(targetHandleBytes));

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(object), nullptr);
    owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    return C_0082f9f8_Start(
        CSF_AT(C_0082f9f8*, CSF_AT(void*, owner, 0x2C), 0x30),
        nullptr, duration, 1);
}

HOOK(0x004B7DD0, AiAttackTasks_StartLinkedObject, "auto");
