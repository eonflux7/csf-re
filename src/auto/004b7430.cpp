#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, AttackTasks_SetCoverAttackPosition, 0x0048CA40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, ReadCoverAttackPosition, 0x004B1310, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0, (void* self, void* , uint32_t* source));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static char __stdcall AiStateFactory_HandleCoverAttackRequest(void* request)
{
    alignas(8) uint8_t positionBytes[0xC];
    std::memset(positionBytes, 0, sizeof(positionBytes));
    auto* position = reinterpret_cast<uint32_t*>(positionBytes);

    const int linkedField = GetLinkedObjectField10(request, nullptr);
    ReadCoverAttackPosition(
        reinterpret_cast<uint8_t*>(linkedField) + 0x64, nullptr, position);

    int linkedValue = GetLinkedObjectValue_4DB5C0(request, nullptr);
    void* linkedBase = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;
    void* owner = CSF_AT(void*, linkedBase, 0x2C);
    AttackTasks_SetCoverAttackPosition(
        CSF_AT(void*, owner, 0x204), nullptr, position);

    linkedValue = GetLinkedObjectValue_4DB5C0(request, nullptr);
    linkedBase = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;
    auto* target = CSF_AT(S_0047ae10*, linkedBase, 0x718);

    (void)CSF_VTBL(0x0047AE10);
    char* stateBuffer = AiCore_GetTaskTargetNameBuffer(target, nullptr);
    AiCore_CopyCoverAttackState(stateBuffer, nullptr, position);
    return 0;
}

HOOK(0x004B7430, AiStateFactory_HandleCoverAttackRequest, "auto");
