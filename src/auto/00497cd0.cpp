#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(uint32_t*, __fastcall, InitializeNearbyCandidateResult, 0x0044FAA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __fastcall, CGuiControl_QueryNearbyCandidates, 0x004520D0, (CGuiControl* self, void* , void* result, void* query, int context, int flags, float radius));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(C_0082d448*, __fastcall, C_0082d448_InitDefaults, 0x004810B0, (C_0082d448* self, void*));
CSF_FUNC(int, __stdcall, CreateCandidateInteraction, 0x0048AA70, (uint32_t* arg0, float a3, int a4, int a2));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GetQueryOriginAndDirection, 0x00496490, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// The exclusions subobject begins at +0x10 and has 0x28 bytes of room.
struct NearbyInteractionQuery {
    int type;
    int filter;
    int option1;
    int option2;
    uint32_t* exclusions;
    uint32_t capacity;
    uint32_t count;
    int growth;
    uint32_t position[3];
    int mode;
    int flags;
    float radius;
};
static_assert(sizeof(NearbyInteractionQuery) == 0x38);
static_assert(offsetof(NearbyInteractionQuery, exclusions) == 0x10);

static uint32_t* __fastcall CPlayerCharacter_FindNearbyInteraction(
    CPlayerCharacter* self, void* /*edx*/, int outputHandle,
    int* outputKind, float* outputPosition, C_00833b88* outputDirection,
    int queryFlags, int option1, int option2)
{
    uint32_t* relatedObject = nullptr;
    uint32_t* providerObject;

    if ((self->field_01bc & 0x80000000) != 0) {
        void* owner = reinterpret_cast<void*>(self->field_0044);
        providerObject =
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0xD8 / 4])(owner, nullptr);
    } else {
        relatedObject = reinterpret_cast<uint32_t*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1B4)));
        if (!relatedObject)
            return relatedObject;

        void* owner = CSF_AT(void*, relatedObject, 0x44);
        if (!owner)
            return relatedObject;

        providerObject =
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, owner, 0)[0xC8 / 4])(owner, nullptr, 0);
    }

    if (!providerObject)
        return providerObject;

    void* value = reinterpret_cast<void*>(
        Handle_GetValuePointer(providerObject, nullptr));
    float radius = CSF_AT(float, value, 0xEC);
    if (CSF_AT(uint32_t, value, 0x74) == 10) {
        void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        radius = CSF_AT(float, globalValues, 0x1840);
    }

    float direction[3] = {0.0f, 0.0f, 0.0f};
    int origin[3] = {0, 0, 0};
    GetQueryOriginAndDirection(
        self, nullptr, reinterpret_cast<uint32_t*>(direction),
        reinterpret_cast<uint32_t*>(origin));

    uint32_t context[6];
    std::memcpy(context, origin, sizeof(origin));
    std::memcpy(context + 3, direction, sizeof(direction));
    NormalizeVector3(context, nullptr);

    NearbyInteractionQuery query;
    query.exclusions = nullptr;
    query.capacity = 0;
    query.count = 0;
    query.position[0] = 0;
    query.position[1] = 0;
    query.position[2] = 0;
    query.growth = 10;
    query.filter = -1;
    query.option1 = -1;
    query.option2 = -1;
    query.mode = -1;
    query.flags = 0;
    query.type = 21;
    query.radius = radius;

    uint32_t* position = reinterpret_cast<uint32_t*>(
        GetObjectPositionVector(self, nullptr));
    query.position[0] = position[0];
    query.position[1] = position[1];
    uint32_t positionZ = position[2];
    query.option1 = option1;
    query.option2 = option2;
    query.position[2] = positionZ;
    query.mode = 1;
    query.flags = queryFlags;

    if (query.count >= query.capacity) {
        (void)CSF_VTBL(0x0041E5F0);
        GrowButtonArray(&query.exclusions, nullptr, query.growth);
    }
    uint32_t* entry = &query.exclusions[query.count++];
    *entry = reinterpret_cast<uint32_t>(self);

    if (relatedObject) {
        if (query.count >= query.capacity) {
            (void)CSF_VTBL(0x0041E5F0);
            GrowButtonArray(&query.exclusions, nullptr, query.growth);
        }
        entry = &query.exclusions[query.count++];
        *entry = reinterpret_cast<uint32_t>(relatedObject);
    }

    uint32_t candidateResult[11] = {};
    InitializeNearbyCandidateResult(candidateResult, nullptr);
    option1 = 5;
    CGuiControl_QueryNearbyCandidates(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        candidateResult, &query, reinterpret_cast<int>(context),
        reinterpret_cast<int>(&option1), query.radius);

    if (candidateResult[0]) {
        C_0082d448 request;
        C_0082d448_InitDefaults(&request, nullptr);
        uint32_t objectMode = self->field_0078;
        uint32_t handlePart2 = self->field_0024;
        uint32_t handlePart1 = self->field_0020;
        request.field_0008 = handlePart2;
        request.field_0004 = handlePart1;
        request.field_0028 = objectMode != 1;

        void* cachedProvider = GetCachedObjectProvider(providerObject, nullptr);
        int interaction = CreateCandidateInteraction(
            reinterpret_cast<uint32_t*>(candidateResult[0]),
            CSF_AT(float, candidateResult, 0x08),
            reinterpret_cast<int>(cachedProvider),
            reinterpret_cast<int>(&request));
        self->field_026c = interaction;

        if (interaction) {
            uint32_t* candidate =
                reinterpret_cast<uint32_t*>(candidateResult[0]);
            CSF_AT(uint32_t, outputHandle, 0x04) =
                CSF_AT(uint32_t, candidate, 0x20);
            uint32_t kind = candidateResult[4];
            CSF_AT(uint32_t, outputHandle, 0x08) =
                CSF_AT(uint32_t, candidate, 0x24);
            *outputKind = static_cast<int>(kind);

            outputPosition[0] = CSF_AT(float, candidateResult, 0x14);
            outputPosition[1] = CSF_AT(float, candidateResult, 0x18);
            outputPosition[2] = CSF_AT(float, candidateResult, 0x1C);

            float* objectPosition = reinterpret_cast<float*>(
                self->vSlotA4(reinterpret_cast<int>(origin)));
            direction[0] =
                CSF_AT(float, candidateResult, 0x14) - objectPosition[0];
            direction[1] =
                CSF_AT(float, candidateResult, 0x18) - objectPosition[1];
            direction[2] =
                CSF_AT(float, candidateResult, 0x1C) - objectPosition[2];

            std::memcpy(&outputDirection->vtbl, &direction[0], sizeof(float));
            std::memcpy(&outputDirection->field_0004, &direction[1], sizeof(float));
            CSF_AT(float, outputDirection, 0x08) = direction[2];
            NormalizeVector3(outputDirection, nullptr);
        }
    }

    if (query.exclusions)
        return reinterpret_cast<uint32_t*>(Mem_Free(query.exclusions));
    return query.exclusions;
}

HOOK(0x00497CD0, CPlayerCharacter_FindNearbyInteraction, "auto");
