#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(char, __fastcall, CGuiControl_RefreshQueryCandidates, 0x00451100, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiControl_EvaluateQueryCandidate, 0x00451E40, (int a1, int a2, float* a3, int a4, int a5, int a6, void* a7));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void __fastcall CGuiControl_QueryNearbyCandidates(
    CGuiControl* self, void* /*edx*/,
    void* result, void* query, int context, int flags, float radius)
{
    CSF_AT(uint32_t, result, 0x00) = 0;
    CSF_AT(uint32_t, result, 0x04) = 0;
    CSF_AT(uint32_t, result, 0x10) = 0;
    CSF_AT(float, result, 0x08) = FLT_MAX;
    CSF_AT(float, result, 0x0C) = FLT_MAX;

    CSF_AT(float, result, 0x20) = g_ZeroVectorX;
    CSF_AT(float, result, 0x24) = g_ZeroVectorY;
    CSF_AT(float, result, 0x28) = g_ZeroVectorZ;
    CSF_AT(float, result, 0x14) = g_ZeroVectorX;
    CSF_AT(float, result, 0x18) = g_ZeroVectorY;
    CSF_AT(float, result, 0x1C) = g_ZeroVectorZ;

    auto* resultVector = static_cast<uint8_t*>(result) + 0x14;
    CGuiControl_RefreshQueryCandidates(self, nullptr);

    uint32_t remaining = CSF_AT(uint32_t, self, 0x7C);
    if (remaining != 0)
    {
        const float radiusSquared = radius * radius;
        uint32_t offset = 0;
        do
        {
            auto* candidate = CSF_AT(uint8_t*, self, 0x74) + offset;
            if (CSF_AT(float, candidate, 0x04) <= radiusSquared)
            {
                CGuiControl_EvaluateQueryCandidate(
                    reinterpret_cast<int>(self),
                    reinterpret_cast<int>(query),
                    static_cast<float*>(result),
                    reinterpret_cast<int>(query),
                    context,
                    flags,
                    CSF_AT(void*, candidate, 0x00));
            }
            offset += 16;
            --remaining;
        } while (remaining != 0);
    }

    if (CSF_AT(float, query, 0x30) > g_fZero &&
        CSF_AT(uint32_t, result, 0x00) != 0)
    {
        void* hit = CSF_AT(void*, result, 0x00);
        void* object = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                CSF_AT(C_0082dd78*, hit, 0x4C), nullptr));

        uint32_t details[6] = {};
        details[3] = 10;
        details[5] = reinterpret_cast<uint32_t>(resultVector);

        const bool succeeded =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int, uint32_t*, int)>(
                CSF_AT(void**, object, 0)[0x40 / 4])(
                    object, nullptr, context, details, 0);

        void* data = reinterpret_cast<void*>(details[0]);
        if (succeeded)
        {
            CSF_AT(uint32_t, result, 0x10) = CSF_AT(uint32_t, data, 0x18);
            CSF_AT(uint32_t, resultVector, 0x00) = CSF_AT(uint32_t, data, 0x00);
            CSF_AT(uint32_t, resultVector, 0x04) = CSF_AT(uint32_t, data, 0x04);
            CSF_AT(uint32_t, resultVector, 0x08) = CSF_AT(uint32_t, data, 0x08);
        }
        if (data)
            Mem_Free(data);
    }
}

HOOK(0x004520D0, CGuiControl_QueryNearbyCandidates, "auto");
