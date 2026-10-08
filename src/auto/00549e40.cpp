#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binding to the original callee at 0x549CF0 (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, CMenuVideoOptions_PrepareMenuState, 0x549CF0, (void* self, void* edx));

// Global string pointers used as button labels.
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x899AD4);

// Refreshes the menu state, then either returns the stored field_0018 value
// (when non-zero) or pushes the matching option label to the child object at +0x14.
static int __fastcall CMenuVideoOptions_UpdateOptionsButtonLabel(CMenuVideoOptions* self, void* /*edx*/)
{
    CMenuVideoOptions_PrepareMenuState(self, nullptr);

    // Virtual call on the child object at +0x14, slot 0x60 (24): (child, this, 0)
    void* child = reinterpret_cast<void*>(self->field_0014);
    ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, child, 0)[0x60 / 4])(
        child, nullptr, self, 0);

    int storedFlag = static_cast<int>(self->field_0018);
    if (storedFlag != 0)
    {
        return storedFlag;
    }

    // Virtual call on self, slot 0x30 (12); result tested as a byte (al).
    uint8_t isActive = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    const char* label = g_pszMenu_P_Option;
    if (isActive != 0)
    {
        label = g_pszMenu_P_OptIng;
    }

    // Re-read child pointer after the previous call, then call slot 0x58 (22) with the label.
    void* childAgain = reinterpret_cast<void*>(self->field_0014);
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, childAgain, 0)[0x58 / 4])(
        childAgain, nullptr, label);
}

HOOK(0x00549E40, CMenuVideoOptions_UpdateOptionsButtonLabel, "auto");
