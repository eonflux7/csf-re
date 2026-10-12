#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiStateFactory_SetRequest, 0x004B71D0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static char __stdcall AiStateFactory_HandleRequest3(int request)
{
    void* requestObject = reinterpret_cast<void*>(request);

    int targetHandle = GetLinkedObjectField10(requestObject, nullptr) + 148;
    int state = GetLinkedObjectField10(requestObject, nullptr);
    AiStateFactory_SetRequest(reinterpret_cast<void*>(state), nullptr, 3, targetHandle);

    int linkedObject = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
    void* owner = reinterpret_cast<void*>(linkedObject ? linkedObject - 4 : 0);
    void* context = CSF_AT(void*, owner, 0x2C);
    void* attackTasks = CSF_AT(void*, context, 0x204);

    targetHandle = GetLinkedObjectField10(requestObject, nullptr) + 148;
    (void)CSF_VTBL(0x0048C9F0);
    AiAttackTasks_CopyTargetHandle(attackTasks, nullptr, targetHandle);
    GetLinkedObjectField10(requestObject, nullptr);
    return 0;
}

HOOK(0x004B73C0, AiStateFactory_HandleRequest3, "auto");
