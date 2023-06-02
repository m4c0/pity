import hai;
import jute;

extern "C" int puts(const char *);

class termbuf {
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

int main() {
  termbuf t{};
  t.add_line("==========================");
  t.add_line("Poem follows");
  t.add_line("==========================");
  t.add_line("Minha terra tem palmeiras,");
  t.add_line("Onde canta o Sabiá;");
  t.add_line("As aves, que aqui gorjeiam,");
  t.add_line("Não gorjeiam como lá.");
  t.print(4, puts);
}
