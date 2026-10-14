#include "globals.h"
#include "hook.h"
CSF_GLOBAL(void*, stream, 0x008BB3E0);
CSF_FUNC(int, __cdecl, _fprintf, 0x007B0228, (void* Stream, void* Format, ...));
CSF_FUNC(int, __cdecl, _fclose, 0x007B01D7, (void* Stream));
static void __cdecl CloseErrorLog() {
    if (stream) {
        _fprintf(stream, const_cast<char*>(CSF_STR(0x0085EA00, "\nCErrorLog ShutDown\n")));
        _fclose(stream);
        stream=nullptr;
    }
}
HOOK(0x0082ADF0, CloseErrorLog, "initfn");
