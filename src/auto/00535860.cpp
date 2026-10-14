#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalIntApply, 0x006B1660, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BAF98, 0x0069C370, ());
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);
CSF_GLOBAL(const char*, g_pszMenu_P_SalDem, 0x00899B98);

// CMenuIntroVideo: +0x10 menu manager, +0x14 rating shown, +0x18 rating panel, +0x24 intro movie,
// +0x29 playing, +0x2A skipped, +0x30 rating pending.
template <typename Fn>
static Fn Slot(void* obj, int offset)
{
    return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[offset / 4]);
}

// 0x5355A0 (not split by IDA; reached only by tail jumps): hand control back and open the main menu page.
static int CMenuIntroVideo_OpenMainMenu(void* self)
{
    void* manager = CSF_AT(void*, self, 0x10);
    Slot<int(__fastcall*)(void*, void*, void*, int)>(manager, 0x60)(manager, nullptr, self, 0);
    manager = CSF_AT(void*, self, 0x10);
    return Slot<int(__fastcall*)(void*, void*, const char*)>(manager, 0x58)(manager, nullptr, g_pszMenu_P_MePrin);
}

// 0x5356C0 (not split by IDA): after the intro, the demo exit page, the age-rating screen once, or the main menu.
static int CMenuIntroVideo_ShowNextPage(void* self)
{
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xD3))
    {
        void* manager = CSF_AT(void*, self, 0x10);
        return Slot<int(__fastcall*)(void*, void*, const char*)>(manager, 0x58)(manager, nullptr, g_pszMenu_P_SalDem);
    }
    if (!CSF_AT(uint8_t, self, 0x30) || !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0x113))
        return CMenuIntroVideo_OpenMainMenu(self);

    CSF_AT(uint8_t, self, 0x30) = 0;
    RwImageSetPath(CSF_STR(0x00841C64, "Menus/Textures/"));
    void* controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const int rating = GetValue_8BAF98();
    const char* path = rating == 4    ? CSF_STR(0x00841E2C, "Menus/Textures/ratingUSK.png")
                       : rating == 12 ? CSF_STR(0x00841E4C, "Menus/Textures/ratingESRB.png")
                                      : CSF_STR(0x00841E6C, "Menus/Textures/ratingPEGI.png");
    std::uint8_t* texture = reinterpret_cast<std::uint8_t*>(
        Slot<void*(__fastcall*)(void*, void*, const char*)>(controls, 0x40)(controls, nullptr, path));
    if (!texture)
        return CMenuIntroVideo_OpenMainMenu(self);

    void* panel = CSF_AT(void*, self, 0x18);
    Slot<void(__fastcall*)(void*, void*, void*)>(panel, 0xD8)(panel, nullptr, texture);
    const int result = ReleaseObjectReference(texture + 4, nullptr);
    CSF_AT(uint32_t, self, 0x14) = 1;
    return result;
}

// The intro was skipped or ended: reset the input state, stop the movie, clear the page name and leave
// for the next page once no movie remains (or the current one has finished).
static int __fastcall CMenuIntroVideo_FinishIntro(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x29))
        CSF_AT(uint8_t, self, 0x2A) = 1;
    GlobalIntApply(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* stateObject = CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
    // The original pushes 0 for slot 0x70, which takes no argument (0x42CDF0, plain ret); slot 0x90 (0x421270,
    // ret 4) then consumes that word as its argument.
    Slot<void(__fastcall*)(void*, void*)>(stateObject, 0x70)(stateObject, nullptr);
    void* game = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    Slot<void(__fastcall*)(void*, void*, int)>(game, 0x90)(game, nullptr, 0);

    AssignStringRange_429B60(CSF_AT(std::uint8_t*, self, 0x10) + 0x7C, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    void* pending = CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x68);
    if (pending)
        Slot<void(__fastcall*)(void*, void*)>(pending, 0x08)(pending, nullptr);

    void* manager = CSF_AT(void*, self, 0x10);
    Slot<void(__fastcall*)(void*, void*, int)>(manager, 0x9C)(manager, nullptr, 1);
    manager = CSF_AT(void*, self, 0x10);
    Slot<void(__fastcall*)(void*, void*)>(manager, 0xA8)(manager, nullptr);

    const std::uint32_t movie = CSF_AT(std::uint32_t, self, 0x24);
    if (movie && (CSF_AT(std::uint32_t, reinterpret_cast<void*>(movie), 0x0C) ||
                  !CSF_AT(std::uint8_t, reinterpret_cast<void*>(movie), 0x10)))
        return static_cast<int>(movie);
    return CMenuIntroVideo_ShowNextPage(self);
}

HOOK(0x00535860, CMenuIntroVideo_FinishIntro, "auto");
