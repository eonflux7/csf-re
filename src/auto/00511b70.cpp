#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callee bound by address. __thiscall-style, takes `this` in ecx.
CSF_FUNC(int, __fastcall, sub_50F6E0, 0x0050F6E0, (void* self, void* edx));

// Notifies the six child objects (vtable slot 0x3C each), then either tail-jumps
// into the field_0064 object's slot 0x3C when the flag at +0x140 is set, or returns
// the flag byte as the result.
static hook::Tail __fastcall C_008407a8_NotifyChildrenAndTailCall(void* self, const uint32_t* args)
{
    (void)args;
    C_008407a8* menu = reinterpret_cast<C_008407a8*>(self);

    sub_50F6E0(menu, nullptr);

    {
        void* child = reinterpret_cast<void*>(menu->field_0134);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }
    {
        void* child = reinterpret_cast<void*>(menu->field_011c);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }
    {
        void* child = reinterpret_cast<void*>(menu->field_0114);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }
    {
        void* child = reinterpret_cast<void*>(menu->field_0120);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }
    {
        void* child = reinterpret_cast<void*>(menu->field_0128);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }
    {
        void* child = reinterpret_cast<void*>(menu->field_0130);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr);
    }

    uint8_t flag = menu->field_0140;
    if (flag != 0) {
        void* target = reinterpret_cast<void*>(menu->field_0064);
        return hook::tail_jump(target, (*reinterpret_cast<void***>(target))[0x3C / 4]);
    }
    return hook::tail_ret(static_cast<uint32_t>(flag));
}

HOOK_TAIL(0x00511B70, C_008407a8_NotifyChildrenAndTailCall, "game_bdd_loader");
