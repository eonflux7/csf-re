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

// Calls the virtual slot at byte offset 0xA8 (index 42) of the object with arg 1.
// Equivalent to: obj->vtbl[0xA8 / 4](obj, nullptr, 1) in the original.
static void CMenuMpServerInfo_CallSetVisibleSlot(void* obj)
{
    void** vtbl = *reinterpret_cast<void***>(obj);
    auto fn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(vtbl[42]);
    fn(obj, nullptr, 1);
}

// Converts a uint32_t-declared pointer field into a void* object pointer.
static void* CMenuMpServerInfo_PtrFromField(uint32_t fieldValue)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(fieldValue));
}

// Shows the panels associated with the current server-info state.
// State 0: fields +0x14, +0x18, +0x28, +0x2C
// State 1: fields +0x28, +0x2C, +0x10
// State 3: field  +0x10
// Other states: nothing.
static void __fastcall CMenuMpServerInfo_ShowPanelsForState(CMenuMpServerInfo* self, void* /*edx*/)
{
    switch (self->field_01e4)
    {
    case 0:
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0014));
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0018));
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0028));
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_002c));
        break;
    case 1:
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0028));
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_002c));
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0010));
        break;
    case 3:
        CMenuMpServerInfo_CallSetVisibleSlot(CMenuMpServerInfo_PtrFromField(self->field_0010));
        break;
    default:
        break;
    }
}

HOOK(0x530770, CMenuMpServerInfo_ShowPanelsForState, "auto");
