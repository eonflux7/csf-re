#pragma once
#include <cstdint>
#include <cstddef>

namespace audio {
struct Pcm { unsigned rate, channels, bits; const void* data; size_t bytes; };
// Backend owns its copy of PCM. All calls are serialized by the adapter.
struct Backend {
    virtual ~Backend() = default;
    virtual void* create(Pcm) = 0;
    virtual void destroy(void*) = 0;
    virtual void play(void*, bool loop, unsigned byteOffset) = 0;
    virtual void stop(void*) = 0;
    virtual void gain(void*, float) = 0;
    virtual void frequency(void*, unsigned hz) = 0;
    // Borrowed process-lifetime IDirectSound8 for the game's separate music
    // buffer path (+124 of the device). Null provides a failure-only COM facade.
    virtual void* device_handle() = 0;
};
Backend& null_backend();
// Opt-in via CSF_AUDIO_BACKEND=dsound; failures fall back to the silent timeline.
Backend& selected_backend();

// Byte cursor, not milliseconds: parameter 7 is clamped against sound byte size
// by game function 007128F0. Unsigned subtraction tolerates GetTickCount wrap.
struct Timeline {
    uint32_t started = 0, offset = 0, bytes = 0, rate = 0;
    bool active = false, loop = false;
    void play(uint32_t now) {
        started = now;
        if (bytes && loop) offset %= bytes;
        else if (offset > bytes) offset = bytes;
        active = bytes && rate && (loop || offset < bytes);
    }
    uint32_t cursor(uint32_t now) {
        if (!active || !rate || !bytes) return offset;
        uint64_t p = uint64_t(offset) + uint64_t(uint32_t(now-started))*rate/1000;
        if (loop) return uint32_t(p % bytes);
        if (p >= bytes) { active = false; offset = bytes; return bytes; }
        return uint32_t(p);
    }
    void stop(uint32_t now) { offset = cursor(now); active = false; }
    void set_rate(uint32_t now, uint32_t value) {
        offset = cursor(now); started = now; rate = value;
        if (!rate) active = false;
    }
    void set_loop(uint32_t now, bool value) {
        offset = cursor(now); started = now; loop = value;
    }
};
}
