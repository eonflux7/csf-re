#ifdef CSF_STANDALONE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "hook.h"
namespace {
// A null output device is not a COM object. Let DirectSound detection take its
// documented failure branch; RWA one-shot playback uses backend.cpp separately.
static HRESULT __stdcall enumerate(void*,void*) { return E_NOTIMPL; }
static HRESULT __stdcall device_id(const GUID* input,GUID* output) { if(!input || !output) return E_INVALIDARG; *output=*input; return S_OK; }
static HRESULT __stdcall create8(const GUID*,void** out,void*) { if(out) *out=nullptr; return E_NOTIMPL; }
struct Callbacks {
    size_t(__cdecl* read)(void*,size_t,size_t,void*);
    int(__cdecl* seek)(void*,int64_t,int);
    int(__cdecl* close)(void*);
    long(__cdecl* tell)(void*);
};
// Vorbis is not vendored. Null backend deliberately fails open; observed game
// callers close the HANDLE themselves on failure. Never return fake vorbis_info.
static int __cdecl vorbis_open(void*,void*,const char*,long,Callbacks) { return -132; }
static int __cdecl vorbis_read(void*,char*,int,int,int,int,int*) { return 0; }
static int __cdecl vorbis_clear(void*) { return 0; }
static double __cdecl vorbis_total(void*,int) { return 0; }
static int __cdecl vorbis_seek(void*,int64_t) { return -131; }
static void* __cdecl vorbis_info(void*,int) { return nullptr; }
}
HOOK(0x007F78E0,enumerate,"boundary-rwa");
HOOK(0x007F78E6,device_id,"boundary-rwa");
HOOK(0x007F78F2,create8,"boundary-rwa");
HOOK(0x0081DD16,vorbis_read,"boundary-rwa");
HOOK(0x0081DD1C,vorbis_clear,"boundary-rwa");
HOOK(0x0081DD22,vorbis_total,"boundary-rwa");
HOOK(0x0081DD28,vorbis_open,"boundary-rwa");
HOOK(0x0081DD2E,vorbis_seek,"boundary-rwa");
HOOK(0x0081DD34,vorbis_info,"boundary-rwa");
#endif
