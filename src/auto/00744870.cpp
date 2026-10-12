#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_RefreshState(C_00858180* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00867B74, "Muere")));

    alignas(8) std::uint8_t commandHashBytes[4];
    CSF_AT(int, commandHashBytes, 0) =
        HashTemporaryString(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(commandHashBytes));
    return AddConfigEntry(self, nullptr, 1, entry);
}

HOOK(0x00744870, C_00858180_RefreshState, "auto");
