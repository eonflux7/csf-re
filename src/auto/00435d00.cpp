#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_ResolveComponent, 0x0042D750, (void* a1, int a2));
CSF_FUNC(std::uint32_t*, __cdecl, CopyTwoDwordArrayWithContext, 0x0042FD90, (std::uint32_t* destination, std::uint32_t count, std::uint32_t* source, void* context));
CSF_FUNC(unsigned int, __fastcall, Hud_InsertComponentRecord, 0x00434190, (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_ID_hud, 0x008995A8);

static const auto g_MenuExtrasVtableReference = CSF_VTBL(0x00692B80);

static void __fastcall Hud_AppendChildComponents(
    void* self, void* /*edx*/, void* node)
{
    if (!node)
        return;

    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    if (!childCount)
        return;

    auto* records = static_cast<std::uint8_t*>(self) + 0x9C;
    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(static_cast<unsigned char*>(node), nullptr, index));

        std::uint32_t record[2];
        record[1] = static_cast<std::uint32_t>(-1);
        record[0] = static_cast<std::uint32_t>(
            Hud_ResolveComponent(component, 4));
        if (component)
        {
            record[1] = static_cast<std::uint32_t>(
                ReadIntegerProperty(
                    component, nullptr, const_cast<char*>(g_pszKey_ID_hud)));
        }

        const std::uint32_t begin = CSF_AT(std::uint32_t, records, 0x04);
        const std::uint32_t size = begin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      CSF_AT(std::uint32_t, records, 0x08) - begin) >> 3)
            : 0;

        if (begin &&
            size < static_cast<std::uint32_t>(
                       static_cast<std::int32_t>(
                           CSF_AT(std::uint32_t, records, 0x0C) - begin) >> 3))
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, records, 0x08);
            CopyTwoDwordArrayWithContext(
                reinterpret_cast<std::uint32_t*>(end), 1, record, records);
            CSF_AT(std::uint32_t, records, 0x08) = end + 8;
        }
        else
        {
            Hud_InsertComponentRecord(
                records, nullptr,
                static_cast<int>(CSF_AT(std::uint32_t, records, 0x08)),
                1, record);
        }
    }
}

HOOK(0x00435D00, Hud_AppendChildComponents, "auto");
