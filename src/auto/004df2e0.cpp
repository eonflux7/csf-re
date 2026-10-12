#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(void, __fastcall, C_004de770_LockAndAddRef, 0x004DE770, (void* self, void*));
CSF_FUNC(void, __fastcall, sub_4DE780, 0x004DE780, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __stdcall, sub_4DF0D0, 0x004DF0D0, (float* a1, float* a2, float* a3, float* a4, float* a5, float* a6));
CSF_FUNC(int, __fastcall, sub_4E08F0, 0x004E08F0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4E0910, 0x004E0910, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4E0B60, 0x004E0B60, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4E0B70, 0x004E0B70, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(void, __fastcall, sub_4E0E90, 0x004E0E90, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, sub_732150, 0x00732150, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Scene_CanTraverseSegment(
    void* self, void* /*edx*/, unsigned int segmentAddress)
{
    float* endpoint = reinterpret_cast<float*>(segmentAddress + 12);
    void* destination = reinterpret_cast<void*>(
        SceneFindSubObject(CSF_AT(void*, self, 0x4), nullptr,
                           segmentAddress + 12));

    if (!destination || (CSF_AT(uint8_t, destination, 0x4) & 1) != 0)
        return 0;

    void* current = reinterpret_cast<void*>(
        SceneFindSubObject(CSF_AT(void*, self, 0x4), nullptr,
                           segmentAddress));

    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t edgeEndBytes[0xC];
    alignas(8) uint8_t edgeStartBytes[0xC];
    alignas(8) uint8_t segmentParameterBytes[0x14];
    alignas(8) uint8_t edgeParameterBytes[0x8];

    float* position = reinterpret_cast<float*>(positionBytes);
    float* edgeStart = reinterpret_cast<float*>(edgeStartBytes);
    float* edgeEnd = reinterpret_cast<float*>(edgeEndBytes);
    float* segmentParameter = reinterpret_cast<float*>(segmentParameterBytes);
    float* edgeParameter = reinterpret_cast<float*>(edgeParameterBytes);

    const void* segment = reinterpret_cast<const void*>(segmentAddress);
    position[0] = CSF_AT(float, segment, 0x0);
    position[1] = CSF_AT(float, segment, 0x4);
    position[2] = CSF_AT(float, segment, 0x8);

    C_004de770_LockAndAddRef(
        reinterpret_cast<void*>(CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr)),
        nullptr);

    while (current &&
           (CSF_AT(uint8_t, current, 0x4) & 1) == 0 &&
           current != destination)
    {
        const uint32_t linkCount = CSF_AT(uint32_t, current, 0x8);
        bool reachedDestination = false;

        for (uint32_t index = 0; index < linkCount; ++index)
        {
            void* link = reinterpret_cast<void*>(
                sub_732150(current, nullptr, static_cast<int>(index)));
            void* linkedObject = reinterpret_cast<void*>(
                GetLinkedObjectValue_4E0B60(link, nullptr));
            if (linkedObject == destination)
            {
                current = destination;
                reachedDestination = true;
                break;
            }
        }

        if (reachedDestination)
            continue;

        sub_4E08F0(current, nullptr);
        sub_4E0E90(current, nullptr, position);

        bool crossedEdge = false;
        for (uint32_t index = 0; index < linkCount; ++index)
        {
            void* link = reinterpret_cast<void*>(
                sub_732150(current, nullptr, static_cast<int>(index)));
            void* linkedObject = reinterpret_cast<void*>(
                GetLinkedObjectValue_4E0B60(link, nullptr));

            if (linkedObject && !sub_4E0910(linkedObject, nullptr))
            {
                edgeStart[0] = 0.0f;
                edgeStart[1] = 0.0f;
                edgeStart[2] = 0.0f;
                edgeEnd[0] = 0.0f;
                edgeEnd[1] = 0.0f;
                edgeEnd[2] = 0.0f;

                sub_4E0B70(link, nullptr,
                           reinterpret_cast<uint32_t*>(edgeStartBytes),
                           reinterpret_cast<uint32_t*>(edgeEndBytes));

                if (static_cast<uint8_t>(sub_4DF0D0(
                        position, endpoint, edgeStart, edgeEnd,
                        segmentParameter, edgeParameter)) != 0)
                {
                    current = linkedObject;
                    crossedEdge = true;
                    position[0] = (edgeEnd[0] - edgeStart[0]) *
                                      edgeParameter[0] + edgeStart[0];
                    position[1] = (edgeEnd[1] - edgeStart[1]) *
                                      edgeParameter[0] + edgeStart[1];
                    position[2] = (edgeEnd[2] - edgeStart[2]) *
                                      edgeParameter[0] + edgeStart[2];
                }
            }

            if (crossedEdge)
                break;
        }

        if (!crossedEdge)
        {
            current = nullptr;
            break;
        }
    }

    sub_4DE780(
        reinterpret_cast<void*>(CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr)),
        nullptr);

    return current == destination ? 1 : 0;
}

HOOK(0x004DF2E0, Scene_CanTraverseSegment, "auto");
