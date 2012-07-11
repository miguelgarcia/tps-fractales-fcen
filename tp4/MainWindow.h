#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include "types.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
  virtual ~MainWindow();

protected:
  //Signal handlers:
  void on_button_draw();
  bool on_image_mouse_move(GdkEventMotion *);
  bool on_image_mouse_click(GdkEventButton *);

  Glib::RefPtr<Gtk::Builder> m_refGlade;
  Gtk::Button* m_pBtnDraw;
  Gtk::Image* m_pImage;
  Gtk::Label *m_pPointerPosition;
  Gtk::SpinButton *m_pRRange[2];
  Gtk::SpinButton *m_pImRange[2];
  Gtk::SpinButton *m_pBlowup;
  Gtk::SpinButton *m_pMaxIteraciones;
  Gtk::Entry *m_pFz;

  complex_d m_p[2];
};

#endif
