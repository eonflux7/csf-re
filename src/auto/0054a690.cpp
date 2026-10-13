#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuVideoOptions_InsertElementArray24, 0x0054A2B0, (void* self, void* edx, char* position, unsigned int count, int* element));
CSF_FUNC(std::uint32_t*, __fastcall, MenuVideoOptions_InsertElement24, 0x0054A590, (void* self, void* , std::uint32_t* result, char* position, int* element));
CSF_FUNC(std::uint32_t*, __fastcall, CMenuVideoOptions_AppendElement24, 0x0054A600, (void* self, void* , std::uint32_t* a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __fastcall CMenuVideoOptions_PopulateResolutionList(
    CMenuVideoOptions* self, void* /*edx*/)
{
    alignas(8) std::uint8_t textBytes[0x200] = {};
    alignas(8) std::uint8_t insertionResultBytes[0x4] = {};
    alignas(8) std::uint8_t modeBytes[0x20] = {};
    alignas(8) std::uint8_t listBytes[0x20];

    auto& modes = *reinterpret_cast<S_0054a590*>(listBytes);
    std::memset(&modes.field_0004, 0, 12);

    // The packet also classifies the insertion function's address as a vtable.
    (void)CSF_VTBL(0x0054A590);

    if (!self->field_0060)
        return 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const std::uint32_t modeCount =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x44 / 4])(display, nullptr);

    char* elements = nullptr;
    std::uint32_t eligibleModeCount = 0;

    for (std::uint32_t modeIndex = 0; modeIndex < modeCount; ++modeIndex)
    {
        ((void (__fastcall*)(void*, void*, std::uint32_t, int*))
            CSF_AT(void**, display, 0)[0x48 / 4])(
                display, nullptr, modeIndex, reinterpret_cast<int*>(modeBytes));

        if (CSF_AT(int, modeBytes, 0x8) != 32 ||
            CSF_AT(int, modeBytes, 0) < 640 ||
            CSF_AT(int, modeBytes, 0x4) < 480)
            continue;

        ++eligibleModeCount;
        bool duplicate = false;
        char* existing = elements;
        for (std::uint32_t index = 0;
             elements &&
             index < static_cast<std::uint32_t>(
                 static_cast<std::int32_t>(
                     modes.field_0008 -
                     reinterpret_cast<std::uintptr_t>(elements)) / 24);
             ++index, existing += 24)
        {
            if (CSF_AT(int, existing, 0) == CSF_AT(int, modeBytes, 0) &&
                CSF_AT(int, existing, 0x4) == CSF_AT(int, modeBytes, 0x4))
            {
                duplicate = true;
                break;
            }
        }

        if (duplicate)
            continue;

        char* position = elements;
        bool inserted = false;
        while (position != reinterpret_cast<char*>(modes.field_0008))
        {
            if (CSF_AT(int, modeBytes, 0) < CSF_AT(int, position, 0))
            {
                MenuVideoOptions_InsertElementArray24(
                    &modes, nullptr, position, 1,
                    reinterpret_cast<int*>(modeBytes));
                inserted = true;
                break;
            }
            if (CSF_AT(int, modeBytes, 0) == CSF_AT(int, position, 0) &&
                CSF_AT(int, modeBytes, 0x4) < CSF_AT(int, position, 0x4))
            {
                MenuVideoOptions_InsertElement24(
                    &modes, nullptr,
                    reinterpret_cast<std::uint32_t*>(insertionResultBytes),
                    position, reinterpret_cast<int*>(modeBytes));
                inserted = true;
                break;
            }
            position += 24;
        }

        if (!inserted)
        {
            CMenuVideoOptions_AppendElement24(
                &modes, nullptr, reinterpret_cast<std::uint32_t*>(modeBytes));
        }
        elements = reinterpret_cast<char*>(modes.field_0004);
    }

    if (self->field_0060)
    {
        void* control = reinterpret_cast<void*>(self->field_0060);
        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[0x2B8 / 4])(control, nullptr);

        if (eligibleModeCount)
        {
            char* mode = elements;
            for (std::uint32_t index = 0;
                 elements &&
                 index < static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(
                         modes.field_0008 -
                         reinterpret_cast<std::uintptr_t>(elements)) / 24);
                 ++index, mode += 24)
            {
                _sprintf(
                    reinterpret_cast<char*>(textBytes),
                    CSF_STR(0x00842F4C, "%d x %d"),
                    CSF_AT(int, mode, 0), CSF_AT(int, mode, 0x4));

                control = reinterpret_cast<void*>(self->field_0060);
                ((void (__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, control, 0)[0x298 / 4])(
                        control, nullptr, reinterpret_cast<char*>(textBytes));
            }
        }
        else
        {
            control = reinterpret_cast<void*>(self->field_0060);
            ((void (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, control, 0)[0x298 / 4])(
                    control, nullptr, CSF_STR(0x008441B4, "640 x 480"));

            control = reinterpret_cast<void*>(self->field_0060);
            ((void (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, control, 0)[0x298 / 4])(
                    control, nullptr, CSF_STR(0x008441A8, "800 x 600"));

            control = reinterpret_cast<void*>(self->field_0060);
            ((void (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, control, 0)[0x298 / 4])(
                    control, nullptr, CSF_STR(0x0084419C, "1024 x 768"));
        }
    }

    std::uint32_t selectedIndex = 0;
    char* mode = elements;
    for (std::uint32_t index = 0;
         elements &&
         index < static_cast<std::uint32_t>(
             static_cast<std::int32_t>(
                 modes.field_0008 -
                 reinterpret_cast<std::uintptr_t>(elements)) / 24);
         ++index, mode += 24)
    {
        if (CSF_AT(std::uint32_t, mode, 0) == self->field_0028 &&
            CSF_AT(std::uint32_t, mode, 0x4) == self->field_002c)
        {
            if (index != static_cast<std::uint32_t>(-1))
                selectedIndex = index;
            break;
        }
    }

    void* control = reinterpret_cast<void*>(self->field_0060);
    const int result =
        ((int (__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, control, 0)[0x2A8 / 4])(
                control, nullptr, selectedIndex);

    if (elements)
        return Mem_FreeDebug(elements);
    return result;
}

HOOK(0x0054A690, CMenuVideoOptions_PopulateResolutionList, "auto");
