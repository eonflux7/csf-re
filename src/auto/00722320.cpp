#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, AssignWideText, 0x00696F90,
         (void* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __fastcall, RefreshTextState, 0x007215F0,
         (void* self, void* edx, char flag));

static int* __fastcall SynchronizeAttachedObjectText(
    void* self, void* /*edx*/, std::int16_t* text)
{
    int* result = CSF_AT(int*, self, 0x134);
    if (!result || !text)
        return result;

    char temporary[8];

    void** attachedVtable = CSF_AT(void**, result, 0);
    int position =
        reinterpret_cast<int (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, self, 0)[88])(self, nullptr, temporary);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        attachedVtable[20])(
            CSF_AT(void*, self, 0x134), nullptr, position);

    attachedVtable = CSF_AT(
        void**, CSF_AT(void*, self, 0x134), 0);

    int state =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[41])(self, nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        attachedVtable[22])(
            CSF_AT(void*, self, 0x134), nullptr, state);

    AssignWideText(
        static_cast<std::uint8_t*>(self) + 0x150, nullptr, text);

    result = CSF_AT(int*, self, 0x164);
    if (!result)
        return result;

    std::uint32_t lastIndex =
        reinterpret_cast<std::uint32_t>(result) - 1u;
    CSF_AT(std::uint32_t, self, 0x1B0) = lastIndex;
    CSF_AT(std::uint32_t, self, 0x1A8) = lastIndex;

    return reinterpret_cast<int*>(
        RefreshTextState(self, nullptr, 0));
}

HOOK(0x00722320, SynchronizeAttachedObjectText, "auto");
