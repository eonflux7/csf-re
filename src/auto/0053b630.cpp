#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees (bound by address).
CSF_FUNC(void*, __cdecl, GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, OptionsSubsystem_Apply, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuGameOptions_RefreshSection, 0x0053AD90, (CMenuGameOptions* self, void* edx));

static int __fastcall CMenuGameOptions_InitializeAndRefresh(CMenuGameOptions* self, void* /*edx*/, int a2, int a3)
{
    (void)a3;  // second stack arg is popped by ret 8 but unused by the original

    // vtbl[+0x60]: base-level setup call on self.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[24])(self, nullptr);

    self->field_0018 = static_cast<uint32_t>(a2);

    void* globalValue = GetGlobalValue();
    OptionsSubsystem_Apply(globalValue, nullptr);

    // Object at self->field_001c: vtbl[+0x16c].
    void* subObject = reinterpret_cast<void*>(self->field_001c);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[91])(subObject, nullptr);

    // vtbl[+0x50], vtbl[+0x70], vtbl[+0x6c] on self.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[20])(self, nullptr);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[28])(self, nullptr);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[27])(self, nullptr);

    CMenuGameOptions_RefreshSection(self, nullptr);

    // vtbl[+0x44](self, self->field_001c)
    void* subObjectArg = reinterpret_cast<void*>(self->field_001c);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[17])(self, nullptr, subObjectArg);

    // Return value is reloaded from self->field_001c after the call.
    return static_cast<int>(self->field_001c);
}

HOOK(0x0053B630, CMenuGameOptions_InitializeAndRefresh, "auto");
