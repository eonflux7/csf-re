#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// --- bindings to original functions -------------------------------------------------
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_521DF0, 0x00521DF0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_521FA0, 0x00521FA0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_522E20, 0x00522E20, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_5237C0, 0x005237C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_524230, 0x00524230, (void* self, void* edx, int arg0, int a3));
CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_528340, 0x00528340, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_75A480, 0x0075A480, (void* self, void* edx, int a2));

// Creates (or reuses) a sound instance for the given request descriptor.
// Returns the instance pointer, or 0 when nothing could be started.
static int __fastcall S_00521df0_SubmitSoundRequest(S_00521df0* self, void* /*edx*/, void* request, int listenerInfo)
{
    uint32_t soundId = CSF_AT(uint32_t, request, 0x8);
    unsigned int instance = 0;

    if (soundId == 0xFFFFFFFF)
        return 0;

    void* soundBank = CSF_AT(void*, GetValue8B0554(), 0x1FD0);
    int soundIndex = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, soundBank, 0)[3])(soundBank, nullptr, soundId);
    if (soundIndex == -1)
        return (int)instance;

    if (CSF_AT(uint8_t, sub_75A480(soundBank, nullptr, soundIndex), 0x22) != 0)
    {
        // Exclusive / singleton sound: served by one lazily created shared instance.
        if (CSF_AT(int, listenerInfo, 0xC) != -1073741824)
            return (int)instance;

        if (self->field_0048 == 0)
        {
            void* memory = Mem_Alloc(96);
            uint32_t* created = nullptr;
            if (memory != nullptr)
                created = sub_5237C0(memory, nullptr);
            self->field_0048 = (uint32_t)created;
        }

        sub_521FA0((uint8_t*)self->field_0048 + 4, nullptr, (int)request);
        CSF_AT(uint32_t, self->field_0048, 0x4) = 5;

        void* shared = (void*)self->field_0048;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, shared, 0)[8])(shared, nullptr);

        shared = (void*)self->field_0048;
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, shared, 0)[10])(shared, nullptr, 0, 0);

        return (int)self->field_0048;
    }

    if (CSF_AT(uint8_t, sub_75A480(soundBank, nullptr, soundIndex), 0x24) != 0)
    {
        uint32_t requestCopy[14];
        std::memset(requestCopy, 0, sizeof(requestCopy));
        sub_522E20(requestCopy, nullptr, (int)request);
        requestCopy[0] = 6;
        instance = (unsigned int)sub_528340((void*)self->field_0040, nullptr, requestCopy);
    }
    else
    {
        instance = (unsigned int)sub_528340((void*)self->field_0040, nullptr, (uint32_t*)request);
    }

    if (instance == 0)
        return 0;

    void* instanceObj = (void*)instance;
    if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, instanceObj, 0)[8])(instanceObj, nullptr))
    {
        sub_524230((uint8_t*)self + 0x10, nullptr, listenerInfo, (int)instance);
        sub_521DF0(self, nullptr, (int)instance);
        return (int)instance;
    }

    // Start failed: hand the instance back to the pool (clears/updates our handle).
    sub_526CF0((void*)self->field_0040, nullptr, &instance);
    return (int)instance;
}

HOOK(0x005242B0, S_00521df0_SubmitSoundRequest, "auto");
