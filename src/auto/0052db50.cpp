#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ResetScenarioStars, 0x0052DA80,
         (CMenuScenarioSelect* self, void* edx));
CSF_FUNC(void*, __cdecl, GetScenarioService, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, GetScenarioCollection, 0x00515CE0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, FormatScenarioText, 0x00692660,
         (void* destination, const char* format, int value, int mode));
CSF_FUNC(int, __fastcall, SetScenarioText, 0x00720DF0,
         (void* self, void* edx, const void* text));
CSF_FUNC(int, __cdecl, FreeScenarioText, 0x00690E60, (void* allocation));
CSF_FUNC(int, __cdecl, FormatStarName, 0x007AF9BB,
         (char* destination, const char* format, ...));

static int __fastcall CMenuScenarioSelect_UpdateSelectedScenarioStars(
    CMenuScenarioSelect* self, void* /*edx*/)
{
    // Neither this routine nor ResetScenarioStars consumes incoming ESI.
    std::uint32_t result = self->field_0068;
    if (!result)
        return static_cast<int>(result);

    ResetScenarioStars(self, nullptr);

    void* service = GetScenarioService();
    result = reinterpret_cast<std::uintptr_t>(service);
    void* scenarioManager = CSF_AT(void*, service, 0x08);
    if (!scenarioManager)
        return static_cast<int>(result);

    void* collection = GetScenarioCollection(scenarioManager, nullptr);
    result = reinterpret_cast<std::uintptr_t>(collection);
    const std::uint32_t begin = CSF_AT(std::uint32_t, collection, 0x04);
    if (!begin)
        return static_cast<int>(result);

    const std::uint32_t byteCount =
        CSF_AT(std::uint32_t, collection, 0x08) - begin;
    result = static_cast<std::int32_t>(byteCount) / 80;
    if (!result)
        return static_cast<int>(result);

    result = self->field_0048 + 196u * self->field_003c;
    const std::uint32_t selectedScenario =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(result), 0);
    if (selectedScenario == UINT32_MAX)
        return static_cast<int>(result);

    void* ratings =
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, scenarioManager, 0)[9])(scenarioManager, nullptr);
    result = reinterpret_cast<std::uintptr_t>(ratings);

    // Original string object: prefix, 16-byte inline storage, size, capacity.
    alignas(4) std::uint8_t scenarioText[28];
    CSF_AT(std::uint32_t, scenarioText, 0x18) = 7;
    CSF_AT(std::uint32_t, scenarioText, 0x14) = 0;
    CSF_AT(std::uint16_t, scenarioText, 0x04) = 0;

    const std::uint32_t ratingsBegin =
        CSF_AT(std::uint32_t, ratings, 0x38);
    std::uint32_t remaining = 0;
    if (ratingsBegin) {
        const std::uint32_t bytes =
            CSF_AT(std::uint32_t, ratings, 0x3C) - ratingsBegin;
        remaining = static_cast<std::int32_t>(bytes) >> 4;
    }
    if (!remaining)
        return static_cast<int>(result);

    char emptyStarName[52];
    char filledStarName[52];
    std::uint32_t offset = 0;

    do {
        const std::uint32_t entryAddress =
            CSF_AT(std::uint32_t, ratings, 0x38) + offset;
        void* entry = reinterpret_cast<void*>(entryAddress);

        if (CSF_AT(std::uint32_t, entry, 0) == selectedScenario &&
            CSF_AT(std::uint32_t, entry, 0x0C) != 0) {
            FormatScenarioText(
                scenarioText, CSF_STR(0x00841608, "sc00"),
                CSF_AT(std::int32_t, entry, 0x04), 1);

            const void* text = CSF_AT(void*, scenarioText, 0x04);
            if (CSF_AT(std::uint32_t, scenarioText, 0x18) < 8)
                text = scenarioText + 4;

            SetScenarioText(
                reinterpret_cast<void*>(self->field_0068), nullptr, text);

            for (std::uint32_t star = 1; star <= 5; ++star) {
                FormatStarName(emptyStarName,
                               CSF_STR(0x008415FC, "EstrellaN%d"), star);
                FormatStarName(filledStarName,
                               CSF_STR(0x008415F0, "Estrella%d"), star);

                void* screen = reinterpret_cast<void*>(self->field_0018);
                void* emptyStar =
                    ((void*(__fastcall*)(void*, void*, const char*))
                         CSF_AT(void**, screen, 0)[63])(
                        screen, nullptr, emptyStarName);

                screen = reinterpret_cast<void*>(self->field_0018);
                void* filledStar =
                    ((void*(__fastcall*)(void*, void*, const char*))
                         CSF_AT(void**, screen, 0)[63])(
                        screen, nullptr, filledStarName);

                if (star > CSF_AT(std::uint32_t, entry, 0x0C)) {
                    if (emptyStar) {
                        ((void(__fastcall*)(void*, void*, int))
                             CSF_AT(void**, emptyStar, 0)[42])(
                            emptyStar, nullptr, 0);
                    }
                } else if (filledStar) {
                    ((void(__fastcall*)(void*, void*, int))
                         CSF_AT(void**, filledStar, 0)[42])(
                        filledStar, nullptr, 0);
                }
            }
            result = reinterpret_cast<std::uintptr_t>(ratings);
        }

        offset += 16;
        --remaining;
    } while (remaining);

    if (CSF_AT(std::uint32_t, scenarioText, 0x18) >= 8) {
        void* allocation = CSF_AT(void*, scenarioText, 0x04);
        result = reinterpret_cast<std::uintptr_t>(allocation);
        if (allocation)
            return FreeScenarioText(allocation);
    }

    return static_cast<int>(result);
}

HOOK(0x0052DB50, CMenuScenarioSelect_UpdateSelectedScenarioStars, "auto");
