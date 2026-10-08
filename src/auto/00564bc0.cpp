#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Globals referenced by the original function.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

// Callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (int managerIndex, void* /*edx*/));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* pool, void* /*edx*/, const char* name));

static void* AsPointer(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static uint32_t AsHandle(const void* pointer)
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(pointer));
}

// Interprets a 32-bit memory value as unsigned (the original uses fild + bias when negative).
static double UnsignedBitsToDouble(uint32_t raw)
{
    const int32_t signedValue = static_cast<int32_t>(raw);
    double value = static_cast<double>(signedValue);
    if (signedValue < 0) {
        value += static_cast<double>(g_UnsignedIntegerBias);
    }
    return value;
}

static float UnsignedRatioToFloat(uint32_t numerator, uint32_t denominator)
{
    return static_cast<float>(UnsignedBitsToDouble(numerator) / UnsignedBitsToDouble(denominator));
}

// Calls vtable slot 6 (offset 0x18) on an object with no arguments and returns its pointer result.
static void* CallNoArgGetterSlot6(void* object)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[6])(object, nullptr);
}

// Resolves ".PANEL_PRINCIPAL" in the owner's container, caches it, sizes it from screen metrics,
// and caches the "ListaCuentas" child handle in field_0014.
static int __fastcall CMenuAccounts_ResolveListaCuentasPanel(CMenuAccounts* self, void* /*edx*/, void* ownerObject, void* keyPoolRef)
{
    // Already resolved: the cached panel handle is the result.
    if (self->field_003c != 0) {
        return static_cast<int>(self->field_003c);
    }

    // Owner exposes its container through vtable slot 21 (offset 0x54), called with the owner as this.
    void* container = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, ownerObject, 0)[21])(ownerObject, nullptr);

    // The original loads the string pool from *keyPoolRef before reading the container's vtable.
    void* stringPool = CSF_AT(void*, keyPoolRef, 0);
    void** containerVtable = CSF_AT(void**, container, 0);

    uint8_t* panelKeyHandle = LookupNamedElement(reinterpret_cast<uint8_t*>(stringPool), nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    // Find the panel child through the container's vtable slot 6 (offset 0x18).
    void* panel = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(containerVtable[6])(container, nullptr, panelKeyHandle);
    self->field_003c = AsHandle(panel);
    if (panel == nullptr) {
        return 0;
    }

    // First metric: dword at offset 0 of the record returned by the first resolved manager.
    const int widthManagerIndex = GetGlobalInt();
    void* widthManager = LookupObjectById(widthManagerIndex, nullptr);
    void* widthRecord = CallNoArgGetterSlot6(widthManager);
    const uint32_t widthMetric = CSF_AT(uint32_t, widthRecord, 0);

    // Second metric: dword at offset 4 of the record returned by the second resolved manager.
    const int heightManagerIndex = GetGlobalInt();
    void* heightManager = LookupObjectById(heightManagerIndex, nullptr);
    void* heightRecord = CallNoArgGetterSlot6(heightManager);
    const uint32_t heightMetric = CSF_AT(uint32_t, heightRecord, 4);

    // Reload the cached panel and read its vtable before the screen-size ratios are computed.
    void* panelObject = AsPointer(self->field_003c);
    void** panelVtable = CSF_AT(void**, panelObject, 0);

    const float heightRatio = UnsignedRatioToFloat(heightMetric, g_ScreenHeight);
    const float widthRatio = UnsignedRatioToFloat(widthMetric, g_ScreenWidth);

    // Vtable slot 84 (offset 0x150) takes the width ratio as the first float argument and the height ratio as the second.
    reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(panelVtable[84])(panelObject, nullptr, widthRatio, heightRatio);

    // Reload the cached panel again and look up "ListaCuentas" through vtable slot 63 (offset 0xFC).
    void* listPanel = AsPointer(self->field_003c);
    const int listHandle = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(CSF_AT(void**, listPanel, 0)[63])(listPanel, nullptr, CSF_STR(0x00845A98, "ListaCuentas"));
    self->field_0014 = static_cast<uint32_t>(listHandle);
    return listHandle;
}

HOOK(0x00564BC0, CMenuAccounts_ResolveListaCuentasPanel, "auto");
