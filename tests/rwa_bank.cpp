#include "../src/boundary/rwa/bank.h"
#include <cassert>
#include <vector>
static void word(std::vector<unsigned char>& p,size_t offset,uint32_t n) { std::memcpy(p.data()+offset,&n,4); }
int main() {
    std::vector<unsigned char> p(84);
    word(p,0,3); // GUID and name
    for(size_t pos : {size_t(4),size_t(32)}) { word(p,pos,44100); word(p,pos+8,176400); p[pos+12]=16; p[pos+13]=2; }
    word(p,60,0); // voice flags
    std::memcpy(p.data()+80,"ABC",4);
    audio::BankEntry e;
    assert(audio::parse_bank_metadata(p.data(),p.size(),4,e));
    assert(e.guid==64 && e.name==80 && e.target.word(8)==176400);
    for(size_t n=0;n<p.size();++n) assert(!audio::parse_bank_metadata(p.data(),n,4,e));
    assert(!audio::parse_bank_metadata(p.data(),p.size(),3,e));
    word(p,8,1); // codec flag without room for both formats
    assert(!audio::parse_bank_metadata(p.data(),p.size(),4,e));
    word(p,8,0); p[16]=0; // zero bits per sample
    assert(!audio::parse_bank_metadata(p.data(),p.size(),4,e));

    // Variable-size codec GUIDs and extra-format payloads, then all optional
    // identity GUIDs. Serialized flags/pointers are not dereferenced as addresses.
    p.assign(4,0); word(p,0,15);
    auto format=[&](uint32_t extra) {
        size_t base=p.size(); p.resize(base+28);
        word(p,base,22050); word(p,base+4,1); word(p,base+8,22050);
        p[base+12]=8; p[base+13]=1;
        word(p,base+16,1); word(p,base+20,extra);
        p.resize(p.size()+16+extra,0x41);
    };
    format(4); format(8);
    size_t at=p.size(); p.resize(at+4+16); word(p,at,1);
    p.insert(p.end(),{'A',0,0,0}); p.resize(p.size()+32);
    assert(audio::parse_bank_metadata(p.data(),p.size(),4,e));
    assert(e.source.codec==32 && e.source.extra==48);
    assert(e.target.codec==80 && e.target.extra==96);
    for(size_t n=0;n<p.size();++n) assert(!audio::parse_bank_metadata(p.data(),n,4,e));
    word(p,24,UINT32_MAX); // overflowing/malicious source extra size
    assert(!audio::parse_bank_metadata(p.data(),p.size(),4,e));
}
