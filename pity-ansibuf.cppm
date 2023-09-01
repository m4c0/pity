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

  constexpr void cup(jute::view str) { m_cursor = 0; }

  [[nodiscard]] constexpr unsigned csi(jute::view str) {
    auto res = 2;
    while (str.size() > res) {
      switch (auto c = str[res++]) {
      case 'H':
        cup(str);
        return res;
      default:
        // Non-ANSI CSIs
        if (c >= 0x60 && c <= 0x7F)
          return res;

        // ANSI CSIs
        if (c >= 0x40 && c <= 0x5F)
          return res;

        res++;
        break;
      }
    }
    return res;
  }

  [[nodiscard]] constexpr unsigned fe(jute::view str) {
    if (str.size() < 1)
      return 1;

    switch (char c = str[1]) {
    case '[':
      return csi(str);
    default:
      if (c >= 0x40 && c <= 0x5F)
        return 2;

      append(0x1b);
      return 1;
    }
  }
  [[nodiscard]] constexpr unsigned c0(jute::view str) {
    switch (char c = str[0]) {
    case 0x1b: // ESC
      return fe(str);
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
static_assert([] {
  using namespace jute::literals;
  ansibuf b{5, 3};

  b.run("aaaaaaa\e[Hbb");
  (b.as_view() == "bbaaaaa\0\0\0\0\0\0\0\0") || fail();

  return true;
}());
static_assert([] {
  using namespace jute::literals;

  ansibuf b{2, 2};

  // Do we suppress unknown CSI combos?
  b.run("\e[9999999Z");
  (b.as_view() == "\0\0\0\0") || fail();

  return true;
}());
} // namespace pity
