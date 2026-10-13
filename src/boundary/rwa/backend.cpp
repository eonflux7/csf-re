#ifdef CSF_STANDALONE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <cstring>
#include <cmath>
#include "backend.h"
namespace audio {
class SilentDevice final : public IDirectSound8 {
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** out) override { if(out) *out=nullptr; return E_NOINTERFACE; }
    ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
    HRESULT STDMETHODCALLTYPE CreateSoundBuffer(LPCDSBUFFERDESC,LPDIRECTSOUNDBUFFER* out,LPUNKNOWN) override { if(out) *out=nullptr; return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetCaps(LPDSCAPS) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER,LPDIRECTSOUNDBUFFER* out) override { if(out) *out=nullptr; return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND,DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE Compact() override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetSpeakerConfig(LPDWORD out) override { if(!out) return E_POINTER; *out=DSSPEAKER_STEREO; return S_OK; }
    HRESULT STDMETHODCALLTYPE SetSpeakerConfig(DWORD) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE Initialize(LPCGUID) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE VerifyCertification(LPDWORD out) override { if(out) *out=DS_UNCERTIFIED; return S_OK; }
};
static void* silent_device() { static SilentDevice* d=new SilentDevice; return d; }
class Null final : public Backend {
    void* create(Pcm) override { return nullptr; }
    void destroy(void*) override {}
    void play(void*, bool, unsigned) override {}
    void stop(void*) override {}
    void gain(void*, float) override {}
    void frequency(void*, unsigned) override {}
    void* device_handle() override { return silent_device(); }
};
// Process-lifetime devices: ABI sidecars have static storage and must still be
// able to release buffers during C++ teardown, regardless of first-use order.
Backend& null_backend() { static Null* b=new Null; return *b; }
class DSound final : public Backend {
    IDirectSound8* device = nullptr;
    HMODULE module = nullptr;
public:
    DSound() {
        module = LoadLibraryA("dsound.dll");
        if (!module) return;
        auto create = reinterpret_cast<HRESULT(WINAPI*)(LPCGUID,LPDIRECTSOUND8*,LPUNKNOWN)>(GetProcAddress(module,"DirectSoundCreate8"));
        if (!create || FAILED(create(nullptr,&device,nullptr))) { device=nullptr; return; }
        if (FAILED(device->SetCooperativeLevel(GetDesktopWindow(),DSSCL_NORMAL))) { device->Release(); device=nullptr; }
    }
    ~DSound() { if(device) device->Release(); if(module) FreeLibrary(module); }
    void* create(Pcm p) override {
        if (!device || !p.data || !p.bytes || p.bytes > DSBSIZE_MAX ||
            (p.channels!=1 && p.channels!=2) || (p.bits!=8 && p.bits!=16) || !p.rate) return nullptr;
        WAVEFORMATEX fmt{}; fmt.wFormatTag=WAVE_FORMAT_PCM; fmt.nChannels=WORD(p.channels);
        fmt.nSamplesPerSec=p.rate; fmt.wBitsPerSample=WORD(p.bits);
        fmt.nBlockAlign=WORD(p.channels*p.bits/8); fmt.nAvgBytesPerSec=p.rate*fmt.nBlockAlign;
        if (p.bytes % fmt.nBlockAlign) return nullptr;
        DSBUFFERDESC desc{}; desc.dwSize=sizeof(desc); desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_GLOBALFOCUS;
        desc.dwBufferBytes=DWORD(p.bytes); desc.lpwfxFormat=&fmt;
        IDirectSoundBuffer* b=nullptr;
        if (FAILED(device->CreateSoundBuffer(&desc,&b,nullptr))) return nullptr;
        void *a,*c; DWORD n,m;
        if (FAILED(b->Lock(0,DWORD(p.bytes),&a,&n,&c,&m,0))) { b->Release(); return nullptr; }
        std::memcpy(a,p.data,n); if(m) std::memcpy(c,static_cast<const char*>(p.data)+n,m);
        b->Unlock(a,n,c,m); return b;
    }
    void destroy(void* p) override { if(p) { auto b=static_cast<IDirectSoundBuffer*>(p); b->Stop(); b->Release(); } }
    void play(void* p,bool loop,unsigned offset) override { if(p) { auto b=static_cast<IDirectSoundBuffer*>(p); b->SetCurrentPosition(offset); b->Play(0,0,loop?DSBPLAY_LOOPING:0); } }
    void stop(void* p) override { if(p) static_cast<IDirectSoundBuffer*>(p)->Stop(); }
    void gain(void* p,float v) override {
        if(p) {
            LONG db = !(v>0)?DSBVOLUME_MIN:LONG(2000*std::log10(v>1?1:v));
            if(db<DSBVOLUME_MIN) db=DSBVOLUME_MIN;
            static_cast<IDirectSoundBuffer*>(p)->SetVolume(db);
        }
    }
    void frequency(void* p,unsigned hz) override {
        if(p && hz>=DSBFREQUENCY_MIN && hz<=DSBFREQUENCY_MAX)
            static_cast<IDirectSoundBuffer*>(p)->SetFrequency(hz);
    }
    void* device_handle() override { return device?static_cast<void*>(device):silent_device(); }
};
Backend& selected_backend() {
    static bool real=[] { char name[32]{}; GetEnvironmentVariableA("CSF_AUDIO_BACKEND",name,sizeof(name)); return std::strcmp(name,"dsound")==0; }();
    if(real) { static DSound* b=new DSound; return *b; } return null_backend();
}
}
#endif
