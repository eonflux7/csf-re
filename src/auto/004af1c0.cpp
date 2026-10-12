#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AttackTasks_SetCoverAttackPosition, 0x0048CA40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* , float angle));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);

static void __fastcall AiStatic_UpdateCoverAttack(void* self, void* /*edx*/)
{
    auto* link = static_cast<std::uint8_t*>(self) + 8;

    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    auto* owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

    // The original also proceeds when the comparison is unordered.
    if (CSF_AT(float, owner, 0x36C) > g_fZero)
        return;

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
    CSF_AT(int, owner, 0x368) = 1;
    CSF_AT(int, owner, 0x370) = 0;
    CSF_AT(int, owner, 0x374) = 0;
    CSF_AT(int, owner, 0x378) = -1073345902;
    CSF_AT(int, owner, 0x37C) = 1074137746;
    CSF_AT(int, owner, 0x380) = 1140457472;

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
    auto* behavior = owner + 0x364;
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, behavior, 0)[0x84 / 4])(behavior, nullptr, link);

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
    if (CSF_AT(int, owner, 0x368) == 3)
    {
        alignas(8) std::uint8_t matrixBytes[0x8];
        S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

        matrix.field_0000 =
            reinterpret_cast<decltype(matrix.field_0000)>(RwMatrixCreate());
        CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;
        GfxMatrix_SetScaleAndUpdate(
            &matrix, nullptr, reinterpret_cast<const uint32_t*>(&g_AxisZ));

        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
        GfxMatrix_SetYRotation(&matrix, nullptr, CSF_AT(float, owner, 0x384));

        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        auto* positionOwner =
            reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        auto* attackOwner =
            reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

        void* positionObject = CSF_AT(void*, positionOwner, 0x2C);
        auto* direction = reinterpret_cast<float*>(
            reinterpret_cast<std::uint8_t*>(matrix.field_0000) + 0x20);
        float* position = GetObjectPositionVector(positionObject, nullptr);

        alignas(8) std::uint8_t attackPositionBytes[0xC];
        float z = direction[2] + position[2];
        float y = direction[1] + position[1];
        CSF_AT(float, attackPositionBytes, 0x0) = direction[0] + position[0];
        CSF_AT(float, attackPositionBytes, 0x4) = y;
        CSF_AT(float, attackPositionBytes, 0x8) = z;

        void* attackObject = CSF_AT(void*, attackOwner, 0x2C);
        void* attackTasks = CSF_AT(void*, attackObject, 0x204);
        AttackTasks_SetCoverAttackPosition(
            attackTasks, nullptr, reinterpret_cast<uint32_t*>(attackPositionBytes));

        if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix.field_0000));
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
    CSF_AT(float, owner, 0x36C) = 5.0f;
}

HOOK(0x004AF1C0, AiStatic_UpdateCoverAttack, "auto");
