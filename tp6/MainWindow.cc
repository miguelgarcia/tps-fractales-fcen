#include "MainWindow.h"
#include <iostream>
#include <cstdlib>

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
  m_refGlade(refGlade)
{
  m_refGlade->get_widget("btnStart", m_pBtnStart);
  m_refGlade->get_widget("btnStop", m_pBtnStop);
  m_refGlade->get_widget("imgResult", m_pImage);
  m_refGlade->get_widget("spinVive", m_pMinLife);
  m_refGlade->get_widget("spinNace", m_pMinBorn);
  m_refGlade->get_widget("spinPoblacion", m_pPoblacion);
  setupSpin(m_pMinBorn, 0, 8, 1, 1, 3);
  setupSpin(m_pMinLife, 0, 8, 1, 1, 2);
  setupSpin(m_pPoblacion, 0, 1, 0.005, 0.05, 0.2);
  
  if(m_pBtnStart)
  {
    m_pBtnStart->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_start) );
  }
  
  if(m_pBtnStop)
  {
    m_pBtnStop->signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_stop ) );
  }

  m_side = 100;
  m_pixel_scale = 6;
  m_grid = new bool[m_side * m_side];
  m_tmp_grid = new bool[m_side * m_side];
  
  
  Glib::RefPtr<Gdk::Pixbuf> pbuf = Gdk::Pixbuf::create(
    Gdk::COLORSPACE_RGB ,
    false,
    8,
		m_side * m_pixel_scale,
    m_side * m_pixel_scale
  );
  m_pImage->set(pbuf);
}

MainWindow::~MainWindow()
{
  delete [] m_grid;
  delete [] m_tmp_grid;
}

void MainWindow::on_button_start()
{
  init_grid();
  paint();
}

void MainWindow::on_button_stop()
{
}

void MainWindow::paint()
{
  guint8 *pixels = m_pImage->get_pixbuf()->get_pixels();
  guint32 rowstride = m_pImage->get_pixbuf()->get_rowstride();
 
  m_pImage->get_pixbuf()->fill(0x000077);
  guint32 x, y, pixX, pixY;
  for(y=0; y<m_side; y++)
  {
    for(x=0; x<m_side; x++)
    {
      if(m_grid[y * m_side + x])
      {
        for(pixY = y * m_pixel_scale; pixY < (y+1) * m_pixel_scale; pixY++)
          for(pixX = x * m_pixel_scale; pixX < (x+1) * m_pixel_scale; pixX++)
            pixels[pixY * rowstride + 3 * pixX + 2] = 255;
      }
    }
  }
  //    pixels[y * rowstride + 3 * x + 2] = palette[3 * idx+2];
  m_pImage->set(m_pImage->get_pixbuf());
}

void MainWindow::init_grid()
{
  guint32 x, y;
  guint32 minLifeInt = m_pPoblacion->get_value() * RAND_MAX;
          
  for(y=0; y<m_side; y++)
  {
    for(x=0; x<m_side; x++)
    {
      m_grid[y * m_side + x] = (rand() < minLifeInt); 
    }
  }
}

