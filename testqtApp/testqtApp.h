#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testqtApp.h"
#include <QTimer>
#include <array>

class testqtApp : public QMainWindow
{
    Q_OBJECT

public:
    testqtApp(QWidget *parent = nullptr);
    ~testqtApp();

    int throwsNumber = 0;

    QTimer* balanceTimer;

    std::vector<int> counts = std::vector<int>(6, 0);

private:
    Ui::testqtAppClass *ui;   

private slots:
    void on_generateButton_clicked();
    void on_resetButton_clicked();
	void on_stopButton_clicked();
    void on_generateUntilBalancedButton_clicked();
    void performBalanceStep();
	void updateResultLabels();
};

