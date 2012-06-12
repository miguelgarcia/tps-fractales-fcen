#include "MainWindow.h"
#include <iostream>
#include <sstream>

void graficar(Glib::RefPtr<Gdk::Pixbuf> pbuf, double alfa_min, double alfa_max)
{
  guint8 *pixels = pbuf->get_pixels();
  int width = pbuf->get_width();
  int height = pbuf->get_height();
  int rowstride = pbuf->get_rowstride();
  double real_width = alfa_max-alfa_min;
  double step = real_width / width;

  double e = 0.0000001;
  double x = 0.;
  double first;
  double ciclo;
  int i;
  for(double alfa=alfa_min; alfa <= alfa_max; alfa += step)
  { // iterar el alfa
    for(first=0.001;first < 1.0; first += 0.01)
    {
      x = first;
      for(i=0; i<400;i++)
      {
        x = alfa * x * (1.0-x);
      }
      ciclo = x;
      i = 0;
      do
      {
        int pixY = height - x * height - 1;
        int pixX = (alfa-alfa_min) / real_width * width - 1;
        if(pixY < 0) pixY = 0;
        if(pixY >= height) pixY = height - 1;
        if(pixX < 0) pixX = 0;
        if(pixX >= width) pixX = width - 1;
        pixels[pixY * rowstride + pixX * 3] = 0;
        pixels[pixY * rowstride + pixX * 3+1] = 0;
        pixels[pixY * rowstride + pixX * 3+2] = 0;
        x = alfa * x * (1.0-x);
      } while(ciclo - x > e && i++ < 1500);
    }
  }
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
  m_refGlade(refGlade),
  m_pBtnDraw(0)
{
  m_p[0][0] = 3.2;
  m_p[0][1] = 0;
  m_p[1][0] = 4;
  m_p[1][1] = 1;
  m_refGlade->get_widget("btnDraw", m_pBtnDraw);
  m_refGlade->get_widget("image1", m_pImage);
  m_refGlade->get_widget("lblPointerPosition", m_pPointerPosition);
  m_refGlade->get_widget("spinAlfaMin", m_pAlfaRange[0]);
  m_pAlfaRange[0]->set_range(0, 5);
  m_pAlfaRange[0]->set_increments(0.1, 0.5);
  m_pAlfaRange[0]->set_value(m_p[0][0]);
  m_refGlade->get_widget("spinAlfaMax", m_pAlfaRange[1]);
  m_pAlfaRange[1]->set_range(0, 5);
  m_pAlfaRange[1]->set_increments(0.1, 0.5);
  m_pAlfaRange[1]->set_value(m_p[1][0]);

  if(m_pBtnDraw)
  {
    m_pBtnDraw->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_draw) );
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
		748
  );
  m_pImage->set(pbuf);
  on_button_draw();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_draw()
{
  m_p[0][0] = m_pAlfaRange[0]->get_value();
  m_p[1][0] = m_pAlfaRange[1]->get_value();

  m_pImage->get_pixbuf()->fill(0xffffffff);
  graficar(m_pImage->get_pixbuf(), m_p[0][0], m_p[1][0]);
  m_pImage->set(m_pImage->get_pixbuf());
}

bool MainWindow::on_image_mouse_move(GdkEventMotion *event)
{
  std::stringstream pos;
  pos << event->x / (double) m_pImage->get_pixbuf()->get_width() * (m_p[1][0] - m_p[0][0]) + m_p[0][0];
  pos << " , ";
  pos << ((double) m_pImage->get_pixbuf()->get_height() - event->y) / (double) m_pImage->get_pixbuf()->get_height() * (m_p[1][1] - m_p[0][1]) + m_p[0][1];
  m_pPointerPosition->set_text(pos.str());
  return true;
}

bool MainWindow::on_image_mouse_click(GdkEventButton *event)
{
  double alfa[2];
  alfa[0] = m_pAlfaRange[0]->get_value();
  alfa[1] = m_pAlfaRange[1]->get_value();
  double diff = alfa[1] - alfa[0];
  
  double real_x = event->x / (double) m_pImage->get_pixbuf()->get_width() * (m_p[1][0] - m_p[0][0]) + m_p[0][0];
  m_pAlfaRange[0]->set_value(real_x - diff / 10.0 >= 0. ? real_x - diff / 10.0 : 0.);
  m_pAlfaRange[1]->set_value(real_x + diff / 10.0);
  on_button_draw();
}
