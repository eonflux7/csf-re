#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CollectBoxEntries, 0x0048AE50,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, S_006abb80_WriteBoxCorners, 0x006ABC40,
         (S_006abb80* self, void* , float* corners));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static int __stdcall FindNearestAvailableBoxFaceId(void* owner, float* position)
{
    alignas(8) uint8_t boxBytes[0x4C] = {};
    alignas(8) uint8_t entriesBytes[0x60];
    alignas(8) uint8_t cornersBytes[0x6C];

    int& selectedId = CSF_AT(int, entriesBytes, 0x5C);
    selectedId = 0;

    void* source = CSF_AT(C_0082dd78*, owner, 0x4C);
    if (!source)
        return 0;

    void* object = C_0082dd78_DispatchReturnedObject(source, nullptr);
    if (!object)
        return 0;

    uint32_t* description =
        reinterpret_cast<uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x38 / 4])(object, nullptr);
    CollectBoxEntries(entriesBytes, nullptr, description);

    uint32_t remaining = CSF_AT(uint32_t, entriesBytes, 0x08);
    float* corners = reinterpret_cast<float*>(cornersBytes);
    float* faceCenters = reinterpret_cast<float*>(entriesBytes + 0x10);

    for (int i = 0; i < 24; ++i)
        corners[i] = 0.0f;
    for (int i = 0; i < 18; ++i)
        faceCenters[i] = 0.0f;

    float& bestDistance = CSF_AT(float, cornersBytes, 0x60);
    bestDistance = -1.0f;

    int result = 0;
    if (remaining != 0)
    {
        uint32_t* entry = CSF_AT(uint32_t*, entriesBytes, 0);
        do
        {
            void* boxObject = reinterpret_cast<void*>(entry[0]);
            CSF_AT(uint32_t, boxBytes, 0x40) = entry[1];
            int candidateId = static_cast<int32_t>(entry[2]);
            CSF_AT(uint32_t, boxBytes, 0x48) = entry[3];

            S_006abb80* box =
                reinterpret_cast<S_006abb80* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, boxObject, 0)[0x1C / 4])(
                        boxObject, nullptr, boxBytes);
            S_006abb80_WriteBoxCorners(box, nullptr, corners);

            for (int axis = 0; axis < 3; ++axis)
            {
                faceCenters[axis] =
                    (corners[3 + axis] + corners[6 + axis] +
                     corners[9 + axis] + corners[axis]) * g_ValueScale;

                float oppositeSum = corners[15 + axis] + corners[18 + axis];
                faceCenters[3 + axis] =
                    (corners[21 + axis] + oppositeSum + corners[12 + axis]) *
                    g_ValueScale;
                faceCenters[6 + axis] =
                    (oppositeSum + corners[3 + axis] + corners[6 + axis]) *
                    g_ValueScale;
                faceCenters[9 + axis] =
                    (corners[21 + axis] + corners[12 + axis] +
                     corners[9 + axis] + corners[axis]) * g_ValueScale;
                faceCenters[12 + axis] =
                    (corners[18 + axis] + corners[21 + axis] +
                     corners[6 + axis] + corners[9 + axis]) * g_ValueScale;
                faceCenters[15 + axis] =
                    (corners[15 + axis] + corners[12 + axis] +
                     corners[3 + axis] + corners[axis]) * g_ValueScale;
            }

            for (int face = 0; face < 6; ++face)
            {
                float dy = position[1] - faceCenters[face * 3 + 1];
                float dz = position[2] - faceCenters[face * 3 + 2];
                float dx = position[0] - faceCenters[face * 3];
                float distance = dz * dz + dy * dy + dx * dx;

                if ((distance < bestDistance ||
                     bestDistance == g_MissingMetricValue) &&
                    candidateId != -1)
                {
                    void* availability = CSF_AT(void*, owner, 0x38);
                    int available =
                        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, availability, 0)[0xAC / 4])(
                                availability, nullptr, candidateId);
                    if (available > 0)
                    {
                        bestDistance = distance;
                        selectedId = candidateId;
                    }
                }
            }

            entry += 4;
            --remaining;
        } while (remaining != 0);

        result = selectedId;
    }

    if (CSF_AT(void*, entriesBytes, 0))
        Mem_Free(CSF_AT(void*, entriesBytes, 0));
    return result;
}

HOOK(0x0048AEE0, FindNearestAvailableBoxFaceId, "auto");
