#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Invokes virtual slot 0xA8 (index 42) with a single argument of 0 on a child object.
// Returns the slot's eax result; callers that ignore it still match the original.
static int __fastcall CallChildSlot42WithZero(void* child)
{
    void** vtable = *reinterpret_cast<void***>(child);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtable[42])(child, nullptr, 0);
}

// Calls the virtual slot at +0xA8 with argument 0 on the six child objects held by
// the menu, in the same order as the original. Returns the result of the last call.
static int __fastcall CMenuMpServerInfo_DisableChildControls(CMenuMpServerInfo* self, void* /*edx*/)
{
    CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_0010));
    CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_0014));
    CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_0018));
    CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_001c));
    CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_0028));
    return CallChildSlot42WithZero(reinterpret_cast<void*>(self->field_002c));
}

HOOK(0x00530710, CMenuMpServerInfo_DisableChildControls, "auto");
