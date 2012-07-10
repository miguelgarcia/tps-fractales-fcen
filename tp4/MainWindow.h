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
  bool on_image_mouse_move(GdkEventMotion *);
  bool on_image_mouse_click(GdkEventButton *);

  Glib::RefPtr<Gtk::Builder> m_refGlade;
  Gtk::Button* m_pBtnDraw;
  Gtk::Image* m_pImage;
  Gtk::Label *m_pPointerPosition;
  Gtk::SpinButton *m_pXRange[2];
  Gtk::SpinButton *m_pYRange[2];
  Gtk::SpinButton *m_pSemillas;
  Gtk::SpinButton *m_pIteraciones;
  Gtk::CheckButton *m_pPerturbar;
  Gtk::Entry *m_pFormulaX;
  Gtk::Entry *m_pFormulaY;

  double m_p[2][2];
};

#endif
