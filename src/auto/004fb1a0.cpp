#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004faea0_OpenMenu, 0x004FAEA0, (void* self, void* edx, int a2, wchar_t* Source));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));

static char __fastcall S_004faea0_PopulateChapterMenu(
    S_004faea0* self, void* /*edx*/, int /*unused*/)
{
    wchar_t* title = reinterpret_cast<wchar_t*>(
        Text_ConvertToDisplayEncoding(
            reinterpret_cast<int>(CSF_STR(0x0083F2C0, "CH01")), 16));
    S_004faea0_OpenMenu(self, nullptr, 2, title);

    for (int offset = 0; offset < 936; offset += 312)
    {
        unsigned int key = CSF_AT(
            unsigned int,
            reinterpret_cast<uint8_t*>(self->field_0148) + offset,
            0x130);
        wchar_t* entry = reinterpret_cast<wchar_t*>(
            CoreParser_ResolveEntryByKey(key, 16));
        S_004faea0_OpenMenu(self, nullptr, 0, entry);
    }

    title = reinterpret_cast<wchar_t*>(
        Text_ConvertToDisplayEncoding(
            reinterpret_cast<int>(CSF_STR(0x0083F2B8, "CH02")), 16));
    S_004faea0_OpenMenu(self, nullptr, 2, title);

    title = reinterpret_cast<wchar_t*>(
        Text_ConvertToDisplayEncoding(
            reinterpret_cast<int>(CSF_STR(0x0083F2B0, "CH03")), 16));
    S_004faea0_OpenMenu(self, nullptr, 0, title);

    title = reinterpret_cast<wchar_t*>(
        Text_ConvertToDisplayEncoding(
            reinterpret_cast<int>(CSF_STR(0x0083F2A8, "CH04")), 16));
    return S_004faea0_OpenMenu(self, nullptr, 0, title);
}

HOOK(0x004FB1A0, S_004faea0_PopulateChapterMenu, "auto");
