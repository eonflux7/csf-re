#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_480CC0, 0x00480CC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_4846D0, 0x004846D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_486FC0, 0x00486FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

static void __fastcall CCharActionFsm_ClearOwnerFlag200AndRefresh(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0028 == 0xFFFFFFFFu)
        return;

    // Owner object lives at field_0008; clear bit 0x200 of its dword at +0x2E4.
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, owner, 0x2E4) &= ~0x200u;
    sub_498F50(owner, nullptr, 0x200, 0);

    // Query the state of the object at field_009c (vtable slot 0x4C / 4 = 19).
    void* target = reinterpret_cast<void*>(self->field_009c);
    if (target) {
        int state = ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x4C / 4])(target, nullptr);
        if (state >= 10 && state <= 11)
            sub_4846D0(self, nullptr);
    }

    sub_480CC0(self, nullptr);
    sub_486FC0(self, nullptr);
}

HOOK(0x488530, CCharActionFsm_ClearOwnerFlag200AndRefresh, "auto");
