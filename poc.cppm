export module poc;

import pity;

extern "C" int printf(const char *, ...);

extern "C" int main() {
  constexpr const auto img_w = 1024;
  constexpr const auto img_h = 256;
  constexpr const auto font_h = img_h / 4;

  pity::wtf t{"VictorMono-Regular.otf", font_h};
  t.add_line("==========================");
  t.add_line("Poem follows");
  t.add_line("==========================");
  t.add_line("Minha terra tem palmeiras,");
  t.add_line("Onde canta o Sabiá;");
  t.add_line("As aves, que aqui gorjeiam,");
  t.add_line("Não gorjeiam como lá.");
  t.set_scroll(16);

  unsigned char img[img_w * img_h]{};
  t.print(img, img_w, img_h);

  printf("P2\n1024 256 256\n");
  for (auto c : img)
    printf("%d ", c);
}
