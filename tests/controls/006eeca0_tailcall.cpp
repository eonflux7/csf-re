#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// The original advances ecx by 8 bytes (two DWORDs) so that it points at an
// inner object embedded in *this, loads that object's vtable pointer, and
// tail-calls the virtual method at vtable byte offset 0x70 (slot 28). The
// callee's return value is passed through unchanged.
static int __fastcall CallInnerObjectVtableSlot70(C_00862110* self, void* /*edx*/)
{
    void* innerObject = reinterpret_cast<uint32_t*>(self) + 2;
    void** innerVtbl = *reinterpret_cast<void***>(innerObject);
    int (__fastcall * method)(void*, void*) =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(innerVtbl[0x70 / 4]);
    return method(innerObject, nullptr);
}

HOOK(0x6EECA0, CallInnerObjectVtableSlot70, "auto");
