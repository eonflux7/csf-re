#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_008673b0_StorePackedColorTable, 0x00739170, (C_008673b0* self, void* , float a2, uint8_t* a3, uint32_t a4));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(float, g_TransitionVertex0U, 0x008BBF40);
CSF_GLOBAL(float, g_TransitionVertex0V, 0x008BBF44);
CSF_GLOBAL(float, g_TransitionVertex1U, 0x008BBF5C);
CSF_GLOBAL(float, g_TransitionVertex1V, 0x008BBF60);
CSF_GLOBAL(float, g_TransitionVertex2U, 0x008BBF78);
CSF_GLOBAL(float, g_TransitionVertex2V, 0x008BBF7C);

using RenderStateSetter = void (__cdecl*)(int, uint32_t);

static void __fastcall C_008673b0_RenderTransition(
    C_008673b0* self, void* /*edx*/)
{
    if (self->field_0004 && self->field_0008 == 1)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x10 / 4])(owner, nullptr))
        {
            reinterpret_cast<void (__fastcall*)(C_008673b0*, void*, int)>(
                self->vtbl[0x10 / 4])(self, nullptr, 0);
        }
    }

    if (self->field_0034)
    {
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(10, 5);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(11, 6);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(12, 1);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(14, 0);

        uint32_t raster = CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_0034), 0);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(1, raster);

        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(9, 2);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(6, 0);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(8, 0);
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(20, 1);

        g_TransitionVertex0U = 0.0f;
        g_TransitionVertex0V = 1.0f;
        g_TransitionVertex1U = 0.0f;
        g_TransitionVertex1V = 1.0f;
        g_TransitionVertex2U = 0.0f;
        g_TransitionVertex2V = 1.0f;

        C_008673b0_StorePackedColorTable(
            self, nullptr, self->field_0040,
            reinterpret_cast<uint8_t*>(&self->field_0038), 0);

        float complementaryWeight = g_TransitionOne - self->field_0040;
        C_008673b0_StorePackedColorTable(
            self, nullptr, complementaryWeight,
            reinterpret_cast<uint8_t*>(&self->field_003c), 1u);
    }
}

HOOK(0x007391F0, C_008673b0_RenderTransition, "auto");
