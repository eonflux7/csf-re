#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, AppMain_SplitCreditsText, 0x005076D0,
         (int a1, uint16_t* a2, uint16_t* a3));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440,
         (int code, int flag));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_CameraHeightOffset, 0x0083F190);
CSF_GLOBAL(float, g_CreditsFadeScale, 0x0083F760);

static void __fastcall AppMain_MenuCreditsUpdateScroller(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0))
        return;

    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    int32_t* dimensions =
        ((int32_t*(__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    alignas(8) uint8_t rightTextBytes[0xC8];
    alignas(8) uint8_t leftTextBytes[0xD8];
    alignas(8) uint8_t colorBytes[0x28] = {};
    alignas(8) uint8_t positionBytes[0xC] = {};
    auto* rightText = reinterpret_cast<uint16_t*>(rightTextBytes);
    auto* leftText = reinterpret_cast<uint16_t*>(leftTextBytes);
    auto* position = reinterpret_cast<int32_t*>(positionBytes);

    position[0] = dimensions[0] / 2;
    position[1] = static_cast<int32_t>(
        static_cast<int64_t>(-CSF_AT(float, self, 0x14)));

    uint32_t index = CSF_AT(uint32_t, self, 0x18);
    uint32_t entryOffset = 56 * index;
    bool keepGoing;

    do
    {
        keepGoing = index < CSF_AT(uint32_t, self, 0x5C);
        if (keepGoing)
        {
            auto* entry = CSF_AT(uint8_t*, self, 0x54) + entryOffset;
            void* font = CSF_AT(void*, entry, 0x04);
            if (!font)
                font = CSF_AT(void*, self, 0x1C);

            int lineHeight =
                ((int(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
            int screenHeight = dimensions[1];
            int bottom = position[1] + lineHeight;
            keepGoing = bottom < screenHeight;
            const bool visibleContinuation = keepGoing;

            if (keepGoing && position[1] > 0)
            {
                uint32_t red = CSF_AT(uint32_t, self, 0x20);
                uint32_t green = CSF_AT(uint32_t, self, 0x24);
                uint32_t blue = CSF_AT(uint32_t, self, 0x28);
                float alpha = CSF_AT(float, self, 0x2C);

                if (CSF_AT(uint8_t, entry, 0) & 0x10)
                {
                    red = CSF_AT(uint32_t, entry, 0x08);
                    green = CSF_AT(uint32_t, entry, 0x0C);
                    blue = CSF_AT(uint32_t, entry, 0x10);
                    alpha = CSF_AT(float, entry, 0x14);
                }

                CSF_AT(uint32_t, colorBytes, 0x00) = red;
                CSF_AT(uint32_t, colorBytes, 0x04) = green;
                CSF_AT(uint32_t, colorBytes, 0x08) = blue;
                CSF_AT(float, colorBytes, 0x0C) = alpha;

                if (static_cast<float>(position[1]) < g_CameraHeightOffset)
                    CSF_AT(float, colorBytes, 0x0C) =
                        static_cast<float>(position[1]) * g_CreditsFadeScale;

                if (static_cast<float>(screenHeight) - g_CameraHeightOffset <
                    static_cast<float>(bottom))
                {
                    CSF_AT(float, colorBytes, 0x0C) =
                        static_cast<float>(screenHeight - bottom) *
                        g_CreditsFadeScale;
                }

                void* renderer = CSF_AT(void*, self, 0x30);
                ((void(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, renderer, 0)[0x60 / 4])(
                    renderer, nullptr, colorBytes);

                void* drawFont = CSF_AT(void*, entry, 0x04);
                if (!drawFont)
                    drawFont = CSF_AT(void*, self, 0x1C);

                renderer = CSF_AT(void*, self, 0x30);
                ((void(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, renderer, 0)[0x24 / 4])(
                    renderer, nullptr, drawFont);

                renderer = CSF_AT(void*, self, 0x30);
                ((void(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, renderer, 0)[0x98 / 4])(
                    renderer, nullptr, static_cast<uint8_t*>(self) + 4);

                renderer = CSF_AT(void*, self, 0x30);
                ((void(__fastcall*)(void*, void*, int32_t*))
                     CSF_AT(void**, renderer, 0)[0x50 / 4])(
                    renderer, nullptr, position);

                if (CSF_AT(uint8_t, entry, 0) & 1)
                {
                    renderer = CSF_AT(void*, self, 0x30);
                    ((void(__fastcall*)(void*, void*, int))
                         CSF_AT(void**, renderer, 0)[0x68 / 4])(
                        renderer, nullptr, 1);
                }

                if (CSF_AT(uint8_t, entry, 0) & 8)
                {
                    int key = CSF_AT(int, entry, 0x18);
                    renderer = CSF_AT(void*, self, 0x30);
                    void** savedVtable = CSF_AT(void**, renderer, 0);
                    int text = MenuMessageBox_ResolveEntryByKey(key, 256);

                    renderer = CSF_AT(void*, self, 0x30);
                    ((void(__fastcall*)(void*, void*, uint16_t*))
                         savedVtable[0x40 / 4])(
                        renderer, nullptr, reinterpret_cast<uint16_t*>(text));

                    renderer = CSF_AT(void*, self, 0x30);
                    ((void(__fastcall*)(void*, void*))
                         CSF_AT(void**, renderer, 0)[0x08 / 4])(
                        renderer, nullptr);
                }
                else
                {
                    uint16_t* text;
                    if (CSF_AT(uint32_t, entry, 0x34) < 8u)
                        text = reinterpret_cast<uint16_t*>(entry + 0x20);
                    else
                        text = CSF_AT(uint16_t*, entry, 0x20);

                    if (AppMain_SplitCreditsText(
                            reinterpret_cast<int>(text), leftText, rightText))
                    {
                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*, int))
                             CSF_AT(void**, renderer, 0)[0x68 / 4])(
                            renderer, nullptr, 2);

                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*, uint16_t*))
                             CSF_AT(void**, renderer, 0)[0x40 / 4])(
                            renderer, nullptr, leftText);

                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*))
                             CSF_AT(void**, renderer, 0)[0x08 / 4])(
                            renderer, nullptr);

                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*, int))
                             CSF_AT(void**, renderer, 0)[0x68 / 4])(
                            renderer, nullptr, 0);

                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*, uint16_t*))
                             CSF_AT(void**, renderer, 0)[0x40 / 4])(
                            renderer, nullptr, rightText);
                    }
                    else
                    {
                        if (CSF_AT(uint32_t, entry, 0x34) < 8u)
                            text = reinterpret_cast<uint16_t*>(entry + 0x20);
                        else
                            text = CSF_AT(uint16_t*, entry, 0x20);

                        renderer = CSF_AT(void*, self, 0x30);
                        ((void(__fastcall*)(void*, void*, uint16_t*))
                             CSF_AT(void**, renderer, 0)[0x40 / 4])(
                            renderer, nullptr, text);
                    }

                    renderer = CSF_AT(void*, self, 0x30);
                    ((void(__fastcall*)(void*, void*))
                         CSF_AT(void**, renderer, 0)[0x08 / 4])(
                        renderer, nullptr);
                }

                keepGoing = visibleContinuation;
            }

            int advance =
                ((int(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
            position[1] = advance + position[1];
        }

        ++index;
        entryOffset += 56;
    }
    while (keepGoing);
}

HOOK(0x005078B0, AppMain_MenuCreditsUpdateScroller, "auto");
