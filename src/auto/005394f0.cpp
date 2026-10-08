#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(char, __fastcall, CMenuLoading_UpdatePortraits, 0x00538DD0,
         (CMenuLoading* self, void* edx, void* context));
CSF_FUNC(void*, __fastcall, CMenuLoading_InitializePanelContents, 0x00539050,
         (CMenuLoading* self, void* edx));
CSF_FUNC(std::uint16_t*, __cdecl, MenuLoading_GetLoadingText, 0x00539340,
         (std::uint16_t* buffer, int capacity));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupNamedElement, 0x00693360,
         (void* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, std::uint16_t* text));

static char __fastcall CMenuLoading_InitializePanel(
    CMenuLoading* self, void* /*edx*/, void* menuSource, void* context)
{
    std::uint32_t result = self->m_ownedObjectA;
    if (result)
        return static_cast<char>(result);

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x58 / 4])(self, nullptr);

    void* panelContainer =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, menuSource, 0)[0x54 / 4])(menuSource, nullptr);

    void* nameRegistry = CSF_AT(void*, context, 0);
    void** containerVtable = CSF_AT(void**, panelContainer, 0);
    void* panelName = LookupNamedElement(
        nameRegistry, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        ((void*(__fastcall*)(void*, void*, void*))
            containerVtable[0x18 / 4])(panelContainer, nullptr, panelName);
    result = reinterpret_cast<std::uint32_t>(panel);
    self->m_ownedObjectA = result;

    if (result)
    {
        void* settings = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* dimensions =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, settings, 0)[0x18 / 4])(settings, nullptr);
        const std::uint32_t viewportWidth =
            CSF_AT(std::uint32_t, dimensions, 0);

        settings = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        dimensions =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, settings, 0)[0x18 / 4])(settings, nullptr);
        const std::uint32_t viewportHeight =
            CSF_AT(std::uint32_t, dimensions, 4);

        panel = reinterpret_cast<void*>(self->m_ownedObjectA);
        void** panelVtable = CSF_AT(void**, panel, 0);

        double height = static_cast<double>(
            static_cast<std::int32_t>(viewportHeight));
        if (static_cast<std::int32_t>(viewportHeight) < 0)
            height += static_cast<double>(g_UnsignedIntegerBias);

        const std::int32_t heightSign =
            static_cast<std::int32_t>(g_ScreenHeight);
        double screenHeight = static_cast<double>(
            static_cast<std::int32_t>(g_ScreenHeight));
        if (heightSign < 0)
            screenHeight += static_cast<double>(g_UnsignedIntegerBias);
        const float heightScale = static_cast<float>(height / screenHeight);

        double width = static_cast<double>(
            static_cast<std::int32_t>(viewportWidth));
        if (static_cast<std::int32_t>(viewportWidth) < 0)
            width += static_cast<double>(g_UnsignedIntegerBias);

        const std::int32_t widthSign =
            static_cast<std::int32_t>(g_ScreenWidth);
        double screenWidth = static_cast<double>(
            static_cast<std::int32_t>(g_ScreenWidth));
        if (widthSign < 0)
            screenWidth += static_cast<double>(g_UnsignedIntegerBias);
        const float widthScale = static_cast<float>(width / screenWidth);

        result =
            ((std::uint32_t(__fastcall*)(void*, void*, float, float))
                panelVtable[0x150 / 4])(
                    panel, nullptr, widthScale, heightScale);
    }

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    if (!panel)
        return static_cast<char>(result);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, panel, 0)[0x248 / 4])(panel, nullptr, 0);

    CMenuLoading_InitializePanelContents(self, nullptr);

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    void* panelState =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, panel, 0)[0xdc / 4])(panel, nullptr);
    self->m_ownedBlock = reinterpret_cast<std::uint32_t>(panelState);
    AddObjectReference(static_cast<std::uint8_t*>(panelState) + 4, nullptr);

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, panel, 0)[0xd8 / 4])(panel, nullptr, 0);

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    void* progress =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, panel, 0)[0xfc / 4])(
                panel, nullptr, CSF_STR(0x008421F8, "Progreso"));
    self->field_0040 = reinterpret_cast<std::uint32_t>(progress);

    if (progress)
    {
        panel = reinterpret_cast<void*>(self->m_ownedObjectA);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, panel, 0)[0x100 / 4])(panel, nullptr, progress);

        progress = reinterpret_cast<void*>(self->field_0040);
        void* progressState =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, progress, 0)[0xdc / 4])(progress, nullptr);

        if (progressState)
        {
            progress = reinterpret_cast<void*>(self->field_0040);
            progressState =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, progress, 0)[0xdc / 4])(progress, nullptr);
            void* extent =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, progressState, 0)[0x3c / 4])(
                        progressState, nullptr);
            self->field_0044 =
                static_cast<float>(CSF_AT(std::int32_t, extent, 0));
        }
    }

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    void* briefing =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, panel, 0)[0xfc / 4])(
                panel, nullptr, CSF_STR(0x00841730, "Briefing"));
    self->field_0028 = reinterpret_cast<std::uint32_t>(briefing);

    if (briefing)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x5c / 4])(self, nullptr);

        std::uint16_t textBuffer[1024];
        std::uint16_t* text = MenuLoading_GetLoadingText(textBuffer, 1024);
        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_0028), nullptr, text);
    }

    panel = reinterpret_cast<void*>(self->m_ownedObjectA);
    void* portraits =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, panel, 0)[0xfc / 4])(
                panel, nullptr, CSF_STR(0x008421F0, "Caretos"));
    self->field_0054 = reinterpret_cast<std::uint32_t>(portraits);

    if (portraits)
    {
        panel = reinterpret_cast<void*>(self->m_ownedObjectA);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, panel, 0)[0x100 / 4])(panel, nullptr, portraits);
        return CMenuLoading_UpdatePortraits(self, nullptr, context);
    }

    return static_cast<char>(reinterpret_cast<std::uint32_t>(portraits));
}

HOOK(0x005394F0, CMenuLoading_InitializePanel, "auto");
