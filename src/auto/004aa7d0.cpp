#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, AiDistracted_GetFutureErrorText, 0x007613D0, (int a1));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));

// Returns the owner object (linked value - 4), or null when there is no linked value.
static uint8_t* AiMoveTasks_ResolveOwnerBase(void* aiState) {
    int linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    if (linkedValue == 0) {
        return nullptr;
    }
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));
}

static const char* __fastcall AiMoveTasks_GetName(void* self, void* /*edx*/, void* aiState) {
    uint8_t* owner = AiMoveTasks_ResolveOwnerBase(aiState);
    uint8_t* ownerData = static_cast<uint8_t*>(CSF_AT(void*, owner, 0x2c));
    uint8_t* body = static_cast<uint8_t*>(CSF_AT(void*, ownerData, 0x34));
    int stateValue = CSF_AT(int, body, 0x2f8);

    if (stateValue >= 0 && stateValue <= 2) {
        uint8_t* selfPlus8 = static_cast<uint8_t*>(self) + 8;
        int result = CoverAttack_SetState_4DC150(
            aiState, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(selfPlus8)));
        return reinterpret_cast<const char*>(static_cast<uintptr_t>(result));
    }

    uint8_t* ownerBase = AiMoveTasks_ResolveOwnerBase(aiState);
    C_00833908* targetObject = reinterpret_cast<C_00833908*>(CSF_AT(void*, ownerBase, 0x2c));
    C_00833908_GetTargetName(targetObject, nullptr);

    char* text = AiDistracted_GetFutureErrorText(stateValue);
    return text;
}

HOOK(0x004AA7D0, AiMoveTasks_GetName, "auto");
