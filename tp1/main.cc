#include <gtkmm.h>
#include <iostream>
#include <cmath>

using namespace std;

bool pixel_darker_than(Glib::RefPtr<Gdk::Pixbuf> pbuf, int y0, int x0, int height, int width, int dark)
{
  int x, y, chan;
  guint8 *pixels = pbuf->get_pixels();
  int rowstride = pbuf->get_rowstride();
  int channels = pbuf->get_n_channels();
  for(y=y0; y < y0 + height && y < pbuf->get_height(); y++)
  {
    for(x=x0; x < x0 + width && x < pbuf->get_width(); x++)
    {
      double avg = 0;
      for(chan=0; chan < channels; chan++)
      {
        avg += (double) pixels[y * rowstride + x * channels + chan];
      }
      if(avg / ((double) channels) < dark)
      {
        return true;
      }
    }
  }
  return false;
}

void dimensionFractal(Glib::RefPtr<Gdk::Pixbuf> pbuf, int dark)
{
  int side, row, col, painted;
  int bigbox = 1;
  double d_side;
  for(bigbox=1;bigbox <= pbuf->get_width() && bigbox <= pbuf->get_height(); bigbox *= 2);
  bigbox /= 2;

  for(side=1;side<bigbox; side *= 2)
  { // Iterar por los tamanyos de box
    painted = 0;
    for(row=0; row < bigbox / side; row++)
    {
      for(col=0; col < bigbox / side; col++)
      { // Por cada box ver si esta pintado y contarlo
        if(pixel_darker_than(pbuf, row * side, col * side, side, side, dark))
        {
          painted++;
        }
      }
    }
    //d_side = ((double) side) / ((double) bigbox);
    //double log_1_side = log(1. / (((double) side) / ((double) bigbox)));
    double log_1_side = log(bigbox / ((double) side));
    double log_painted = log(painted);
    double dim = log_painted / log_1_side;
    cout << side << " " << painted << " " << log_1_side << " " << log_painted << " " << dim << endl;
  }
}

int main (int argc, char **argv)
{
  Gtk::Main kit(argc, argv);
  string fileName(argv[1]);
  cout << fileName << endl;
  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create_from_file(fileName);
  dimensionFractal(pbuf, 250);
  return 0;
}
