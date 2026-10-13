#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, EntryLookup_FindEntryForOwnerWhenState1, 0x0040C8F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField90Null, 0x0040C910, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiSearchOrders_AimControllerAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttack_IsStoredValueNegative, 0x0047AEB0, (float* self, void*));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083dd58_FindCurrentTarget, 0x004A8D70, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_0083dd58_CanAimAtTarget, 0x004A8F40, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083dd58_UpdateTargetData, 0x004D5780, (void* self, void* edx, int a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083dd58_UpdateTargetControl(
    C_0083dd58* self, void* /*edx*/, int /*unused*/)
{
    void* controller = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x4C);

    if (static_cast<uint8_t>(IsField90Null(controller, nullptr)))
    {
        uint32_t* target = C_0083dd58_FindCurrentTarget(self, nullptr);

        if (CSF_AT(uint8_t, self, 0x99))
        {
            EntryLookup_FindEntryForOwnerWhenState1(controller, nullptr);
            CSF_AT(uint8_t, self, 0x99) = 0;
        }

        if (target)
        {
            if (C_0083dd58_CanAimAtTarget(self, nullptr, target) &&
                !CSF_AT(uint8_t, self, 0x99))
            {
                float* position = GetObjectPositionVector(target, nullptr);
                AiSearchOrders_AimControllerAtPosition(
                    controller, nullptr, position, 0);
                CSF_AT(uint8_t, self, 0x99) = 1;
            }

            if (!static_cast<uint8_t>(AiAttack_IsStoredValueNegative(
                    reinterpret_cast<float*>(
                        reinterpret_cast<uint8_t*>(self) + 0x60),
                    nullptr)))
            {
                C_0083dd58_UpdateTargetData(
                    self, nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(target) + 0x1C));
                CSF_AT(uint8_t, self, 0x9A) = 0;
            }
        }
        else if (!CSF_AT(uint8_t, self, 0x9A) &&
                 static_cast<uint8_t>(
                     ((int(__fastcall*)(void*, void*))
                         CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr)))
        {
            void* scene = g_pCurrentScene;
            int handle = GetControlEntryByIndex(
                reinterpret_cast<void*>(self->field_0014), nullptr, 0);
            void* entry = reinterpret_cast<void*>(
                CHandleTable_Resolve(scene, nullptr, handle));

            ((void(__fastcall*)(void*, void*, int, int, int, int))
                CSF_AT(void**, entry, 0)[0x8C / 4])(
                    entry, nullptr, 0, 0, -1, 0);
            CSF_AT(uint8_t, self, 0x9A) = 1;
        }
    }

    char result = CSF_AT(char, self, 0x98);
    if (!result ||
        !(result = static_cast<char>(
              ((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr))))
    {
        self->field_0074 = 1;
    }

    return result;
}

HOOK(0x004A9450, C_0083dd58_UpdateTargetControl, "auto");
