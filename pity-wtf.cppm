export module pity:wtf;
import :ansibuf;
import :termbuf;
import jute;
import silog;
import sires;
import what_the_font;

namespace pity {
export struct font_unavailable {};
#ifdef __wasm__
void abort();
inline void or_fail(const char *) { abort(); }
#else
inline void or_fail(const char *) { throw font_unavailable(); }
#endif

inline auto new_face(::wtf::library &l, jute::view font, unsigned font_h) {
  auto data = sires::slurp(font).take(or_fail);
  return l.new_memory_face(data.begin(), data.size(), font_h);
}

export class termwtf {
  termbuf m_term{};
  ::wtf::library m_lib{};
  ::wtf::face m_face;
  unsigned m_font_h;
  // TODO: maybe align by the whole line, instead of using baseline?
  unsigned m_pad_bottom;

public:
  termwtf(jute::view font, unsigned font_h)
      : m_face{new_face(m_lib, font, font_h)}, m_font_h{font_h},
        m_pad_bottom{font_h / 4} {}

  void add_line(jute::view line) { m_term.add_line(line); }

  void print(unsigned char *img, unsigned img_w, unsigned img_h) {
    auto lines = 1 + img_h / m_font_h; // One extra for pixels below baseline
    auto x = 0;
    auto y = img_h - (lines - 1) * m_font_h - m_pad_bottom;
    m_term.print(lines, [&](auto line) {
      m_face.shape_pt(line).draw(img, img_w, img_h, x, y);
      y += m_font_h;
    });
  }
};

export class ansiwtf {
  ansibuf m_buf;
  ::wtf::library m_lib{};
  ::wtf::face m_face;
  unsigned m_font_h;
  unsigned m_img_w;
  unsigned m_img_h;

  auto em() {
    auto [w, h] = m_face.shape_pt("m").bounding_box();
    return static_cast<unsigned>(w);
  }

public:
  ansiwtf(jute::view font, unsigned font_h, unsigned con_w, unsigned con_h)
      : m_buf{con_w, con_h}, m_face{new_face(m_lib, font, font_h)},
        m_font_h{font_h}, m_img_w{em() * con_w}, m_img_h{con_h * font_h} {}

  void run(jute::view input) { m_buf.run(input); }

  [[nodiscard]] constexpr auto image_w() const noexcept { return m_img_w; }
  [[nodiscard]] constexpr auto image_h() const noexcept { return m_img_h; }

  void print(unsigned char *img) {
    auto y = 0;
    for (auto row : m_buf) {
      y += m_font_h;
      m_face.shape_pt(row).draw(img, m_img_w, m_img_h, 0, y);
    }
  }
};
}; // namespace pity
