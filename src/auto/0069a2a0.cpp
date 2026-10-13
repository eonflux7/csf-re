#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0085e760_IncrementAndForwardIsOne(C_0085e760* self, void* /*edx*/, int a2)
{
    self->field_0008++;

    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    auto innerSlot2 = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, innerObject, 0)[2]);
    return innerSlot2(innerObject, nullptr, a2, 1) == 1;
}

HOOK(0x0069A2A0, C_0085e760_IncrementAndForwardIsOne, "auto");
