#ifndef __DISCO_Disco__
#define __DISCO_Disco__

#include <Config.hh>
#include <RAT/AnyParse.hh>
#include <RAT/ProcAllocator.hh>
#include <RAT/ProcBlockManager.hh>
#include <RAT/Rat.hh>

namespace DISCO {
class Disco : public RAT::Rat {
public:
  Disco(RAT::AnyParse *p, int argc, char **argv);
};
} // namespace DISCO

#endif // __DISCO_Disco__
