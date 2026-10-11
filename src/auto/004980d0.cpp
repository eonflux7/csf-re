#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, BuildSearchSegment, 0x0040FDA0,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0,
         (void* self, void* edx, int growth));
CSF_FUNC(uint32_t*, __fastcall, InitializeCandidateResult, 0x0044FAA0,
         (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiControl_QueryNearbyCandidates, 0x004520D0,
         (CGuiControl* self, void* , void* result, void* query, int context, int flags, float radius));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, EulerAnglesToDirection, 0x006AE640,
         (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void CPlayerCharacter_ClearNearbyCandidates(CPlayerCharacter* self)
{
    self->field_0244 = -1;
    self->field_0248 = -1;
    self->field_0250 = -1;
    self->field_0254 = -1;
    self->field_0258 = 0;
    std::memcpy(&self->field_025c, &g_ZeroVectorX, sizeof(float));
    self->field_0260 = g_ZeroVectorY;
    std::memcpy(&self->field_0264, &g_ZeroVectorZ, sizeof(float));
}

static void __fastcall CPlayerCharacter_UpdateNearbyCandidates(
    CPlayerCharacter* self, void* /*edx*/)
{
    if (reinterpret_cast<uint32_t>(self) !=
        GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    if ((self->field_0248 & 0x80000000) != 0)
        CPlayerCharacter_ClearNearbyCandidates(self);

    // Unordered comparisons also proceed in the original.
    if (self->field_020c < g_SoundActivityThreshold)
        return;

    self->field_020c = 0.0f;
    CPlayerCharacter_ClearNearbyCandidates(self);

    if ((self->field_02e0 & 0x2000) != 0)
        return;

    uint32_t result[0x2C / sizeof(uint32_t)];
    InitializeCandidateResult(result, nullptr);

    uint32_t query[0x34 / sizeof(uint32_t)];
    query[4] = 0;
    query[5] = 0;
    query[6] = 0;
    query[7] = 10;
    query[8] = 0;
    query[9] = 0;
    query[10] = 0;
    query[1] = -1;
    query[2] = -1;
    query[3] = -1;
    query[11] = -1;
    query[12] = 0;
    query[0] = 26;

    // The packet also lists this direct call address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);
    GrowButtonArray(query + 4, nullptr, 10);

    auto** excludedObjects = reinterpret_cast<CPlayerCharacter**>(query[4]);
    const uint32_t excludedIndex = query[6]++;
    excludedObjects[excludedIndex] = self;

    float direction[3];
    float positionBuffer[3];
    uint32_t angleBuffer = 0;
    uint32_t searchSegment[0x18 / sizeof(uint32_t)];

    uint32_t* angles = self->vSlotAC(&angleBuffer);
    float* searchDirection =
        EulerAnglesToDirection(angles, nullptr, direction);
    int position = self->vSlotA4(reinterpret_cast<int>(positionBuffer));
    BuildSearchSegment(searchSegment, nullptr,
                       reinterpret_cast<void*>(position),
                       reinterpret_cast<int>(searchDirection));

    int flags = 5;
    CGuiControl_QueryNearbyCandidates(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        result, query, reinterpret_cast<int>(searchSegment),
        reinterpret_cast<int>(&flags), self->field_03d8);

    auto* candidate = reinterpret_cast<C_00833908*>(result[1]);
    if (candidate)
    {
        self->field_0244 = candidate->field_0020;
        const uint32_t candidateId = candidate->field_0024;
        const uint32_t candidateDetail = result[3];
        self->field_0248 = candidateId;
        self->field_0258 = candidateDetail;

        const uint32_t pointIndex = result[0] ? 5 : 8;
        const uint32_t x = result[pointIndex];
        const uint32_t y = result[pointIndex + 1];
        const uint32_t z = result[pointIndex + 2];
        self->field_025c = x;
        std::memcpy(&self->field_0260, &y, sizeof(y));
        self->field_0264 = z;
    }
    else
    {
        flags = 64;
        CGuiControl_QueryNearbyCandidates(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
            result, query, reinterpret_cast<int>(searchSegment),
            reinterpret_cast<int>(&flags), self->field_03d8);

        candidate = reinterpret_cast<C_00833908*>(result[1]);
        if (candidate)
        {
            self->field_0250 = candidate->field_0020;
            self->field_0254 = candidate->field_0024;
        }
    }

    if (query[4])
        Mem_Free(reinterpret_cast<void*>(query[4]));
}

HOOK(0x004980D0, CPlayerCharacter_UpdateNearbyCandidates, "auto");
