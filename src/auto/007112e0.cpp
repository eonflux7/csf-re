#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// One stdcall call through the buffer's vtable with the registers the original leaves at that call: eax, ecx and
// edx as given; the stack words are the callee's arguments (stack[0] first), and the callee pops them.
static uint32_t CallBufferSlot(uintptr_t fn, uint32_t eax, uint32_t ecx, uint32_t edx,
                               const uint32_t* stack, unsigned count)
{
    hook::Regs r{};
    r.eax = eax;
    r.ecx = ecx;
    r.edx = edx;
    return hook::call_regs(fn, &r, stack, count);
}

static bool __fastcall C_00863cd4_WriteSoundBuffer(
    C_00863cd4* self, void* /*edx*/, char* source,
    unsigned int byteCount, int offset)
{
    int playCursor = 0;
    int writeCursor = 0;
    void* buffer = reinterpret_cast<void*>(self->field_0004);

    // mov eax, [ebx+4]; mov ecx, [eax]; lea edx, [playCursor]; push writeCursor's address; push playCursor's
    // address; push eax; call [ecx+0x10]. GetCurrentPosition is __stdcall: (buffer, &playCursor, &writeCursor).
    const uint32_t positionArgs[3] = {reinterpret_cast<uint32_t>(buffer),
                                      reinterpret_cast<uint32_t>(&playCursor),
                                      reinterpret_cast<uint32_t>(&writeCursor)};
    CallBufferSlot(reinterpret_cast<uintptr_t>(CSF_AT(void**, buffer, 0)[0x10 / 4]),
                   reinterpret_cast<uint32_t>(buffer), CSF_AT(uint32_t, buffer, 0),
                   reinterpret_cast<uint32_t>(&playCursor), positionArgs, 3);

    bool canWrite;
    if (writeCursor > playCursor)
        canWrite = offset < playCursor || offset >= writeCursor;
    else if (writeCursor < playCursor)
        canWrite = offset < playCursor && offset >= writeCursor;
    else
        canWrite = true;

    char* lockedData = nullptr;
    int lockedBytes = 0;
    if (!canWrite)
        return false;

    // Lock(offset, byteCount, &lockedData, &lockedBytes, 0, 0, 0): the original's edx is &lockedData (lea edx,
    // [ebp-4]), ecx the buffer's vtable (mov ecx, [eax]) and eax the buffer.
    buffer = reinterpret_cast<void*>(self->field_0004);
    const uint32_t lockArgs[8] = {reinterpret_cast<uint32_t>(buffer), static_cast<uint32_t>(offset), byteCount,
                                  reinterpret_cast<uint32_t>(&lockedData), reinterpret_cast<uint32_t>(&lockedBytes),
                                  0, 0, 0};
    int status = static_cast<int>(CallBufferSlot(
        reinterpret_cast<uintptr_t>(CSF_AT(void**, buffer, 0)[0x2C / 4]),
        reinterpret_cast<uint32_t>(buffer), CSF_AT(uint32_t, buffer, 0),
        reinterpret_cast<uint32_t>(&lockedData), lockArgs, 8));

    if (status == -2005401450)
    {
        // Restore(buffer): mov eax, [ebx+4]; mov ecx, [eax]; push eax; call [ecx+0x50]. edx still holds &lockedData.
        buffer = reinterpret_cast<void*>(self->field_0004);
        const uint32_t restoreArgs[1] = {reinterpret_cast<uint32_t>(buffer)};
        status = static_cast<int>(CallBufferSlot(
            reinterpret_cast<uintptr_t>(CSF_AT(void**, buffer, 0)[0x50 / 4]),
            reinterpret_cast<uint32_t>(buffer), CSF_AT(uint32_t, buffer, 0),
            reinterpret_cast<uint32_t>(&lockedData), restoreArgs, 1));
        if (status != 0)
            return false;

        // The retry of Lock: mov eax, [ebx+4]; mov edx, [eax]; lea ecx, [lockedData]; ... call [edx+0x2c].
        buffer = reinterpret_cast<void*>(self->field_0004);
        status = static_cast<int>(CallBufferSlot(
            reinterpret_cast<uintptr_t>(CSF_AT(void**, buffer, 0)[0x2C / 4]),
            reinterpret_cast<uint32_t>(buffer), reinterpret_cast<uint32_t>(&lockedData),
            CSF_AT(uint32_t, buffer, 0), lockArgs, 8));
    }

    if (status != 0)
        return false;

    const unsigned int wholeBytes = 4 * (byteCount >> 2);
    if (source)
    {
        std::memcpy(lockedData, source, wholeBytes);
        std::memcpy(lockedData + wholeBytes, source + wholeBytes, byteCount & 3);
    }
    else
    {
        std::memset(lockedData, 0, wholeBytes);
        std::memset(lockedData + wholeBytes, 0, byteCount & 3);
    }

    // Unlock(buffer, lockedData, byteCount, 0, 0): mov ecx, [ebp-4]; mov ebx, [ebx+4]; mov eax, [ebx]; push 0;
    // push 0; push esi; push ecx; push ebx; call [eax+0x4c]. edx is still byteCount (mov edx, ecx earlier).
    buffer = reinterpret_cast<void*>(self->field_0004);
    const uint32_t unlockArgs[5] = {reinterpret_cast<uint32_t>(buffer), reinterpret_cast<uint32_t>(lockedData),
                                    byteCount, 0, 0};
    CallBufferSlot(reinterpret_cast<uintptr_t>(CSF_AT(void**, buffer, 0)[0x4C / 4]),
                   CSF_AT(uint32_t, buffer, 0), reinterpret_cast<uint32_t>(lockedData), byteCount, unlockArgs, 5);

    return true;
}

HOOK(0x007112E0, C_00863cd4_WriteSoundBuffer, "auto");
