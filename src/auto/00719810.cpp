#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_UpdateAndRenderDebugRect(
    CGuiControl* self, void* /*edx*/)
{
    const int enabledResult =
        reinterpret_cast<int(__fastcall*)(CGuiControl*, void*)>(
            self->vtbl[0x184 / 4])(self, nullptr);

    if (static_cast<uint8_t>(enabledResult) == 0)
        return enabledResult;

    uint32_t rectangleData[4];

    void* debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    void** controlVtable = self->vtbl;
    void** debugVtable = CSF_AT(void**, debugRect, 0);

    const int boundsResult =
        reinterpret_cast<int(__fastcall*)(CGuiControl*, void*, uint32_t*)>(
            controlVtable[0x160 / 4])(self, nullptr, rectangleData + 2);

    debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        debugVtable[0x1C / 4])(debugRect, nullptr, boundsResult);

    debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    debugVtable = CSF_AT(void**, debugRect, 0);
    controlVtable = self->vtbl;

    const int propertyResult =
        reinterpret_cast<int(__fastcall*)(CGuiControl*, void*)>(
            controlVtable[0xA4 / 4])(self, nullptr);

    debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        debugVtable[0x24 / 4])(debugRect, nullptr, propertyResult);

    debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    rectangleData[0] = 0;
    rectangleData[1] = 0x437F0000;
    rectangleData[2] = 0;
    rectangleData[3] = 0x42480000;
    debugVtable = CSF_AT(void**, debugRect, 0);

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        debugVtable[0x2C / 4])(debugRect, nullptr, rectangleData);

    debugRect = reinterpret_cast<void*>(self->m_pDebugRect);
    debugVtable = CSF_AT(void**, debugRect, 0);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        debugVtable[0x08 / 4])(debugRect, nullptr);
}

HOOK(0x00719810, CGuiControl_UpdateAndRenderDebugRect, "auto");
