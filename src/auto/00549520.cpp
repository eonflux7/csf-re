#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x004FBDE0 - fetches the global system object that receives the pushed value.
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x4FBDE0, ());

// 0x004FBFE0 - __thiscall(global, value): pushes a control value into the
// global system (ecx = object, one stack argument, pops 4 bytes).
CSF_FUNC(int, __fastcall, GlobalValue_PushControlValue, 0x4FBFE0,
         (int self, void* /*edx*/, int value));

// Slot 20 (0x00549520) of CMenuVideoOptionsInGame (vtable 0x008440A0).
// Asks the control object held at +0x24 for its current value (virtual slot 12),
// fetches the global system object and pushes that value into it.
static int __fastcall CMenuVideoOptionsInGame_PushControlValueToGlobal(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_0024);

    int value = ((int(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(control))[12])(control, nullptr);
    int globalSystem = GetGlobalValue_4FBDE0();
    return GlobalValue_PushControlValue(globalSystem, nullptr, value);
}

HOOK(0x549520, CMenuVideoOptionsInGame_PushControlValueToGlobal, "auto");
