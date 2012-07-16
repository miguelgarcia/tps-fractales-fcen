#include "MainWindow.h"
#include <iostream>
#include <sstream>

#include "julia.h"
#include "mandelbrot.h"

using namespace std;

guint32 array_max(guint32 *array, guint32 size)
{
  guint32 max = array[0];
  for(guint32 i=0; i<size;i++)
  {
    if(array[i] > max)
    {
      max = array[i];
    }
  }
  return max;
}

void setupSpin(Gtk::SpinButton *spin, double min, double max,
    double incrementSmall, double incrementBig, double value)
{
  spin->set_range(min, max);
  spin->set_increments(incrementSmall, incrementBig);
  spin->set_value(value);
}

void MainWindow::updateDrawRange()
{
  m_p[0].real(m_pRRange[0]->get_value());
  m_p[1].real(m_pRRange[1]->get_value());
  m_p[0].imag(m_pImRange[0]->get_value());
  m_p[1].imag(m_pImRange[1]->get_value());
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
  m_refGlade(refGlade),
  m_pBtnDrawIteracionDirecta(0)
{
  m_p[0] = complex_d(-2, -2);
  m_p[1] = complex_d(2, 2);
  
  m_Palette = Gdk::Pixbuf::create_from_file("palette1.png");
  
  m_refGlade->get_widget("btnDrawIteracionDirecta", m_pBtnDrawIteracionDirecta);
  m_refGlade->get_widget("btnDrawIteracionInversa", m_pBtnDrawIteracionInversa);
  m_refGlade->get_widget("btnDrawPreimagen", m_pBtnDrawPreimagen);
  m_refGlade->get_widget("btnDrawMandelbrot", m_pBtnDrawMandelbrot);
  m_refGlade->get_widget("image1", m_pImage);
  m_refGlade->get_widget("lblPointerPosition", m_pPointerPosition);
  m_refGlade->get_widget("spinRMin", m_pRRange[0]);
  m_refGlade->get_widget("spinRMax", m_pRRange[1]);
  m_refGlade->get_widget("spinImMin", m_pImRange[0]);
  m_refGlade->get_widget("spinImMax", m_pImRange[1]);
  m_refGlade->get_widget("spinBlowup", m_pBlowup);
  m_refGlade->get_widget("spinMaxIteraciones", m_pMaxIteraciones);
  m_refGlade->get_widget("txtFz", m_pFz);
  m_pFz->set_text("z*z+(0,285+0,01i)");
  m_refGlade->get_widget("txtFzInv", m_pFzInv);
  m_pFzInv->set_text("z-(0,285+0,01i)");
  m_refGlade->get_widget("txtFzPreimagen", m_pFzPreimagen);
  m_pFzPreimagen->set_text("z-(0,285+0,01i)");
  m_refGlade->get_widget("spinMaxIteracionesInv", m_pMaxIteracionesInv);
  m_refGlade->get_widget("spinSemillasInv", m_pSemillasInv);
  m_refGlade->get_widget("spinIteracionesPreimagen", m_pMaxIteracionesPreimagen);
  m_refGlade->get_widget("spinSemillasPreimagen", m_pSemillasPreimagen);
  setupSpin(m_pRRange[0], -4, 4, 0.025, 0.05, m_p[0].real());
  setupSpin(m_pRRange[1], -4, 4, 0.025, 0.05, m_p[1].real());
  setupSpin(m_pImRange[0], -4, 4, 0.025, 0.05, m_p[0].imag());
  setupSpin(m_pImRange[1], -4, 4, 0.025, 0.05, m_p[1].imag());
  setupSpin(m_pBlowup, 0, 16, 0.025, 0.05, 2);
  setupSpin(m_pMaxIteraciones, 1, 255, 1, 3, 255);
  setupSpin(m_pMaxIteracionesInv, 1, 100000, 1, 5, 1000);
  setupSpin(m_pSemillasInv, 1, 10000, 1, 5, 200);
  setupSpin(m_pMaxIteracionesPreimagen, 1, 1000, 1, 5, 10);
  setupSpin(m_pSemillasPreimagen, 1, 1000, 1, 5, 8);
  
  if(m_pBtnDrawIteracionDirecta)
  {
    m_pBtnDrawIteracionDirecta->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_iteracion_directa) );
  }

  if(m_pBtnDrawIteracionInversa)
  {
    m_pBtnDrawIteracionInversa->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_iteracion_inversa) );
  }
  
  if(m_pBtnDrawPreimagen)
  {
    m_pBtnDrawPreimagen->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_preimagen) );
  }

  if(m_pBtnDrawMandelbrot)
  {
    m_pBtnDrawMandelbrot->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_mandelbrot) );
  }

  m_width = 1024;
  m_height = 768;
  m_result = new guint32[m_height * m_width];
  
  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create(
    Gdk::COLORSPACE_RGB ,
    false,
    8,
		m_width,
    m_height
  );
  m_pImage->set(pbuf);
  
  Gtk::Viewport *viewport;
  m_refGlade->get_widget("viewport1", viewport);
  viewport->add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  viewport->signal_motion_notify_event().connect( sigc::mem_fun(*this, &MainWindow::on_image_mouse_move) );
  viewport->signal_button_press_event().connect( sigc::mem_fun(*this, &MainWindow::on_image_mouse_down) );
  viewport->signal_button_release_event().connect( sigc::mem_fun(*this, &MainWindow::on_image_mouse_up) );
  
  // Configurar menu
  Gtk::MenuItem *mnuItem;
  m_refGlade->get_widget("mnuAbrirPaleta", mnuItem);
  mnuItem->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_abrir_paleta));
  
  m_lastAction = NONE;
}

MainWindow::~MainWindow()
{
  delete [] m_result;
}

void MainWindow::on_button_draw_iteracion_directa()
{
  updateDrawRange();
  double blowup = m_pBlowup->get_value();
  guint32 max_iteraciones = m_pMaxIteraciones->get_value();
  string fz = m_pFz->get_text();
  if(fz != "")
  {
    zero_result();
    julia_direct_iteration(m_result, m_width, m_height, fz, m_p[0], m_p[1], blowup, max_iteraciones);
    m_palette_scale = 1. / ((double) max_iteraciones);
    paint();
  }
  m_lastAction = JULIA_DIRECT_ITERATION;
}

void MainWindow::on_button_draw_iteracion_inversa()
{
  updateDrawRange();
  guint32 max_iteraciones = m_pMaxIteracionesInv->get_value();
  guint32 semillas = m_pSemillasInv->get_value();
  string fz = m_pFzInv->get_text();
  
  if(fz != "")
  {
    zero_result();
    julia_inverse_iteration(m_result, m_width, m_height, fz, m_p[0], m_p[1], max_iteraciones, semillas);
    m_palette_scale = 2. / ((double) array_max(m_result, m_height * m_width));
    paint();
  }
    m_lastAction = JULIA_INVERSE_ITERATION;
}

void MainWindow::on_button_draw_preimagen()
{
  updateDrawRange();
  guint32 max_iteraciones = m_pMaxIteracionesPreimagen->get_value();
  guint32 semillas = m_pSemillasPreimagen->get_value();
  string fz = m_pFzPreimagen->get_text();
  
  if(fz != "")
  {
    zero_result();
    julia_preimage(m_result, m_width, m_height, fz, m_p[0], m_p[1], max_iteraciones, semillas);
    m_palette_scale = 2. / ((double) array_max(m_result, m_height * m_width));
    paint();
  }
    m_lastAction = JULIA_PREIMAGE;
}

void MainWindow::on_button_draw_mandelbrot()
{
  updateDrawRange();
  double blowup = m_pBlowup->get_value();
  guint32 max_iteraciones = m_pMaxIteraciones->get_value();
  string fz = m_pFz->get_text();
  
  if(fz != "")
  {
    zero_result();
    mandelbrot_direct_iteration(m_result, m_width, m_height, fz, m_p[0], m_p[1], blowup, max_iteraciones);
    m_palette_scale = 1. / ((double) max_iteraciones);
    paint();
  }
  m_lastAction = MANDELBROT;
}

complex_d MainWindow::img_coords_to_real(double x, double y)
{
  return complex_d(x / (double) m_width * (m_p[1].real() - m_p[0].real()) + m_p[0].real(),
                   y / (double) m_height * (m_p[1].imag() - m_p[0].imag()) + m_p[0].imag());
 
}
bool MainWindow::on_image_mouse_move(GdkEventMotion *event)
{
  std::stringstream status;
  status << img_coords_to_real(event->x, event->y);
  status << " = " << m_result[min(m_height-1, (guint32) event->y) * m_width + min(m_width-1, (guint32) event->x)];
  m_pPointerPosition->set_text(status.str());
  return true;
}

bool MainWindow::on_image_mouse_down(GdkEventButton *event)
{
  complex_d coord = img_coords_to_real(event->x, event->y);
  m_pRRange[0]->set_value(coord.real());
  m_pImRange[0]->set_value(coord.imag());
  return true;
}

bool MainWindow::on_image_mouse_up(GdkEventButton *event)
{
  complex_d coord = img_coords_to_real(event->x, event->y);
  if(m_pRRange[0]->get_value() > coord.real())
  {
    m_pRRange[1]->set_value(m_pRRange[0]->get_value());
    m_pRRange[0]->set_value(coord.real());
  }
  else
  {
    m_pRRange[1]->set_value(coord.real());
  }

  if(m_pImRange[0]->get_value() > coord.imag())
  {
    m_pImRange[1]->set_value(m_pImRange[0]->get_value());
    m_pImRange[0]->set_value(coord.imag());
  }
  else
  {
    m_pImRange[1]->set_value(coord.imag());
  }
  redo();
  return true;
}

/**
 * Pinta una matriz de valores sobre el pixbuf utilizando la paleta cargada
 * 
 * @param scale
 */
void MainWindow::paint()
{
  
  guint32 y, x;
  guint8 *pixels = m_pImage->get_pixbuf()->get_pixels();
  guint8 *palette = m_Palette->get_pixels();
  guint32 palette_width = m_Palette->get_width() - 1;
  guint32 rowstride = m_pImage->get_pixbuf()->get_rowstride();
  
  for(y=0; y<m_height; y++)
  {
    for(x=0; x<m_width; x++)
    {
      guint32 idx = min(1., m_palette_scale * m_result[y * m_width + x]) * palette_width;
      pixels[y * rowstride + 3 * x] = palette[3 * idx];
      pixels[y * rowstride + 3 * x + 1] = palette[3 * idx+1];
      pixels[y * rowstride + 3 * x + 2] = palette[3 * idx+2];
    }
  }
  m_pImage->set(m_pImage->get_pixbuf());
}

void MainWindow::zero_result()
{
  memset(m_result, 0, m_height * m_width * sizeof(guint32));
}

void MainWindow::on_abrir_paleta()
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

void MainWindow::redo()
{
  switch(m_lastAction)
  {
    case JULIA_DIRECT_ITERATION:
      on_button_draw_iteracion_directa();
      break;
    case JULIA_INVERSE_ITERATION:
      on_button_draw_iteracion_inversa();
      break;
    case JULIA_PREIMAGE:
      on_button_draw_preimagen();
      break;
    case MANDELBROT:
      on_button_draw_mandelbrot();
      break;
  }
}