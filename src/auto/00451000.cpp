#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(char, __fastcall, SceneSelection_TransitionObjects, 0x00458040,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));

static char __fastcall SceneSelection_CycleEligibleObject(
    unsigned int* self, void* /*edx*/, char backward)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x68);
    if (count < 2)
        return static_cast<char>(count);

    uint32_t forwardCandidate = static_cast<uint32_t>(-1);
    uint32_t backwardCandidate = static_cast<uint32_t>(-1);
    bool keepBackwardCandidate = false;
    bool foundAfterCurrent = false;

    uint32_t index = 0;
    do
    {
        void* object = CSF_AT(void**, self, 0x60)[index];
        if (C_GetByte023C(static_cast<char*>(object), nullptr) &&
            static_cast<char>(SceneObject_IsSecondCheckTrue(object, nullptr)))
        {
            const uint32_t current = CSF_AT(uint32_t, self, 0x70);
            if (index < current)
            {
                if (forwardCandidate == static_cast<uint32_t>(-1))
                    forwardCandidate = index;
                backwardCandidate = index;
            }

            if (index == current &&
                backwardCandidate != static_cast<uint32_t>(-1))
            {
                keepBackwardCandidate = true;
            }

            if (index > current)
            {
                if (!keepBackwardCandidate)
                    backwardCandidate = index;
                if (!foundAfterCurrent)
                {
                    forwardCandidate = index;
                    foundAfterCurrent = true;
                }
            }
        }
        ++index;
    } while (index < CSF_AT(uint32_t, self, 0x68));

    const uint32_t candidate = backward ? backwardCandidate : forwardCandidate;
    if (candidate == static_cast<uint32_t>(-1))
        return backward;

    const uint32_t current = CSF_AT(uint32_t, self, 0x70);
    if (candidate == current)
        return backward;

    void** objects = CSF_AT(void**, self, 0x60);
    void* nextObject = objects[candidate];
    void* currentObject = objects[current];

    const char eligible =
        static_cast<char>(SceneObject_IsSecondCheckTrue(nextObject, nullptr));
    if (!eligible)
        return eligible;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_457A80());
    return SceneSelection_TransitionObjects(
        manager, nullptr, currentObject, reinterpret_cast<int>(nextObject));
}

HOOK(0x00451000, SceneSelection_CycleEligibleObject, "auto");
