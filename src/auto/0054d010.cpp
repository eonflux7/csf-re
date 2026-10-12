#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __stdcall, AiTaskStates_ResetSoundSlotFlags, 0x0041F6C0, (std::uint32_t* slots, char* state));
CSF_FUNC(unsigned int, __stdcall, AiCore_RunCoverTaskStageA, 0x004A3730, (S_0041e5f0* entries, char* context));

static bool TypeRegistry_PrepareHermiteField(bool* state)
{
    const bool ready = !*state;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(bool, state, 0x15) = false;
        CSF_AT(bool, state, 0x16) = false;
    }
    return ready;
}

static int __stdcall TypeRegistry_SerializeHermite(void* object, bool* state)
{
    auto* data = static_cast<std::uint8_t*>(object);
    bool failed = false;
    if (!*state)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, const char*, bool*))
                       CSF_AT(void**, serializer, 0)[128 / 4])(
            serializer, nullptr, CSF_STR(0x008443A0, "CHermite"), state);
    }
    *state = failed;

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[84 / 4])(
                     serializer, nullptr, data + 68, state) == -1;
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[84 / 4])(
                     serializer, nullptr, data + 72, state) == -1;
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[84 / 4])(
                     serializer, nullptr, data + 76, state) == -1;
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[88 / 4])(
                     serializer, nullptr, data + 80, state) == -1;
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[88 / 4])(
                     serializer, nullptr, data + 84, state) == -1;
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        AiTaskStates_ResetSoundSlotFlags(
            reinterpret_cast<std::uint32_t*>(data + 4),
            reinterpret_cast<char*>(state));
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        AiCore_RunCoverTaskStageA(
            reinterpret_cast<S_0041e5f0*>(data + 20),
            reinterpret_cast<char*>(state));
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        AiTaskStates_ResetSoundSlotFlags(
            reinterpret_cast<std::uint32_t*>(data + 36),
            reinterpret_cast<char*>(state));
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        AiCore_RunCoverTaskStageA(
            reinterpret_cast<S_0041e5f0*>(data + 52),
            reinterpret_cast<char*>(state));
    }

    if (TypeRegistry_PrepareHermiteField(state))
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        *state = ((int(__fastcall*)(void*, void*, void*, bool*))
                      CSF_AT(void**, serializer, 0)[56 / 4])(
                     serializer, nullptr, data + 88, state) == -1;

        if (!*state)
        {
            serializer = CSF_AT(void*, state, 0x04);
            if (((bool(__fastcall*)(void*, void*, bool*))
                     CSF_AT(void**, serializer, 0)[124 / 4])(
                    serializer, nullptr, state))
            {
                *state = false;
                return 0;
            }
        }
    }

    *state = true;
    return -1;
}

HOOK(0x0054D010, TypeRegistry_SerializeHermite, "auto");
