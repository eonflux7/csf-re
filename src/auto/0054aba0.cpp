#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x0054ABA0 (ret 4)
// Looks up the "BotonBack" control on the passed menu object via virtual slot 63
// (byte offset 0xFC) and caches the returned pointer in CMenuMpGameMode::field_0010.
static int __fastcall CMenuMpGameMode_SetBackButton(CMenuMpGameMode* self, void* /*edx*/, int a2)
{
    void* menuObj = reinterpret_cast<void*>(a2);
    void** vtbl = *reinterpret_cast<void***>(menuObj);

    int result = (reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(vtbl[0xFC / 4]))(
        menuObj, nullptr, CSF_STR(0x00843B00, "BotonBack"));

    self->field_0010 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x0054ABA0, CMenuMpGameMode_SetBackButton, "auto");
