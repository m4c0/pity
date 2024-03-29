export module pity:ansibuf;
import hai;
import jute;

namespace pity {
constexpr int atoi(jute::view a) {
  int r = 0;
  for (auto c : a) {
    if (c < '0' || c > '9')
      return r;

    r = r * 10 + (c - '0');
  }
  return r;
}
static_assert(atoi("") == 0);
static_assert(atoi("12a") == 12);
static_assert(atoi("0000002") == 2);

class it {
  jute::view m_view{};
  unsigned m_len{};

public:
  constexpr it() = default;
  constexpr explicit it(jute::view v, unsigned l) : m_view{v}, m_len{l} {}

  [[nodiscard]] constexpr bool operator==(const it &o) const noexcept {
    return m_view == o.m_view;
  }

  [[nodiscard]] constexpr auto &operator++() noexcept {
    auto [a, b] = m_view.subview(m_len);
    m_view = b;
    return *this;
  }

  [[nodiscard]] constexpr auto operator*() const noexcept {
    auto [a, b] = m_view.subview(m_len);
    return a;
  }
};

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
  constexpr int coord(jute::view str) {
    auto i = atoi(str);
    return i == 0 ? 0 : i - 1;
  }

  constexpr void cup(jute::view str) {
    auto [n, m] = str.split(';');
    auto ni = coord(n);
    auto mi = coord(m);

    m_cursor = ni * m_cols + mi;
  }

  [[nodiscard]] constexpr unsigned csi(jute::view str) {
    auto res = 2;
    while (str.size() > res) {
      switch (auto c = str[res++]) {
      case 'H': {
        auto [_, nm, __] = str.subview(2, str.size() - 3);
        cup(nm);
        return res;
      }
      default:
        // Non-ANSI CSIs
        if (c >= 0x60 && c <= 0x7F)
          return res;

        // ANSI CSIs
        if (c >= 0x40 && c <= 0x5F)
          return res;

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
      : m_buf{w * h}, m_rows{h}, m_cols{w} {
    for (auto &c : m_buf)
      c = ' ';
  }

  /// Interpret a series of ANSI chars
  constexpr void run(jute::view str) {
    auto p = 0;
    while (p < str.size()) {
      auto [_, s] = str.subview(p);
      p += c0(s);
    }
  }

  constexpr auto as_view() const noexcept {
    return jute::view{m_buf.begin(), m_buf.size()};
  }

  constexpr auto begin() const noexcept { return it{as_view(), m_cols}; }
  constexpr auto end() const noexcept { return it{}; }
};

constexpr const auto fail = []() -> bool { throw 0; };
constexpr const auto check = [](auto &b, jute::view s) {
  (b.as_view() == s) || fail();
};
static_assert([] {
  using namespace jute::literals;

  ansibuf b{5, 3};

  b.run("ok");
  (b.as_view() == "ok             "_s) || fail();

  b.run("ay");
  (b.as_view() == "okay           "_s) || fail();

  b.run("\n12\n");
  (b.as_view() == "okay 12        "_s) || fail();

  b.run("\n");
  (b.as_view() == "12             "_s) || fail();

  b.run("\n\n:q!");
  (b.as_view() == "          :q!  "_s) || fail();

  return true;
}());
static_assert([] {
  using namespace jute::literals;
  ansibuf b{5, 3};

  b.run("aaaaaaa\e[Hb");
  check(b, "baaaa"
           "aa   "
           "     ");

  b.run("\e[2Hc");
  check(b, "baaaa"
           "ca   "
           "     ");

  b.run("\e[;3Hd");
  check(b, "badaa"
           "ca   "
           "     ");

  b.run("\e[2;3He");
  check(b, "badaa"
           "cae  "
           "     ");

  return true;
}());
static_assert([] {
  using namespace jute::literals;

  ansibuf b{2, 2};

  // Do we suppress unknown CSI combos?
  b.run("\e[9999999Z");
  (b.as_view() == "    ") || fail();

  return true;
}());
static_assert([] {
  ansibuf b{5, 3};
  b.run("abcdefghijklmn");

  jute::view rows[3];
  jute::view *r = rows;
  for (auto row : b) {
    *r++ = row;
  }
  (rows[0] == "abcde") || fail();
  (rows[1] == "fghij") || fail();
  (rows[2] == "klmn ") || fail();
  return true;
}());
} // namespace pity
