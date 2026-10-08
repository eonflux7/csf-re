#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(char*, g_PositionKey, 0x008A85EC);
CSF_GLOBAL(char*, g_SizeKey, 0x008A85F0);
CSF_GLOBAL(char*, g_TextKey, 0x008A8600);
CSF_GLOBAL(char*, g_ModeKey, 0x008A872C);

CSF_FUNC(unsigned char*, __fastcall, FindNamedNode, 0x006932B0,
         (unsigned char* self, void* edx, char* key));
CSF_FUNC(std::uint32_t, __fastcall, GetIndexedNode, 0x00692BD0,
         (unsigned char* self, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadNodeInteger, 0x00692C40,
         (std::uint32_t node));
CSF_FUNC(int, __fastcall, ReadNamedInteger, 0x00693910,
         (unsigned char* self, void* edx, char* key));
CSF_FUNC(char, __fastcall, ReadNamedText, 0x006939C0,
         (unsigned char* self, void* edx, char* key, char* buffer, int capacity));

static void __stdcall ApplyConfiguredRectangle(unsigned char* configuration,
                                                int objectAddress)
{
    if (!configuration || !objectAddress)
        return;

    std::uint32_t left = 0;
    std::uint32_t top = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;

    unsigned char* position = FindNamedNode(configuration, nullptr, g_PositionKey);
    if (position)
    {
        const std::uint32_t first = GetIndexedNode(position, nullptr, 0);
        const std::uint32_t second = GetIndexedNode(position, nullptr, 1);
        top = static_cast<std::uint32_t>(ReadNodeInteger(second));
        left = static_cast<std::uint32_t>(ReadNodeInteger(first));
    }

    unsigned char* size = FindNamedNode(configuration, nullptr, g_SizeKey);
    if (size)
    {
        const std::uint32_t first = GetIndexedNode(size, nullptr, 0);
        const std::uint32_t second = GetIndexedNode(size, nullptr, 1);
        height = static_cast<std::uint32_t>(ReadNodeInteger(second));
        width = static_cast<std::uint32_t>(ReadNodeInteger(first));
    }

    char* textKey = g_TextKey;
    std::uint32_t rectangle[4];
    rectangle[0] = left;
    rectangle[2] = left + width;

    char text[256];
    std::memset(text, 0, sizeof(text));

    rectangle[3] = top + height;
    rectangle[1] = top;

    unsigned char* textNode = FindNamedNode(configuration, nullptr, textKey);
    if (textNode)
        ReadNamedText(textNode, nullptr, g_TextKey, text, 256);

    int mode = 2;
    if (FindNamedNode(configuration, nullptr, g_ModeKey))
        mode = ReadNamedInteger(configuration, nullptr, g_ModeKey);

    void* object = reinterpret_cast<void*>(objectAddress);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, char*)>(
        CSF_AT(void**, object, 0)[0x204 / 4])(
            object, nullptr, mode, text);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*, char*)>(
        CSF_AT(void**, object, 0)[0x208 / 4])(
            object, nullptr, rectangle, text);
}

HOOK(0x0071E960, ApplyConfiguredRectangle, "auto");
