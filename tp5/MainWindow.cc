#include "MainWindow.h"
#include <iostream>
#include <sstream>
#include "mdp2d.h"

using namespace std;

void setupSpin(Gtk::SpinButton *spin, double min, double max,
    double incrementSmall, double incrementBig, double value)
{
  spin->set_range(min, max);
  spin->set_increments(incrementSmall, incrementBig);
  spin->set_value(value);
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
  m_refGlade(refGlade),
  m_pBtnDraw(0)
{
  m_Palette = Gdk::Pixbuf::create_from_file("palette1.png");
  
  m_refGlade->get_widget("btnDraw", m_pBtnDraw);
  m_refGlade->get_widget("imgResult", m_pImage);
  m_refGlade->get_widget("spinH", m_pH);
  m_refGlade->get_widget("spinNoise", m_pNoise);
  setupSpin(m_pH, 0, 2, 0.025, 0.05, 0.5);
  setupSpin(m_pNoise, 0, 10, 0.025, 0.05, 0);
  
  if(m_pBtnDraw)
  {
    m_pBtnDraw->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw) );
  }

  m_side = 1025;
  m_result = new double[m_side * m_side];
  
  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create(
    Gdk::COLORSPACE_RGB ,
    false,
    8,
		m_side,
    m_side
  );
  m_pImage->set(pbuf);
  
  // Configure menu
  Gtk::MenuItem *mnuItem;
  m_refGlade->get_widget("mnuOpenPalette", mnuItem);
  mnuItem->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_open_palette));
}

MainWindow::~MainWindow()
{
  delete [] m_result;
}

void MainWindow::on_button_draw()
{
  double h = m_pH->get_value();
  double noise = m_pNoise->get_value();
  
  zero_result();
  mdp2d(m_result, m_side, h, noise);
  paint();
}

void MainWindow::paint()
{
  guint32 y, x;
  guint8 *pixels = m_pImage->get_pixbuf()->get_pixels();
  guint8 *palette = m_Palette->get_pixels();
  guint32 palette_width = m_Palette->get_width() - 1;
  guint32 rowstride = m_pImage->get_pixbuf()->get_rowstride();
  
  for(y=0; y<m_side; y++)
  {
    for(x=0; x<m_side; x++)
    {
      guint32 idx = min(255., max(0., (m_result[y * m_side + x] + 1.) * 128.));
      pixels[y * rowstride + 3 * x] = palette[3 * idx];
      pixels[y * rowstride + 3 * x + 1] = palette[3 * idx+1];
      pixels[y * rowstride + 3 * x + 2] = palette[3 * idx+2];
    }
  }
  m_pImage->set(m_pImage->get_pixbuf());
}

void MainWindow::zero_result()
{
  memset(m_result, 0, m_side * m_side * sizeof(guint32));
}

void MainWindow::on_open_palette()
{
  Gtk::FileChooserDialog dialog("Seleccione nueva paleta", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("Abrir", Gtk::RESPONSE_OK);
  dialog.set_default_response(Gtk::RESPONSE_OK);
  Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
  filter->set_name("Imagenes");
  filter->add_pixbuf_formats();
  dialog.add_filter(filter);
  if(dialog.run() == Gtk::RESPONSE_OK)
  {
    m_Palette =  Gdk::Pixbuf::create_from_file(dialog.get_filename());
    paint();
  }
}
