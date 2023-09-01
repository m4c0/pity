export module pity:termbuf;
import hai;
import jute;
import traits;

namespace pity {
export class termbuf {
  static constexpr const auto max_lines = 128;

  hai::cstr m_buf[max_lines]{};
  unsigned m_count{};

public:
  constexpr void add_line(jute::view msg) {
    if (m_count < max_lines) {
      m_buf[m_count++] = msg.cstr();
      return;
    }

    for (auto i = 0; i < max_lines - 1; i++) {
      m_buf[i] = traits::move(m_buf[i + 1]);
    }
    m_buf[max_lines - 1] = msg.cstr();
  }

  constexpr void print(unsigned qty, auto &&fn) {
    auto start = qty >= m_count ? 0 : m_count - qty;
    for (auto idx = start; idx < m_count; idx++) {
      fn(jute::view{m_buf[idx]});
    }
  }
};
} // namespace pity

static_assert([] {
  pity::termbuf t{};
  t.add_line("I'm a test");

  jute::view res[4];
  t.print(4, [ptr = &res[0]](auto line) mutable { *ptr++ = line; });

  using namespace jute::literals;
  return res[0] == "I'm a test"_s && res[1] == ""_s && res[2] == ""_s &&
         res[3] == ""_s;
}());
static_assert([] {
  pity::termbuf t{};
  t.add_line("==========================");
  t.add_line("Poem follows");
  t.add_line("==========================");
  t.add_line("Minha terra tem palmeiras,");
  t.add_line("Onde canta o Sabiá;");
  t.add_line("As aves, que aqui gorjeiam,");
  t.add_line("Não gorjeiam como lá.");

  jute::view res[4];
  t.print(4, [ptr = &res[0]](auto line) mutable { *ptr++ = line; });

  using namespace jute::literals;
  return res[0] == "Minha terra tem palmeiras,"_s &&
         res[1] == "Onde canta o Sabiá;"_s &&
         res[2] == "As aves, que aqui gorjeiam,"_s &&
         res[3] == "Não gorjeiam como lá."_s;
}());
static_assert([] {
  pity::termbuf t{};
  for (auto i = 0; i < 200; i++) {
    t.add_line("");
  }
  t.add_line("ok");

  jute::view res[4];
  t.print(4, [ptr = &res[0]](auto line) mutable { *ptr++ = line; });

  using namespace jute::literals;
  return res[0] == ""_s && res[1] == ""_s && res[2] == ""_s && res[3] == "ok"_s;
}());
