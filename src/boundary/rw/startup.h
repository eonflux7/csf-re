#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
namespace csf_rw {
struct VideoMode { int width, height, depth; uint32_t flags, refresh, format; };
static_assert(sizeof(VideoMode) == 24);
// Opt-in startup trace for the first Windows run; no address is used as a
// callable/raw image number. Slot identity is a diagnostic string only.
// Im2D primitives submitted / refused since the last Present (printed on the Present trace line).
inline int im2DFrameCalls[2];
inline bool startupTracing() { return std::getenv("CSF_RW_TRACE") != nullptr; }
inline void startupTrace(const char* slot,const char* operation) {
    if (startupTracing()) std::fprintf(stderr,"RW startup %s %s\n",slot,operation);
}
}
