#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __stdcall, CharActionFsm_SelectMissionBriefing, 0x00482850, (int slot));
CSF_FUNC(int, __fastcall, CCharActionFsm_FindSubObjectIndex, 0x00485A20, (CCharActionFsm* self, void* , int targetValue));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void*));
CSF_FUNC(void, __fastcall, CPlayerCharacter_SetSelectedEntry, 0x0049BAF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

namespace {

// Handle-like sub-object (vtable 0x82D448) carrying two ids.
struct S_ObjectRef
{
    void*    vtbl;      // +0x00
    uint32_t idLow;     // +0x04
    uint32_t idHigh;    // +0x08
};

// Event record (vtable 0x82D478) sent to the global dispatcher, 0x34 bytes.
struct S_SelectionEvent
{
    void*       vtbl;       // +0x00
    int         type;       // +0x04
    S_ObjectRef target;     // +0x08
    S_ObjectRef source;     // +0x14
    uint32_t    extra[4];   // +0x20
    char        handled;    // +0x30
};

} // namespace

static int __fastcall CPlayerCharacter_ApplyAlternateSelection(CPlayerCharacter* self, void* /*edx*/, void* target)
{
    CCharActionFsm* fsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);

    int result = CCharActionFsm_FindSubObjectIndex(fsm, nullptr, 28);
    if (result == -1)
        return result;

    CPlayerCharacter_SetSelectedEntry(self, nullptr, CSF_AT(int, CSF_AT(void*, target, 0x2C), 0));

    if (CCharActionFsm_GetSlot28ItemIfUsable(fsm, nullptr))
    {
        int slot28Item = CCharActionFsm_GetSlot28ItemIfUsable(fsm, nullptr);
        CharActionFsm_SelectMissionBriefing(CSF_AT(int, reinterpret_cast<void*>(slot28Item), 0x80));
    }

    CSF_AT(uint32_t, target, 0x1F0) &= ~4u;

    int globalValue = CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1824);

    void* owner = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x28 / 4])(target, nullptr);
    void* component = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x50 / 4])(owner, nullptr);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, component, 0)[0x14 / 4])(component, nullptr, globalValue);

    S_SelectionEvent event;
    event.target.vtbl = (void*)CSF_VTBL(0x0082D448);
    event.target.idLow = CSF_AT(uint32_t, target, 0x20);
    event.target.idHigh = CSF_AT(uint32_t, target, 0x24);
    event.source.vtbl = (void*)CSF_VTBL(0x0082D448);
    event.source.idLow = self->field_0020;
    event.source.idHigh = self->field_0024;
    event.vtbl = (void*)CSF_VTBL(0x0082D478);
    event.type = 29;
    event.extra[0] = 0;
    event.extra[1] = 0;
    event.extra[2] = 0;
    event.extra[3] = 0;
    event.handled = 0;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return (int)DispatchRecordToGlobalObject(dispatcher, nullptr, &event, 0);
}

HOOK(0x0049BF50, CPlayerCharacter_ApplyAlternateSelection, "auto");
