#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../traits/build.hpp"
#include "../what-the-font/build.hpp"

auto pity() {
  using namespace ecow;

  auto m = unit::create<mod>("pity");
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("traits", traits());
  m->add_wsdep("what-the-font", wtf());
  m->add_part("termbuf");
  m->add_part("wtf");
  return m;
}
