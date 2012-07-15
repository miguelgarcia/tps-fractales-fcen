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
  void on_button_draw_iteracion_directa();
  void on_button_draw_iteracion_inversa();
  void on_button_draw_preimagen();
  bool on_image_mouse_move(GdkEventMotion *);
  bool on_image_mouse_click(GdkEventButton *);

  void updateDrawRange();
  void paint(guint32 *values, double scale);
  Glib::RefPtr<Gtk::Builder> m_refGlade;
  Gtk::Button* m_pBtnDrawIteracionDirecta;
  Gtk::Button* m_pBtnDrawIteracionInversa;
  Gtk::Button* m_pBtnDrawPreimagen;
  Gtk::Image* m_pImage;
  Gtk::Label *m_pPointerPosition;
  Gtk::SpinButton *m_pRRange[2];
  Gtk::SpinButton *m_pImRange[2];
  Gtk::SpinButton *m_pBlowup;
  Gtk::SpinButton *m_pMaxIteraciones;
  Gtk::SpinButton *m_pMaxIteracionesInv;
  Gtk::SpinButton *m_pSemillasInv;
  Gtk::SpinButton *m_pMaxIteracionesPreimagen;
  Gtk::SpinButton *m_pSemillasPreimagen;
  Gtk::Entry *m_pFz;
  Gtk::Entry *m_pFzInv;
  Gtk::Entry *m_pFzPreimagen;
  Glib::RefPtr<Gdk::Pixbuf> m_Palette;
  
  complex_d m_p[2];
};

#endif
