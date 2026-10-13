#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
namespace audio {
// On-disc contract inferred from exported stream readers, not RWA SDK types.
struct BankFormat {
    unsigned char raw[28]{};
    size_t codec = 0, extra = 0;
    uint32_t word(size_t offset) const { uint32_t n; std::memcpy(&n,raw+offset,4); return n; }
};
struct BankEntry {
    BankFormat source, target;
    uint32_t flags=0, voiceFlags=0;
    size_t guid=0, name=0;
};
inline bool parse_bank_metadata(const unsigned char* p,size_t bytes,unsigned alignment,BankEntry& out) {
    if(!p || bytes<4 || !alignment || alignment>64 || (alignment&(alignment-1))) return false;
    BankEntry e{}; std::memcpy(&e.flags,p,4); size_t pos=4;
    auto take=[&](size_t n) { if(n>bytes-pos) return false; pos+=n; return true; };
    auto format=[&](BankFormat& f) {
        size_t start=pos; if(!take(28)) return false; std::memcpy(f.raw,p+start,28);
        if(f.word(4)) { f.codec=pos; if(!take(16)) return false; }
        if(f.word(16)) { f.extra=pos; if(!take(f.word(20))) return false; }
        return f.word(0)!=0 && f.raw[12]!=0 && f.raw[13]!=0;
    };
    if(!format(e.source) || !format(e.target)) return false;
    size_t flagPos=pos; if(!take(4)) return false; std::memcpy(&e.voiceFlags,p+flagPos,4);
    if(e.flags&1) { e.guid=pos; if(!take(16)) return false; }
    if(e.flags&2) {
        e.name=pos; auto end=static_cast<const unsigned char*>(std::memchr(p+pos,0,bytes-pos));
        if(!end) return false;
        size_t n=size_t(end-(p+pos))+1;
        if(!take((n+alignment-1)&~size_t(alignment-1))) return false;
    }
    // Optional implementation/device GUIDs are not needed by a silent timeline.
    if(e.flags&4) { if(!take(16)) return false; }
    if(e.flags&8) { if(!take(16)) return false; }
    out=e; return true;
}
}
