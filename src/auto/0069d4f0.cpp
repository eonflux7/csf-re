#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, _fopen, 0x007B2FA5, (char* FileName, char* Mode));
CSF_FUNC(int, __cdecl, _fprintf, 0x007B0228, (void* Stream, void* Format, ...));
CSF_FUNC(int, __cdecl, _fflush, 0x007B2BEF, (void* Stream, int a2, int a3));

CSF_GLOBAL(void*, Stream, 0x008BB3E0);

// Opens default.log for writing on first call and writes the init banner.
// The byte at *self acts as the "already initialized" flag.
static void __fastcall CErrorLog_InitOnce(uint8_t* self, void* /*edx*/)
{
    if (*self != 0)
        return;

    *self = 1;
    void* logFile = _fopen(const_cast<char*>(CSF_STR(0x0085E9F4, "default.log")),
                           const_cast<char*>(CSF_STR(0x00840044, "w")));
    Stream = logFile;
    if (logFile != nullptr)
    {
        _fprintf(logFile,
                 reinterpret_cast<void*>(const_cast<char*>(CSF_STR(0x0085E9DC, "CErrorLog Initialized\n\n"))));
        _fflush(Stream, 0, 0);
    }
}

HOOK(0x69D4F0, CErrorLog_InitOnce, "auto");
