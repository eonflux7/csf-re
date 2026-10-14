// Link probes do not include generated standalone image data. Give this one
// global a test-owned cell; production uses the generated shared-image slot.
#include "globals.h"
namespace { void* engineCell; }
template <> unsigned char* const csf::gv<0x008B68C0>::d = reinterpret_cast<unsigned char*>(&engineCell);
