#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto all = unit::create<exe>("poc");

  auto poc = all->add_unit<>("poc");
  poc->add_wsdep("hai", hai());
  poc->add_wsdep("jute", jute());

  return run_main(all, argc, argv);
}
