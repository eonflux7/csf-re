#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __stdcall, GfxCollision_TestQueryBounds, 0x006EBD10, (uint32_t* a1, float* a2, float* a3));
CSF_FUNC(char, __fastcall, S_006ebe90_TestWorldObject, 0x006EBE90, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, S_006ebe90_TestAdditionalObject, 0x006EBEF0, (void* self, void* edx, int a2, int a3));

static char __fastcall S_006ebe90_TestSceneIntersection(
    S_006ebe90* self, void* /*edx*/, uint32_t* bounds, int query, unsigned int flags)
{
    // The packet also labels the direct-call address as a vtable reference.
    (void)CSF_VTBL(0x006EBE90);

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    bool testScene = (flags & 1) != 0;
    bool testAdditional = (flags & 2) != 0;

    alignas(8) uint8_t scratch[0xC]{};
    CSF_AT(char, scratch, 0xB) = 0;

    unsigned int sceneOverride;
    unsigned int additionalOverride;
    if (self->field_0004 == 0)
    {
        sceneOverride = (flags >> 4) & 1;
        additionalOverride = (flags >> 5) & 1;
    }
    else
    {
        sceneOverride = (flags >> 2) & 1;
        additionalOverride = (flags >> 3) & 1;
    }

    if (sceneOverride || additionalOverride)
    {
        testScene = sceneOverride != 0;
        testAdditional = additionalOverride != 0;
    }

    char boundsResult = GfxCollision_TestQueryBounds(
        bounds, reinterpret_cast<float*>(query),
        reinterpret_cast<float*>(scratch));
    if (boundsResult == 1)
        return boundsResult;

    if (testScene)
    {
        int worldObject;
        if (self->field_0004)
        {
            void* reference =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneController, 0)[0x40 / 4])(
                        sceneController, nullptr);
            worldObject = reinterpret_cast<int>(
                GetReferencedObject(reference, nullptr));
        }
        else
        {
            void* loader =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneController, 0)[0x44 / 4])(
                        sceneController, nullptr);
            worldObject = SceneLoader_GetWorldObjectCollection(loader, nullptr);
        }

        uint32_t index = 0;
        if (worldObject)
        {
            CSF_AT(char, scratch, 0xB) =
                S_006ebe90_TestWorldObject(self, nullptr, worldObject, query);
        }

        if (!worldObject || !CSF_AT(char, scratch, 0xB))
        {
            int candidate = 0;
            CSF_AT(uint32_t, scratch, 0x4) =
                ((uint32_t(__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneController, 0)[0x88 / 4])(
                        sceneController, nullptr);

            do
            {
                if (index >= CSF_AT(uint32_t, scratch, 0x4))
                    break;

                if (self->field_0004)
                {
                    void* reference =
                        ((void*(__fastcall*)(void*, void*, uint32_t))
                            CSF_AT(void**, sceneController, 0)[0x8C / 4])(
                                sceneController, nullptr, index);
                    if (reference)
                    {
                        candidate = reinterpret_cast<int>(
                            GetReferencedObject(reference, nullptr));
                    }
                }
                else
                {
                    void* loader =
                        ((void*(__fastcall*)(void*, void*, uint32_t))
                            CSF_AT(void**, sceneController, 0)[0x90 / 4])(
                                sceneController, nullptr, index);
                    if (loader)
                    {
                        candidate =
                            SceneLoader_GetWorldObjectCollection(loader, nullptr);
                    }
                }

                // A null entry deliberately retains the previous candidate.
                if (candidate)
                {
                    CSF_AT(char, scratch, 0xB) =
                        S_006ebe90_TestWorldObject(
                            self, nullptr, candidate, query);
                }
                ++index;
            }
            while (!CSF_AT(char, scratch, 0xB));
        }
    }

    if (testAdditional && !CSF_AT(char, scratch, 0xB))
    {
        if (self->field_0004)
        {
            if (CSF_AT(uint32_t, reinterpret_cast<void*>(query), 0x18) == 1)
            {
                uint32_t count = CSF_AT(uint32_t, self, 0x5C);
                for (uint32_t index = 0; index < count; ++index)
                {
                    int object = reinterpret_cast<int*>(
                        CSF_AT(uint32_t, self, 0x54))[index];
                    if (S_006ebe90_TestAdditionalObject(
                            self, nullptr, object, query))
                    {
                        CSF_AT(char, scratch, 0xB) = 1;
                        break;
                    }
                }
            }
        }
    }

    return CSF_AT(char, scratch, 0xB);
}

HOOK(0x006EC0B0, S_006ebe90_TestSceneIntersection, "auto");
