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

// Resolves the object linked to an AI state: returns (linked - 4) or 0 when none.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCore_ApplyDebugFinParams, 0x004BF740, (int a1));

static int AiState_GetLinkedBase(int stateObject)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(stateObject), nullptr);
    if (linked)
        return linked - 4;
    return 0;
}

static int __stdcall AiState_ApplyDefaultStanceAndFinParams(int a1)
{
    int ownerDataEdi = AiState_GetLinkedBase(a1);
    int ownerBaseV4 = AiState_GetLinkedBase(a1);

    // Owner character object at +0x2C; its TCharTask at +0x204.
    void* charObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerBaseV4, 0x2C));
    TCharTask* charTask = reinterpret_cast<TCharTask*>(CSF_AT(uint32_t, charObj, 0x204));
    TaskCharAction_48CFE0(charTask, nullptr, ownerDataEdi, static_cast<char>(7), static_cast<char>(-100));

    // Result of this lookup is not used by the original; the call is kept for its side effects.
    AiState_GetLinkedBase(a1);

    int ownerBaseV5 = AiState_GetLinkedBase(a1);
    void* stanceObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerBaseV5, 0x2C));
    CoverAttack_SetStance_492720(stanceObj, nullptr, 3);

    int ownerBaseV6 = AiState_GetLinkedBase(a1);
    void* actorObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerBaseV6, 0x2C));
    // Virtual slot 0x8C / 4 = 35: (obj, nullptr, 0, -1, 0)
    ((void(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, actorObj, 0)[0x8C / 4])(
        actorObj, nullptr, 0, -1, 0);

    return AiCore_ApplyDebugFinParams(a1);
}

HOOK(0x004C3A80, AiState_ApplyDefaultStanceAndFinParams, "auto");
