#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls the vSub_3c slot (byte offset 0x3c, vtable index 15) on a child object.
// C_008407a8 has no typed wrapper, so the slot is called through the vtable.
static void __fastcall C_00840754_CallChildSub3c(void* child) {
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3c / 4])(child, nullptr);
}

// Calls vSub_3c on the nine child objects in the original's order, then tail-jumps
// through field_0044's vSub_3c slot, forwarding the caller's frame.
static hook::Tail __fastcall C_00840754_CallSub3cOnChildrenAndTail(void* self, const uint32_t* args) {
    (void)args;
    C_00840754* obj = static_cast<C_00840754*>(self);

    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0008));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_000c));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0010));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0014));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0018));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_001c));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0020));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_003c));
    C_00840754_CallChildSub3c(reinterpret_cast<void*>(obj->field_0024));

    void* tailObj = reinterpret_cast<void*>(obj->field_0044);
    return hook::tail_jump(tailObj, (*reinterpret_cast<void***>(tailObj))[0x3c / 4]);
}

HOOK_TAIL(0x509F50, C_00840754_CallSub3cOnChildrenAndTail, "app_main");
