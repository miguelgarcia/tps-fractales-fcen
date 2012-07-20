#include "MainWindow.h"
#include <iostream>

int main (int argc, char **argv)
{
  //Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
  Gtk::Main kit(argc, argv);

  //Load the Glade file and instiate its widgets:
  Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("win.ui");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated dialog::
  MainWindow* pDialog = 0;
  refBuilder->get_widget_derived("window", pDialog);
  if(pDialog)
  {
    //Start:
    kit.run(*pDialog);
  }

  delete pDialog;

  return 0;
}
