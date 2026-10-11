#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, DestroyTemporaryMenuResource, 0x004FBA10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

static void __fastcall CMenuManager_ResetForTacticScreen(
    CMenuManager* self, void* /*edx*/)
{
    void* temporaryResource = reinterpret_cast<void*>(self->field_0078);
    if (temporaryResource)
    {
        DestroyTemporaryMenuResource(temporaryResource, nullptr);
        Mem_Free(temporaryResource);
        self->field_0078 = 0;
    }

    void* object = reinterpret_cast<void*>(self->m_pFader);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);

    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self->field_0014) + 0x7C,
        nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    object = reinterpret_cast<void*>(self->field_00ec);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->m_pScreenProfiles);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_0028);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_002c);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_003c);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_0040);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->m_pScreenMainMenu);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_0034);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->m_pScreenGameOptions);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr);

    object = reinterpret_cast<void*>(self->field_007c);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    const char* tacticPanelName = g_pszMenu_P_Tactic;
    self->vSlot2C(static_cast<int>(self->m_pScreenTactic),
                  reinterpret_cast<int>(tacticPanelName));
}

HOOK(0x0050F6E0, CMenuManager_ResetForTacticScreen, "auto");
