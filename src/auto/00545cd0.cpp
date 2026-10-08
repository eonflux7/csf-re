#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GetGlobalIndexedObject, 0x00652160, (int globalIndex, void* edx));
CSF_FUNC(uint8_t*, __fastcall, UiNameTable_Lookup, 0x00693360, (void* self, void* edx, const char* name));

// Field words hold raw 32-bit values; these convert without changing bits.
static void* AsPointer(uint32_t raw)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(raw));
}

static uint32_t AsWord(void* p)
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p));
}

// Matches the original fild (signed load) followed by a conditional fadd of the unsigned bias.
static double UnsignedValueAsDouble(uint32_t raw)
{
    const int32_t signedValue = static_cast<int32_t>(raw);
    double value = static_cast<double>(signedValue);
    if (signedValue < 0)
        value += static_cast<double>(g_UnsignedIntegerBias);
    return value;
}

static int __fastcall CMenuExtras_LocatePanelAndInitThumbs(CMenuExtras* self, void* /*edx*/, void* ownerNode, void* nameTableRef)
{
    // Panel already located: return the cached panel without touching anything else.
    int result = static_cast<int>(self->field_0018);
    if (result != 0)
        return result;

    // ownerNode->vtbl[21]() yields the root container that holds the named panels.
    void* rootContainer = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, ownerNode, 0)[21])(ownerNode, nullptr);
    void* nameTable = CSF_AT(void*, nameTableRef, 0);
    void** rootVtable = CSF_AT(void**, rootContainer, 0);
    void* panelKey = UiNameTable_Lookup(nameTable, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));
    void* panel = ((void* (__fastcall*)(void*, void*, void*))rootVtable[6])(rootContainer, nullptr, panelKey);
    self->field_0018 = AsWord(panel);
    result = static_cast<int>(AsWord(panel));
    if (panel == nullptr)
        return result;

    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, panel, 0)[56])(panel, nullptr, 1);

    // Two layout components, each read from a freshly fetched global-indexed record (+0 and +4).
    void* layoutObjectX = GetGlobalIndexedObject(GetGlobalInt(), nullptr);
    void* pointRecordX = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, layoutObjectX, 0)[6])(layoutObjectX, nullptr);
    uint32_t pixelX = CSF_AT(uint32_t, pointRecordX, 0);

    void* layoutObjectY = GetGlobalIndexedObject(GetGlobalInt(), nullptr);
    void* pointRecordY = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, layoutObjectY, 0)[6])(layoutObjectY, nullptr);
    uint32_t pixelY = CSF_AT(uint32_t, pointRecordY, 4);

    // Normalize X by screen width and Y by screen height.
    const float normalizedX = static_cast<float>(UnsignedValueAsDouble(pixelX) / UnsignedValueAsDouble(g_ScreenWidth));
    const float normalizedY = static_cast<float>(UnsignedValueAsDouble(pixelY) / UnsignedValueAsDouble(g_ScreenHeight));

    void* panelForLayout = AsPointer(self->field_0018);
    ((void (__fastcall*)(void*, void*, float, float))CSF_AT(void**, panelForLayout, 0)[84])(panelForLayout, nullptr, normalizedX, normalizedY);

    // Child lookups on the panel; each result is stored in the object.
    void* panelForShot = AsPointer(self->field_0018);
    void* shotChild = ((void* (__fastcall*)(void*, void*, const char*))CSF_AT(void**, panelForShot, 0)[63])(panelForShot, nullptr, CSF_STR(0x00843BF8, "thumbShot"));
    self->field_002c = AsWord(shotChild);

    void* panelForDesc = AsPointer(self->field_0018);
    void* descChild = ((void* (__fastcall*)(void*, void*, const char*))CSF_AT(void**, panelForDesc, 0)[63])(panelForDesc, nullptr, CSF_STR(0x00843BEC, "thumbDesc"));
    self->field_0030 = AsWord(descChild);
    result = static_cast<int>(AsWord(descChild));

    void* shotNow = AsPointer(self->field_002c);
    if (shotNow != nullptr)
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, shotNow, 0)[42])(shotNow, nullptr, 1);

    void* descNow = AsPointer(self->field_0030);
    if (descNow != nullptr)
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, descNow, 0)[42])(descNow, nullptr, 1);

    return result;
}

HOOK(0x00545CD0, CMenuExtras_LocatePanelAndInitThumbs, "auto");
