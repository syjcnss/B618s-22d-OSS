
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_javax_print_PrinterDialog$AppearancePanel$Color__
#define __gnu_javax_print_PrinterDialog$AppearancePanel$Color__

#pragma interface

#include <javax/swing/JPanel.h>
extern "Java"
{
  namespace gnu
  {
    namespace javax
    {
      namespace print
      {
          class PrinterDialog$AppearancePanel;
          class PrinterDialog$AppearancePanel$Color;
      }
    }
  }
  namespace java
  {
    namespace awt
    {
      namespace event
      {
          class ActionEvent;
      }
    }
  }
  namespace javax
  {
    namespace swing
    {
        class JRadioButton;
    }
  }
}

class gnu::javax::print::PrinterDialog$AppearancePanel$Color : public ::javax::swing::JPanel
{

public: // actually package-private
  PrinterDialog$AppearancePanel$Color(::gnu::javax::print::PrinterDialog$AppearancePanel *);
public:
  void actionPerformed(::java::awt::event::ActionEvent *);
public: // actually package-private
  void updateForSelectedService();
private:
  ::javax::swing::JRadioButton * __attribute__((aligned(__alignof__( ::javax::swing::JPanel)))) bw;
  ::javax::swing::JRadioButton * color;
public: // actually package-private
  ::gnu::javax::print::PrinterDialog$AppearancePanel * this$1;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_javax_print_PrinterDialog$AppearancePanel$Color__