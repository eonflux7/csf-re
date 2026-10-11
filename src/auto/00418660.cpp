#include <cstdint>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AdvanceAxisEntry, 0x004080C0, (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0082e1e8_UpdateWheelTravel(
    C_0082e1e8* self, void* /*edx*/, int entryAddress)
{
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController =
        reinterpret_cast<void*>(SceneLoader_GetSceneController(global, nullptr));
    void* queryService =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, sceneController, 0)[0x3C / 4])(
            sceneController, nullptr);

    void* entry = reinterpret_cast<void*>(entryAddress);
    float queryHeight = CSF_AT(float, entry, 0x04) + CSF_AT(float, entry, 0x14);
    alignas(4) uint8_t position[0xC] = {};
    uint32_t* forwardedPosition = reinterpret_cast<uint32_t*>(
        IndexedChildForwardValue(
            reinterpret_cast<void*>(self->field_0030), nullptr,
            reinterpret_cast<int>(position), CSF_AT(int, entry, 0x00)));

    uint32_t query[4];
    query[0] = forwardedPosition[0];
    query[1] = forwardedPosition[1];
    query[2] = forwardedPosition[2];
    std::memcpy(&query[3], &queryHeight, sizeof(queryHeight));

    int querySize = 16;
    void* hit =
        ((void*(__fastcall*)(void*, void*, uint32_t*, int*, int))
            CSF_AT(void**, queryService, 0)[0x2C / 4])(
                queryService, nullptr, query, &querySize, 3);

    if (!hit)
    {
        float delta = -CSF_AT(float, entry, 0x14);
        AdvanceAxisEntry(reinterpret_cast<void*>(self->field_0030), nullptr,
                         CSF_AT(int, entry, 0x00), delta);
        CSF_AT(int, entry, 0x0C) = 0;
        CSF_AT(uint32_t, entry, 0x14) = 0;
        CSF_AT(uint32_t, entry, 0x10) = 0;
        return;
    }

    alignas(4) uint8_t hitResult[0x68] = {};
    CSF_AT(float, hitResult, 0x00) = FLT_MAX;
    CSF_AT(uint32_t, hitResult, 0x1C) = UINT32_MAX;
    CSF_AT(uint8_t, hitResult, 0x20) = 0xFF;
    CSF_AT(uint32_t, hitResult, 0x2C) = UINT32_MAX;
    CSF_AT(uint8_t, hitResult, 0x30) = 0xFF;
    CSF_AT(int, hitResult, 0x64) = 10;

    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, hit, 0)[0x0C / 4])(
        hit, nullptr, hitResult);

    float heightDifference =
        CSF_AT(float, hitResult, 0x00) - CSF_AT(float, entry, 0x04);
    if (heightDifference == g_fZero)
        return;

    float requestedTravel = CSF_AT(float, entry, 0x14) - heightDifference;
    if (requestedTravel > CSF_AT(float, entry, 0x08))
    {
        float delta = CSF_AT(float, entry, 0x08) - CSF_AT(float, entry, 0x14);
        AdvanceAxisEntry(reinterpret_cast<void*>(self->field_0030), nullptr,
                         CSF_AT(int, entry, 0x00), delta);

        float excess = requestedTravel - CSF_AT(float, entry, 0x08);
        uint32_t maximumTravel = CSF_AT(uint32_t, entry, 0x08);
        CSF_AT(int, entry, 0x0C) = 3;
        CSF_AT(float, entry, 0x10) = excess;
        CSF_AT(uint32_t, entry, 0x14) = maximumTravel;
    }
    else if (requestedTravel > g_fZero)
    {
        float delta = requestedTravel - CSF_AT(float, entry, 0x14);
        AdvanceAxisEntry(reinterpret_cast<void*>(self->field_0030), nullptr,
                         CSF_AT(int, entry, 0x00), delta);

        if (self->field_0080 * CSF_AT(float, entry, 0x08) > requestedTravel)
        {
            CSF_AT(uint32_t, entry, 0x10) = 0;
            CSF_AT(int, entry, 0x0C) = 1;
        }
        else
        {
            CSF_AT(uint32_t, entry, 0x10) = 0;
            CSF_AT(int, entry, 0x0C) = 2;
        }
        CSF_AT(float, entry, 0x14) = requestedTravel;
    }
}

HOOK(0x00418660, C_0082e1e8_UpdateWheelTravel, "auto");
