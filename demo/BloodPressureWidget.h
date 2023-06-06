#pragma once

#include "datatype.h"
#include "ui_BloodPressureWidget.h"
#include <QWidget>

class BloodPressureWidget : public QWidget {
  Q_OBJECT

public:
  BloodPressureWidget(QWidget *parent = nullptr);
  ~BloodPressureWidget();

  void setSerialPort(QSerialPort *ser) { serial_ = ser; }

signals:
  void onFinished(BloodPressureRes);

private:
  Ui::BloodPressureWidgetClass ui;
  QSerialPort *serial_; //
  bool receiveMsg_;
  QTimer timer_;
  BloodPressureRes res;

private slots:
  void Btn_OnStartClicked();
  void Btn_OnNextClicked();
  void Btn_OnStopClicked();
  void handleReadyRead();
  void sendData();
};
