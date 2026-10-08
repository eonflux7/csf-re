#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x005342E0 - __thiscall(int this, int a2, int a3), ret 4 - handles the forwarded command payload.
CSF_FUNC(uint32_t*, __fastcall, CMenuLanguage_DispatchCommand, 0x5342E0, (CMenuLanguage* self, void* /*edx*/, uint32_t* a3));

// 0x005344B0 - __thiscall(CMenuLanguage* this, uint32_t* a3), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the handler,
// otherwise the command pointer itself is returned untouched.
static uint32_t* __fastcall CMenuLanguage_HandleCommand(CMenuLanguage* self, void* /*edx*/, uint32_t* a3)
{
    if (*a3 == 13)
        return CMenuLanguage_DispatchCommand(self, nullptr, a3);
    return a3;
}

HOOK(0x5344B0, CMenuLanguage_HandleCommand, "auto");
