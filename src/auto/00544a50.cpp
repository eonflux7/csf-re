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

// Global scheduler state accessor (cdecl, no arguments, returns in eax).
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());

// Concurrency::details::SchedulerBase::GetSchedulerProxy (thiscall, no stack args, ret 0).
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Resets the listbox subobject embedded at +0x20 of CMenuSaveGame (cdecl, one stack argument).
CSF_FUNC(int*, __cdecl, CMenuSaveGame_ResetListboxSubobject, 0x00541650, (int* subobject));

// Forwards the listbox state to the MP stats layout. The original passes two stack
// arguments (subobject pointer, then field_001c) and also takes ebx as an input register,
// which this calling convention cannot express (see notes).
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x00541570, (int subobjectPtr, int selectedIndex));

static constexpr int kSchedulerProxyReadyValue = 0x10;
static constexpr std::size_t kListboxSubobjectOffset = 0x20;
static constexpr std::size_t kVtblSlotGetHost = 0x20 / 4;          // byte offset 0x20 -> slot 8
static constexpr std::size_t kVtblSlotRefreshHost = 0x16C / 4;     // byte offset 0x16C -> slot 91

// Refreshes the save-game listbox when the global scheduler proxy reports the ready value.
// Plain __fastcall (ecx = this, edx unused, no stack args) so the callee pops 0 bytes.
static void __fastcall CMenuSaveGame_RefreshListboxIfProxyReady(CMenuSaveGame* self, void* /*edx*/)
{
    void* globalState = GetGlobalState();
    if (SchedulerBase_GetSchedulerProxy(globalState, nullptr) != kSchedulerProxyReadyValue)
        return;

    self->field_0050 = 0xFFFFFFFFu;

    // Virtual call on self: slot 8 returns the host object.
    void* host = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[kVtblSlotGetHost])(self, nullptr);

    // Virtual call on host: slot 91 refresh.
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, host, 0)[kVtblSlotRefreshHost])(host, nullptr);

    int* listboxSubobject = reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + kListboxSubobjectOffset);
    CMenuSaveGame_ResetListboxSubobject(listboxSubobject);

    // Re-read the selected index after the reset call, as the original does.
    int selectedIndex = static_cast<int>(self->field_001c);
    MenuMpStats_ForwardToLayout541230(reinterpret_cast<int>(listboxSubobject), selectedIndex);
}

HOOK(0x00544A50, CMenuSaveGame_RefreshListboxIfProxyReady, "auto");
