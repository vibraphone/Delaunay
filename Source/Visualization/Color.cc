#include "Color.hh"

namespace Delaunay
{
namespace Visualization
{

const Color Clear(0,0,0,0);
const Color White(255,255,255,255);
const Color Black(0,0,0,255);
const Color Red(255,0,0,255);
const Color Orange(255,128,0,255);
const Color Yellow(255,255,0,255);
const Color Green(128,255,0,255);
const Color Blue(0,0,255,255);
const Color Indigo(75,0,130,255);
const Color Violet(139,0,255,255);
const Color Cyan(0,255,255,255);
const Color Purple(128,0,255,255);

Color::Color(unsigned char r,
	     unsigned char g,
	     unsigned char b,
	     unsigned char al) : red(r), green(g), blue(b), alpha(al) {}

Color::Color(unsigned hex, unsigned char al) : alpha(al)
{
  red   = ((hex & 0xFF0000) >> 16);
  green = ((hex & 0x00FF00) >> 8);
  blue  = (hex & 0x0000FF);
}

Color::Color(double range, Color::Palette palette, unsigned char al) : alpha(al)
{
  if (range < 0.) range = 0.;
  if (range > 1.) range = 1.;

  switch(palette)
  {
  case Rainbow:
  {
    if (range == 1.)
    {
      this->red = Red.red;
      this->green = Red.green;
      this->blue = Red.blue;
      return;
    }

    static const Color rainbow[7] =
      {Violet,Indigo,Blue,Green,Yellow,Orange,Red};

    int floor = 0;
    int ceil = 1;

    while (range > 1./7.)
    {
      ++floor;
      ++ceil;
      range -= 1./7.;
    }

    range *= 7.;

    this->red = rainbow[floor].red*(1.-range) + rainbow[ceil].red*range;
    this->green = rainbow[floor].green*(1.-range) + rainbow[ceil].green*range;
    this->blue = rainbow[floor].blue*(1.-range) + rainbow[ceil].blue*range;
    break;
  }
  case BlueToRed:
  {
    this->red = Blue.red*(1.-range) + Red.red*range;
    this->green = Blue.green*(1.-range) + Red.green*range;
    this->blue = Blue.blue*(1.-range) + Red.blue*range;

    break;
  }
  case Grayscale:
  {
    this->red = Black.red*(1.-range) + White.red*range;
    this->green = Black.green*(1.-range) + White.green*range;
    this->blue = Black.blue*(1.-range) + White.blue*range;
    break;
  }
  }
}

bool Color::operator==(const Color& other) const
{
  return (red == other.red && green == other.green &&
	  blue == other.green && alpha == other.alpha);
}

}
}
