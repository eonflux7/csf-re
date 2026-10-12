#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_4E0870, 0x004E0870, (void* self, void* edx, int a2));

// __thiscall: forwards to the object stored at +0x08, passing the original `this` as the stack argument.
static int __fastcall ForwardToFieldAt8Handler(unsigned int** self, void* /*edx*/) {
    void* child = reinterpret_cast<void*>(self[2]);
    return sub_4E0870(child, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)));
}

HOOK(0x004E0BE0, ForwardToFieldAt8Handler, "auto");
