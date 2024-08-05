#include "helloworld.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

HelloICPP::HelloICPP(QObject *parent) : QObject(parent) {}

HelloICPP::~HelloICPP() {}

void HelloICPP::hello() {
  QMessageBox::information(nullptr, "icpp-qt", "Hello world.");
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  HelloICPP slot, signal;
  slot.connect(&signal, SIGNAL(sayHello()), SLOT(hello()));
  emit signal.sayHello();
  
  return app.exec();
}
