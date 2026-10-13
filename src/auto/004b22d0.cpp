#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));

static char __fastcall S_004ad990_SelectAlertCandidate(
    S_004ad990* self, void* /*edx*/, uint32_t* candidates, int outputAddress)
{
    uint32_t remaining = candidates[2];
    char selectedState = 0;
    if (remaining == 0)
        return 0;

    auto* output = reinterpret_cast<uint8_t*>(outputAddress);
    uint32_t offset = 0;

    do
    {
        auto* candidate = reinterpret_cast<uint8_t*>(candidates[0] + offset);
        const int32_t currentPriority = CSF_AT(int32_t, output, 0x04);

        if (currentPriority == -1 ||
            currentPriority < CSF_AT(int32_t, candidate, 0x04))
        {
            void* owner = reinterpret_cast<void*>(self->field_0000);
            void* stateChecker = CSF_AT(void*, owner, 0x34);

            if (((bool(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, stateChecker, 0)[0xB4 / 4])(
                    stateChecker, nullptr, output + 8))
            {
                const bool closer =
                    CSF_AT(float, output, 0x20) >
                    CSF_AT(float, candidate, 0x20);
                const char candidateState = CoverAttack_CheckState_4ADAE0(
                    self, nullptr, reinterpret_cast<int>(candidate + 8));

                if (!selectedState || (candidateState && closer))
                {
                    CSF_AT(uint32_t, output, 0x00) = CSF_AT(uint32_t, candidate, 0x00);
                    CSF_AT(uint32_t, output, 0x04) = CSF_AT(uint32_t, candidate, 0x04);
                    CSF_AT(uint32_t, output, 0x0C) = CSF_AT(uint32_t, candidate, 0x0C);
                    CSF_AT(uint32_t, output, 0x10) = CSF_AT(uint32_t, candidate, 0x10);
                    CSF_AT(uint32_t, output, 0x14) = CSF_AT(uint32_t, candidate, 0x14);
                    CSF_AT(uint32_t, output, 0x18) = CSF_AT(uint32_t, candidate, 0x18);
                    CSF_AT(uint32_t, output, 0x1C) = CSF_AT(uint32_t, candidate, 0x1C);
                    CSF_AT(uint32_t, output, 0x20) = CSF_AT(uint32_t, candidate, 0x20);
                    CSF_AT(uint32_t, output, 0x24) = CSF_AT(uint32_t, candidate, 0x24);
                    selectedState = candidateState;
                }
            }
        }

        offset += 40;
        --remaining;
    } while (remaining != 0);

    return selectedState;
}

HOOK(0x004B22D0, S_004ad990_SelectAlertCandidate, "auto");
