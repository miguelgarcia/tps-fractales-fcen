#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
  MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
  virtual ~MainWindow();

protected:
  //Signal handlers:
  void on_button_start();
  void on_button_stop();
  bool on_timeout();
  
  void paint();
  void init_grid();
  
  Glib::RefPtr<Gtk::Builder> m_refGlade;
  Gtk::Button* m_pBtnStart;
  Gtk::Button* m_pBtnStop;
  Gtk::Image* m_pImage;
  Gtk::SpinButton *m_pMinLife;
  Gtk::SpinButton *m_pMaxLife;
  Gtk::SpinButton *m_pMinBorn;
  Gtk::SpinButton *m_pMaxBorn;
  Gtk::SpinButton *m_pPoblacion;
  
  Glib::RefPtr<Gdk::Pixbuf> m_Palette;
  int m_side;
  guint32 m_pixel_scale;
  bool m_running;
  guint32 m_refresh_millisecs;
  bool *m_grid;
  bool *m_tmp_grid;
};

#endif
