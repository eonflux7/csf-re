#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(char, __fastcall, S_004f41e0_ApplyContext, 0x004F41E0, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(const char*, g_pszKey_CONTEXTO_GLOBAL, 0x008998E0);
CSF_GLOBAL(const char*, g_pszKey_MARCO, 0x008998E4);
CSF_GLOBAL(const char*, g_pszKey_POS_8998E8, 0x008998E8);
CSF_GLOBAL(const char*, g_pszKey_SIZE_8998EC, 0x008998EC);

static char __fastcall S_004f41e0_LoadGlobalContext(
    S_004f41e0* self, void* /*edx*/, S_00692b80* node)
{
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0035 = 0;

    auto* context = FindProperty(
        reinterpret_cast<std::uint8_t*>(node), nullptr,
        const_cast<char*>(g_pszKey_CONTEXTO_GLOBAL));
    if (!context) {
        self->field_0035 = 0;
        self->field_0034 = 0;
        return 0;
    }

    self->field_0035 = 1;
    if (FindProperty(context, nullptr, const_cast<char*>(g_pszKey_MARCO))) {
        alignas(8) std::uint8_t textBytes[0x100] = {};
        char* text = reinterpret_cast<char*>(textBytes);
        MenuExtras_ReadText(context, nullptr,
                           const_cast<char*>(g_pszKey_MARCO), text, 256);
        self->field_0034 = (__stricmp(text, CSF_STR(0x0083F12C, "ON")) == 0);
    }

    alignas(8) std::uint8_t positionBytes[0x8];
    alignas(8) std::uint8_t sizeBytes[0x8];
    int* position = reinterpret_cast<int*>(positionBytes);
    int* size = reinterpret_cast<int*>(sizeBytes);

    auto* positionNode = LookupNamedElement(context, nullptr, g_pszKey_POS_8998E8);
    const auto xComponent = GetComponent(positionNode, nullptr, 0);
    const auto yComponent = GetComponent(positionNode, nullptr, 1u);
    const int y = ParseIntegerComponent(yComponent);
    position[0] = ParseIntegerComponent(xComponent);
    position[1] = y;

    auto* sizeNode = LookupNamedElement(context, nullptr, g_pszKey_SIZE_8998EC);
    const auto widthComponent = GetComponent(sizeNode, nullptr, 0);
    const auto heightComponent = GetComponent(sizeNode, nullptr, 1u);
    const int height = ParseIntegerComponent(heightComponent);
    size[0] = ParseIntegerComponent(widthComponent);
    size[1] = height;

    const std::uint32_t right =
        static_cast<std::uint32_t>(position[0]) +
        static_cast<std::uint32_t>(size[0]);
    if (position[0] > static_cast<std::int32_t>(right)) {
        self->field_0034 = 0;
        return static_cast<char>(right);
    }

    const std::uint32_t bottom =
        static_cast<std::uint32_t>(position[1]) +
        static_cast<std::uint32_t>(height);
    if (position[1] > static_cast<std::int32_t>(bottom)) {
        self->field_0034 = 0;
        return static_cast<char>(position[1]);
    }

    ScaleHudSizePair(position);
    ScaleHudSizePair(size);

    const std::uint32_t scaledX = position[0];
    const std::uint32_t scaledY = position[1];
    const std::uint32_t scaledWidth = size[0];
    const std::uint32_t scaledHeight = size[1];
    self->field_0024 = scaledX;
    self->field_0028 = scaledY;
    self->field_002c = scaledX + scaledWidth;
    self->field_0030 = scaledY + scaledHeight;

    // The packet also lists the called function's address as a vtable reference.
    (void)CSF_VTBL(0x004F41E0);
    return S_004f41e0_ApplyContext(self, nullptr);
}

HOOK(0x004F4370, S_004f41e0_LoadGlobalContext, "auto");
