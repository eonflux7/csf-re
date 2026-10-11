#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

using CreateDirectoryWFunction = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWFunction, g_CreateDirectoryW, 0x0082D218);

static char __fastcall CProfileManager_SelectProfile(
    CProfileManager* self, void* /*edx*/, unsigned int profileIndex)
{
    const std::uint32_t profilesBegin = self->field_0008;
    if (!profilesBegin)
        return 0;

    std::uint32_t profileCount = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(self->field_000c - profilesBegin) / 80);
    if (profileIndex >= profileCount)
        return 0;

    void* application = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
    if (CSF_AT(std::uint8_t, application, 0xCC))
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* service = CSF_AT(void*, manager, 0x4);
        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, service, 0)[0x10 / 4])(service, nullptr);

        // String object: allocator prefix, inline wide buffer, size, capacity.
        alignas(8) std::uint8_t pathBytes[0x1C];
        char numberBuffer[8];

        CSF_AT(std::uint32_t, pathBytes, 0x18) = 7;
        CSF_AT(std::uint32_t, pathBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, pathBytes, 0x4) = 0;

        AssignStringRange_488830(
            pathBytes, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x30,
            0, 0xFFFFFFFF);

        const wchar_t* path = CSF_AT(std::uint32_t, pathBytes, 0x18) < 8
            ? reinterpret_cast<const wchar_t*>(pathBytes + 0x4)
            : CSF_AT(const wchar_t*, pathBytes, 0x4);
        const auto createDirectory = g_CreateDirectoryW;
        createDirectory(path, nullptr);

        AppendNarrowToWideString(
            pathBytes, nullptr,
            reinterpret_cast<std::uint8_t*>(
                const_cast<char*>(CSF_STR(0x00840888, "Games"))));

        void* profile = reinterpret_cast<void*>(
            self->vSlot1C(static_cast<int>(profileIndex)));
        _sprintf(numberBuffer, CSF_STR(0x0083090C, "%d"),
                 CSF_AT(int, profile, 0x48));

        AppendNarrowToWideString(
            pathBytes, nullptr, reinterpret_cast<std::uint8_t*>(numberBuffer));

        path = CSF_AT(std::uint32_t, pathBytes, 0x18) < 8
            ? reinterpret_cast<const wchar_t*>(pathBytes + 0x4)
            : CSF_AT(const wchar_t*, pathBytes, 0x4);
        createDirectory(path, nullptr);

        if (CSF_AT(std::uint32_t, pathBytes, 0x18) >= 8)
        {
            void* allocation = CSF_AT(void*, pathBytes, 0x4);
            if (allocation)
                Mem_FreeDebug(allocation);
        }
    }

    std::uint32_t offset = 0;
    while (profileCount)
    {
        void* profile = reinterpret_cast<void*>(self->field_0008 + offset);
        offset += 80;
        --profileCount;
        CSF_AT(std::uint8_t, profile, 0x4C) = 0;
    }

    CSF_AT(std::uint8_t,
           reinterpret_cast<void*>(self->field_0008 + 80 * profileIndex),
           0x4C) = 1;

    return ((char (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr);
}

HOOK(0x00514C70, CProfileManager_SelectProfile, "auto");
