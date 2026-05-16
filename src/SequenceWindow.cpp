#include "SequenceWindow.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp"

using namespace myLib;

SequenceWindow::SequenceWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), seq(nullptr) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &SequenceWindow::onCreateSequence);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SequenceWindow::onInsertEnd);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &SequenceWindow::onInsertStart);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &SequenceWindow::onInsertByIndex);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &SequenceWindow::onGetByIndex);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &SequenceWindow::onGetFirst);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &SequenceWindow::onGetLast);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &SequenceWindow::onDisplaySequence);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &SequenceWindow::onApplyMap);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &SequenceWindow::onApplyWhere);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &SequenceWindow::onApplyReduce);
}

void SequenceWindow::onCreateSequence() {
    delete seq;
    seq = nullptr;
    

    std::string s = ui->comboBox->currentText().toStdString();
    if (s == "На динамическом списке") {
        seq = new ArraySequence<int>();
    } else if (s == "На связном списке") {
        seq = new ListSequence<int>();
    }

    ui->textEdit_2->setPlainText("Создано: " + ui->comboBox->currentText() + " / " + ui->comboBox_2->currentText());
}

SequenceWindow::~SequenceWindow() {
    delete seq;
    delete ui;
}

void SequenceWindow::onInsertEnd() {
    ui->textEdit_2->setPlainText("Вставка в конец: " + ui->lineEdit->text());
    seq->Append(seq->FromString(ui->lineEdit->text().toStdString()));
}
void SequenceWindow::onInsertStart() {
    ui->textEdit_2->setPlainText("Вставка в начало: " + ui->lineEdit->text());
}
void SequenceWindow::onInsertByIndex() {
    ui->textEdit_2->setPlainText("Вставка по индексу " + ui->lineEdit_2->text() + ": " + ui->lineEdit->text());
}
void SequenceWindow::onGetByIndex() {
    ui->textEdit_2->setPlainText("Элемент по индексу " + ui->lineEdit_2->text());
}
void SequenceWindow::onGetFirst() {
    ui->textEdit_2->setPlainText("Первый элемент получен");
}
void SequenceWindow::onGetLast() {
    ui->textEdit_2->setPlainText("Последний элемент получен");
}
void SequenceWindow::onDisplaySequence() {
    ui->textEdit_2->setPlainText(QString("Полный вывод последовательности: ") + QString::fromStdString(seq->ToString()));
}
void SequenceWindow::onApplyMap() {
    ui->textEdit_2->setPlainText("Map: " + ui->comboBox_4->currentText());
}
void SequenceWindow::onApplyWhere() {
    ui->textEdit_2->setPlainText("Where: " + ui->comboBox_5->currentText());
}
void SequenceWindow::onApplyReduce() {
    ui->textEdit_2->setPlainText("Reduce: " + ui->comboBox_6->currentText());
}

