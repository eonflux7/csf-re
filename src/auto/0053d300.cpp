#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto FindPanelKey =
    reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(CSF_ADDR_FN(0x00693360));
static const auto GetGlobalContext =
    reinterpret_cast<int (__cdecl*)()>(CSF_ADDR_FN(0x006B1380));
static const auto GetDisplayObject =
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00652160));
static const auto AllocatePanel =
    reinterpret_cast<void* (__cdecl*)(std::uint32_t)>(CSF_ADDR_FN(0x006910F0));
static const auto ConstructInterfacePanel =
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00438010));
static const auto ConstructInterfaceDocument =
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00692B10));
static const auto LoadInterfaceDocument =
    reinterpret_cast<char (__fastcall*)(void*, void*, const char*)>(CSF_ADDR_FN(0x00694770));
static const auto ConfigureInterfacePanel =
    reinterpret_cast<char (__fastcall*)(void*, void*, std::uint32_t)>(CSF_ADDR_FN(0x00437A50));
static const auto DestroyInterfaceDocument =
    reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00694380));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static double ConvertUnsignedDisplayDimension(std::uint32_t value)
{
    double result = static_cast<double>(static_cast<std::int32_t>(value));
    if (static_cast<std::int32_t>(value) < 0)
        result += static_cast<double>(g_UnsignedIntegerBias);
    return result;
}

static int __fastcall CMenuMissionInfo_InitializeMainPanel(
    CMenuMissionInfo* self, void* /*edx*/, void* screen, void* lookupContext)
{
    int result = static_cast<int>(self->field_001c);
    if (result != 0)
        return result;

    void* panelContainer =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x54 / 4])(screen, nullptr);

    void* lookupObject = CSF_AT(void*, lookupContext, 0);
    void** containerVtable = CSF_AT(void**, panelContainer, 0);
    void* panelKey = FindPanelKey(
        lookupObject, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            containerVtable[0x18 / 4])(panelContainer, nullptr, panelKey);

    self->field_001c = reinterpret_cast<std::uint32_t>(panel);
    if (!panel)
        return 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, panel, 0)[0xE0 / 4])(panel, nullptr, 1);

    void* display = GetDisplayObject(
        reinterpret_cast<void*>(GetGlobalContext()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const std::uint32_t displayWidth = CSF_AT(std::uint32_t, dimensions, 0);

    display = GetDisplayObject(
        reinterpret_cast<void*>(GetGlobalContext()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const std::uint32_t displayHeight = CSF_AT(std::uint32_t, dimensions, 4);

    panel = reinterpret_cast<void*>(self->field_001c);
    auto setScale = reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
        CSF_AT(void**, panel, 0)[0x150 / 4]);
    const float heightScale = static_cast<float>(
        ConvertUnsignedDisplayDimension(displayHeight) /
        ConvertUnsignedDisplayDimension(g_ScreenHeight));
    const float widthScale = static_cast<float>(
        ConvertUnsignedDisplayDimension(displayWidth) /
        ConvertUnsignedDisplayDimension(g_ScreenWidth));
    setScale(panel, nullptr, widthScale, heightScale);

    panel = reinterpret_cast<void*>(self->field_001c);
    void* control =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x00842CA8, "StaticContinueExit"));
    panel = reinterpret_cast<void*>(self->field_001c);
    self->field_0020 = reinterpret_cast<std::uint32_t>(control);

    control =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x00842C94, "StaticCreditsExit"));
    panel = reinterpret_cast<void*>(self->field_001c);
    self->field_002c = reinterpret_cast<std::uint32_t>(control);

    control =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x00842C80, "StaticRestartExit"));
    panel = reinterpret_cast<void*>(self->field_001c);
    self->field_0024 = reinterpret_cast<std::uint32_t>(control);

    control =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x00842C68, "StaticRestartExitDemo"));
    self->field_0028 = reinterpret_cast<std::uint32_t>(control);

    void* interfacePanel = AllocatePanel(400);
    if (interfacePanel)
        interfacePanel = ConstructInterfacePanel(interfacePanel, nullptr);

    panel = reinterpret_cast<void*>(self->field_001c);
    self->field_0084 = reinterpret_cast<std::uint32_t>(interfacePanel);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, panel, 0)[0xF8 / 4])(panel, nullptr, interfacePanel);

    interfacePanel = reinterpret_cast<void*>(self->field_0084);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, interfacePanel, 0)[0xC8 / 4])(interfacePanel, nullptr, 1);

    std::uint32_t document[15];
    ConstructInterfaceDocument(document, nullptr);
    LoadInterfaceDocument(
        document, nullptr, CSF_STR(0x00841F84, "Gfx/Interfaz.txt"));

    const std::uint32_t documentRoot = document[0];
    ConfigureInterfacePanel(
        reinterpret_cast<void*>(self->field_0084), nullptr, documentRoot);

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x50 / 4])(self, nullptr);

    return DestroyInterfaceDocument(document, nullptr);
}

HOOK(0x0053D300, CMenuMissionInfo_InitializeMainPanel, "auto");
