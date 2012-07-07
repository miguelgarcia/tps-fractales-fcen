#include "MainWindow.h"
#include <iostream>
#include <sstream>
#include "graficador.h"

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
  m_p[0][0] = 0;
  m_p[0][1] = 0;
  m_p[1][0] = 1;
  m_p[1][1] = 1;
  m_refGlade->get_widget("btnDraw", m_pBtnDraw);
  m_refGlade->get_widget("image1", m_pImage);
  m_refGlade->get_widget("lblPointerPosition", m_pPointerPosition);
  m_refGlade->get_widget("spinXMin", m_pXRange[0]);
  m_refGlade->get_widget("spinXMax", m_pXRange[1]);
  m_refGlade->get_widget("spinYMin", m_pYRange[0]);
  m_refGlade->get_widget("spinYMax", m_pYRange[1]);
  m_refGlade->get_widget("spinSemillas", m_pSemillas);
  m_refGlade->get_widget("spinIteraciones", m_pIteraciones);
  m_refGlade->get_widget("chkPerturbar", m_pPerturbar);
  m_refGlade->get_widget("txtFormulaX", m_pFormulaX);
  m_refGlade->get_widget("txtFormulaY", m_pFormulaY);

  setupSpin(m_pXRange[0], -1000, 1000, 0.1, 0.5, m_p[0][0]);
  setupSpin(m_pXRange[1], -1000, 1000, 0.1, 0.5, m_p[1][1]);
  setupSpin(m_pYRange[0], -1000, 1000, 0.1, 0.5, m_p[0][0]);
  setupSpin(m_pYRange[1], -1000, 1000, 0.1, 0.5, m_p[1][1]);
  setupSpin(m_pSemillas, 1, 100000, 1, 5, 10);
  setupSpin(m_pIteraciones, 1, 1000, 1, 5, 10);

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
  m_p[0][0] = m_pXRange[0]->get_value();
  m_p[1][0] = m_pXRange[1]->get_value();
  m_p[0][1] = m_pYRange[0]->get_value();
  m_p[1][1] = m_pYRange[1]->get_value();
  unsigned int semillas = m_pSemillas->get_value();
  unsigned int iteraciones = m_pIteraciones->get_value();
  string formulaX = m_pFormulaX->get_text();
  string formulaY = m_pFormulaY->get_text();
  m_pImage->get_pixbuf()->fill(0xffffffff);
  graficar(m_pImage->get_pixbuf(), formulaX, formulaY,
    m_p[0], m_p[1], semillas, iteraciones);
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
}
