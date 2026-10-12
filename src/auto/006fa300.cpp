#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_DrawQuad, 0x00708AB0, (void* self, void* edx, int a2, char a3, int a4, int a5, int a6, uint8_t a7, float a8, float a9, float a10, float a11));
CSF_FUNC(int, __fastcall, GfxScreenEffects_BeginDrawing, 0x00708D10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_EndDrawing, 0x00708D90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FilmNoiseAlphaScale, 0x00862C30);

static void __fastcall C_00862c74_RenderFilmEffects(C_00862c74* self, void* /*edx*/)
{
    GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    SceneLoader_NoOp();

    void* effect = reinterpret_cast<void*>(self->field_0080);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, effect, 0)[0x14 / 4])(effect, nullptr);

    self->field_0030 = 0;
    GfxScreenEffects_BeginDrawing(self, nullptr);

    void* viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const int screenWidth = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, viewport, 0)[0x20 / 4])(viewport, nullptr);

    viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const int screenHeight = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, viewport, 0)[0x24 / 4])(viewport, nullptr);

    int count = self->field_0088
        ? static_cast<int32_t>(self->field_008c - self->field_0088) / 20
        : 0;
    for (int index = 0, offset = 0; index < count; ++index, offset += 20)
    {
        void* stripe = reinterpret_cast<void*>(self->field_0088 + offset);
        if (CSF_AT(float, stripe, 0x04) > g_fZero)
        {
            void* texture = reinterpret_cast<void*>(self->field_0068);
            self->field_0034 = static_cast<float>(screenWidth) * CSF_AT(float, stripe, 0x00);
            self->field_0038 = 0.0f;
            GfxScreenEffects_DrawQuad(
                self, nullptr, (index & 1) ? 0xFFFFFF : 0,
                static_cast<char>(CSF_AT(uint8_t, stripe, 0x10)),
                2, screenHeight, CSF_AT(int, texture, 0x00),
                1u, 0.0f, 0.0f, 1.0f, 1.0f);
        }
    }

    count = self->field_0098
        ? static_cast<int32_t>(self->field_009c - self->field_0098) / 20
        : 0;
    if (count > 0)
    {
        const float height = static_cast<float>(screenHeight);
        const float width = static_cast<float>(screenWidth);
        int offset = 0;
        do
        {
            void* rectangle = reinterpret_cast<void*>(self->field_0098 + offset);
            void* texture = reinterpret_cast<void*>(self->field_0060);
            self->field_0034 = width * CSF_AT(float, rectangle, 0x00);
            self->field_0038 = height * CSF_AT(float, rectangle, 0x04);
            const int textureId = CSF_AT(int, texture, 0x00);
            const int rectangleHeight = static_cast<int>(
                static_cast<int64_t>(height * CSF_AT(float, rectangle, 0x0C)));
            const int rectangleWidth = static_cast<int>(
                static_cast<int64_t>(width * CSF_AT(float, rectangle, 0x08)));
            GfxScreenEffects_DrawQuad(
                self, nullptr, 0xFFFFFF,
                static_cast<char>(CSF_AT(uint8_t, rectangle, 0x10)),
                rectangleWidth, rectangleHeight, textureId,
                1u, 0.0f, 0.0f, 1.0f, 1.0f);
            offset += 20;
        } while (--count);
    }

    count = self->field_00a8
        ? static_cast<int32_t>(self->field_00ac - self->field_00a8) >> 5
        : 0;
    if (count > 0)
    {
        const float height = static_cast<float>(screenHeight);
        const float width = static_cast<float>(screenWidth);
        int offset = 0;
        do
        {
            void* rectangle = reinterpret_cast<void*>(self->field_00a8 + offset);
            self->field_0034 = width * CSF_AT(float, rectangle, 0x00);
            self->field_0038 = height * CSF_AT(float, rectangle, 0x04);
            const float v1 = CSF_AT(float, rectangle, 0x1C);
            const float u1 = CSF_AT(float, rectangle, 0x14);
            const float v0 = CSF_AT(float, rectangle, 0x18);
            const float u0 = CSF_AT(float, rectangle, 0x10);
            const int textureId = CSF_AT(int, reinterpret_cast<void*>(self->field_0064), 0x00);
            const int rectangleHeight = static_cast<int>(
                static_cast<int64_t>(height * CSF_AT(float, rectangle, 0x0C)));
            const int rectangleWidth = static_cast<int>(
                static_cast<int64_t>(width * CSF_AT(float, rectangle, 0x08)));
            GfxScreenEffects_DrawQuad(
                self, nullptr, 0, static_cast<char>(255),
                rectangleWidth, rectangleHeight, textureId,
                1u, u0, v0, u1, v1);
            offset += 32;
        } while (--count);
    }

    count = self->field_00b8
        ? static_cast<int32_t>(self->field_00bc - self->field_00b8) / 20
        : 0;
    if (count > 0)
    {
        const float height = static_cast<float>(screenHeight);
        const float width = static_cast<float>(screenWidth);
        for (int index = 0, offset = 0; index < count; ++index, offset += 20)
        {
            void* rectangle = reinterpret_cast<void*>(self->field_00b8 + offset);
            self->field_0034 = width * CSF_AT(float, rectangle, 0x00);
            self->field_0038 = height * CSF_AT(float, rectangle, 0x04);
            void* texture = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x6C + 4 * (index & 3)));
            const int textureId = CSF_AT(int, texture, 0x00);
            const int rectangleHeight = static_cast<int>(
                static_cast<int64_t>(height * CSF_AT(float, rectangle, 0x0C)));
            const int rectangleWidth = static_cast<int>(
                static_cast<int64_t>(width * CSF_AT(float, rectangle, 0x08)));
            GfxScreenEffects_DrawQuad(
                self, nullptr, 0xFFFFFF,
                static_cast<char>(CSF_AT(uint8_t, rectangle, 0x10)),
                rectangleWidth, rectangleHeight, textureId,
                1u, 0.0f, 0.0f, 1.0f, 1.0f);
        }
    }

    self->field_0038 = -0.5f;
    self->field_0034 = -0.5f;
    GfxScreenEffects_EndDrawing(self, nullptr);
    self->field_0030 = 0;
    GfxScreenEffects_BeginDrawing(self, nullptr);
    const int64_t alpha = static_cast<int64_t>(
        static_cast<double>(_rand()) * g_FilmNoiseAlphaScale);
    GfxScreenEffects_DrawQuad(
        self, nullptr, 0, static_cast<char>(alpha),
        screenWidth, screenHeight, 0, 1u, 0.0f, 0.0f, 1.0f, 1.0f);
    GfxScreenEffects_EndDrawing(self, nullptr);
    SceneLoader_NoOp();
}

HOOK(0x006FA300, C_00862c74_RenderFilmEffects, "auto");
