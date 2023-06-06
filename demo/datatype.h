#pragma once

#include <QString>
#include <QVector>

#include "qcustomplot.h"
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QRadioButton>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#pragma execution_character_set("utf-8")

struct EmployeeInfo {
  QString name;
  QString id;
};

struct BloodPressureRes {
  QVector<double> time;
  QVector<int> systolic_pressure;  //收缩压
  QVector<int> diastolic_pressure; //舒张压
  int sp_res;
  int dp_res;
};

struct ECGRes {
  QVector<double> time;
  QVector<float> ECG; //舒张压
  float sum;
};
/////.....
