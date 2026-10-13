#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_74b150_SetFlag8, 0x0074B150, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0074b2a0_SetFlag, 0x0074B2A0, (void* self, void*));

static char __fastcall NetCallbacks_SynchronizeCogibleState(
    void* self, void* /*edx*/, int /*unused*/)
{
    auto* callback = static_cast<std::uint8_t*>(self);
    void* owner = callback - 0xB8;

    CSF_AT(std::uint32_t, callback, 0x0C) = 13;
    if (static_cast<unsigned char>(Sub_64B250(owner, nullptr)) != 0)
        NetCogibleDDL_UpdateAuthoritativeState(owner, nullptr);
    else
        NetCogibleDDL_UpdateReplicaState(owner, nullptr);

    const char authoritative = static_cast<char>(Sub_64B250(owner, nullptr));
    if (!authoritative)
        return authoritative;

    std::uint32_t previous = CSF_AT(std::uint32_t, callback, -0x6C);
    std::uint32_t current = CSF_AT(std::uint32_t, callback, -0x74);
    bool changed = current != previous;
    CSF_AT(std::uint32_t, callback, -0x6C) = current;
    if (changed)
        C_0074b2a0_SetFlag(callback - 0x74, nullptr);

    previous = CSF_AT(std::uint32_t, callback, -0x44);
    current = CSF_AT(std::uint32_t, callback, -0x4C);
    changed = current != previous;
    CSF_AT(std::uint32_t, callback, -0x44) = current;
    if (changed)
        C_0074b2a0_SetFlag(callback - 0x4C, nullptr);

    previous = CSF_AT(std::uint32_t, callback, -0x60);
    current = CSF_AT(std::uint32_t, callback, -0x68);
    changed = current != previous;
    CSF_AT(std::uint32_t, callback, -0x60) = current;
    if (changed)
        C_0074b2a0_SetFlag(callback - 0x68, nullptr);

    previous = CSF_AT(std::uint32_t, callback, -0x38);
    current = CSF_AT(std::uint32_t, callback, -0x40);
    changed = current != previous;
    CSF_AT(std::uint32_t, callback, -0x38) = current;
    if (changed)
        C_0074b2a0_SetFlag(callback - 0x40, nullptr);

    current = CSF_AT(std::uint32_t, callback, -0x18);
    previous = CSF_AT(std::uint32_t, callback, -0x08);
    std::uint32_t second = CSF_AT(std::uint32_t, callback, -0x14);
    changed = current != previous ||
              second != CSF_AT(std::uint32_t, callback, -0x04);
    CSF_AT(std::uint32_t, callback, -0x08) = current;
    CSF_AT(std::uint32_t, callback, -0x04) = second;
    if (changed)
        C_74b150_SetFlag8(callback - 0x18, nullptr);

    current = CSF_AT(std::uint32_t, callback, -0x30);
    previous = CSF_AT(std::uint32_t, callback, -0x20);
    second = CSF_AT(std::uint32_t, callback, -0x2C);
    changed = current != previous ||
              second != CSF_AT(std::uint32_t, callback, -0x1C);
    CSF_AT(std::uint32_t, callback, -0x20) = current;
    CSF_AT(std::uint32_t, callback, -0x1C) = second;
    if (changed)
        return C_74b150_SetFlag8(callback - 0x30, nullptr);

    return static_cast<char>(current);
}

HOOK(0x00743450, NetCallbacks_SynchronizeCogibleState, "auto");
