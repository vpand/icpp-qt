#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMessageBox::information(nullptr, "icpp-qt", "Hello world.");
  return app.exec();
}
