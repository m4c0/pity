export module pity:termbuf;
import hai;
import jute;

namespace pity {
export class termbuf {
  static constexpr const auto max_lines = 1024;

  hai::cstr m_buf[max_lines]{};
  unsigned m_count{};

public:
  constexpr void add_line(jute::view msg) {
    if (m_count == max_lines) {
      return;
    }

    m_buf[m_count++] = msg.cstr();
  }

  constexpr void print(unsigned qty, auto &&fn) {
    auto start = qty >= m_count ? 0 : m_count - qty;
    for (auto idx = start; idx < m_count; idx++) {
      fn(m_buf[idx].data());
    }
  }
};
} // namespace pity

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
  t.print(4, [ptr = &res[0]](auto line) mutable {
    *ptr++ = jute::view::unsafe(line);
  });

  using namespace jute::literals;
  return res[0] == "Minha terra tem palmeiras,"_s &&
         res[1] == "Onde canta o Sabiá;"_s &&
         res[2] == "As aves, que aqui gorjeiam,"_s &&
         res[3] == "Não gorjeiam como lá."_s;
}());
