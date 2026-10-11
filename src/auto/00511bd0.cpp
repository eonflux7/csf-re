#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_50DFF0, 0x0050DFF0, (void* self, void* edx));

// Shuts down the five sub-objects (vtable slot 0x3C each), then tail-jumps
// into the vtable slot 0x3C of the last sub-object with the caller's args forwarded.
static hook::Tail __fastcall C_008407a8_ReleaseChildrenAndTail(void* selfPtr, const uint32_t* /*args*/) {
    auto* self = reinterpret_cast<C_008407a8*>(selfPtr);

    sub_50DFF0(self, nullptr);

    void* childA = reinterpret_cast<void*>(self->field_0110);
    ((void(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(childA))[0x3c / 4])(childA, nullptr);

    void* childB = reinterpret_cast<void*>(self->field_0118);
    ((void(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(childB))[0x3c / 4])(childB, nullptr);

    void* childC = reinterpret_cast<void*>(self->field_0124);
    ((void(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(childC))[0x3c / 4])(childC, nullptr);

    void* childD = reinterpret_cast<void*>(self->field_012c);
    ((void(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(childD))[0x3c / 4])(childD, nullptr);

    void* lastChild = reinterpret_cast<void*>(self->field_0138);
    return hook::tail_jump(lastChild, (*reinterpret_cast<void***>(lastChild))[0x3c / 4]);
}

HOOK_TAIL(0x00511BD0, C_008407a8_ReleaseChildrenAndTail, "game_bdd_loader");
