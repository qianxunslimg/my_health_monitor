#pragma once

#include "datatype.h"
#include "ui_ECGWidget.h"
#include <QWidget>

class ECGWidget : public QWidget {
  Q_OBJECT

public:
  ECGWidget(QWidget *parent = nullptr);
  ~ECGWidget();

  void setSerialPort(QSerialPort *ser) { serial_ = ser; }

signals:
  void onFinished(ECGRes);

private:
  Ui::ECGWidgetClass ui;

  QSerialPort *serial_; //
  bool receiveMsg_;
  QTimer timer_;
  QTimer timer_plot_;
  ECGRes res;

  QCustomPlot *customPlot;

  QVector<double> xData;
  QVector<double> yData;
private slots:
  void Btn_OnStartClicked();
  void Btn_OnNextClicked();
  void Btn_OnFinishClicked();
  void Btn_OnStopClicked();
  void handleReadyRead();
  void sendData();
  void updatePlot();
};
