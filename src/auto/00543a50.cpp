#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x50D190: returns a pointer (typed int by the packet) to the global game state object.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// 0x4285D0: __thiscall helper, bool result in al, pops 4 stack bytes (one stack argument).
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x4285D0, (void* self, void* edx, int value));

// 0x543A50: if the global state's byte at +0xD0 is set, query helper 0x4285D0 about the
// sub-object held at this+0x14 and forward its answer to this object's virtual slot 24
// (byte offset 0x60); otherwise just return the global state pointer value.
static int __fastcall CMenuControls_HandleGlobalStateFlag(void* self, void* /*edx*/)
{
    int result = GetGlobalState_50D190();
    if (reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(result))[0xD0])
    {
        // mov ecx, [esi+0x14]
        void* subObject = *reinterpret_cast<void**>(static_cast<uint8_t*>(self) + 0x14);
        // mov edi, [esi]  -- vtable pointer loaded before the helper call, as in the original
        void** vtable = *reinterpret_cast<void***>(self);
        // push 0x16 ; call 0x4285D0
        int value = MenuControls_QueryFlag(subObject, nullptr, 0x16);
        // mov ecx, esi ; push eax ; call dword ptr [edi+0x60]
        return ((int (__fastcall*)(void*, void*, int))vtable[0x60 / 4])(self, nullptr, value);
    }
    return result;
}

HOOK(0x543A50, CMenuControls_HandleGlobalStateFlag, "auto");
