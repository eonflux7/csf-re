#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, C_00834050_ForwardToInnerHandler, 0x004DE7E0, (C_00834050* self, void* , int a2, int a3, int a4));

static int __fastcall C_00834050_ConfigureFocusEntry(
    C_00834050* self, void* /*edx*/, int* entry)
{
    if (CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr))
    {
        auto* handler = reinterpret_cast<C_00834050*>(
            CGuiControl_GetField012C(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));

        C_00834050_ForwardToInnerHandler(
            handler, nullptr,
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(&self->field_0084),
            reinterpret_cast<int>(entry));
    }

    self->field_0084 = entry[0];
    CSF_AT(uint32_t, self, 0x88) = entry[1];
    CSF_AT(uint32_t, self, 0x8C) = entry[2];

    void* interfaceObject = reinterpret_cast<void*>(self->vGetField016c());
    void* focusHandler = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, interfaceObject, 0)[0x18 / 4])(
                interfaceObject, nullptr));

    return ((int(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, focusHandler, 0)[0x14 / 4])(
            focusHandler, nullptr, entry);
}

HOOK(0x0048E460, C_00834050_ConfigureFocusEntry, "auto");
