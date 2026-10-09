#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __cdecl, sub_42D750, 0x0042D750, (void* a1, int a2));
CSF_FUNC(void*, __fastcall, sub_42E280, 0x0042E280, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_42E950, 0x0042E950, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, sub_42FB70, 0x0042FB70, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Hud_CreateMinimapRenderer, 0x0043B450, (int type));
CSF_FUNC(char*, __fastcall, sub_45A7D0, 0x0045A7D0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszKey_VIDA, 0x00899578);
CSF_GLOBAL(const char*, g_pszKey_CANSANCIO, 0x0089957C);
CSF_GLOBAL(const char*, g_pszKey_POSTURA, 0x00899580);
CSF_GLOBAL(const char*, g_pszKey_MARCADORES, 0x00899584);
CSF_GLOBAL(const char*, g_pszKey_BOTIQUIN, 0x00899588);
CSF_GLOBAL(const char*, g_pszKey_ARMAS, 0x0089958C);
CSF_GLOBAL(const char*, g_pszKey_ARMAS_INTERCAMBIO, 0x00899590);
CSF_GLOBAL(const char*, g_pszKey_ITEMS, 0x00899594);
CSF_GLOBAL(const char*, g_pszKey_NUMEROS, 0x00899598);
CSF_GLOBAL(const char*, g_pszKey_NUMEROS_RED, 0x0089959C);
CSF_GLOBAL(const char*, g_pszKey_FONDOS, 0x008995A0);
CSF_GLOBAL(const char*, g_pszKey_ID_hud, 0x008995A8);
CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_WIDTH, 0x008995B4);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT, 0x008995B8);
CSF_GLOBAL(const char*, g_pszKey_SPRITES_MINIMAPA, 0x008995D0);
CSF_GLOBAL(const char*, g_pszKey_DELTA_X, 0x00899600);
CSF_GLOBAL(const char*, g_pszKey_DELTA_Y, 0x00899604);
CSF_GLOBAL(const char*, g_pszKey_MAX_VALUE, 0x00899608);

// The packet also identifies these call-target addresses as vtable aliases.
static const auto g_MenuNodeVtableAlias = CSF_VTBL(0x00692B80);
static const auto g_ButtonArrayVtableAlias = CSF_VTBL(0x0041E5F0);

static std::uint8_t* Hud_FindSection(std::uint8_t* root, const char* key)
{
    return FindProperty(root, nullptr, const_cast<char*>(key));
}

static std::uint8_t* Hud_GetChild(std::uint8_t* node, std::uint32_t index)
{
    return reinterpret_cast<std::uint8_t*>(GetComponent(node, nullptr, index));
}

static int Hud_ReadInteger(std::uint8_t* node, const char* key)
{
    return ReadIntegerProperty(node, nullptr, const_cast<char*>(key));
}

static void* Hud_CreateWidget(std::uint8_t* node)
{
    return reinterpret_cast<void*>(sub_42D750(node, 0));
}

static void Hud_ReallocatePointers(void* array, std::uint32_t growth,
                                   std::uint32_t bytes)
{
    void* storage = Mem_Alloc(bytes);
    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
    if (count)
        std::memcpy(storage, CSF_AT(void*, array, 0x00), 4 * count);

    void* oldStorage = CSF_AT(void*, array, 0x00);
    CSF_AT(std::uint32_t, array, 0x04) += growth;
    if (oldStorage)
    {
        Mem_Free(oldStorage);
        CSF_AT(void*, array, 0x00) = nullptr;
    }
    CSF_AT(void*, array, 0x00) = storage;
}

static void Hud_AppendPointer(void* array, void* widget, bool useGrowButtonArray)
{
    const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x04);
    if (CSF_AT(std::uint32_t, array, 0x08) >= capacity)
    {
        const std::uint32_t growth = CSF_AT(std::uint32_t, array, 0x0C);
        if (useGrowButtonArray)
            GrowButtonArray(array, nullptr, static_cast<int>(growth));
        else
            Hud_ReallocatePointers(array, growth, 4 * (capacity + growth));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
    void** destination = CSF_AT(void**, array, 0x00) + count;
    CSF_AT(std::uint32_t, array, 0x08) = count + 1;
    *destination = widget;
}

static void Hud_AppendIdentifiedWidget(void* array, int id, void* widget)
{
    if (CSF_AT(std::uint32_t, array, 0x08) >=
        CSF_AT(std::uint32_t, array, 0x04))
        sub_42E280(array, nullptr, CSF_AT(int, array, 0x0C));

    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
    auto* destination = static_cast<std::uint8_t*>(CSF_AT(void*, array, 0x00))
                      + 8 * count;
    CSF_AT(std::uint32_t, array, 0x08) = count + 1;
    CSF_AT(int, destination, 0x00) = id;
    CSF_AT(void*, destination, 0x04) = widget;
}

static void* Hud_GetArrayWidget(void* array, std::uint32_t index)
{
    return CSF_AT(void**, array, 0x00)[index];
}

static int* Hud_GetWidgetExtent(void* widget)
{
    return reinterpret_cast<int* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0x00)[0x3C / 4])(widget, nullptr);
}

static void Hud_GetWidgetBounds(void* widget, int* bounds)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, widget, 0x00)[0x58 / 4])(widget, nullptr, bounds);
}

static int* Hud_GetWidgetPosition(void* widget)
{
    return reinterpret_cast<int* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0x00)[0x2C / 4])(widget, nullptr);
}

static int Hud_LoadMinimapSprites(void* renderer, std::uint8_t* property)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*, std::uint8_t*)>(
        CSF_AT(void**, renderer, 0x00)[0x14 / 4])(renderer, nullptr, property);
}

static bool Hud_HasExpectedChildren(std::uint8_t* section, std::uint32_t expected)
{
    if (!section)
        return false;
    if (MenuExtras_GetChildCount(section, nullptr) == expected)
        return true;
    MenuExtras_GetChildCount(section, nullptr);
    return false;
}

static void Hud_LoadNumberWidgets(void* array, std::uint8_t* section,
                                 std::uint8_t* enabled)
{
    if (!Hud_HasExpectedChildren(section, 10))
    {
        *enabled = 0;
        return;
    }

    const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x04);
    if (capacity < 0xA)
        Hud_ReallocatePointers(array, 10 - capacity, 40);

    std::uint32_t index = 0;
    while (true)
    {
        void* widget = Hud_CreateWidget(Hud_GetChild(section, index));
        Hud_AppendPointer(array, widget, false);
        if (!widget)
            break;
        if (++index >= 0xA)
            break;
    }
    if (index >= 0xA)
        *enabled = 1;
}

static void Hud_LoadIdentifiedWidgetsWithBase(std::uint8_t* self,
                                             std::uint8_t* section,
                                             std::size_t arrayOffset,
                                             std::size_t baseOffset,
                                             std::size_t flagOffset)
{
    if (!section || !MenuExtras_GetChildCount(section, nullptr))
    {
        CSF_AT(std::uint8_t, self, flagOffset) = 0;
        return;
    }

    CSF_AT(void*, self, baseOffset) = Hud_CreateWidget(Hud_GetChild(section, 0));
    std::uint32_t index = 1;
    if (MenuExtras_GetChildCount(section, nullptr) > 1)
    {
        do
        {
            std::uint8_t* child = Hud_GetChild(section, index);
            void* widget = Hud_CreateWidget(child);
            if (!widget)
                break;
            const int id = Hud_ReadInteger(child, g_pszKey_ID_hud);
            Hud_AppendIdentifiedWidget(self + arrayOffset, id, widget);
            ++index;
        }
        while (index < MenuExtras_GetChildCount(section, nullptr));
    }
    if (index >= MenuExtras_GetChildCount(section, nullptr))
        CSF_AT(std::uint8_t, self, flagOffset) = 1;
}

static void __fastcall Hud_LoadDefinition(void* object, void* /*edx*/,
                                          S_00692b80* definition)
{
    auto* self = static_cast<std::uint8_t*>(object);
    auto* root = reinterpret_cast<std::uint8_t*>(definition);
    if (!root)
    {
        CSF_AT(std::uint8_t, self, 0x118) = 0;
        CSF_AT(std::uint8_t, self, 0x119) = 0;
        CSF_AT(std::uint8_t, self, 0x11A) = 0;
        CSF_AT(std::uint8_t, self, 0x11B) = 0;
        CSF_AT(std::uint8_t, self, 0x11C) = 0;
        CSF_AT(std::uint8_t, self, 0x120) = 0;
        CSF_AT(std::uint8_t, self, 0x121) = 0;
        CSF_AT(std::uint8_t, self, 0x122) = 0;
        CSF_AT(std::uint8_t, self, 0x124) = 0;
        CSF_AT(std::uint8_t, self, 0x125) = 0;
        return;
    }

    CSF_AT(void*, self, 0xC4) = Hud_CreateMinimapRenderer(1);
    CSF_AT(void*, self, 0xC0) = Hud_CreateMinimapRenderer(0);

    std::uint8_t* section = Hud_FindSection(root, g_pszKey_SPRITES_MINIMAPA);
    if (section)
    {
        void* storage = Mem_Alloc(56);
        char* sprites = storage
            ? sub_45A7D0(storage, nullptr, reinterpret_cast<int>(section))
            : nullptr;
        void* renderer = CSF_AT(void*, self, 0xC0);
        CSF_AT(void*, self, 0x248) = sprites;
        const int firstResult = Hud_LoadMinimapSprites(renderer, section);
        void* sharedSprites = CSF_AT(void*, self, 0x248);
        CSF_AT(std::uint8_t, self, 0x11D) = static_cast<std::uint8_t>(firstResult);
        CSF_AT(void*, CSF_AT(void*, self, 0xC0), 0x04) = sharedSprites;

        const int secondResult =
            Hud_LoadMinimapSprites(CSF_AT(void*, self, 0xC4), section);
        sharedSprites = CSF_AT(void*, self, 0x248);
        CSF_AT(std::uint8_t, self, 0x11E) = static_cast<std::uint8_t>(secondResult);
        CSF_AT(void*, CSF_AT(void*, self, 0xC4), 0x04) = sharedSprites;
    }

    // These locals intentionally retain the original's indeterminate state
    // when FONDOS is absent and no subsequent valid section initializes index.
    std::uint32_t backgroundCount;
    std::uint32_t index;
    section = Hud_FindSection(root, g_pszKey_FONDOS);
    if (section)
    {
        backgroundCount = MenuExtras_GetChildCount(section, nullptr);
        const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0xB0);
        if (backgroundCount > capacity)
            GrowButtonArray(self + 0xAC, nullptr,
                            static_cast<int>(backgroundCount - capacity));
        index = 0;
        while (index < backgroundCount)
        {
            void* widget = Hud_CreateWidget(Hud_GetChild(section, index));
            Hud_AppendPointer(self + 0xAC, widget, true);
            if (!widget)
            {
                CSF_AT(std::uint8_t, self, 0x126) = 0;
                break;
            }
            ++index;
        }
        CSF_AT(std::uint8_t, self, 0x126) = index >= backgroundCount;
    }
    else
    {
        index = backgroundCount;
        CSF_AT(std::uint8_t, self, 0x126) = 0;
    }

    section = Hud_FindSection(root, g_pszKey_VIDA);
    if (!Hud_HasExpectedChildren(section, 3))
        CSF_AT(std::uint8_t, self, 0x118) = 0;
    else
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x08);
        if (capacity < 3)
            GrowButtonArray(self + 0x04, nullptr, static_cast<int>(3 - capacity));
        index = 0;
        while (true)
        {
            void* widget = Hud_CreateWidget(Hud_GetChild(section, index));
            Hud_AppendPointer(self + 0x04, widget, true);
            if (!widget)
                break;
            if (++index >= 3)
                break;
        }
        if (index >= 3)
        {
            int* extent = Hud_GetWidgetExtent(Hud_GetArrayWidget(self + 0x04, 1));
            CSF_AT(int, self, 0x130) = extent[0];
            int bounds[3];
            Hud_GetWidgetBounds(Hud_GetArrayWidget(self + 0x04, 1), bounds);
            CSF_AT(int, self, 0x134) = bounds[2] - bounds[0];
            int* position = Hud_GetWidgetPosition(Hud_GetArrayWidget(self + 0x04, 1));
            (void)position[1];
            const int x = position[0];
            const int width = CSF_AT(int, self, 0x134);
            sub_42E950(self, nullptr, x + CSF_AT(int, self, 0x130), 0,
                       bounds[0] + width, bounds[0] + width);
            CSF_AT(std::uint8_t, self, 0x118) = 1;
        }
    }

    section = Hud_FindSection(root, g_pszKey_CANSANCIO);
    if (!Hud_HasExpectedChildren(section, 1))
        CSF_AT(std::uint8_t, self, 0x119) = 0;
    else
    {
        if (!CSF_AT(std::uint32_t, self, 0x18))
            GrowButtonArray(self + 0x14, nullptr, 1);
        index = 0;
        void* widget = Hud_CreateWidget(Hud_GetChild(section, index));
        Hud_AppendPointer(self + 0x14, widget, false);
        if (widget)
            ++index;
        if (index)
        {
            int* extent = Hud_GetWidgetExtent(Hud_GetArrayWidget(self + 0x14, 0));
            CSF_AT(int, self, 0x138) = extent[0];
            int bounds[3];
            Hud_GetWidgetBounds(Hud_GetArrayWidget(self + 0x14, 0), bounds);
            CSF_AT(int, self, 0x13C) = bounds[2] - bounds[0];
            CSF_AT(std::uint8_t, self, 0x119) = 1;
        }
    }

    section = Hud_FindSection(root, g_pszKey_POSTURA);
    if (!Hud_HasExpectedChildren(section, 2))
        CSF_AT(std::uint8_t, self, 0x11A) = 0;
    else
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x28);
        if (capacity < 2)
            GrowButtonArray(self + 0x24, nullptr, static_cast<int>(2 - capacity));
        index = 0;
        while (true)
        {
            void* widget = Hud_CreateWidget(Hud_GetChild(section, index));
            Hud_AppendPointer(self + 0x24, widget, false);
            if (!widget)
                break;
            if (++index >= 2)
                break;
        }
        if (index >= 2)
            CSF_AT(std::uint8_t, self, 0x11A) = 1;
    }

    CSF_AT(std::uint8_t, self, 0x23C) =
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x18BC);

    section = Hud_FindSection(root, g_pszKey_MARCADORES);
    if (!Hud_HasExpectedChildren(section, 9))
        CSF_AT(std::uint8_t, self, 0x11B) = 0;
    else
    {
        std::uint8_t* header = Hud_GetChild(section, 0);
        if (header)
        {
            CSF_AT(int, self, 0x94) = Hud_ReadInteger(header, g_pszKey_DELTA_X);
            CSF_AT(int, self, 0x98) = Hud_ReadInteger(header, g_pszKey_DELTA_Y);
            for (std::uint32_t marker = 0; marker < 8; ++marker)
            {
                int left = 0, top = 0, right = 0, bottom = 0;
                std::uint8_t* child = Hud_GetChild(section, marker + 1);
                if (child)
                {
                    top = Hud_ReadInteger(child, g_pszKey_TOP);
                    left = Hud_ReadInteger(child, g_pszKey_LEFT);
                    right = left + Hud_ReadInteger(child, g_pszKey_WIDTH);
                    bottom = top + Hud_ReadInteger(child, g_pszKey_HEIGHT);
                }
                const int maximum = Hud_ReadInteger(child, g_pszKey_MAX_VALUE);
                if (CSF_AT(std::uint32_t, self, 0x3C) >=
                    CSF_AT(std::uint32_t, self, 0x38))
                    sub_42FB70(self + 0x34, nullptr, CSF_AT(int, self, 0x40));
                const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x3C);
                auto* destination =
                    static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x34)) + 20 * count;
                CSF_AT(std::uint32_t, self, 0x3C) = count + 1;
                CSF_AT(int, destination, 0x00) = left;
                CSF_AT(int, destination, 0x04) = top;
                CSF_AT(int, destination, 0x08) = right;
                CSF_AT(int, destination, 0x0C) = bottom;
                CSF_AT(int, destination, 0x10) = maximum;
            }
            CSF_AT(std::uint8_t, self, 0x11B) = 1;
        }
        else if (index >= 8)
            CSF_AT(std::uint8_t, self, 0x11B) = 1;
    }

    section = Hud_FindSection(root, g_pszKey_ARMAS);
    Hud_LoadIdentifiedWidgetsWithBase(self, section, 0xC8, 0xE8, 0x120);

    section = Hud_FindSection(root, g_pszKey_ARMAS_INTERCAMBIO);
    std::uint32_t exchangeCount;
    if (section && (exchangeCount = MenuExtras_GetChildCount(section, nullptr)) != 0)
    {
        std::uint32_t exchangeIndex = 0;
        do
        {
            std::uint8_t* child = Hud_GetChild(section, exchangeIndex);
            void* widget = Hud_CreateWidget(child);
            if (!widget)
                break;
            const int id = Hud_ReadInteger(child, g_pszKey_ID_hud);
            Hud_AppendIdentifiedWidget(self + 0xD8, id, widget);
            ++exchangeIndex;
        }
        while (exchangeIndex < exchangeCount);
        if (exchangeIndex >= MenuExtras_GetChildCount(section, nullptr))
            CSF_AT(std::uint8_t, self, 0x121) = 1;
    }
    else
        CSF_AT(std::uint8_t, self, 0x121) = 0;

    section = Hud_FindSection(root, g_pszKey_ITEMS);
    Hud_LoadIdentifiedWidgetsWithBase(self, section, 0x108, 0xEC, 0x122);

    section = Hud_FindSection(root, g_pszKey_BOTIQUIN);
    if (!Hud_HasExpectedChildren(section, 2))
        CSF_AT(std::uint8_t, self, 0x123) = 0;
    else
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x88);
        if (capacity < 2)
            Hud_ReallocatePointers(self + 0x84, 2 - capacity, 8);
        std::uint32_t medicalIndex = 0;
        while (true)
        {
            void* widget = Hud_CreateWidget(Hud_GetChild(section, medicalIndex));
            Hud_AppendPointer(self + 0x84, widget, false);
            if (!widget)
                break;
            if (++medicalIndex >= 2)
                break;
        }
        if (medicalIndex >= 2)
            CSF_AT(std::uint8_t, self, 0x123) = 1;
    }

    section = Hud_FindSection(root, g_pszKey_NUMEROS);
    Hud_LoadNumberWidgets(self + 0x44, section,
                          &CSF_AT(std::uint8_t, self, 0x124));
    section = Hud_FindSection(root, g_pszKey_NUMEROS_RED);
    Hud_LoadNumberWidgets(self + 0x54, section,
                          &CSF_AT(std::uint8_t, self, 0x125));
}

HOOK(0x00431F80, Hud_LoadDefinition, "auto");

