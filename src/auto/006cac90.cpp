#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binding for the tail-jump target (world add/remove clump handler).
CSF_FUNC(int, __fastcall, sub_6D5C10, 0x006D5C10, (void* self, void* edx, char* a2));

// Reads the object handle stored at +0xBC and forwards it as `this` to the
// handler, along with the caller's edx and stack argument (tail jump).
static int __fastcall C_00860bb8_ForwardToStoredObject(C_00860bb8* self, void* edx, int a1)
{
    return sub_6D5C10(reinterpret_cast<void*>(self->field_00bc), edx, reinterpret_cast<char*>(a1));
}

HOOK(0x006CAC90, C_00860bb8_ForwardToStoredObject, "auto");
