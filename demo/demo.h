#pragma once

#include "BloodPressureWidget.h"
#include "ECGWidget.h"
#include "datatype.h"
#include "ui_demo.h"
#include <QtWidgets/QWidget>

class demo : public QWidget {
  Q_OBJECT

public:
  demo(QWidget *parent = nullptr);
  ~demo();

private:
  Ui::demoClass ui;

  EmployeeInfo e_info;
  BloodPressureRes bp_res;
  ECGRes ecg_res;

  BloodPressureWidget *blood_pressure_widget;
  ECGWidget *ecg_widget;

  QWidget *res_widget;
  QSerialPort *serial; //串口

  void saveFile(QString filename, int type);
  // res

private slots:
  void Btn_OnStartClicked();
  void OnAllFinished();
  void OnBPFinished(BloodPressureRes);
  void OnECGFinished(ECGRes);
};
