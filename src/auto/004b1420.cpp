#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndResetFlags, 0x0047BF90, (C_0082f9f8* self, void* , int));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndInvoke, 0x0047C3A0, (C_0082f9f8* self, void* , int a2, float a3, int a4));
CSF_FUNC(void, __fastcall, S_0048c9f0_CopyTargetHandle, 0x0048C9F0, (S_0048c9f0* self, void* , const void* handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_IsCoverActionComplete, 0x004AA110, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AiFncMacroOrder_GetLinkedActor(void* link)
{
    const int value = GetLinkedObjectValue_4DB5C0(link, nullptr);
    return value ? reinterpret_cast<void*>(value - 4) : nullptr;
}

static void __fastcall TFncMacroOrden_UpdateLinkedTask(
    TFncMacroOrden* self, void* /*edx*/, void* link,
    int updateContext, char checkEligibility)
{
    // The packet also identifies these callee entries as vtable references.
    (void)CSF_VTBL(0x0047AE10);
    (void)CSF_VTBL(0x0048C9F0);

    if (!CSF_AT(uint8_t, self, 0xB0))
        return;

    bool updateTask = true;
    void* actor = AiFncMacroOrder_GetLinkedActor(link);
    if (CSF_AT(int, actor, 0x608) == 14)
    {
        actor = AiFncMacroOrder_GetLinkedActor(link);
        uint8_t* selectedTarget = static_cast<uint8_t*>(actor)
            + (CSF_AT(int, actor, 0x630) < 0 ? 0x610 : 0x628);
        CSF_AT(uint32_t, self, 0x98) = CSF_AT(uint32_t, selectedTarget, 0x4);
        CSF_AT(uint32_t, self, 0x9C) = CSF_AT(uint32_t, selectedTarget, 0x8);
    }

    actor = AiFncMacroOrder_GetLinkedActor(link);
    void* targetHandle = reinterpret_cast<uint8_t*>(self) + 0x94;
    auto* targetData = reinterpret_cast<S_0048c9f0*>(
        AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, actor, 0x718), nullptr));
    AiCore_UpdateTargetData(targetData, nullptr, reinterpret_cast<int>(targetHandle));

    if (checkEligibility)
    {
        actor = AiFncMacroOrder_GetLinkedActor(link);
        void* state = CSF_AT(void*, actor, 0x2C);
        if (CSF_AT(int, state, 0x1C4) == 1)
        {
            updateTask = false;
        }
        else
        {
            actor = AiFncMacroOrder_GetLinkedActor(link);
            state = CSF_AT(void*, actor, 0x2C);
            if (!CSF_AT(int, state, 0x1E0))
                updateTask = false;
        }
    }

    actor = AiFncMacroOrder_GetLinkedActor(link);
    void* state = CSF_AT(void*, actor, 0x2C);
    void* action = CSF_AT(void*, state, 0x48);
    const int actionActive =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, action, 0)[0x40 / 4])(action, nullptr);

    if (!actionActive
        && CSF_AT(uint8_t, self, 0xB1)
        && CSF_AT(int, self, 0x9C) >= 0)
    {
        actor = AiFncMacroOrder_GetLinkedActor(link);
        if (CSF_AT(int, actor, 0x38C))
        {
            actor = AiFncMacroOrder_GetLinkedActor(link);
            void* coverAction = static_cast<uint8_t*>(actor) + 0x388;
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, coverAction, 0)[0x2C / 4])(
                    coverAction, nullptr, link, updateContext);

            actor = AiFncMacroOrder_GetLinkedActor(link);
            coverAction = static_cast<uint8_t*>(actor) + 0x388;
            if (static_cast<uint8_t>(
                    AiCoverAttackTasks_IsCoverActionComplete(coverAction, nullptr)))
            {
                actor = AiFncMacroOrder_GetLinkedActor(link);
                state = CSF_AT(void*, actor, 0x2C);
                void* attack = CSF_AT(void*, state, 0x44);
                if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, attack, 0)[0x90 / 4])(attack, nullptr))
                {
                    actor = AiFncMacroOrder_GetLinkedActor(link);
                    if (CSF_AT(int, actor, 0x38C) == 3)
                    {
                        void* target = reinterpret_cast<void*>(
                            CHandleTable_Resolve(
                                g_pCurrentScene, nullptr,
                                reinterpret_cast<int>(targetHandle)));
                        if (target)
                        {
                            float* position = GetObjectPositionVector(target, nullptr);
                            self->field_00b4 = CSF_AT(uint32_t, position, 0);
                            CSF_AT(uint32_t, self, 0xB8) =
                                CSF_AT(uint32_t, position, 0x4);
                            CSF_AT(uint32_t, self, 0xBC) =
                                CSF_AT(uint32_t, position, 0x8);

                            void* movementActor = AiFncMacroOrder_GetLinkedActor(link);
                            actor = AiFncMacroOrder_GetLinkedActor(link);
                            auto* movement =
                                CSF_AT(C_0082f9f8*, actor, 0x718);
                            C_0082f9f8_StartMovementAndInvoke(
                                movement, nullptr,
                                reinterpret_cast<int>(
                                    static_cast<uint8_t*>(movementActor) + 0x3A8),
                                50.0f, 1120403456);
                        }
                    }

                    actor = AiFncMacroOrder_GetLinkedActor(link);
                    CSF_AT(int, actor, 0x38C) = 0;
                }
            }
        }
        else
        {
            void* target = reinterpret_cast<void*>(
                CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(targetHandle)));
            if (target)
            {
                float* position = GetObjectPositionVector(target, nullptr);
                alignas(8) uint8_t facingBytes[0x14];
                float* facing = BuildFacingVectorBetween(
                    position, nullptr, reinterpret_cast<float*>(facingBytes),
                    reinterpret_cast<float*>(
                        reinterpret_cast<uint8_t*>(self) + 0xB4));

                if (Vector_GetLength(facing, nullptr) > g_MaxPercent)
                {
                    CSF_AT(uint8_t, self, 0xB1) = 0;
                    actor = AiFncMacroOrder_GetLinkedActor(link);
                    C_0082f9f8_StartMovementAndResetFlags(
                        CSF_AT(C_0082f9f8*, actor, 0x718), nullptr,
                        reinterpret_cast<int>(targetHandle));
                }
            }
        }
    }

    if (updateTask)
    {
        actor = AiFncMacroOrder_GetLinkedActor(link);
        void* task = CSF_AT(void*, actor, 0x718);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, task, 0)[0xA4 / 4])(task, nullptr))
        {
            actor = AiFncMacroOrder_GetLinkedActor(link);
            C_0082f9f8_StartMovementAndResetFlags(
                CSF_AT(C_0082f9f8*, actor, 0x718), nullptr,
                reinterpret_cast<int>(targetHandle));
        }

        if (CSF_AT(int, self, 0x9C) >= 0)
        {
            actor = AiFncMacroOrder_GetLinkedActor(link);
            state = CSF_AT(void*, actor, 0x2C);
            void* targetSelector = CSF_AT(void*, state, 0x3C);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, const void*, int)>(
                    CSF_AT(void**, targetSelector, 0)[0x9C / 4])(
                        targetSelector, nullptr, targetHandle, 14))
            {
                actor = AiFncMacroOrder_GetLinkedActor(link);
                state = CSF_AT(void*, actor, 0x2C);
                S_0048c9f0_CopyTargetHandle(
                    CSF_AT(S_0048c9f0*, state, 0x204), nullptr, targetHandle);
            }
        }

        actor = AiFncMacroOrder_GetLinkedActor(link);
        task = CSF_AT(void*, actor, 0x718);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, task, 0)[0x90 / 4])(task, nullptr, updateContext);
    }
}

HOOK(0x004B1420, TFncMacroOrden_UpdateLinkedTask, "auto");
