#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ControlAssociationRadiansPerDegree, 0x00831144);

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, HasValidId, 0x004AD7F0, (void* self, void*));
CSF_FUNC(int*, __fastcall, C_Object_SelectBuffer, 0x004ADAD0, (int* self, void*));
CSF_FUNC(int, __fastcall, AiCore_IsTargetRecordValid, 0x004B2290, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiSearch_HasAttackTarget, 0x004B22B0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

static char __stdcall ApplyControlAssociation(int handle, CGuiControl* control)
{
    // These writes span the nominal GUI fields: the caller uses this as
    // an association record rather than as ordinary control state.
    *reinterpret_cast<std::uint32_t*>(&control->m_bShown) = -1;
    control->m_id.alloc = -1;

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (!resolved)
        return 0;

    auto* source = reinterpret_cast<C_008342e8*>(resolved);
    void* attached = reinterpret_cast<void*>(source->field_0034);
    if (!attached)
        return static_cast<char>(resolved);

    int type = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, attached, 0)[0x98 / 4])(attached, nullptr);
    if (type != 1)
        return static_cast<char>(type);

    // The virtual call may have changed the attached object.
    attached = reinterpret_cast<void*>(source->field_0034);
    void* aiObject = CSF_AT(void*, attached, 0x30C);
    if (!aiObject)
        return 0;

    auto* targetRecord = reinterpret_cast<std::uint8_t*>(aiObject) + 0x5DC;
    char valid = static_cast<char>(
        AiCore_IsTargetRecordValid(targetRecord, nullptr));
    if (!valid)
        return valid;

    valid = static_cast<char>(HasValidId(targetRecord + 0x28, nullptr));
    if (!valid)
        return valid;

    valid = AiSearch_HasAttackTarget(targetRecord, nullptr);
    if (!valid)
        return valid;

    void* scene = g_pCurrentScene;
    int* targetHandle = C_Object_SelectBuffer(
        reinterpret_cast<int*>(targetRecord), nullptr);
    resolved = CHandleTable_Resolve(
        scene, nullptr, reinterpret_cast<int>(targetHandle));
    if (!resolved)
        return 0;

    auto* target = reinterpret_cast<C_008342e8*>(resolved);
    float* sourcePosition = GetObjectPositionVector(source, nullptr);
    float* targetPosition = GetObjectPositionVector(target, nullptr);

    alignas(8) std::uint8_t facingBytes[0xC];
    auto* facing = reinterpret_cast<float*>(facingBytes);
    BuildFacingVectorBetween(sourcePosition, nullptr, facing, targetPosition);

    *reinterpret_cast<std::uint32_t*>(&control->m_bShown) = target->field_0020;
    control->m_id.alloc = target->field_0024;
    reinterpret_cast<float*>(&control->m_id.bx.ptr)[0] =
        static_cast<float>(Vector_GetLength(facing, nullptr));

    float heading = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(target, nullptr) *
        g_ControlAssociationRadiansPerDegree);
    float angle = static_cast<float>(S_00526d40_GetVectorAngleRadians(
        reinterpret_cast<S_00526d40*>(facingBytes), nullptr));
    reinterpret_cast<float*>(&control->m_id.bx.ptr)[1] =
        static_cast<float>(AngleDiffNormalized(angle, heading));

    // The original leaves only incidental EAX contents on this path.
    return 0;
}

HOOK(0x00448450, ApplyControlAssociation, "auto");
