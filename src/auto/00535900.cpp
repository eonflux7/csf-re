#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalIntApply, 0x006B1660, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (void* self, void* edx, const char* text));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, const char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
// One stack argument (callers clean 4); the two pushed before it are 0x6B15D0's.
CSF_FUNC(void*, __cdecl, MenuPage_LookupByName, 0x00541580, (void* stringObject));
CSF_FUNC(char, __fastcall, MenuRuntime_SetPageByIndex, 0x006B15D0, (void* self, void* edx, int page, int a3, int a4));
CSF_FUNC(int, __fastcall, sub_535860, 0x00535860, (void* self, void* edx));

// Pages: 5 entries of 0x104 bytes at 0x899EB8, the name text first, the page option dword at +0x100 (0x899FB8).
static const char* PageName(uint32_t i) { return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x00899EB8) + 0x104 * i); }
static int PageOption(uint32_t i) { return *reinterpret_cast<const int*>(CSF_ADDR_DATA(0x00899FB8) + 0x104 * i); }

// GetGlobalState()->+0x10 vtable slot 0x70 (on = 0 / 1)
static void SetGlobalVideoState(int on)
{
    void* controller = CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
    (*reinterpret_cast<int(__fastcall***)(void*, void*, int)>(controller))[0x70 / 4](controller, nullptr, on);
}

static void SetPage(void* name, int a3, int scaleBits)
{
    void* page = MenuPage_LookupByName(name);
    MenuRuntime_SetPageByIndex(reinterpret_cast<void*>(GetGlobalInt()), nullptr, reinterpret_cast<int>(page), a3,
                               scaleBits);
}

// Plays the intro video pages: an override record at +0x24 shows its page once (then hands over to 0x535860);
// otherwise the five pages of the table in turn (+0x20 counts them), then once the page from the settings.
static void* __fastcall CMenuIntroVideo_RefreshState(void* self, void* /*edx*/)
{
    GlobalIntApply(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    SetGlobalVideoState(0);

    uint8_t* record = CSF_AT(uint8_t*, self, 0x24);
    const char* name = record ? CSF_AT(const char*, record, 0) : PageName(CSF_AT(uint32_t, self, 0x20));
    uint8_t str[0x1C];  // local MSVC 7.1 std::string: buffer at +4, size +0x14, capacity +0x18

    if (record) {
        if (CSF_AT(uint8_t, record, 8) || CSF_AT(uint8_t, record, 9)) {
            CSF_AT(uint8_t, record, 9) = 0;
            CSF_AT(uint8_t, CSF_AT(uint8_t*, self, 0x24), 8) = 1;
            return reinterpret_cast<void*>(sub_535860(self, nullptr));
        }
        float scale = 1.0f;
        void* view = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        void* cam = (*reinterpret_cast<void*(__fastcall***)(void*, void*)>(view))[0x70 / 4](view, nullptr);
        if (cam)
            scale = (*reinterpret_cast<float(__fastcall***)(void*, void*)>(cam))[0x20 / 4](cam, nullptr);
        C_0082d410_ConstructFromText(str, nullptr, name);
        int scaleBits;
        std::memcpy(&scaleBits, &scale, 4);
        SetPage(str, CSF_AT(int, CSF_AT(uint8_t*, self, 0x24), 4), scaleBits);
        CSF_AT(uint8_t, CSF_AT(uint8_t*, self, 0x24), 9) = 1;
        SetGlobalVideoState(1);
        CSF_AT(uint8_t, self, 0x29) = 1;
        return reinterpret_cast<void*>(CMenuSaveGame_ResetStringStorage(str, nullptr));
    }

    if (CSF_AT(uint32_t, self, 0x20) < 5) {
        CSF_AT(uint32_t, str, 0x18) = 15;
        CSF_AT(uint32_t, str, 0x14) = 0;
        CSF_AT(uint8_t, str, 0x04) = 0;
        AssignStringRange_429B60(str, nullptr, name, static_cast<unsigned int>(std::strlen(name)));
        SetPage(str, PageOption(CSF_AT(uint32_t, self, 0x20)), 0x3F800000);
        ++CSF_AT(uint32_t, self, 0x20);
        SetGlobalVideoState(1);
        if (CSF_AT(uint32_t, str, 0x18) >= 0x10 && CSF_AT(void*, str, 4))
            return reinterpret_cast<void*>(Mem_FreeDebug(CSF_AT(void*, str, 4)));
        return nullptr;
    }

    if (!CSF_AT(uint8_t, self, 0x28))
        return reinterpret_cast<void*>(sub_535860(self, nullptr));
    CSF_AT(uint8_t, self, 0x28) = 0;
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(uint32_t, str, 0x18) = 15;
    CSF_AT(uint32_t, str, 0x14) = 0;
    CSF_AT(uint8_t, str, 0x04) = 0;
    AssignStringSubstring(str, nullptr, static_cast<uint8_t*>(settings) + 0x21B8, 0, 0xFFFFFFFFu);
    SetPage(str, 1, 0x3F800000);
    SetGlobalVideoState(1);
    CSF_AT(uint8_t, self, 0x29) = 1;
    return reinterpret_cast<void*>(CMenuSaveGame_ResetStringStorage(str, nullptr));
}

HOOK(0x00535900, CMenuIntroVideo_RefreshState, "auto");
