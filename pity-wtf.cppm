export module pity:wtf;
import :termbuf;
import jute;
import what_the_font;

namespace pity {
export class wtf {
  termbuf m_term{};
  ::wtf::library m_lib{};
  ::wtf::face m_face;
  unsigned m_font_h;
  unsigned m_scroll{};

public:
  wtf(const char *font, unsigned font_h)
      : m_face{m_lib.new_face(font, font_h)}, m_font_h{font_h} {}

  void add_line(jute::view line) { m_term.add_line(line); }
  void set_scroll(unsigned s) { m_scroll = s; }

  void print(unsigned char *img, unsigned img_w, unsigned img_h) {
    auto lines = 1 + img_h / m_font_h; // One extra for pixels below baseline
    auto x = 0;
    auto y = img_h - (lines - 1) * m_font_h - m_scroll;
    m_term.print(lines, [&](auto line) {
      m_face.shape_pt(line).draw(img, img_w, img_h, x, y);
      y += m_font_h;
    });
  }
};
}; // namespace pity
