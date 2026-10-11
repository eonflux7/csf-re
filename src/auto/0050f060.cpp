#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(char, __fastcall, MenuControls_SetDisplayString, 0x006AF940, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MenuControls_SetDisplayValue, 0x006AFB10, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static int __cdecl GameBddLoader_ApplyConfiguredDisplayString()
{
    auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    int result = CompareStringSubstring(
        settings + 0x1D14, nullptr, 0,
        CSF_AT(unsigned int, settings, 0x1D28),
        reinterpret_cast<const char*>(&g_byte_82E65C), 0);
    if (!result)
        return result;

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(unsigned int, reinterpret_cast<void*>(result), 0x1D34) == 0)
        return result;

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(unsigned int, reinterpret_cast<void*>(result), 0x1D38) == 0)
        return result;

    alignas(8) std::uint8_t stringBytes[0x1C];
    alignas(8) std::uint8_t sizeBytes[0xC];

    CSF_AT(std::uint32_t, stringBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(std::uint8_t, stringBytes, 0x4) = 0;
    CSF_AT(int, sizeBytes, 0x0) = 0;
    CSF_AT(int, sizeBytes, 0x4) = 0;
    CSF_AT(int, sizeBytes, 0x8) = 0;

    void* firstSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int secondWidth = CSF_AT(int, secondSettings, 0x1D34);
    const int firstWidth = CSF_AT(int, firstSettings, 0x1D34);
    CSF_AT(int, sizeBytes, 0x0) = secondWidth;
    CSF_AT(int, sizeBytes, 0x4) = firstWidth;

    settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    CSF_AT(int, sizeBytes, 0x8) = CSF_AT(int, settings, 0x1D38);

    settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    AssignStringSubstring(stringBytes, nullptr, settings + 0x1D14, 0, -1);
    ScaleHudSizePair(reinterpret_cast<int*>(sizeBytes));

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* display = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controls, 0)[0x68 / 4])(controls, nullptr);

    MenuControls_SetDisplayString(
        display, nullptr, reinterpret_cast<int>(stringBytes));

    settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    MenuControls_SetDisplayValue(
        display, nullptr, CSF_AT(unsigned int, settings, 0x1D3C));

    return CMenuSaveGame_ResetStringStorage(stringBytes, nullptr);
}

HOOK(0x0050F060, GameBddLoader_ApplyConfiguredDisplayString, "auto");

