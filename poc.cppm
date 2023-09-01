export module poc;

import hai;
import pity;
import stubby;

constexpr const auto img_w = 1024;
constexpr const auto img_h = 256;
constexpr const auto font_h = img_h / 4;
constexpr const auto img_len = img_w * img_h;

void term_test() {
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

  stbi::write_rgba("out/test.png", img_w, img_h, pixels);
}

extern "C" int main() { term_test(); }
