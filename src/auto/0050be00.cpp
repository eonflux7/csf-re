#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, LoadState_Construct, 0x00694C80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_50BD10, 0x0050BD10, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

// Builds a load-state buffer, fills it from the object, writes it out through the
// object's virtual slot 0x34, then resets the buffer. Returns the write result.
static char __fastcall OptionsFile_CaptureAndWriteStateToFile(C_0083fdf4* self, void* /*edx*/, int a2)
{
    alignas(8) uint8_t loadStateBytes[0x918];
    void* loadState = loadStateBytes;

    LoadState_Construct(loadState, nullptr);
    sub_50BD10(self, nullptr, loadState);

    char writeResult = ((char(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, self, 0)[0x34 / 4])(
        self, nullptr, a2, loadState);

    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(loadState), nullptr);
    return writeResult;
}

HOOK(0x0050BE00, OptionsFile_CaptureAndWriteStateToFile, "auto");
