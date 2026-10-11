#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, NotifyDepleted, 0x0049D400, (void* self, void* edx, void* a3, int a4));
CSF_FUNC(int, __fastcall, RefreshDepletedStatus, 0x0049DA70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, UpdateActionMonitor, 0x004DC3E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_0083ad48_UpdateActionState(
    C_0083ad48* self, void* /*edx*/, float deltaTime)
{
    const char active = static_cast<char>(self->field_0038);
    if (!active)
        return active;

    int deltaBits;
    std::memcpy(&deltaBits, &deltaTime, sizeof(deltaBits));

    bool monitorHandled = false;
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(std::uint8_t, globalState, 0xD0))
    {
        if (!self->field_004c)
        {
            void* monitor = Mem_Alloc(8);
            if (monitor)
            {
                CSF_AT(std::uint32_t, monitor, 0) = 0;
                CSF_AT(std::uint32_t, monitor, 4) = 0;
            }

            self->field_004c = reinterpret_cast<std::uint32_t>(monitor);
            CSF_AT(C_0083ad48*, monitor, 0) = self;
            CSF_AT(std::uint8_t, monitor, 4) = 0;
        }

        monitorHandled = UpdateActionMonitor(
            reinterpret_cast<void*>(self->field_004c), nullptr, deltaBits) != 0;
    }

    if (!monitorHandled)
    {
        switch (self->field_0040)
        {
        case 0:
        {
            auto* owner = reinterpret_cast<C_008342e8*>(self->field_0014);
            auto* actor = reinterpret_cast<C_0082dd78*>(owner->field_004c);
            if (CSF_AT(std::uint8_t, actor, 0x75) &&
                !((char(__fastcall*)(void*, void*))
                    CSF_AT(void**, actor, 0)[0x4C / 4])(actor, nullptr))
            {
                self->field_0040 = 1;
            }
            break;
        }

        case 1:
        {
            alignas(8) std::uint8_t directionBytes[0xC];
            CSF_AT(float, directionBytes, 0) = 0.0f;
            CSF_AT(float, directionBytes, 4) = 1.0f;
            CSF_AT(float, directionBytes, 8) = 0.0f;

            NotifyDepleted(reinterpret_cast<void*>(self->field_0014),
                           nullptr, directionBytes, 0);
            self->field_0048 = 2.0f;
            self->field_0040 = 2;
            break;
        }

        case 2:
        {
            const float reminderTime = self->field_0048 - deltaTime;
            self->field_0048 = reminderTime;
            if (reminderTime < g_fZero)
            {
                void* owner = reinterpret_cast<void*>(self->field_0014);
                self->field_0048 = 1.0f;
                RefreshDepletedStatus(owner, nullptr);
            }

            const float remainingTime = self->field_0044 - deltaTime;
            self->field_0044 = remainingTime;
            if (remainingTime < g_fZero)
            {
                void* owner = reinterpret_cast<void*>(self->field_0014);
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[0xA0 / 4])(owner, nullptr);
            }
            break;
        }
        }
    }

    return Autonomo_DispatchAction(self, nullptr, deltaBits);
}

HOOK(0x004CE960, C_0083ad48_UpdateActionState, "auto");
