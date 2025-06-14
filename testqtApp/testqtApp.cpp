#include "testqtApp.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFile>

testqtApp::testqtApp(QWidget *parent) 
    : QMainWindow(parent)
{   
    ui = new Ui::testqtAppClass();
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/dice.png"));
    this->setWindowTitle("Symulator rzutów kością");

    balanceTimer = new QTimer(this);
    connect(balanceTimer, &QTimer::timeout, this, &testqtApp::performBalanceStep);

    // Ustaw styl aplikacji
    this->setStyleSheet(
        "QMainWindow {"
        "  background-color: #f0f2f5;"
        "  font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
        "  color: #333;"
        "}"
        "QPushButton {"
        "  background-color: #4a90e2;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 12px;"
        "  padding: 10px 20px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  box-shadow: 0px 4px 12px rgba(0, 0, 0, 0.1);"
        "}"
        "QPushButton:hover {"
        "  background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2c6cab;"
        "}"
        "QLabel {"
        "  font-size: 16px;"
        "  color: #444;"
        "}"
        "#resultLabel {"
        "  font-size: 32px;"
        "  font-weight: bold;"
        "  color: #2e7d32;"
        "  margin: 10px 0;"
        "}"
        "#attemptsLabel {"
        "  font-size: 20px;"
        "  color: #555;"
        "  margin-bottom: 20px;"
        "}"
        "QSpinBox, QSlider, QLineEdit {"
        "  background-color: white;"
        "  border: 1px solid #ccc;"
        "  border-radius: 6px;"
        "  padding: 4px;"
        "  font-size: 14px;"
        "}"
        "#label1, #label2, #label3, #label4, #label5, #label6 {"
        "  background-color: white;"
        "  border-radius: 12px;"
        "  padding: 12px;"
        "  font-size: 24px;"
        "  font-weight: bold;"
        "  color: #1976d2;"
        "  border: 2px solid #d0d0d0;"
        "  min-width: 60px;"
        "  min-height: 40px;"
        "  text-align: center;"
        "}"
        "#label10{"
        "border-top: 2px solid grey;"
        "}"
    );

}


void testqtApp::on_generateButton_clicked() {
    int throws = ui->throwsNumber->value();
    int lastNumber = 0;
    throwsNumber += throws; // zwieksz liczbe rzutow

    for (int i = 0; i < throws; ++i) {
        int number = QRandomGenerator::global()->bounded(1, 7); // losuj 1-6
        lastNumber = number;
		counts[number - 1]++; // inkrementuj odpowiedni licznik
    }
    updateResultLabels(); // Aktualizuj etykiety przed każdym rzutem

    ui->resultLabel->setText("Ostatnia wyrzucona liczba to: " + QString::number(lastNumber));
    ui->attemptsLabel->setText("Rzutów: " + QString::number(throwsNumber));

    bool allEqual = std::all_of(counts.begin(), counts.end(),
        [this](int x) { return x == counts[0]; });

    if (allEqual) {
        QMessageBox::information(this, "Gotowe!",
            "Każda liczba wypadła dokładnie " + QString::number(counts[0]) + " razy.\n"
            "W sumie rzutów: " + QString::number(throwsNumber));
    }
}

void testqtApp::on_resetButton_clicked() {
	// Resetuj licznik rzutów i wyniki
    if (balanceTimer->isActive()) {
        QMessageBox::warning(this, "Błąd",
			"Musisz zatrzymać rzucanie przed resetem!");
    }
    else {
        throwsNumber = 0;
        std::fill(counts.begin(), counts.end(), 0);
        ui->attemptsLabel->setText("Rzutów: " + QString::number(throwsNumber));
        ui->label1->setText(QString::number(counts[0]));
        ui->label2->setText(QString::number(counts[1]));
        ui->label3->setText(QString::number(counts[2]));
        ui->label4->setText(QString::number(counts[3]));
        ui->label5->setText(QString::number(counts[4]));
        ui->label6->setText(QString::number(counts[5]));
        QStringList styles(6, "background-color: white; color: #1976d2; font-weight: bold; border-radius: 12px; padding: 10px;");

        ui->label1->setStyleSheet(styles[0]);
        ui->label2->setStyleSheet(styles[1]);
        ui->label3->setStyleSheet(styles[2]);
        ui->label4->setStyleSheet(styles[3]);
        ui->label5->setStyleSheet(styles[4]);
        ui->label6->setStyleSheet(styles[5]);
    }


}

void testqtApp::on_stopButton_clicked() {
    // Zatrzymaj timer, jeśli jest uruchomiony
    if (balanceTimer->isActive()) {
        balanceTimer->stop();
        QMessageBox::information(this, "Zatrzymano",
            "Rzuty zostały zatrzymane.\n"
            "W sumie rzutów: " + QString::number(throwsNumber));
    }

}

void testqtApp::on_generateUntilBalancedButton_clicked() {
	int ms = ui->msBox->value(); // Pobierz wartość z SpinBoxa

    balanceTimer->start(ms);
}

void testqtApp::performBalanceStep() {
    int number = QRandomGenerator::global()->bounded(1, 7);
    counts[number - 1]++;
    throwsNumber++;
	updateResultLabels(); // Aktualizuj etykiety po każdym rzucie

    int maxCount = *std::max_element(counts.begin(), counts.end());
    int minCount = *std::min_element(counts.begin(), counts.end());
    int tolerance = ui->toleranceSpinBox->value(); // Próg różnicy między max a min

    ui->resultLabel->setText("Ostatnia wyrzucona liczba to: " + QString::number(number));
    ui->attemptsLabel->setText("Rzutów: " + QString::number(throwsNumber));

     //Sprawdź czy wszystkie równe
    bool allEqual = std::all_of(counts.begin(), counts.end(),
        [this](int x) { return x == counts[0]; });

    if (allEqual) {
        balanceTimer->stop();
        QMessageBox::information(this, "Gotowe!",
            "Każda liczba wypadła dokładnie " + QString::number(counts[0]) + " razy.\n"
            "W sumie rzutów: " + QString::number(throwsNumber));
    }

	// czy wszystkie liczby wypadły przynajmniej raz
    bool numbersCameOut = std::all_of(counts.begin(), counts.end(), [](int x) { return x > 0; });

	//sprawdź czy liczby są w granicach tolerancji
    if (numbersCameOut && tolerance != 0) {
        double ratio = static_cast<double>(minCount) / static_cast<double>(maxCount);
        double toleranceRatio = static_cast<double>(tolerance) / 100.0;

        if (ratio >= toleranceRatio) {
            balanceTimer->stop();
            QMessageBox::information(this, "Gotowe!",
                "Każda liczba wypadła w granicach tolerancji.\n"
				"W sumie rzutów: " + QString::number(throwsNumber) + "\n"
				"Różnica między max=" + QString::number(maxCount) + " a min=" + QString::number(minCount) + " to:" + QString::number(maxCount - minCount) + " (liczba min to " + QString::number(ratio) + " liczby max)"
            );
        }
    }
}

void testqtApp::updateResultLabels() {
    ui->label1->setText(QString::number(counts[0]));
    ui->label2->setText(QString::number(counts[1]));
    ui->label3->setText(QString::number(counts[2]));
    ui->label4->setText(QString::number(counts[3]));
    ui->label5->setText(QString::number(counts[4]));
    ui->label6->setText(QString::number(counts[5]));

    int maxVal = *std::max_element(counts.begin(), counts.end());
    int minVal = *std::min_element(counts.begin(), counts.end());

    QStringList styles(6, "background-color: white; color: #1976d2; font-weight: bold; border-radius: 12px; padding: 10px;");

    for (int i = 0; i < counts.size(); ++i) {
        if (counts[i] == maxVal && maxVal != 0) {
            styles[i] = "background-color: #c8e6c9; color: #2e7d32; font-weight: bold; border-radius: 12px; padding: 10px;";
        }
        else if (counts[i] == minVal && maxVal != minVal) {
            styles[i] = "background-color: #ffcdd2; color: #c62828; font-weight: bold; border-radius: 12px; padding: 10px;";
        }
    }

    ui->label1->setStyleSheet(styles[0]);
    ui->label2->setStyleSheet(styles[1]);
    ui->label3->setStyleSheet(styles[2]);
    ui->label4->setStyleSheet(styles[3]);
    ui->label5->setStyleSheet(styles[4]);
    ui->label6->setStyleSheet(styles[5]);
}

    

    testqtApp::~testqtApp()
{
        delete ui;
}