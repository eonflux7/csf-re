#include <cstdint>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SceneQueries_InitializeFilter, 0x004011F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneQueries_ConstructRay, 0x0040FDA0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, SceneQueries_CheckSelectionLimit, 0x0044F1D0, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, SceneQueries_IsSelectionEligible, 0x0044F200, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneQueries_InitializeHits, 0x0044FAA0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetCachedSignedAngleToSceneSelection, 0x00450AD0, (void* self, void*));
CSF_FUNC(void, __fastcall, SceneQueries_UpdateClosestObjectHits, 0x00451E40, (void*, void*, float* results, int* query, void* origin, int collisionMask, C_008342e8* object));
CSF_FUNC(char*, __fastcall, GetEmbeddedBlockAt74, 0x00452F10, (S_004503e0* self, void*));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CPlayerCharacter_QuerySelectionFallback, 0x00497CD0, (void* self, void* edx, int a2, int* a3, float* a4, void* a5, int a6, int a7, int a8));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SelectionDegreesToRadians, 0x00833DA0);
CSF_GLOBAL(double, g_SelectionRetryInterval, 0x00833E30);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also identifies the growth entry point as a vtable binding.
static const auto g_ButtonArrayGrowthBinding = CSF_VTBL(0x0041E5F0);

static float ReadStoredFloat(uint32_t value)
{
    float result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static uint32_t StoreFloat(float value)
{
    uint32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static int __fastcall C_00833908_UpdateAngularSelection(
    C_00833908* self, void* /*edx*/, float requestedAngle)
{
    const uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (reinterpret_cast<C_00833908*>(context) != self)
        return static_cast<int>(context);

    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* service = CSF_AT(void*, manager, 0x18);
    void* input = CSF_AT(void*, service, 0x10);
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, input, 0)[0x18 / 4])(input, nullptr))
    {
        CSF_AT(int, self, 0x290) = -1;
        CSF_AT(int, self, 0x294) = -1;
        return -1;
    }

    float selectionDegrees;
    if ((CSF_AT(uint8_t, self, 0x2E0) & 0x40) != 0)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
        selectionDegrees = CSF_AT(float, settings, 0x5C);
    }
    else
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
        selectionDegrees = CSF_AT(float, settings, 0x1C);
    }

    if (selectionDegrees == g_fZero)
    {
        CSF_AT(int, self, 0x290) = -1;
        CSF_AT(int, self, 0x294) = -1;
        return -1;
    }

    void* owner = reinterpret_cast<void*>(self->field_0044);
    void* handle = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0xD8 / 4])(owner, nullptr);
    if (!handle || (CSF_AT(uint32_t, self, 0x2E0) & 0x40000) != 0)
    {
        CSF_AT(int, self, 0x290) = -1;
        CSF_AT(int, self, 0x294) = -1;
        return -1;
    }

    uint8_t* value = reinterpret_cast<uint8_t*>(Handle_GetValuePointer(handle, nullptr)) + 4;
    char* entries = GetEmbeddedBlockAt74(
        reinterpret_cast<S_004503e0*>(g_pCurrentScene), nullptr);

    // Uninitialized stack contents are represented by zero.
    // sized by the original's frame (0x134 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t selectionBytes[0x134] = {};
    S_0041e5f0& selection = *reinterpret_cast<S_0041e5f0*>(selectionBytes);
    selection.field_007c = static_cast<uint32_t>(-1);
    selection.field_0074 = static_cast<uint32_t>(-1);
    selection.field_0078 = StoreFloat(selectionDegrees * g_SelectionDegreesToRadians);
    selection.field_006c = StoreFloat(FLT_MAX);
    CSF_AT(float, &selection, 0x70) = -FLT_MAX;
    CSF_AT(uint32_t, &selection, 0x60) = CSF_AT(uint32_t, value, 0xE8);

    int filter[4]{};
    SceneQueries_InitializeFilter(filter, nullptr);
    filter[0] = 153;

    if (selection.field_0008 >= selection.field_0004)
        GrowButtonArray(&selection, nullptr, static_cast<int>(selection.field_000c));

    auto* excludedObjects = reinterpret_cast<C_00833908**>(selection.field_0000);
    const uint32_t exclusionIndex = selection.field_0008++;
    excludedObjects[exclusionIndex] = self;

    auto* position = reinterpret_cast<uint32_t*>(GetObjectPositionVector(self, nullptr));
    selection.field_0010 = position[0];
    selection.field_0014 = position[1];
    selection.field_0018 = position[2];
    selection.field_001c = 1;
    selection.field_0020 = selection.field_0078;
    selection.field_0064 = CSF_AT(uint32_t, entries, 8);

    struct CollisionState {
        int mask;
        float angle;
        uint32_t index;
    } collision{};

    float positiveHits[11]{};
    float negativeHits[11]{};
    alignas(4) uint8_t positiveRay[0x18]{};
    alignas(4) uint8_t negativeRay[0x18]{};
    float origin[3]{};

    uint32_t entryOffset = 0;
    if (selection.field_0064 != 0)
    {
        selection.field_0068 = 0;
        do
        {
            uint8_t* entry = CSF_AT(uint8_t*, entries, 0) + entryOffset;
            C_008342e8* object = CSF_AT(C_008342e8*, entry, 0);

            if (object->field_0050 == 1
                && SceneQueries_IsSelectionEligible(entry, nullptr)
                && static_cast<uint8_t>(SceneQueries_CheckSelectionLimit(
                    entry, nullptr, CSF_AT(float, &selection, 0x60))) != 0)
            {
                const double signedAngle =
                    GetCachedSignedAngleToSceneSelection(entry, nullptr);
                collision.angle = static_cast<float>(signedAngle);

                if (signedAngle > g_fZero)
                {
                    if (collision.angle < ReadStoredFloat(selection.field_006c))
                    {
                        SceneQueries_InitializeHits(positiveHits, nullptr);
                        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                            self->vtbl[0xA4 / 4])(
                                self, nullptr, &selection.field_0054);
                        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                            object->vtbl[0xA4 / 4])(
                                object, nullptr, &selection.field_0030);

                        selection.field_0024 = StoreFloat(
                            ReadStoredFloat(selection.field_0030)
                            - ReadStoredFloat(selection.field_0054));
                        selection.field_0028 = StoreFloat(
                            CSF_AT(float, &selection, 0x34)
                            - ReadStoredFloat(selection.field_0058));
                        selection.field_002c = StoreFloat(
                            CSF_AT(float, &selection, 0x38)
                            - ReadStoredFloat(selection.field_005c));

                        SceneQueries_ConstructRay(
                            positiveRay, nullptr, &selection.field_0054,
                            reinterpret_cast<int>(&selection.field_0024));
                        collision.mask = 5;
                        SceneQueries_UpdateClosestObjectHits(
                            g_pCurrentScene, nullptr, positiveHits, filter,
                            positiveRay, reinterpret_cast<int>(&collision.mask), object);
                        if (CSF_AT(uint32_t, positiveHits, 0) != 0)
                        {
                            selection.field_006c = StoreFloat(collision.angle);
                            selection.field_007c = collision.index;
                        }
                    }
                }
                else if (collision.angle > CSF_AT(float, &selection, 0x70))
                {
                    SceneQueries_InitializeHits(negativeHits, nullptr);
                    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                        self->vtbl[0xA4 / 4])(self, nullptr, origin);
                    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                        object->vtbl[0xA4 / 4])(
                            object, nullptr, &selection.pad_0034[8]);

                    selection.field_0048 = StoreFloat(
                        CSF_AT(float, &selection, 0x3C) - origin[0]);
                    selection.field_004c = StoreFloat(
                        ReadStoredFloat(selection.field_0040) - origin[1]);
                    selection.field_0050 = StoreFloat(
                        ReadStoredFloat(selection.field_0044) - origin[2]);

                    SceneQueries_ConstructRay(
                        negativeRay, nullptr, origin,
                        reinterpret_cast<int>(&selection.field_0048));
                    collision.mask = 5;
                    SceneQueries_UpdateClosestObjectHits(
                        g_pCurrentScene, nullptr, negativeHits, filter,
                        negativeRay, reinterpret_cast<int>(&collision.mask), object);
                    if (CSF_AT(uint32_t, negativeHits, 0) != 0)
                    {
                        CSF_AT(float, &selection, 0x70) = collision.angle;
                        selection.field_0074 = collision.index;
                    }
                }
            }

            entryOffset = selection.field_0068 + 16;
            ++collision.index;
            selection.field_0068 += 16;
        } while (collision.index < selection.field_0064);
    }

    C_00833908* selected = nullptr;
    if (requestedAngle >= 0.0)
    {
        if (selection.field_007c != static_cast<uint32_t>(-1))
        {
            const uint32_t offset = 16 * selection.field_007c;
            const double angle = GetCachedSignedAngleToSceneSelection(
                CSF_AT(uint8_t*, entries, 0) + offset, nullptr);
            if (!(angle - requestedAngle >= 0.0))
                selected = CSF_AT(C_00833908*, CSF_AT(uint8_t*, entries, 0), offset);
        }
    }
    else if (selection.field_0074 != static_cast<uint32_t>(-1))
    {
        const uint32_t offset = 16 * selection.field_0074;
        const double angle = GetCachedSignedAngleToSceneSelection(
            CSF_AT(uint8_t*, entries, 0) + offset, nullptr);
        if (!(angle - requestedAngle <= 0.0))
            selected = CSF_AT(C_00833908*, CSF_AT(uint8_t*, entries, 0), offset);
    }

    if (selected)
    {
        CSF_AT(int, self, 0x290) = -1;
        CSF_AT(int, self, 0x294) = -1;
        CSF_AT(uint32_t, self, 0x290) = selected->field_0020;
        CSF_AT(uint32_t, self, 0x294) = selected->field_0024;
    }
    else
    {
        if (CSF_AT(int, self, 0x294) < 0)
        {
            CSF_AT(int, self, 0x290) = -1;
            CSF_AT(int, self, 0x294) = -1;
        }

        if (CSF_AT(float, self, 0x214) >= g_SelectionRetryInterval)
        {
            CSF_AT(uint32_t, self, 0x214) = 0;
            CSF_AT(int, self, 0x290) = -1;
            CSF_AT(int, self, 0x294) = -1;
            origin[0] = 0.0;
            origin[1] = 0.0;
            origin[2] = 0.0;
            CPlayerCharacter_QuerySelectionFallback(
                self, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x28C),
                reinterpret_cast<int*>(&requestedAngle), origin, origin,
                static_cast<int>(selection.field_0078), 1, -1);
        }
    }

    const uint32_t allocation = selection.field_0000;
    if (allocation != 0)
        return Mem_Free(reinterpret_cast<void*>(allocation));
    return static_cast<int>(allocation);
}

HOOK(0x004983C0, C_00833908_UpdateAngularSelection, "auto");
