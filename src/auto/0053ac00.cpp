#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: void sub_521BB0(void) - cdecl, returns a global value (interpreted as an int handle).
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());

// Original: void __thiscall sub_5231D0(int this, int a2) - ret 4.
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x005231D0, (void* self, void* edx, int a2));

// Original: void nullsub_2(void) - empty.
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());

// Virtual slots of the object at this->field_001c (type unknown):
//   +0xBC (slot 47): uint8_t IsReady(void)   (__thiscall)
//   +0xB8 (slot 46): void SetState(int)       (__thiscall, 1 stack arg)

static void __fastcall CMenuOptions_SetActiveTab(CMenuOptions* self, void* /*edx*/, int a2)
{
    // Store the requested tab index first.
    self->field_0034 = static_cast<uint32_t>(a2);

    // If the child object exists and is not ready, ask it to switch state to 1.
    void* child = reinterpret_cast<void*>(self->field_001c);
    if (child != nullptr) {
        uint8_t ready = ((uint8_t(__fastcall*)(void*, void*))(CSF_AT(void**, child, 0)[47]))(child, nullptr);
        if (!ready) {
            void* childAgain = reinterpret_cast<void*>(self->field_001c);
            ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, childAgain, 0)[46]))(childAgain, nullptr, 1);
        }
    }

    // Original pushes field_0034 as the stack argument consumed by ApplyGlobalSelection
    // (the push also remains on the stack during GetGlobalValue_521BB0, which ignores it).
    int selection = static_cast<int>(self->field_0034);
    int globalHandle = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(reinterpret_cast<void*>(static_cast<intptr_t>(globalHandle)), nullptr, selection);
    Nullsub_506AC0();
}

HOOK(0x0053AC00, CMenuOptions_SetActiveTab, "auto");
