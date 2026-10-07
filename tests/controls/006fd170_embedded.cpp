#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// NEGATIVE CONTROL (the shape the grunt originally wrote, crashed in-mission): treats the object embedded at +0x10
// as a pointer stored at +0x10. Written as a call, as the grunt did, so two checks apply.
static int __fastcall CallInnerObjectVtblSlot4(C_00862f70* self, void* /*edx*/)
{
    void* inner = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 0x10);
    void** innerVtbl = *reinterpret_cast<void***>(inner);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(innerVtbl[4])(inner, nullptr);
}

HOOK(0x6FD170, CallInnerObjectVtblSlot4, "auto");
