#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, MenuControls_ReadVertexBuffer2DValues, 0x00439950, (float* a1, void* a2));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_VERTEX_BUF2D, 0x00899670);

static char __fastcall MenuControls_LoadVertexBuffer2D(
    void* self, void* /*edx*/, S_00692b80* settings)
{
    if (!settings)
        return 0;

    alignas(8) std::uint8_t textBytes[0x100];
    alignas(8) std::uint8_t valueBytes[0x10];
    char* text = reinterpret_cast<char*>(textBytes);

    MenuExtras_ReadText(
        reinterpret_cast<std::uint8_t*>(settings), nullptr,
        const_cast<char*>(g_pszKey_VERTEX_BUF2D), text, 256);

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* controls = MenuControls_ResolveObject(globalObject, nullptr);

    void* vertexBuffer =
        reinterpret_cast<void* (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, controls, 0)[0x5C / 4])(
                controls, nullptr, text);

    CSF_AT(void*, self, 0x30) = vertexBuffer;

    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, vertexBuffer, 0)[0x2C / 4])(
            vertexBuffer, nullptr, 162, 0);

    vertexBuffer = CSF_AT(void*, self, 0x30);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, vertexBuffer, 0)[0x20 / 4])(
            vertexBuffer, nullptr, 1);

    float* values = MenuControls_ReadVertexBuffer2DValues(
        reinterpret_cast<float*>(valueBytes), settings);

    CSF_AT(std::uint32_t, self, 0x18) = CSF_AT(std::uint32_t, values, 0x00);
    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, values, 0x04);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, values, 0x08);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, values, 0x0C);

    return 1;
}

HOOK(0x00439EE0, MenuControls_LoadVertexBuffer2D, "auto");
