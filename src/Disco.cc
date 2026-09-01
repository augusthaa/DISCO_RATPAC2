#include <Disco.hh>
#include <GeoMuTrackerFactory.hh>
#include <GeoPMTHousingFactory.hh>
#include <GeoTestCellFactory.hh>
#include <RAT/AnyParse.hh>
#include <RAT/ProcAllocator.hh>
#include <RAT/ProcBlockManager.hh>
#include <RAT/Rat.hh>

namespace DISCO {
Disco::Disco(RAT::AnyParse *p, int argc, char **argv) : Rat(p, argc, argv) {

  // Append an additional data directory (for ratdb and geo)
  char *discodata = getenv("DISCODATA");
  if (discodata != NULL) {
    ratdb_directories.insert(static_cast<std::string>(discodata) + "/ratdb");
    model_directories.insert(static_cast<std::string>(discodata) + "/models");
  }
  // Initialize a geometry factory
  new RAT::GeoMuTrackerFactory();
  new RAT::GeoTestCellFactory();
  new RAT::GeoPMTHousingFactory();

  // Include a new type of processor
  // Add a unique component to the datastructure
}
} // namespace DISCO
