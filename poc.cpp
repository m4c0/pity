import pity;

extern "C" int puts(const char *);

int main() {
  pity::termbuf t{};
  t.add_line("==========================");
  t.add_line("Poem follows");
  t.add_line("==========================");
  t.add_line("Minha terra tem palmeiras,");
  t.add_line("Onde canta o Sabiá;");
  t.add_line("As aves, que aqui gorjeiam,");
  t.add_line("Não gorjeiam como lá.");
  t.print(4, puts);
}
