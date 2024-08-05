#pragma once

#include <QtCore/QObject>

class HelloICPP : public QObject {
  Q_OBJECT

public:
  explicit HelloICPP(QObject *parent = nullptr);
  ~HelloICPP() override;

public slots:
  void hello();

signals:
  void sayHello();
};
