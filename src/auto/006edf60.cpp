#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __stdcall, GfxCollision_TestDirectIntersection, 0x006EBD10,
         (uint32_t* a1, float* a2, float* a3));
CSF_FUNC(char, __fastcall, GfxCollision_RecordIntersection, 0x006ED350,
         (void* self, void* edx, float* a2, char a3));
CSF_FUNC(void, __fastcall, C_00862028_TestRegisteredObject, 0x006EDE50,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, C_00862028_TestWorldCollection, 0x006EDED0,
         (void* self, void* edx, int a2, int a3));

static int __fastcall C_00862028_QueryIntersection(
    C_00862028* self, void* /*edx*/, float* query,
    uint32_t* parameters, int* filter, unsigned int flags)
{
    bool testWorld = (flags & 1) != 0;
    const uint8_t mode = self->field_0004;
    bool testRegistered = (flags & 2) != 0;
    const bool modeWorld = ((flags >> (mode ? 2 : 4)) & 1) != 0;
    const bool modeRegistered = ((flags >> (mode ? 3 : 5)) & 1) != 0;
    if (modeWorld || modeRegistered)
    {
        testWorld = modeWorld;
        testRegistered = modeRegistered;
    }

    alignas(8) uint8_t contextBytes[0x18] = {};
    CSF_AT(uint8_t, contextBytes, 0x00) = mode;
    CSF_AT(int, contextBytes, 0x04) = *filter;
    CSF_AT(uint8_t, contextBytes, 0x09) = (flags & 0x40) == 0;
    CSF_AT(uint8_t, contextBytes, 0x0B) = (flags >> 8) & 1;

    void* result = reinterpret_cast<uint8_t*>(self) + 8;
    CSF_AT(void*, contextBytes, 0x0C) = result;
    CSF_AT(uint8_t, contextBytes, 0x08) = 0;
    CSF_AT(uint8_t, contextBytes, 0x0A) = (flags & 0x80) != 0;
    CSF_AT(C_00862028*, contextBytes, 0x10) = self;
    CSF_AT(uint8_t, contextBytes, 0x17) = testRegistered;

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[1])(
        result, nullptr);

    uint8_t parameterCopy[0x1C];
    std::memcpy(parameterCopy, parameters, sizeof(parameterCopy));
    std::memcpy(reinterpret_cast<uint8_t*>(result) + 0x1C,
                parameterCopy, 0x1C);
    self->field_0040 = 0;

    void* scene = reinterpret_cast<void*>(SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    // The original uses the filter argument's stack slot as the output.
    const uint32_t initialOutput = reinterpret_cast<uint32_t>(filter);
    float hitParameter;
    std::memcpy(&hitParameter, &initialOutput, sizeof(hitParameter));

    if (GfxCollision_TestDirectIntersection(
            reinterpret_cast<uint32_t*>(query),
            reinterpret_cast<float*>(parameters), &hitParameter) == 1)
    {
        alignas(8) uint8_t hitBytes[0x68] = {};
        CSF_AT(float, hitBytes, 0x00) = hitParameter;
        CSF_AT(float, hitBytes, 0x10) = query[3];
        CSF_AT(float, hitBytes, 0x14) = query[4];
        CSF_AT(float, hitBytes, 0x18) = query[5];
        CSF_AT(float, hitBytes, 0x1C) = query[1];
        CSF_AT(uint8_t, hitBytes, 0x20) = CSF_AT(uint8_t, query, 8);
        std::memset(hitBytes + 4, 0, 12);
        CSF_AT(int, hitBytes, 0x24) = 0;
        std::memcpy(hitBytes + 0x28, query, 64);
        GfxCollision_RecordIntersection(
            result, nullptr, reinterpret_cast<float*>(hitBytes),
            CSF_AT(char, query, 0));

        const uint8_t empty =
            ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[2])(
                result, nullptr);
        return empty == 0 ? reinterpret_cast<int>(result) : 0;
    }

    if (testWorld)
    {
        int collection;
        if (CSF_AT(C_00862028*, contextBytes, 0x10)->field_0004)
        {
            void* reference =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, scene, 0)[16])(
                    scene, nullptr);
            collection = reinterpret_cast<int>(
                GetReferencedObject(reference, nullptr));
        }
        else
        {
            void* reference =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, scene, 0)[17])(
                    scene, nullptr);
            collection = SceneLoader_GetWorldObjectCollection(reference, nullptr);
        }

        if (collection)
        {
            C_00862028_TestWorldCollection(
                CSF_AT(C_00862028*, contextBytes, 0x10), nullptr,
                collection, reinterpret_cast<int>(contextBytes));
        }

        int lastCollection = 0;
        const unsigned int count =
            ((unsigned int(__fastcall*)(void*, void*))CSF_AT(void**, scene, 0)[34])(
                scene, nullptr);

        for (unsigned int index = 0; index < count; ++index)
        {
            if (CSF_AT(C_00862028*, contextBytes, 0x10)->field_0004)
            {
                void* reference =
                    ((void*(__fastcall*)(void*, void*, unsigned int))
                         CSF_AT(void**, scene, 0)[35])(scene, nullptr, index);
                if (reference)
                    lastCollection = reinterpret_cast<int>(
                        GetReferencedObject(reference, nullptr));
            }
            else
            {
                void* reference =
                    ((void*(__fastcall*)(void*, void*, unsigned int))
                         CSF_AT(void**, scene, 0)[36])(scene, nullptr, index);
                if (reference)
                    lastCollection =
                        SceneLoader_GetWorldObjectCollection(reference, nullptr);
            }

            // A missing reference deliberately leaves the previous collection.
            if (lastCollection)
            {
                C_00862028_TestWorldCollection(
                    CSF_AT(C_00862028*, contextBytes, 0x10), nullptr,
                    lastCollection, reinterpret_cast<int>(contextBytes));
            }
        }
    }

    if (CSF_AT(uint8_t, contextBytes, 0x17))
    {
        C_00862028* owner = CSF_AT(C_00862028*, contextBytes, 0x10);
        if (owner->field_0004 && parameters[6] == 1)
        {
            const uint32_t count = owner->field_005c;
            for (uint32_t index = 0; index < count; ++index)
            {
                const int object =
                    reinterpret_cast<int*>(owner->field_0054)[index];
                C_00862028_TestRegisteredObject(
                    owner, nullptr, object, reinterpret_cast<int>(contextBytes));
            }
        }
    }

    const uint8_t empty =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[2])(
            result, nullptr);
    return empty == 0 ? reinterpret_cast<int>(result) : 0;
}

HOOK(0x006EDF60, C_00862028_QueryIntersection, "auto");
