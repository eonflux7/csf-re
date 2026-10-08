#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// void __thiscall sub_5464E0(CMenuExtras* this, int a2)
CSF_FUNC(void, __fastcall, CMenuExtras_SelectThumbnailFromControl, 0x5464E0, (CMenuExtras* self, void* edx, int a2));
// char __thiscall sub_548730(CMenuExtras* this, uint32_t* a2)
CSF_FUNC(char, __fastcall, CMenuExtras_HandleSelect, 0x548730, (CMenuExtras* self, void* edx, uint32_t* a2));

// Dispatches a menu message: id 13 -> HandleSelect(this, a2); id 15 -> HandleConfirm(this, a2[1]).
static void __fastcall CMenuExtras_HandleMessage(CMenuExtras* self, void* /*edx*/, uint32_t* a2)
{
    if (a2[0] == 13)
    {
        CMenuExtras_HandleSelect(self, nullptr, a2);
    }
    else if (a2[0] == 15)
    {
        CMenuExtras_SelectThumbnailFromControl(self, nullptr, static_cast<int>(a2[1]));
    }
}

HOOK(0x548870, CMenuExtras_HandleMessage, "auto");
