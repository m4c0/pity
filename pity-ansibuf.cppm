export module pity:ansibuf;
import hai;
import jute;

namespace pity {
class ansibuf {
  hai::array<char> m_buf;
  unsigned m_rows;
  unsigned m_cols;
  unsigned m_cursor{};

  constexpr void append(char c) {
    m_buf[m_cursor++] = c;
    if (m_cursor != m_rows * m_cols)
      return;

    m_cursor -= m_cols;
    for (auto i = 0; i < m_cursor; i++) {
      m_buf[i] = m_buf[i + m_cols];
    }
    for (auto i = 0; i < m_cols; i++) {
      m_buf[m_cursor + i] = ' ';
    }
  }

  [[nodiscard]] constexpr unsigned c0(jute::view str) {
    switch (char c = str[0]) {
    case 0x1b: // ESC
      return 1;
    case '\n':
      do {
        append(' ');
      } while (m_cursor % m_cols != 0);

      return 1;
    default:
      append(c);
      return 1;
    }
  }

public:
  constexpr ansibuf(unsigned w, unsigned h)
      : m_buf{w * h}, m_cols{w}, m_rows{h} {}

  /// Interpret a series of ANSI chars
  constexpr void run(jute::view str) {
    auto p = 0;
    while (p < str.size()) {
      auto [_, s] = str.subview(p);
      p += c0(s);
    }
  }

  constexpr auto begin() const noexcept { return m_buf.begin(); }
  constexpr auto end() const noexcept { return m_buf.end(); }

  constexpr auto as_view() const noexcept {
    return jute::view{m_buf.begin(), m_buf.size()};
  }
};

constexpr const auto fail = [] -> bool { throw 0; };
static_assert([] {
  using namespace jute::literals;

  ansibuf b{5, 3};

  b.run("ok");
  (b.as_view() == "ok\0\0\0\0\0\0\0\0\0\0\0\0\0"_s) || fail();

  b.run("ay");
  (b.as_view() == "okay\0\0\0\0\0\0\0\0\0\0\0"_s) || fail();

  b.run("\n12\n");
  (b.as_view() == "okay 12   \0\0\0\0\0"_s) || fail();

  b.run("\n");
  (b.as_view() == "12             "_s) || fail();

  b.run("\n\n:q!");
  (b.as_view() == "          :q!  "_s) || fail();

  return true;
}());
} // namespace pity
