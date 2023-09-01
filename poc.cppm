export module poc;

import hai;
import pity;
import silog;
import stubby;

extern "C" int main() {
  pity::wtf t{"VictorMono-Regular.otf", 16, 80, 24};
  t.run("\e[10;15HMinha terra tem palmeiras,");
  t.run("\e[11;15HOnde canta o Sabiá;");
  t.run("\e[12;15HAs aves, que aqui gorjeiam,");
  t.run("\e[13;15HNão gorjeiam como lá.");

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
