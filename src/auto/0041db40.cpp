#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SoundMessage_Initialize, 0x00405A50, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_408500, 0x00408500, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_41AEC0, 0x0041AEC0, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, sub_41D270, 0x0041D270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_41D590, 0x0041D590, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_41D970, 0x0041D970, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_DispatchGlobalValueMessage, 0x00522330, (void* self, void*, const void* message));

static void C_0082e428_SendStateSound(C_0082e428* self, void* messageBuffer)
{
    self->field_008c = 0;
    uint32_t* message = SoundMessage_Initialize(messageBuffer, nullptr, (int)(self->field_0004 + 28), 1232348240);
    int globalValue = GetGlobalValue_521BB0();
    AudioSoundManager_DispatchGlobalValueMessage(reinterpret_cast<void*>(globalValue), nullptr, message);
}

static char __fastcall C_0082e428_UpdateState(C_0082e428* self, void* /*edx*/, int a2)
{
    uint8_t buffer[0x20];
    char result = (char)self->field_000c;

    if (self->field_000c != 0)
    {
        sub_41AEC0(self, nullptr, a2);

        switch (self->field_008c)
        {
        case 1:
            if (sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 25))
            {
                sub_41D590(self, nullptr);
                sub_41D970(self, nullptr);
            }
            break;
        case 2:
            if (sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 26))
            {
                C_0082e428_SendStateSound(self, buffer + 0x10);
            }
            break;
        case 3:
            if (sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 27))
                self->field_008c = 0;
            break;
        case 4:
            if (sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 25))
            {
                sub_41D590(self, nullptr);
                sub_41D270(self, nullptr);
            }
            break;
        case 5:
            if (sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 26))
            {
                C_0082e428_SendStateSound(self, buffer);
            }
            break;
        default:
            break;
        }

        result = (char)(self->field_0090 - 1);
        if (self->field_0090 == 1)
        {
            result = sub_408500(reinterpret_cast<void*>(self->field_0030), nullptr, 28);
            if (result)
                self->field_0090 = 0;
        }
    }
    return result;
}

HOOK(0x0041DB40, C_0082e428_UpdateState, "auto");
