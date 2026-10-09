#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// sub_42E0E0: pre-attach step, original signature kept.
CSF_FUNC(int, __fastcall, sub_42E0E0, 0x0042E0E0, (void* self, void* edx, int a2));

// Attaches this HUD's two child widget slots (+0x314, +0x31C) to the target object a2
// through its vtable (slots 0x28 and 0x38). Returns the result of the second call.
static int __fastcall CHud_AttachPanelsToTarget(CHud* self, void* /*edx*/, int a2)
{
    sub_42E0E0(self, nullptr, a2);

    void* target = reinterpret_cast<void*>(a2);
    void* firstChild = reinterpret_cast<uint8_t*>(self) + 0x314;
    ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[0x28 / 4])(target, nullptr, firstChild);

    void* secondChild = reinterpret_cast<uint8_t*>(self) + 0x31C;
    return ((int (__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[0x38 / 4])(target, nullptr, secondChild);
}

HOOK(0x0042E1A0, CHud_AttachPanelsToTarget, "auto");
