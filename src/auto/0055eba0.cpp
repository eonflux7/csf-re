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

// Game functions bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, (void));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (int objectId, void* edx));
CSF_FUNC(void*, __fastcall, LookupNamedElement, 0x00693360, (void* finder, void* edx, const char* name));

// Game data bound by address.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

// Converts a uint32 to double the way the original x87 sequence does:
// load as signed 32-bit, and add the unsigned bias when the sign bit is set.
static double MenuMpOnline_UnsignedToDouble(uint32_t value)
{
    const int32_t signedValue = static_cast<int32_t>(value);
    double result = static_cast<double>(signedValue);
    if (signedValue < 0)
        result += static_cast<double>(g_UnsignedIntegerBias);
    return result;
}

static uint32_t MenuMpOnline_PointerToField(void* pointer)
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(pointer));
}

static void* MenuMpOnline_FieldToPointer(uint32_t field)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(field));
}

static int __fastcall CMenuMpOnline_InitMainPanelAndFriendsButton(CMenuMpOnline* self, void* /*edx*/, void* a4, void* a5)
{
    // Already initialised: return the cached main panel.
    if (self->field_0014 != 0)
        return static_cast<int>(self->field_0014);

    // a4 vtable slot 21 (+0x54) yields the host object that owns the panel lookup.
    void* hostObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, a4, 0)[21])(a4, nullptr);

    // The finder object is the pointer stored at a5 (passed as ecx to the name lookup).
    void* finderObject = *reinterpret_cast<void**>(a5);
    void** hostVtable = CSF_AT(void**, hostObject, 0);

    void* panelNameMatch = LookupNamedElement(finderObject, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));
    void* mainPanel = ((void* (__fastcall*)(void*, void*, void*))hostVtable[6])(hostObject, nullptr, panelNameMatch);
    self->field_0014 = MenuMpOnline_PointerToField(mainPanel);
    if (mainPanel == nullptr)
        return 0;

    // Record word 0 comes from the first resolved object.
    const int firstGlobalId = GetGlobalInt();
    void* firstResolved = LookupObjectById(firstGlobalId, nullptr);
    void* firstRecord = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstResolved, 0)[6])(firstResolved, nullptr);
    const uint32_t layoutXRaw = CSF_AT(uint32_t, firstRecord, 0);

    // Record word 1 comes from the second resolved object.
    const int secondGlobalId = GetGlobalInt();
    void* secondResolved = LookupObjectById(secondGlobalId, nullptr);
    void* secondRecord = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, secondResolved, 0)[6])(secondResolved, nullptr);
    const uint32_t layoutYRaw = CSF_AT(uint32_t, secondRecord, 4);

    // Re-read the panel and its vtable before positioning it.
    void* panel = MenuMpOnline_FieldToPointer(self->field_0014);
    void** panelVtable = CSF_AT(void**, panel, 0);

    const float ratioY = static_cast<float>(MenuMpOnline_UnsignedToDouble(layoutYRaw) /
                                            MenuMpOnline_UnsignedToDouble(g_ScreenHeight));
    const float ratioX = static_cast<float>(MenuMpOnline_UnsignedToDouble(layoutXRaw) /
                                            MenuMpOnline_UnsignedToDouble(g_ScreenWidth));

    // Panel slot 84 (+0x150) takes two float stack arguments (x, y) and pops them.
    ((void(__fastcall*)(void*, void*, float, float))panelVtable[84])(panel, nullptr, ratioX, ratioY);

    // Reload the panel, then fetch the BotonFriends child and store it.
    void* buttonPanel = MenuMpOnline_FieldToPointer(self->field_0014);
    void* friendsButton = ((void* (__fastcall*)(void*, void*, const char*))CSF_AT(void**, buttonPanel, 0)[63])(
        buttonPanel, nullptr, CSF_STR(0x00841DDC, "BotonFriends"));
    self->m_friendsButton = MenuMpOnline_PointerToField(friendsButton);
    if (friendsButton == nullptr)
        return 0;

    // Button slot 42 (+0xA8) is called with 1; its return value is the function result.
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, friendsButton, 0)[42])(friendsButton, nullptr, 1);
}

HOOK(0x0055EBA0, CMenuMpOnline_InitMainPanelAndFriendsButton, "auto");
