export module poc;

import hai;
import jute;
import pity;
import silog;
import sith;
import sitime;
import stubby;
import terminus;
import traits;

class thread : public sith::thread {
  static constexpr const auto con_w = 80;
  static constexpr const auto con_h = 24;

  pity::wtf m_term{"VictorMono-Regular.otf", 16, con_w, con_h};
  hai::uptr<terminus::prog> m_prog = terminus::spawn(args());
  hai::array<unsigned char> m_backbuf{m_term.image_w() * m_term.image_h()};

  static terminus::spawn_params args() {
    using namespace jute::literals;
    return {
        .con_width = con_w,
        .con_height = con_h,
        .args = hai::array<jute::view>::make("vim"_s, "-T"_s, "builtin_dumb"_s),
    };
  }

public:
  void run() override {
    terminus::buffer b{};
    while (!interrupted()) {
      m_prog->recv(&b);
      m_term.run({b.data(), b.size()});
    }
  }

  const auto &update() {
    m_term.print(m_backbuf.begin());
    return m_backbuf;
  }

  [[nodiscard]] auto width() const noexcept { return m_term.image_w(); }
  [[nodiscard]] auto height() const noexcept { return m_term.image_h(); }

  void send(jute::view chrs) { m_prog->send(chrs); }
};

extern "C" int main() {
  thread th{};
  th.start();

  sitime::sleep(1);
  th.send(":q!\n");
  sitime::sleep(1);

  auto &img = th.update();

  hai::array<stbi::pixel> pixels{img.size()};
  for (auto i = 0; i < img.size(); i++) {
    auto c = img[i];
    pixels[i] = {c, c, c, 255};
  }
  stbi::write_rgba("out/ansi.png", th.width(), th.height(), pixels);
}
