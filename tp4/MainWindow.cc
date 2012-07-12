#include "MainWindow.h"
#include <iostream>
#include <sstream>

#include "julia.h"

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
  m_pBtnDrawIteracionDirecta(0)
{
  m_p[0] = complex_d(-2, -2);
  m_p[1] = complex_d(2, 2);
  
  m_refGlade->get_widget("btnDrawIteracionDirecta", m_pBtnDrawIteracionDirecta);
  m_refGlade->get_widget("btnDrawIteracionInversa", m_pBtnDrawIteracionInversa);
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
  setupSpin(m_pRRange[0], -4, 4, 0.025, 0.05, m_p[0].real());
  setupSpin(m_pRRange[1], -4, 4, 0.025, 0.05, m_p[1].real());
  setupSpin(m_pImRange[0], -4, 4, 0.025, 0.05, m_p[0].imag());
  setupSpin(m_pImRange[1], -4, 4, 0.025, 0.05, m_p[1].imag());
  setupSpin(m_pBlowup, 0, 16, 0.025, 0.05, 2);
  setupSpin(m_pMaxIteraciones, 1, 255, 1, 3, 255);

  if(m_pBtnDrawIteracionDirecta)
  {
    m_pBtnDrawIteracionDirecta->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_iteracion_directa) );
  }

  if(m_pBtnDrawIteracionInversa)
  {
    m_pBtnDrawIteracionInversa->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw_iteracion_inversa) );
  }
  
  Gtk::Viewport *viewport;
  m_refGlade->get_widget("viewport1", viewport);
  viewport->add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK);
  viewport->signal_motion_notify_event().connect( sigc::mem_fun(*this, &MainWindow::on_image_mouse_move) );
  viewport->signal_button_press_event().connect( sigc::mem_fun(*this, &MainWindow::on_image_mouse_click) );

  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create(
    Gdk::COLORSPACE_RGB ,
    false,
    8,
		1024,
		768
  );
  m_pImage->set(pbuf);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_draw_iteracion_directa()
{
  m_p[0].real(m_pRRange[0]->get_value());
  m_p[1].real(m_pRRange[1]->get_value());
  m_p[0].imag(m_pImRange[0]->get_value());
  m_p[1].imag(m_pImRange[1]->get_value());
  double blowup = m_pBlowup->get_value();
  guint32 max_iteraciones = m_pMaxIteraciones->get_value();
  string fz = m_pFz->get_text();
  m_pImage->get_pixbuf()->fill(0);
  if(fz != "")
  {
    julia_iteration(m_pImage->get_pixbuf(), fz, m_p[0], m_p[1], blowup, max_iteraciones);
    m_pImage->set(m_pImage->get_pixbuf());
  }
}

void MainWindow::on_button_draw_iteracion_inversa()
{
  m_p[0].real(m_pRRange[0]->get_value());
  m_p[1].real(m_pRRange[1]->get_value());
  m_p[0].imag(m_pImRange[0]->get_value());
  m_p[1].imag(m_pImRange[1]->get_value());
  double min_diff = m_pBlowup->get_value();
  guint32 max_iteraciones = m_pMaxIteraciones->get_value();
  string fz = m_pFz->get_text();
  m_pImage->get_pixbuf()->fill(0);
  if(fz != "")
  {
    julia_inverse_iteration(m_pImage->get_pixbuf(), fz, m_p[0], m_p[1], min_diff, max_iteraciones);
    m_pImage->set(m_pImage->get_pixbuf());
  }
}

bool MainWindow::on_image_mouse_move(GdkEventMotion *event)
{
  std::stringstream pos;
  pos << event->x / (double) m_pImage->get_pixbuf()->get_width() * (m_p[1].real() - m_p[0].real()) + m_p[0].real();
  pos << " , ";
  pos << ((double) event->y) / (double) m_pImage->get_pixbuf()->get_height() * (m_p[1].imag() - m_p[0].imag()) + m_p[0].imag();
  m_pPointerPosition->set_text(pos.str());
  return true;
}

bool MainWindow::on_image_mouse_click(GdkEventButton *event)
{
}
