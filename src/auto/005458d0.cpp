#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int*, __cdecl, MenuSaveGame_ClearSlotState, 0x00544480, (int* a1));
CSF_FUNC(std::uint32_t*, __fastcall, CMenuSaveGame_AppendElement36, 0x005454A0, (void* self, void* , int element));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

CSF_GLOBAL(const char*, g_pszMenu_P_EditG, 0x00899B04);

static inline __attribute__((always_inline))
void MenuSaveGame_ResetTemporaryString(C_0082d410& text)
{
    CSF_AT(std::uint32_t, &text, 0x18) = 15;
    CSF_AT(std::uint32_t, &text, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&text.field_0004)[0] = 0;
}

static inline __attribute__((always_inline))
void MenuSaveGame_ReleaseTemporaryString(C_0082d410& text)
{
    if (CSF_AT(std::uint32_t, &text, 0x18) >= 0x10 &&
        text.field_0004 != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(text.field_0004));
    }
    MenuSaveGame_ResetTemporaryString(text);
}

static void* __fastcall CMenuSaveGame_BeginNewSaveFlow(void* self, void* /*edx*/)
{
    auto* globalState =
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
    void* result = C_00861120_GetSchedulerProxy(globalState, nullptr);

    if (result == reinterpret_cast<void*>(8))
    {
        CSF_AT(std::int32_t, self, 0x50) = -1;
        return result;
    }
    if (result != reinterpret_cast<void*>(4))
        return result;

    void* menu = CSF_AT(void*, self, 0x40);
    void* editDialog =
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_EditG);

    auto* selfBytes = static_cast<std::uint8_t*>(self);
    CSF_AT(void*, self, 0x5C) = self;
    MenuSaveGame_ClearSlotState(reinterpret_cast<int*>(selfBytes + 0x60));

    alignas(8) std::uint8_t editNameBytes[0x1C];
    alignas(8) std::uint8_t editElementBytes[0x24];
    alignas(8) std::uint8_t okNameBytes[0x1C];
    alignas(8) std::uint8_t okElementBytes[0x24];
    alignas(8) std::uint8_t cancelNameBytes[0x20];
    alignas(8) std::uint8_t cancelElementBytes[0x24];

    auto& editName = *reinterpret_cast<C_0082d410*>(editNameBytes);
    auto& editElement = *reinterpret_cast<C_0082d410*>(editElementBytes);
    auto& okName = *reinterpret_cast<C_0082d410*>(okNameBytes);
    auto& okElement = *reinterpret_cast<C_0082d410*>(okElementBytes);
    auto& cancelName = *reinterpret_cast<C_0082d410*>(cancelNameBytes);
    auto& cancelElement = *reinterpret_cast<C_0082d410*>(cancelElementBytes);

    MenuSaveGame_ResetTemporaryString(editName);
    AssignStringRange_429B60(
        &editName, nullptr,
        const_cast<char*>(CSF_STR(0x008413F0, "EstaticoEdit")), 12);
    MenuSaveGame_ResetTemporaryString(editElement);
    AssignStringSubstring(&editElement, nullptr, &editName, 0, -1);
    _strncpy(reinterpret_cast<char*>(editElementBytes + 0x1C),
             CSF_STR(0x00843BD8, "GEN5"), 4);
    editElementBytes[0x20] = 0;
    CMenuSaveGame_AppendElement36(
        selfBytes + 0x60, nullptr,
        reinterpret_cast<int>(&editElement));
    MenuSaveGame_ReleaseTemporaryString(editElement);
    MenuSaveGame_ReleaseTemporaryString(editName);

    MenuSaveGame_ResetTemporaryString(okName);
    AssignStringRange_429B60(
        &okName, nullptr,
        const_cast<char*>(CSF_STR(0x008413D4, "BotonOkQuestion")), 15);
    MenuSaveGame_ResetTemporaryString(okElement);
    AssignStringSubstring(&okElement, nullptr, &okName, 0, -1);
    _strncpy(reinterpret_cast<char*>(okElementBytes + 0x1C),
             CSF_STR(0x00843BD0, "GEN3"), 4);
    okElementBytes[0x20] = 0;
    CMenuSaveGame_AppendElement36(
        selfBytes + 0x60, nullptr,
        reinterpret_cast<int>(&okElement));
    MenuSaveGame_ReleaseTemporaryString(okElement);
    MenuSaveGame_ReleaseTemporaryString(okName);

    MenuSaveGame_ResetTemporaryString(cancelName);
    AssignStringRange_429B60(
        &cancelName, nullptr,
        const_cast<char*>(CSF_STR(0x008413C0, "BotonCancelQuestion")), 19);
    MenuSaveGame_ResetTemporaryString(cancelElement);
    AssignStringSubstring(&cancelElement, nullptr, &cancelName, 0, -1);
    _strncpy(reinterpret_cast<char*>(cancelElementBytes + 0x1C),
             CSF_STR(0x00843BC8, "GEN4"), 4);
    cancelElementBytes[0x20] = 0;
    CMenuSaveGame_AppendElement36(
        selfBytes + 0x60, nullptr,
        reinterpret_cast<int>(&cancelElement));
    MenuSaveGame_ReleaseTemporaryString(cancelElement);
    MenuSaveGame_ReleaseTemporaryString(cancelName);

    const std::uint32_t slotOffset =
        200 * CSF_AT(std::uint32_t, self, 0x54);
    const std::uint32_t slots = CSF_AT(std::uint32_t, self, 0x24);
    AssignStringRange_488830(
        selfBytes + 0x70, nullptr,
        reinterpret_cast<void*>(slots + slotOffset), 0, 0xFFFFFFFF);

    CSF_AT(std::uint8_t, self, 0x8C) = 1;
    CSF_AT(void*, editDialog, 0x54) = selfBytes + 0x5C;

    void* view =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, view, 0)[0xC0 / 4])(view, nullptr, 0);

    menu = CSF_AT(void*, self, 0x40);
    result =
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_pszMenu_P_EditG);
    CSF_AT(std::int32_t, self, 0x50) = 2;
    return result;
}

HOOK(0x005458D0, CMenuSaveGame_BeginNewSaveFlow, "auto");
