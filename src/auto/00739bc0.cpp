#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00867444_AppendGlyphGeometry, 0x00739620, (void* self, void* edx, float* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(void*, g_Block8BBF8C, 0x008BBF8C);
CSF_GLOBAL(void*, g_Block8BBF90, 0x008BBF90);

static void FontWapa_SetRenderState(int state, std::uint32_t value)
{
    reinterpret_cast<void(__cdecl*)(int, std::uint32_t)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(state, value);
}

static int __fastcall C_00867444_RenderGlyphs(
    C_00867444* self, void* /*edx*/, std::uint32_t* glyphs,
    unsigned int first, unsigned int last)
{
    FontWapa_SetRenderState(10, 5);
    FontWapa_SetRenderState(11, 6);
    FontWapa_SetRenderState(12, 1);
    FontWapa_SetRenderState(14, 0);
    FontWapa_SetRenderState(9, 2);
    FontWapa_SetRenderState(6, 0);
    FontWapa_SetRenderState(8, 0);
    FontWapa_SetRenderState(20, 1);

    void* backend = reinterpret_cast<void*>(self->field_0008);
    void* pages = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, backend, 0)[180 / 4])(backend, nullptr);

    std::uint32_t remaining = glyphs[2];
    std::uint32_t recordOffset = 0;
    while (remaining != 0)
    {
        void* record = reinterpret_cast<std::uint8_t*>(glyphs[0]) + recordOffset;
        void* glyph = CSF_AT(void*, record, 0);
        std::uint32_t page = CSF_AT(std::uint32_t, glyph, 0x4);

        std::uint32_t count = CSF_AT(std::uint32_t, pages, 0x8);
        std::uint32_t found = 0;
        if (count != 0)
        {
            auto* entries = CSF_AT(std::uint32_t*, pages, 0);
            while (found < count)
            {
                if (entries[found] == page)
                    break;
                ++found;
            }
        }

        if (found == count)
        {
            std::uint32_t capacity = CSF_AT(std::uint32_t, pages, 0x4);
            if (count >= capacity)
            {
                std::uint32_t growth = CSF_AT(std::uint32_t, pages, 0xC);
                void* replacement = reinterpret_cast<void*>(
                    CoreMemory_AllocUntagged(4 * (growth + capacity)));
                std::uint32_t copyCount = CSF_AT(std::uint32_t, pages, 0x8);
                if (copyCount != 0)
                {
                    std::memcpy(replacement, CSF_AT(void*, pages, 0), 4 * copyCount);
                }
                CSF_AT(std::uint32_t, pages, 0x4) += growth;
                if (CSF_AT(void*, pages, 0) != nullptr)
                {
                    CoreMemory_Free(CSF_AT(void*, pages, 0));
                    CSF_AT(void*, pages, 0) = nullptr;
                }
                CSF_AT(void*, pages, 0) = replacement;
            }

            std::uint32_t insertionIndex = CSF_AT(std::uint32_t, pages, 0x8);
            auto* destination = CSF_AT(std::uint32_t*, pages, 0) + insertionIndex;
            CSF_AT(std::uint32_t, pages, 0x8) = insertionIndex + 1;
            *destination = page;
        }

        recordOffset += 28;
        --remaining;
    }

    const std::uint32_t pageCount = CSF_AT(std::uint32_t, pages, 0x8);
    for (std::uint32_t pageIndex = 0; pageIndex < pageCount; ++pageIndex)
    {
        std::uint32_t page = CSF_AT(std::uint32_t*, pages, 0)[pageIndex];
        void* texture = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0034), 4 * page);
        if (texture != nullptr)
            FontWapa_SetRenderState(1, CSF_AT(std::uint32_t, texture, 0));
        else
            FontWapa_SetRenderState(1, 0);

        const std::uint32_t glyphCount = glyphs[2];
        std::uint32_t current = 0;
        std::uint32_t end = glyphCount;
        std::uint32_t indexCapacity = 0;
        std::uint32_t* indices = nullptr;

        if (first != static_cast<unsigned int>(-1) && first < glyphCount)
        {
            current = first;
            end = first + 1;
            if (last != static_cast<unsigned int>(-1))
            {
                if (last >= glyphCount)
                    end = glyphCount;
                else
                    end = last + 1;
            }
        }

        if (current < end)
        {
            do
            {
                std::uint32_t batchCount = 0;
                self->field_0050 = 0;
                std::uint32_t offset = 28 * current;

                while (current < end && batchCount < 0x64)
                {
                    auto* record = reinterpret_cast<float*>(
                        reinterpret_cast<std::uint8_t*>(glyphs[0]) + offset);
                    void* glyph = CSF_AT(void*, record, 0);
                    if (CSF_AT(std::uint32_t, glyph, 0x4) ==
                        CSF_AT(std::uint32_t*, pages, 0)[pageIndex])
                    {
                        C_00867444_AppendGlyphGeometry(self, nullptr, record);

                        if (batchCount >= indexCapacity)
                        {
                            auto* replacement = reinterpret_cast<std::uint32_t*>(
                                CoreMemory_AllocUntagged(4 * indexCapacity + 40));
                            if (batchCount != 0)
                                std::memcpy(replacement, indices, 4 * batchCount);
                            indexCapacity += 10;
                            if (indices != nullptr)
                                CoreMemory_Free(indices);
                            indices = replacement;
                        }
                        indices[batchCount++] = current;
                    }
                    ++current;
                    offset += 28;
                }

                int quadCount = static_cast<int>(
                    batchCount * ((self->field_005c != 0) + 1));
                backend = reinterpret_cast<void*>(self->field_0008);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, int, void*, int, void*, int)>(
                    CSF_AT(void**, backend, 0)[40 / 4])(
                        backend, nullptr, 3,
                        g_Block8BBF8C, 4 * quadCount,
                        g_Block8BBF90, 6 * quadCount);
            } while (current < end);

            if (indices != nullptr)
                CoreMemory_Free(indices);
        }
    }

    backend = reinterpret_cast<void*>(self->field_0008);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, backend, 0)[184 / 4])(backend, nullptr);
}

HOOK(0x00739BC0, C_00867444_RenderGlyphs, "auto");
