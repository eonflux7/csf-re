#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee bindings (by address).
CSF_FUNC(char, __fastcall, AiTaskStates_UpdateSubStateLinks, 0x0041EFD0,
         (void* self, void* edx, uint32_t* a2, void* a3));
CSF_FUNC(int, __fastcall, AiTaskStates_SetCurrentStateParam, 0x0041EE60,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, GetLastElementAddress, 0x0041E7D0, (void* self, void*));

// Re-reads the owner pointer stored at self+8 (the original reloads it after each call).
static uint8_t* ReadOwnerPointer(void* self)
{
    return reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
}

static int __fastcall AiTaskStates_RefreshStateFromLastElement(void* self, void* /*edx*/)
{
    uint8_t* owner = ReadOwnerPointer(self);
    AiTaskStates_UpdateSubStateLinks(owner + 0x29C, nullptr,
                                     reinterpret_cast<uint32_t*>(owner + 0x218),
                                     owner + 0x25C);

    owner = ReadOwnerPointer(self);
    AiTaskStates_SetCurrentStateParam(owner + 0x29C, nullptr, CSF_AT(float, owner, 0x200));

    owner = ReadOwnerPointer(self);
    uint32_t* lastElement = reinterpret_cast<uint32_t*>(
        GetLastElementAddress(owner + 0x29C, nullptr));

    owner = ReadOwnerPointer(self);
    CSF_AT(uint32_t, owner, 0x224) = lastElement[0];
    CSF_AT(uint32_t, owner, 0x228) = lastElement[1];
    CSF_AT(uint32_t, owner, 0x22C) = lastElement[2];
    return static_cast<int>(lastElement[2]);
}

HOOK(0x421050, AiTaskStates_RefreshStateFromLastElement, "auto");
