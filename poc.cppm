export module poc;

import hai;
import pity;
import stubby;

void term_test() {
  constexpr const auto img_w = 1024;
  constexpr const auto img_h = 256;
  constexpr const auto font_h = img_h / 4;
  constexpr const auto img_len = img_w * img_h;

  pity::termwtf t{"VictorMono-Regular.otf", font_h};
  t.add_line("==========================");
  t.add_line("Poem follows");
  t.add_line("==========================");
  t.add_line("Minha terra tem palmeiras,");
  t.add_line("Onde canta o Sabiá;");
  t.add_line("As aves, que aqui gorjeiam,");
  t.add_line("Não gorjeiam como lá.");

  unsigned char img[img_len]{};
  t.print(img, img_w, img_h);

  hai::array<stbi::pixel> pixels{img_len};
  for (auto i = 0; i < img_len; i++) {
    auto c = img[i];
    pixels[i] = {c, c, c, 255};
  }

  stbi::write_rgba("out/term.png", img_w, img_h, pixels);
}

void ansi_test() {
  pity::ansiwtf t{"VictorMono-Regular.otf", 16, 80, 24};
  t.run("\e[10;15HHello World!");

  auto w = t.image_w();
  auto h = t.image_h();
  hai::array<unsigned char> img{w * h};
  t.print(img.begin());

  hai::array<stbi::pixel> pixels{w * h};
  for (auto i = 0; i < w * h; i++) {
    auto c = img[i];
    pixels[i] = {c, c, c, 255};
  }
  stbi::write_rgba("out/ansi.png", w, h, pixels);
}

extern "C" int main() {
  term_test();
  ansi_test();
}
