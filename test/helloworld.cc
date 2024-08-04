#include <icpp/qt/QMessageBox.h>

int main(int argc, const char *argv[]) {
  QMessageBox::information(nullptr, "icpp-qt", "Hello world.");
  return 0;
}
