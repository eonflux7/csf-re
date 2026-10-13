#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x4EF1F0 takes 3 stack args (cdecl). The original also pushes a fourth dword
// (an unwritten [ebp-4] local) and pops 0x10 afterwards; the callee never reads it,
// so only the three caller-supplied arguments are forwarded.
CSF_FUNC(std::uint32_t*, __cdecl, CopyAssignTwelveByteRange, 0x004EF1F0, (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));

static std::uint32_t* __cdecl CopyAssignTwelveBytePrefix(std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination)
{
  return CopyAssignTwelveByteRange(first, last, destination);
}

HOOK(0x004EF420, CopyAssignTwelveBytePrefix, "auto");
