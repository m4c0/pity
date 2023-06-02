#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../traits/build.hpp"

auto pity() {
  using namespace ecow;

  auto m = unit::create<mod>("pity");
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("traits", traits());
  m->add_part("termbuf");
  return m;
}
