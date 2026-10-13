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

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_00443a00_ClearEntryByteForIndex, 0x00443B80, (S_00443a00* self, void* , int a2));
CSF_FUNC(int, __fastcall, AttackTasks_SetMode, 0x0047B210, (void* self, void* , int mode));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall AiCoverTasks_ClearEntryAndSetAttackMode(TAiState* a1)
{
    PrepareStaticAiState(a1);

    // Resolve the linked object; a null result leaves the base at 0.
    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    void* linkedBase = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue ? linkedValue - 4 : 0));

    // Clear the entry byte for (linkedBase->field_002C + 0x1C) in the current scene's table.
    int entryIndex = CSF_AT(int, linkedBase, 0x2C) + 0x1C;
    S_00443a00* sceneTable = CSF_AT(S_00443a00*, g_pCurrentScene, 0xB8);
    S_00443a00_ClearEntryByteForIndex(sceneTable, nullptr, entryIndex);

    // Re-query the linked object and switch the attack mode on it.
    GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int linkedAgain = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    if (linkedAgain != 0)
    {
        void* attackObj = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(linkedAgain)), 0x714);
        return AttackTasks_SetMode(attackObj, nullptr, 2);
    }

    // Original loads [0 + 0x718] when the linked object is absent; keep the base in a void* variable.
    void* nullBase = reinterpret_cast<void*>(static_cast<uintptr_t>(0));
    void* attackObjFallback = CSF_AT(void*, nullBase, 0x718);
    return AttackTasks_SetMode(attackObjFallback, nullptr, 2);
}

HOOK(0x004AC270, AiCoverTasks_ClearEntryAndSetAttackMode, "auto");
