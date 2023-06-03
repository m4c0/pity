#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../sires/build.hpp"
#include "../traits/build.hpp"
#include "../what-the-font/build.hpp"

auto pity() {
  using namespace ecow;

  auto m = unit::create<mod>("pity");
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("sires", sires());
  m->add_wsdep("traits", traits());
  m->add_wsdep("what-the-font", wtf());
  m->add_part("termbuf");
  m->add_part("wtf");
  m->add_resource("VictorMono-Regular.otf");
  return m;
}
