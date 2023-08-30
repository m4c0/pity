#include "build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto poc = unit::create<app>("poc");
  poc->add_ref(pity());
  poc->add_unit<box>("poc");
  return run_main(poc, argc, argv);
}
