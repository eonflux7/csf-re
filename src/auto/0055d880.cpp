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

// Obtains the global game-state root object (__cdecl, no stack args).
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// Builds a message/resource handle from a 4-char code and a flag (__cdecl, 2 args).
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x692440, (int code, int flag));

// Refreshes the friend-request/voice label of the player info panel based on whether
// the local state object reports the target player as eligible.
// Called as a __thiscall method: ecx = this, edx unused.
static int __fastcall CMenuPlayerInfo_SetFriendRequestTextByState(CMenuPlayerInfo* self, void* /*edx*/)
{
    uintptr_t globalBase = static_cast<uintptr_t>(GetGlobal8B0688());

    // Object at globalBase+0x1484 (state manager); its vtable slot 6 is queried
    // with the target player's data at selfData+0x204.
    void* stateObj = *reinterpret_cast<void**>(globalBase + 0x1484);
    void* playerData = reinterpret_cast<void*>(self->field_0048);
    uint32_t queryArg = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(playerData) + 0x204);

    uint8_t isEligible = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, stateObj, 0)[6])(stateObj, nullptr, queryArg);

    // Element object (+0x24) is read after the query call, as in the original.
    void* labelObj = reinterpret_cast<void*>(self->field_0024);

    // Original picks between two message codes ("OP13" / "OP12") depending on eligibility.
    // Both branches call slot 68 on the label object first, then MenuMessageBox_ResolveEntryByKey(code, 1).
    void* labelHandle = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, labelObj, 0)[68])(labelObj, nullptr);

    int messageCode = isEligible ? 0x4f503133 : 0x4f503132;
    int messageHandle = MenuMessageBox_ResolveEntryByKey(messageCode, 1);

    // Final virtual call on the label handle, slot 16, with the message handle.
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, labelHandle, 0)[16])(labelHandle, nullptr, messageHandle);
}

HOOK(0x0055D880, CMenuPlayerInfo_SetFriendRequestTextByState, "auto");
