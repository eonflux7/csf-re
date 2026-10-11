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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __stdcall, sub_4C8800, 0x004C8800, (int a1));

// Resolves the object pointer the original derives from a linked value (value - 4, or null).
static void* ResolveLinkedBase(int linkedValue)
{
    if (linkedValue)
        return reinterpret_cast<void*>(linkedValue - 4);
    return nullptr;
}

static int __stdcall AiCoverAttack_ClearLinkedStateAndStop(int a1)
{
    void* base = ResolveLinkedBase(GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr));
    CSF_AT(int, base, 0x6DC) = 0;

    void* base2 = ResolveLinkedBase(GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr));
    void* controller = CSF_AT(void*, base2, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, controller, 0)[0x8C / 4]))(
        controller, nullptr, 0, 0, -1, 0);

    return sub_4C8800(a1);
}

HOOK(0x004CA540, AiCoverAttack_ClearLinkedStateAndStop, "auto");
