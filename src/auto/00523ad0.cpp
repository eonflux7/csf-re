#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_521C10, 0x00521C10, (void* self, void* edx, uint32_t* a2, int a3, float a4));
CSF_FUNC(int, __fastcall, sub_521DF0, 0x00521DF0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_521FA0, 0x00521FA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_522160, 0x00522160, (void* self, void* edx, float* a2));
CSF_FUNC(unsigned int, __fastcall, sub_523220, 0x00523220, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_5269B0, 0x005269B0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_528020, 0x00528020, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, sub_75A480, 0x0075A480, (void* self, void* edx, int a2));

// Starts (or restarts) a positional sound in a slot of the manager's slot table.
// soundRef: +8 = sound id; position: 3 floats + owner/key at +0xC.
static int __fastcall S_00521df0_PlaySoundInSlot(S_00521df0* self, void* /*edx*/, int soundRef, int position)
{
    // The original keeps these two adjacent on the stack ([ebp-8] channel, [ebp-4] sound
    // definition) and passes the address of the first to sub_526CF0, so they form one
    // 8-byte object here as well.
    struct ChannelAndSoundDef
    {
        unsigned int channel;
        int soundDef;
    };
    ChannelAndSoundDef locals = {};

    int soundId = CSF_AT(int, soundRef, 8);
    if (soundId == -1)
        return 0;

    void* soundBank = CSF_AT(void*, GetValue8B0554(), 0x1FD0);
    int soundIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, soundBank, 0)[0xC / 4])(soundBank, nullptr, soundId);
    if (soundIndex == -1)
        return 0;

    locals.soundDef = sub_75A480(soundBank, nullptr, soundIndex);

    unsigned int slotIndex = (unsigned int)sub_522160(self, nullptr, reinterpret_cast<float*>(position));
    if (slotIndex == 0xFFFFFFFFu)
    {
        slotIndex = sub_523220(self, nullptr);
    }
    else
    {
        uint8_t* usedSlot = reinterpret_cast<uint8_t*>(self->field_004c) + 12 * slotIndex;
        void* playingInstance = CSF_AT(void*, usedSlot, 8);
        if (playingInstance)
        {
            locals.channel = CSF_AT(unsigned int, playingInstance, 0x60);
            if (locals.channel)
            {
                void* channelObj = reinterpret_cast<void*>(locals.channel);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, channelObj, 0)[0x2C / 4])(channelObj, nullptr);
                sub_5269B0(CSF_AT(void*, usedSlot, 8), nullptr, 0);
                sub_526CF0(reinterpret_cast<void*>(self->field_0040), nullptr,
                           reinterpret_cast<unsigned int*>(&locals));
            }
        }
    }

    uint8_t* slot = reinterpret_cast<uint8_t*>(self->field_004c) + 12 * slotIndex;
    void* existingInstance = CSF_AT(void*, slot, 8);
    CSF_AT(uint8_t, slot, 0) = 1;
    CSF_AT(uint32_t, slot, 4) = CSF_AT(uint32_t, position, 0xC);
    if (!existingInstance)
    {
        void* memory = Mem_Alloc(100);
        uint32_t* newInstance = memory ? sub_528020(memory, nullptr) : nullptr;
        CSF_AT(uint32_t*, slot, 8) = newInstance;
    }

    sub_521FA0(CSF_AT(uint8_t*, slot, 8) + 4, nullptr, soundRef);

    int variationsBegin = CSF_AT(int, locals.soundDef, 8);
    if (variationsBegin)
    {
        // Number of 28-byte variation entries: signed (end - begin) / 28,
        // written as the original's reciprocal multiply (0x92492493, shift 4).
        int32_t byteSpan = CSF_AT(int, locals.soundDef, 0xC) - variationsBegin;
        int32_t quotient = (int32_t)(((int64_t)byteSpan * (int32_t)0x92492493) >> 32);
        quotient += byteSpan;
        quotient >>= 4;
        unsigned int variationCount = ((unsigned int)quotient >> 31) + (unsigned int)quotient;
        if (variationCount > 1)
        {
            unsigned int randomValue = (unsigned int)GlobalStateValue_50DCF0(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
            CSF_AT(uint32_t, CSF_AT(void*, slot, 8), 0x10) = randomValue % variationCount;
        }
    }

    void* instance = CSF_AT(void*, slot, 8);
    if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, instance, 0)[0x20 / 4])(instance, nullptr))
    {
        sub_521DF0(self, nullptr, reinterpret_cast<int>(instance));

        int listener = GetGlobalValue_70DDE0();
        float listenerPos[3] = {};
        ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, listener, 0)[0x44 / 4])(
            reinterpret_cast<void*>(listener), nullptr, listenerPos);

        sub_521C10(self, nullptr, reinterpret_cast<uint32_t*>(slot), reinterpret_cast<int>(listenerPos),
                   CSF_AT(float, locals.soundDef, 0x18));
    }
    return reinterpret_cast<int>(instance);
}

HOOK(0x00523AD0, S_00521df0_PlaySoundInSlot, "auto");
