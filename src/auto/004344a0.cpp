#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static uint32_t __fastcall Hud_UpdateResourceWidget(void* self, void* /*edx*/)
{
    auto* state = reinterpret_cast<uint8_t*>(GetValue8B0554());
    const uint8_t enabled = CSF_AT(uint8_t, state, 0x972);
    auto* resourceState = state + 0x54;
    uint32_t result = reinterpret_cast<uint32_t>(resourceState);

    if (!enabled)
    {
        const uint32_t widget = CSF_AT(uint32_t, self, 0xC0);
        const uint8_t mode = CSF_AT(uint8_t, self, 0x11D);
        CSF_AT(uint32_t, self, 0xBC) = widget;
        CSF_AT(uint8_t, self, 0x11C) = mode;
        return result;
    }

    const uint32_t widget = CSF_AT(uint32_t, self, 0xC4);
    const uint8_t mode = CSF_AT(uint8_t, self, 0x11E);
    CSF_AT(uint32_t, self, 0xBC) = widget;
    CSF_AT(uint8_t, self, 0x11C) = mode;

    const char* resourceName;
    if (CSF_AT(uint32_t, resourceState, 0xD58) < 0x10u)
        resourceName = reinterpret_cast<const char*>(resourceState + 0xD44);
    else
        resourceName = CSF_AT(const char*, resourceState, 0xD44);

    // The original keeps source at ebp-0x21C, destination at ebp-0x11C and the string header at ebp-0x1C, so the
    // strlen below runs past destination into the frame. One frame object pins that order.
    struct TemporaryString
    {
        C_0082d410 header;
        uint32_t remainingInlineStorage[2];
        uint32_t length;
        uint32_t capacity;
    };
    struct Frame
    {
        char source[256];
        char destination[256];
        TemporaryString text;
    } frame;
    char (&source)[256] = frame.source;
    char (&destination)[256] = frame.destination;
    std::strcpy(source, resourceName);
    Hud_ExtractResourcePath(source, reinterpret_cast<uint8_t*>(destination), 256);

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* resolver = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
        CSF_AT(void**, resolver, 0)[0x10 / 4])(resolver, nullptr, destination);

    NormalizeResourceName(source, destination, 256);

    frame.text.capacity = 15;
    frame.text.length = 0;
    reinterpret_cast<char*>(&frame.text.header.field_0004)[0] = 0;
    AssignStringRange_429B60(&frame.text.header, nullptr, destination,
                           static_cast<unsigned int>(std::strlen(destination)));

    void* activeWidget = CSF_AT(void*, self, 0xC4);
    result = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, C_0082d410*)>(
        CSF_AT(void**, activeWidget, 0)[0x1C / 4])(
            activeWidget, nullptr, &frame.text.header);

    if (frame.text.capacity >= 0x10u)
    {
        result = frame.text.header.field_0004;
        if (result)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x004344A0, Hud_UpdateResourceWidget, "auto");
