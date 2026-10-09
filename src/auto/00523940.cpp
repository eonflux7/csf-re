#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees (bound by address)
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_521DF0, 0x00521DF0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_521FA0, 0x00521FA0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_522E20, 0x00522E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_523290, 0x00523290, (void* self, void* edx, float a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, sub_5237C0, 0x005237C0, (void* self, void* edx));
CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_528340, 0x00528340, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_75A480, 0x0075A480, (void* self, void* edx, int a2));

// Starts (or restarts) one of the two streamed-sound slots with the given request.
static int StartStreamedSlot(uint32_t& slot, void* request)
{
    if (slot == 0)
    {
        void* memory = Mem_Alloc(0x60);
        slot = memory ? reinterpret_cast<uint32_t>(sub_5237C0(memory, nullptr)) : 0;
    }

    sub_521FA0(reinterpret_cast<uint8_t*>(slot) + 4, nullptr, reinterpret_cast<int>(request));

    void* stream = reinterpret_cast<void*>(slot);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, stream, 0)[0x20 / 4])(stream, nullptr);

    stream = reinterpret_cast<void*>(slot);
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, stream, 0)[0x28 / 4])(stream, nullptr, 0, 0);

    return static_cast<int>(slot);
}

static int __fastcall S_00521df0_PlaySoundRequest(S_00521df0* self, void* /*edx*/, void* request, float volume)
{
    uint32_t soundId = CSF_AT(uint32_t, request, 8);
    unsigned int instance = 0;

    if (soundId == 0xFFFFFFFF)
        return 0;

    void* soundBank = CSF_AT(void*, GetValue8B0554(), 0x1FD0);
    int soundIndex = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, soundBank, 0)[0xC / 4])(
        soundBank, nullptr, soundId);
    if (soundIndex == -1)
        return static_cast<int>(instance);

    int soundInfo = sub_75A480(soundBank, nullptr, soundIndex);

    if (CSF_AT(char, soundInfo, 0x22) != 0)
    {
        // Streamed sound: handled by one of two dedicated stream slots.
        if (CSF_AT(char, GetGlobalInt(), 4) != 0)
            return static_cast<int>(instance);

        if (CSF_AT(uint32_t, request, 0) == 5)
            return StartStreamedSlot(self->field_0048, request);

        CSF_AT(uint32_t, request, 0) = 3;
        return StartStreamedSlot(CSF_AT(uint32_t, self, 0x44), request);
    }

    if (CSF_AT(char, soundInfo, 0x24) != 0)
    {
        uint32_t requestCopy[14];
        sub_522E20(requestCopy, nullptr, reinterpret_cast<int>(request));
        requestCopy[0] = 6;
        instance = reinterpret_cast<unsigned int>(
            sub_528340(reinterpret_cast<void*>(self->field_0040), nullptr, requestCopy));
    }
    else
    {
        instance = reinterpret_cast<unsigned int>(
            sub_528340(reinterpret_cast<void*>(self->field_0040), nullptr,
                       reinterpret_cast<uint32_t*>(request)));
    }

    if (instance == 0)
        return 0;

    void* instanceObj = reinterpret_cast<void*>(instance);
    bool started = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, instanceObj, 0)[0x20 / 4])(instanceObj, nullptr);
    if (started)
    {
        sub_523290(reinterpret_cast<uint8_t*>(self) + 0x10, nullptr, volume, static_cast<int>(instance));
        sub_521DF0(self, nullptr, static_cast<int>(instance));
        return static_cast<int>(instance);
    }

    // Could not start: give the instance back to the pool (may clear the pointer).
    sub_526CF0(reinterpret_cast<void*>(self->field_0040), nullptr, &instance);
    return static_cast<int>(instance);
}

HOOK(0x00523940, S_00521df0_PlaySoundRequest, "auto");
