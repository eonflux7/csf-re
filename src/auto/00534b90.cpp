#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, StringFormat_CopyTrucoStringToWide, 0x00695590, (char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, uint16_t* text));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(uint32_t, g_Notification20Prefix, 0x00841D18);
CSF_GLOBAL(uint16_t, g_Notification20Suffix, 0x00841D1C);
CSF_GLOBAL(uint32_t, g_Notification54Prefix, 0x00841D10);
CSF_GLOBAL(uint16_t, g_Notification54Suffix, 0x00841D14);

static char __fastcall CMenuMpPause_UpdateFriends(CMenuMpPause* self, void* /*edx*/)
{
    alignas(8) uint8_t wideTextBytes[0x20];
    alignas(8) uint8_t textOrColorBytes[0x10];
    alignas(8) uint8_t initialColorBytes[0x18];
    alignas(8) uint8_t measuredSizeBytes[0x8];
    alignas(8) uint8_t notificationPositionBytes[0x8];

    auto* text = reinterpret_cast<char*>(textOrColorBytes);
    auto* wideText = reinterpret_cast<uint16_t*>(wideTextBytes);
    auto* color = reinterpret_cast<float*>(textOrColorBytes);
    auto* initialColor = reinterpret_cast<float*>(initialColorBytes);
    auto* measuredSize = reinterpret_cast<int32_t*>(measuredSizeBytes);
    auto* notificationPosition = reinterpret_cast<int32_t*>(notificationPositionBytes);

    if (self->field_0045)
    {
        // Compare the unrounded x87 sum, not the float stored at +0x48.
        const double alpha = static_cast<double>(self->field_0048) + self->field_004c;
        self->field_0048 = static_cast<float>(alpha);
        if (alpha < g_fZero)
        {
            self->field_0048 = 0.0f;
            self->field_004c = 8.0f;
        }
        else if (alpha > g_ColorChannelScale)
        {
            self->field_0048 = 255.0f;
            self->field_004c = -8.0f;
        }

        color[0] = 255.0f;
        color[1] = 255.0f;
        color[2] = 255.0f;
        color[3] = self->field_0048;
        void* notification = reinterpret_cast<void*>(self->field_0034);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, notification, 0)[0xD0 / 4])
            (notification, nullptr, color);
    }

    if (GetGlobalState_6A6D70())
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void* notificationState =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, state, 0)[0xF8 / 4])(state, nullptr);

        if (notificationState)
        {
            std::memset(text, 0, 15);

            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            notificationState =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, state, 0)[0xF8 / 4])(state, nullptr);
            if (((uint8_t(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, notificationState, 0)[0x10 / 4])
                    (notificationState, nullptr, 0))
            {
                char* end = text;
                while (*end)
                    ++end;
                const uint32_t prefix = g_Notification20Prefix;
                const uint16_t suffix = g_Notification20Suffix;
                std::memcpy(end, &prefix, 4);
                std::memcpy(end + 4, &suffix, 2);
            }

            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            notificationState =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, state, 0)[0xF8 / 4])(state, nullptr);
            if (((uint8_t(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, notificationState, 0)[0x10 / 4])
                    (notificationState, nullptr, 1))
            {
                char* end = text;
                while (*end)
                    ++end;
                const uint32_t prefix = g_Notification54Prefix;
                const uint16_t suffix = g_Notification54Suffix;
                std::memcpy(end, &prefix, 4);
                std::memcpy(end + 4, &suffix, 2);
            }

            if (text[0])
            {
                StringFormat_CopyTrucoStringToWide(text, wideText, 15);
                if (!self->field_0045)
                {
                    void* anchor = reinterpret_cast<void*>(self->field_002c);
                    int32_t* anchorPosition =
                        ((int32_t*(__fastcall*)(void*, void*))
                            CSF_AT(void**, anchor, 0)[0x9C / 4])(anchor, nullptr);
                    const int32_t anchorX = anchorPosition[0];
                    const int32_t anchorY = anchorPosition[1];

                    anchor = reinterpret_cast<void*>(self->field_002c);
                    int32_t* anchorSize =
                        ((int32_t*(__fastcall*)(void*, void*))
                            CSF_AT(void**, anchor, 0)[0xA4 / 4])(anchor, nullptr);
                    const int32_t anchorWidth = anchorSize[0];
                    const int32_t anchorHeight = anchorSize[1];

                    anchor = reinterpret_cast<void*>(self->field_002c);
                    void* content =
                        ((void*(__fastcall*)(void*, void*))
                            CSF_AT(void**, anchor, 0)[0x110 / 4])(anchor, nullptr);
                    ((void(__fastcall*)(void*, void*, int32_t*, int))
                        CSF_AT(void**, content, 0)[0x78 / 4])
                        (content, nullptr, measuredSize, 0);

                    const int32_t centeredY =
                        anchorY + anchorHeight / 2 - measuredSize[1] / 2;
                    const int32_t notificationX =
                        anchorX + measuredSize[0] + anchorWidth / 2
                        - measuredSize[0] / 2;

                    void* notification = reinterpret_cast<void*>(self->field_0034);
                    int32_t* notificationSize =
                        ((int32_t*(__fastcall*)(void*, void*))
                            CSF_AT(void**, notification, 0)[0xA4 / 4])
                            (notification, nullptr);
                    notificationPosition[1] =
                        centeredY + measuredSize[1] / 2 - notificationSize[1] / 2;
                    notificationPosition[0] = notificationX;

                    notification = reinterpret_cast<void*>(self->field_0034);
                    self->field_0048 = 0.0f;
                    self->field_004c = 8.0f;
                    ((void(__fastcall*)(void*, void*, int32_t*))
                        CSF_AT(void**, notification, 0)[0x98 / 4])
                        (notification, nullptr, notificationPosition);

                    notification = reinterpret_cast<void*>(self->field_0034);
                    ((int(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, notification, 0)[0xA8 / 4])
                        (notification, nullptr, 0);

                    initialColor[0] = 255.0f;
                    initialColor[1] = 255.0f;
                    initialColor[2] = 255.0f;
                    initialColor[3] = self->field_0048;
                    notification = reinterpret_cast<void*>(self->field_0034);
                    ((void(__fastcall*)(void*, void*, float*))
                        CSF_AT(void**, notification, 0)[0xD0 / 4])
                        (notification, nullptr, initialColor);
                    self->field_0045 = 1;
                }

                C_00720DF0_ForwardCallToChildVtblSlot16(
                    reinterpret_cast<void*>(self->field_0034), nullptr, wideText);
            }
        }
    }

    char result = static_cast<char>(self->field_0045);
    if (result)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void* notificationState =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, state, 0)[0xF8 / 4])(state, nullptr);
        result =
            ((uint8_t(__fastcall*)(void*, void*, int))
                CSF_AT(void**, notificationState, 0)[0x10 / 4])
                (notificationState, nullptr, 0);

        if (!result)
        {
            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            notificationState =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, state, 0)[0xF8 / 4])(state, nullptr);
            result =
                ((uint8_t(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, notificationState, 0)[0x10 / 4])
                    (notificationState, nullptr, 1);

            if (!result)
            {
                self->field_0045 = 0;
                void* notification = reinterpret_cast<void*>(self->field_0034);
                return static_cast<char>(
                    ((int(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, notification, 0)[0xA8 / 4])
                        (notification, nullptr, 1));
            }
        }
    }
    return result;
}

HOOK(0x00534B90, CMenuMpPause_UpdateFriends, "auto");
