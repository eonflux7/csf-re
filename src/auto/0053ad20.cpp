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

// Callee: CMenuMpGameMode_HandleLinkedItemChange (__thiscall, 1 stack arg, ret 4)
CSF_FUNC(int, __fastcall, CMenuMpGameMode_HandleLinkedItemChange, 0x54ABC0, (void* self, void* edx, void* a2));

// Data: menu string pointers (values loaded from these addresses)
CSF_GLOBAL(const char*, g_pszMenu_P_Contro, 0x899AA8);
CSF_GLOBAL(const char*, g_pszMenu_P_ConIng, 0x899AAC);

// Vtable slot signatures used by the sub-object at +0x14
using SubObjectSlot60Fn = int(__fastcall*)(void* self, void* edx, void* arg, int flag);
using SubObjectSlot58Fn = int(__fastcall*)(void* self, void* edx, const char* text);

// Slot 0x30 (index 12) on the owning menu: returns a byte flag in al
using OwnerFlagSlot30Fn = uint8_t(__fastcall*)(void* self, void* edx);

static int __fastcall CMenuOptionsPC_HandleLinkedItemMenuChange(CMenuOptionsPC* self, void* /*edx*/, void* eventData)
{
    CMenuMpGameMode_HandleLinkedItemChange(self, nullptr, eventData);

    // Virtual slot 0x30 on the owner: boolean-like flag in al
    uint8_t ownerFlag = ((OwnerFlagSlot30Fn)CSF_AT(void**, self, 0)[12])(self, nullptr);

    uint32_t result = self->field_0020;

    if (ownerFlag == 0)
    {
        if (result != 0)
        {
            if (result == CSF_AT(uint32_t, eventData, 4))
            {
                void* subObject = reinterpret_cast<void*>(self->field_0014);
                ((SubObjectSlot60Fn)CSF_AT(void**, subObject, 0)[24])(subObject, nullptr, self, 0);

                subObject = reinterpret_cast<void*>(self->field_0014);
                return ((SubObjectSlot58Fn)CSF_AT(void**, subObject, 0)[22])(subObject, nullptr, g_pszMenu_P_Contro);
            }
        }
    }
    else if (result != 0 && result == CSF_AT(uint32_t, eventData, 4))
    {
        void* subObject = reinterpret_cast<void*>(self->field_0014);
        ((SubObjectSlot60Fn)CSF_AT(void**, subObject, 0)[24])(subObject, nullptr, self, 0);

        subObject = reinterpret_cast<void*>(self->field_0014);
        return ((SubObjectSlot58Fn)CSF_AT(void**, subObject, 0)[22])(subObject, nullptr, g_pszMenu_P_ConIng);
    }

    return static_cast<int>(result);
}

HOOK(0x0053AD20, CMenuOptionsPC_HandleLinkedItemMenuChange, "auto");
