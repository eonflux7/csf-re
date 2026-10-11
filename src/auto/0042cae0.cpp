#include <cstdint>
#include <cstring>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CheatMenu_RefreshEntries, 0x0042B880, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CheatMenu_UpdateCheckboxes, 0x0042BE10, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CheatMenu_UpdateSelection, 0x0042C7F0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CheatMenu_RenderEntries, 0x0042C3B0, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static void* CheatMenu_ChildObject(C_00830578* self) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
}

// Virtual slot 0x80 (index 32) on the child object at field_0018.
static uintptr_t CheatMenu_CallChildSlot80(C_00830578* self) {
    void* child = CheatMenu_ChildObject(self);
    auto fn = reinterpret_cast<uintptr_t (__fastcall*)(void*, void*)>(CSF_AT(void**, child, 0)[0x80 / 4]);
    return fn(child, nullptr);
}

static void* CheatMenu_GetGlobalStatePointer() {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
}

static void __fastcall CheatMenu_UpdateAndRender(C_00830578* self, void* /*edx*/, int a2) {
    if (self->field_0018 != 0 &&
        CheatMenu_CallChildSlot80(self) != 0 &&
        CheatMenu_CallChildSlot80(self) != reinterpret_cast<uintptr_t>(self)) {
        void* globalState = CheatMenu_GetGlobalStatePointer();
        if (CSF_AT(uint8_t, globalState, 0xD0)) {
            reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[0x68 / 4])(self, nullptr);
        }
    } else {
        CheatMenu_RefreshEntries(self, nullptr, a2);

        float floatArg;
        std::memcpy(&floatArg, &a2, sizeof floatArg);
        CheatMenu_UpdateCheckboxes(self, nullptr, floatArg);

        GetGlobalState_50D190();

        CheatMenu_UpdateSelection(self, nullptr, floatArg);
        CheatMenu_RenderEntries(self, nullptr, floatArg);
    }
}

HOOK(0x0042CAE0, CheatMenu_UpdateAndRender, "auto");
