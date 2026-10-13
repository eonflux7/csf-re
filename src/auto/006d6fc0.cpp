#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_AttachVisibilityProxy, 0x006D6EE0, (void* self, void* edx, void* a2));
CSF_FUNC(uint8_t*, __fastcall, ClumpLoader_CopySixDwordsAndMarkValid, 0x006FE390, (void* self, void* , const uint32_t* src));
CSF_FUNC(char, __fastcall, SetByteProperty22, 0x006FE3D0, (void* self, void* , char value));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_SetSceneVector, 0x007FAFE0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwImageSetHeight, 0x007FB000, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwImageSetWidth, 0x007FB010, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpToonInkSetPerspectiveScale, 0x007FB020, (int a1, float a2));
CSF_FUNC(int, __cdecl, RpToonInkSetNearDistance, 0x007FB030, (int a1, float a2));
CSF_FUNC(int, __cdecl, RpToonInkSetFarScale, 0x007FB040, (int a1, float a2));
CSF_FUNC(int, __cdecl, RpToonInkSetFarPerspectiveScale, 0x007FB050, (int a1, float a2));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_CreateRenderHandle, 0x007FD8D0, (int a2, float a3, int a4));
CSF_FUNC(int, __cdecl, RtDictGetSchema, 0x007FDAD0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RtBinMtxCreate, 0x007FDB00, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RpTeamPlayerSetHAnimHierarchy, 0x007FF0A0, (uint32_t* a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RtWorldImportParametersInit, 0x007FFB90, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_SetImportSectorLimits, 0x007FFBF0, (uint32_t* a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16));
CSF_FUNC(int, __cdecl, RtWorldImportParametersSetMaxSectorSize, 0x007FFC20, (int a1, int a2));
CSF_FUNC(float*, __cdecl, Gfx2d_ScaleBounds, 0x008006D0, (float* a1, float a2));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_SetImportPrimaryPair, 0x0081BCE0, (uint32_t* a1, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_SetImportSecondaryPair, 0x0081CFD0, (uint32_t* a1, int a2, int a3));

static uint32_t* __fastcall C_00863ac0_ActivateRenderHandle(
    C_00863ac0* self, void* /*edx*/)
{
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* scene = reinterpret_cast<void*>(
        (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x44 / 4]))(controller, nullptr));

    uint32_t* world = reinterpret_cast<uint32_t*>(
        SceneLoader_GetWorldObjectCollection(scene, nullptr));
    if (!world)
        return world;

    alignas(8) uint8_t parameterBytes[0x7C];
    alignas(8) uint8_t boundsBytes[0x20];
    uint32_t* parameters = reinterpret_cast<uint32_t*>(parameterBytes);
    uint32_t* bounds = reinterpret_cast<uint32_t*>(boundsBytes);

    bounds[0] = CSF_AT(uint32_t, world, 0x50);
    bounds[1] = CSF_AT(uint32_t, world, 0x54);
    bounds[3] = 0;
    bounds[2] = CSF_AT(uint32_t, world, 0x58);
    bounds[4] = CSF_AT(uint32_t, world, 0x5C);
    bounds[5] = CSF_AT(uint32_t, world, 0x60);
    bounds[7] = 0;
    bounds[6] = CSF_AT(uint32_t, world, 0x64);

    const float boundsScale = 1.2f;
    Gfx2d_ScaleBounds(reinterpret_cast<float*>(boundsBytes), boundsScale);
    RtWorldImportParametersInit(parameters);

    const int primarySecond = static_cast<int>(self->field_0008);
    const int primaryFirst = static_cast<int>(self->field_0004);
    Gfx2d_SetImportPrimaryPair(parameters, primaryFirst, primarySecond);

    const int secondarySecond = static_cast<int>(self->field_0010);
    const int secondaryFirst = static_cast<int>(self->field_000c);
    Gfx2d_SetImportSecondaryPair(parameters, secondaryFirst, secondarySecond);

    RtWorldImportParametersSetMaxSectorSize(
        reinterpret_cast<int>(parameters), 150);

    // The binding exposes the previous calls' still-pending stack arguments.
    // Preserve those defined values, including the float argument representation.
    int scaleArgument;
    std::memcpy(&scaleArgument, &boundsScale, sizeof(scaleArgument));
    Gfx2d_SetImportSectorLimits(
        parameters, 100, 400, 500, 50,
        reinterpret_cast<int>(parameters), 150,
        reinterpret_cast<int>(parameters), secondaryFirst, secondarySecond,
        reinterpret_cast<int>(parameters), primaryFirst, primarySecond,
        reinterpret_cast<int>(parameters), reinterpret_cast<int>(bounds),
        scaleArgument);

    uint32_t* renderHandle = Gfx2d_CreateRenderHandle(
        reinterpret_cast<int>(parameters), 120.0f,
        reinterpret_cast<int>(bounds));
    CSF_AT(uint32_t, self, 0x3C) = reinterpret_cast<uint32_t>(renderHandle);

    RwImageSetHeight(
        Rt2dSceneGetChildCount(reinterpret_cast<int>(renderHandle)), 300);
    RwImageSetWidth(
        Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C)), 200);
    Gfx2d_SetSceneVector(
        reinterpret_cast<uint32_t*>(
            Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C))),
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0xDC));
    RpToonInkSetPerspectiveScale(
        Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C)), 7.0f);
    RpToonInkSetFarScale(
        Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C)), 0.2f);
    RpToonInkSetNearDistance(
        Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C)), 200.0f);
    RpToonInkSetFarPerspectiveScale(
        Rt2dSceneGetChildCount(CSF_AT(int, self, 0x3C)), 5000.0f);

    SceneLoader_AttachVisibilityProxy(self, nullptr, scene);

    uint32_t* matrix = RtBinMtxCreate(3, 3);
    CSF_AT(uint32_t, self, 0x50) = reinterpret_cast<uint32_t>(matrix);
    uint32_t bit = CSF_AT(uint32_t, matrix, 0x04) + 1;
    CSF_AT(uint32_t, matrix, 0x0C + 4 * (bit >> 5)) |=
        uint32_t{1} << (bit & 0x1F);

    matrix = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x50));
    bit = 2 * CSF_AT(uint32_t, matrix, 0x04);
    CSF_AT(uint32_t, matrix, 0x0C + 4 * (bit >> 5)) |=
        uint32_t{1} << (bit & 0x1F);

    matrix = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x50));
    bit = 2 * CSF_AT(uint32_t, matrix, 0x04) + 1;
    CSF_AT(uint32_t, matrix, 0x0C + 4 * (bit >> 5)) |=
        uint32_t{1} << (bit & 0x1F);

    matrix = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x50));
    bit = 2 * CSF_AT(uint32_t, matrix, 0x04) + 2;
    CSF_AT(uint32_t, matrix, 0x0C + 4 * (bit >> 5)) |=
        uint32_t{1} << (bit & 0x1F);

    const int hierarchy = CSF_AT(int, self, 0x50);
    uint32_t* schema = reinterpret_cast<uint32_t*>(
        RtDictGetSchema(CSF_AT(int, self, 0x3C)));
    RpTeamPlayerSetHAnimHierarchy(schema, hierarchy);

    controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* properties = reinterpret_cast<void*>(
        CSF_AT(uint32_t, controller, 0xBC));
    void* localProperties = reinterpret_cast<uint8_t*>(self) + 0x74;

    ClumpLoader_CopySixDwordsAndMarkValid(
        localProperties, nullptr, &self->field_0024);
    const char result = SetByteProperty22(
        localProperties, nullptr, CSF_AT(char, properties, 0x01));
    CSF_AT(uint8_t, self, 0xD9) = 1;

    return reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(static_cast<uint8_t>(result)));
}

HOOK(0x006D6FC0, C_00863ac0_ActivateRenderHandle, "auto");
