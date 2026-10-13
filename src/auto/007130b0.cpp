#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// IDirectInputDevice8 methods are stdcall with one stack argument (the device pointer).
// The original leaves a vtable or device pointer in ecx at the call; this signature
// passes it in ecx (first fastcall argument) and the device on the stack.
using DirectInputDeviceCall = int(__fastcall*)(void* ecx, void* edx, void* deviceArg);

// DIERR_INPUTLOST (0x8007001E): the acquire loop retries while the device reports this.
static constexpr int kDiErrInputLost = static_cast<int>(0x8007001Eu);

// Vtable slots of IDirectInputDevice8 (byte offset / 4).
static constexpr int kDeviceSlotAcquire = 0x1C / 4;
static constexpr int kDeviceSlotUnacquire = 0x20 / 4;

static int __fastcall CInputDirectInput_ToggleAcquire(CInputDirectInput* self, void* /*edx*/)
{
    if (self->field_0c91 != 0)
    {
        // Device is currently acquired: clear the flag and unacquire it.
        self->field_0c91 = 0;
        void* device = reinterpret_cast<void*>(self->field_0024);
        // Original: ecx = device, push device, call [vtbl + 0x20].
        auto unacquire = reinterpret_cast<DirectInputDeviceCall>(CSF_AT(void**, device, 0)[kDeviceSlotUnacquire]);
        return unacquire(device, nullptr, device);
    }

    // Device is not acquired: set the flag and acquire, retrying while input is lost.
    void* device = reinterpret_cast<void*>(self->field_0024);
    self->field_0c91 = 1;
    // Original: ecx = vtable (loaded from device), push device, call [vtbl + 0x1C].
    void* vtable = CSF_AT(void*, device, 0);
    auto acquire = reinterpret_cast<DirectInputDeviceCall>(CSF_AT(void**, device, 0)[kDeviceSlotAcquire]);
    int result = acquire(vtable, nullptr, device);
    while (result == kDiErrInputLost)
    {
        vtable = CSF_AT(void*, device, 0);
        acquire = reinterpret_cast<DirectInputDeviceCall>(CSF_AT(void**, device, 0)[kDeviceSlotAcquire]);
        result = acquire(vtable, nullptr, device);
    }
    return result;
}

HOOK(0x007130B0, CInputDirectInput_ToggleAcquire, "auto");
