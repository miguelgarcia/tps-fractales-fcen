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
  void on_button_draw();
  void on_open_palette();
  
  void paint();
  void zero_result();
  
  Glib::RefPtr<Gtk::Builder> m_refGlade;
  Gtk::Button* m_pBtnDraw;
  Gtk::Image* m_pImage;
  Gtk::SpinButton *m_pH;
  Gtk::SpinButton *m_pNoise;
  
  Glib::RefPtr<Gdk::Pixbuf> m_Palette;
  guint32 m_side;
  double *m_result;
};

#endif
