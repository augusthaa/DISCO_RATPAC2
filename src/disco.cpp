#include <Disco.hh>
#include <RAT/AnyParse.hh>
#include <RAT/Rat.hh>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  auto parser = new RAT::AnyParse(argc, argv);
  auto disco = DISCO::Disco(parser, argc, argv);
  disco.Begin();
  disco.Report();
}
