#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ApplyVideoPreset, 0x0050AF90,
         (void* self, void* edx, int preset));
CSF_FUNC(int, __fastcall, CycleVideoResolution, 0x0050B5B0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, ParseSelectedResolution, 0x00549840,
         (CMenuVideoOptions* self, void* edx, int selection,
          int* width, int* height));
CSF_FUNC(void*, __fastcall, CommitVideoOptions, 0x00549FF0,
         (CMenuVideoOptions* self, void* edx));
CSF_FUNC(int, __cdecl, FormatResolution, 0x007AF9BB,
         (char* destination, const char* format, ...));

CSF_GLOBAL(const char*, g_AdvancedVideoMenuName, 0x00899B84);

template<typename Result, typename... Args>
static Result InvokeVideoMethod(void* object, unsigned slot, Args... args)
{
    return reinterpret_cast<Result(__fastcall*)(void*, void*, Args...)>(
        CSF_AT(void**, object, 0)[slot])(object, nullptr, args...);
}

static void __fastcall CMenuVideoOptions_HandleControlEvent(
    CMenuVideoOptions* self, void* /*edx*/, void* event)
{
    void* settings = reinterpret_cast<void*>(self->field_0024);
    if (!settings)
        return;

    if (self->field_0070 &&
        self->field_0070 == CSF_AT(std::uint32_t, event, 4))
    {
        CommitVideoOptions(self, nullptr);
        return;
    }

    if (self->field_006c &&
        self->field_006c == CSF_AT(std::uint32_t, event, 4))
    {
        settings = reinterpret_cast<void*>(self->field_0024);
        void** savedTable = CSF_AT(void**, settings, 0);
        const std::uint32_t state =
            InvokeVideoMethod<std::uint32_t>(settings, 0x30 / 4);
        const std::uint32_t toggled =
            (state & 0xFFFFFF00u) | ((state & 0xFFu) == 0 ? 1u : 0u);

        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
            savedTable[0x34 / 4])(
                reinterpret_cast<void*>(self->field_0024), nullptr, toggled);
        InvokeVideoMethod<void>(self, 0x50 / 4);
        return;
    }

    if (self->field_0058 &&
        self->field_0058 == CSF_AT(std::uint32_t, event, 4))
    {
        const int preset = InvokeVideoMethod<int>(
            reinterpret_cast<void*>(self->field_0058), 0x2AC / 4);
        InvokeVideoMethod<void>(
            reinterpret_cast<void*>(self->field_0024), 0x1C / 4, preset);
        ApplyVideoPreset(
            reinterpret_cast<void*>(self->field_0024), nullptr, preset);
    }
    else if (self->field_005c &&
             self->field_005c == CSF_AT(std::uint32_t, event, 4))
    {
        CycleVideoResolution(settings, nullptr);
    }
    else
    {
        if (self->field_0068 &&
            self->field_0068 == CSF_AT(std::uint32_t, event, 4))
        {
            void* advancedMenu = InvokeVideoMethod<void*>(
                reinterpret_cast<void*>(self->field_0014),
                0x5C / 4, g_AdvancedVideoMenuName);
            if (advancedMenu)
            {
                CSF_AT(void*, advancedMenu, 0x50) =
                    reinterpret_cast<std::uint8_t*>(self) + 0x28;
                InvokeVideoMethod<void>(
                    reinterpret_cast<void*>(self->field_001c),
                    0xA8 / 4, 1);
                InvokeVideoMethod<void>(
                    reinterpret_cast<void*>(self->field_0014),
                    0x58 / 4, g_AdvancedVideoMenuName);
            }
        }
        else if (self->field_0064 &&
                 self->field_0064 == CSF_AT(std::uint32_t, event, 4))
        {
            void* slider = reinterpret_cast<void*>(self->field_0064);
            const float value = static_cast<float>(
                InvokeVideoMethod<double>(slider, 0x274 / 4));
            const std::uint32_t divisorBits =
                static_cast<std::uint32_t>(
                    InvokeVideoMethod<int>(slider, 0x264 / 4)) - 1u;
            const float normalized = static_cast<float>(
                static_cast<double>(value) /
                static_cast<double>(static_cast<std::int32_t>(divisorBits)));
            InvokeVideoMethod<void>(
                reinterpret_cast<void*>(self->field_0024),
                0x2C / 4, normalized);
        }
        else if (self->field_0060 &&
                 self->field_0060 == CSF_AT(std::uint32_t, event, 4))
        {
            void* selector = reinterpret_cast<void*>(self->field_0060);
            void** savedTable = CSF_AT(void**, selector, 0);
            float width = 0.0f;
            float height = 0.0f;
            const int index = InvokeVideoMethod<int>(
                selector, 0x2AC / 4, &width, &height);
            const int selection =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    savedTable[0x2B4 / 4])(
                        reinterpret_cast<void*>(self->field_0060),
                        nullptr, index);

            // The original supplies saved registers as the two output
            // pointers and over-pops its stack. Normalize that malformed
            // call to local storage; leave the virtual getter's floats intact.
            int parsedWidth;
            int parsedHeight;
            ParseSelectedResolution(
                self, nullptr, selection, &parsedWidth, &parsedHeight);

            InvokeVideoMethod<void>(
                reinterpret_cast<void*>(self->field_0024),
                0x24 / 4, width, height);
        }
        return;
    }

    char buffer[512];
    std::memset(buffer, 0, sizeof(buffer));
    const int* dimensions = InvokeVideoMethod<const int*>(
        reinterpret_cast<void*>(self->field_0024), 0x20 / 4);
    const int height = dimensions[1];
    const int width = dimensions[0];
    FormatResolution(
        buffer, CSF_STR(0x00842F4C, "%d x %d"), width, height);
    InvokeVideoMethod<void>(
        reinterpret_cast<void*>(self->field_0060), 0x2A0 / 4, buffer);
}

HOOK(0x0054A090, CMenuVideoOptions_HandleControlEvent, "auto");
