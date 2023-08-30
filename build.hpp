#include "../ecow/ecow.hpp"
#include "../what-the-font/build.hpp"

auto pity() {
  using namespace ecow;

  auto m = unit::create<box>("pity");
  m->add_wsdep("what-the-font", wtf());
  m->add_resource("VictorMono-Regular.otf");
  return m;
}
