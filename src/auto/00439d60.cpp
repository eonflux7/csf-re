#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(float*, __cdecl, Hud_ReadQuadFromProperties, 0x00439950, (float* a1, void* a2));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_VERTEX_BUF2D, 0x00899670);

static char __fastcall S_00692b80_LoadHudVertexBuffer(
    S_00692b80* self, void* /*edx*/, S_00692b80* properties)
{
    if (!properties)
        return 0;

    auto* component = reinterpret_cast<S_00692b80*>(
        GetComponent(reinterpret_cast<unsigned char*>(properties), nullptr, 0));
    if (!component)
        return 0;

    char buffer[256]{};
    MenuExtras_ReadText(
        reinterpret_cast<std::uint8_t*>(component), nullptr,
        const_cast<char*>(g_pszKey_VERTEX_BUF2D), buffer, 256);

    int globalContext = GetGlobalInt();
    void* manager = MenuControls_ResolveObject(
        reinterpret_cast<void*>(globalContext), nullptr);
    void* vertexBuffer =
        reinterpret_cast<void* (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, buffer);

    CSF_AT(std::uint32_t, self, 0xD0) =
        reinterpret_cast<std::uint32_t>(vertexBuffer);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, vertexBuffer, 0)[0x20 / 4])(
            vertexBuffer, nullptr, 1);

    component = reinterpret_cast<S_00692b80*>(
        GetComponent(reinterpret_cast<unsigned char*>(properties), nullptr, 1));
    if (!component)
        return 0;

    alignas(8) std::uint8_t rectBytes[0x14]{};
    int* rect = reinterpret_cast<int*>(rectBytes);
    Hud_ReadRectFromProperties(component, rect);

    CSF_AT(int, self, 0xD4) = rect[2] - rect[0];
    CSF_AT(int, self, 0xD8) = rect[3] - rect[1];
    ScaleHudSizePair(&CSF_AT(int, self, 0xD4));

    alignas(8) std::uint8_t quadBytes[0x34]{};
    std::uint8_t* record = quadBytes + 0x10;
    CSF_AT(float, record, 0x10) = -1.0f;
    std::memset(record + 0x14, 0, 9);

    std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(self) + 0x50;
    for (unsigned int index = 2; index < 6; ++index)
    {
        void* quadProperties = reinterpret_cast<void*>(
            GetComponent(reinterpret_cast<unsigned char*>(properties), nullptr, index));
        float* quad = Hud_ReadQuadFromProperties(
            reinterpret_cast<float*>(quadBytes), quadProperties);

        std::uint32_t first = CSF_AT(std::uint32_t, quad, 0);
        std::uint32_t second = CSF_AT(std::uint32_t, quad, 4);
        CSF_AT(std::uint32_t, record, 0) = first;
        CSF_AT(std::uint32_t, record, 8) = CSF_AT(std::uint32_t, quad, 8);
        CSF_AT(std::uint32_t, record, 4) = second;
        CSF_AT(std::uint32_t, record, 0xC) = CSF_AT(std::uint32_t, quad, 0xC);

        std::memcpy(destination, record, 0x20);
        destination += 0x20;
    }

    vertexBuffer = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0xD0));
    CSF_AT(float, self, 0x60) = 0.0f;
    CSF_AT(float, self, 0x80) = 90.0f;
    CSF_AT(float, self, 0xA0) = 180.0f;
    CSF_AT(float, self, 0xC0) = 270.0f;

    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, vertexBuffer, 0)[0x2C / 4])(
            vertexBuffer, nullptr, 66, 0);

    vertexBuffer = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0xD0));
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, vertexBuffer, 0)[0x20 / 4])(
            vertexBuffer, nullptr, 1);
    return 1;
}

HOOK(0x00439D60, S_00692b80_LoadHudVertexBuffer, "auto");
