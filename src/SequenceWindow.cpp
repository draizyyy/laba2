#include "SequenceWindow.hpp"
#include "ui_SequenceWindow.h"
#include "SequenceModel.hpp"
#include <sequences/array_sequence.hpp>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

int square(int x) { return x * x; }
int multiplyByTen(int x) { return x * 10; }
bool isEven(int x) { return x % 2 == 0; }
bool isPositive(int x) { return x > 0; }
int sumReducer(int acc, int x) { return acc + x; }
int productReducer(int acc, int x) { return acc * x; }

SequenceWindow::SequenceWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SequenceWindow) {
    ui->setupUi(this);
    
    connect(ui->pushButtonAddTab, &QPushButton::clicked, this, &SequenceWindow::onAddTab);
    connect(ui->pushButtonPrepend, &QPushButton::clicked, this, &SequenceWindow::onPrependElement);
    connect(ui->pushButtonAppend, &QPushButton::clicked, this, &SequenceWindow::onAppendElement);
    connect(ui->pushButtonMap, &QPushButton::clicked, this, &SequenceWindow::onApplyMap);
    connect(ui->pushButtonWhere, &QPushButton::clicked, this, &SequenceWindow::onApplyWhere);
    connect(ui->pushButtonReduce, &QPushButton::clicked, this, &SequenceWindow::onApplyReduce);
    connect(ui->pushButtonConcat, &QPushButton::clicked, this, &SequenceWindow::onConcat);
    connect(ui->pushButtonGetFirst, &QPushButton::clicked, this, &SequenceWindow::onGetFirst);
    connect(ui->pushButtonGetLast, &QPushButton::clicked, this, &SequenceWindow::onGetLast);
    connect(ui->pushButtonGetLength, &QPushButton::clicked, this, &SequenceWindow::onGetLength);
}

SequenceWindow::~SequenceWindow() {
    for (auto model : m_models) {
        delete model;
    }
    delete ui;
}

void SequenceWindow::onAddTab() {
    m_tabCounter++;
    
    QWidget *tabContent = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabContent);
    QListView *listView = new QListView();
    
    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(90, 90));
    listView->setSpacing(8);

    listView->setWrapping(true); 
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    auto* newSeq = new myLib::MutableArraySequence<int>();
    SequenceModel* newModel = new SequenceModel(newSeq, this);
    listView->setModel(newModel);
    
    layout->addWidget(listView);
    
    int index = ui->tabWidgetSequences->addTab(tabContent, QString("Последовательность %1").arg(m_tabCounter));
    
    m_models[index] = newModel;
    m_views[index] = listView;
    
    ui->tabWidgetSequences->setCurrentIndex(index);
}

void SequenceWindow::onAppendElement() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) return;

    bool ok;
    int val = QInputDialog::getInt(this, "Вставка в конец", "Введите число:", 0, -99999, 99999, 1, &ok);
    if (!ok) {
        return;
    }

    m_models[currentIndex]->appendElement(val);
}

void SequenceWindow::onPrependElement() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) return;

    bool ok;
    int val = QInputDialog::getInt(this, "Вставка в начало", "Введите число:", 0, -99999, 99999, 1, &ok);
    if (!ok) return;

    m_models[currentIndex]->prependElement(val);
}

void SequenceWindow::onApplyMap() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) return;

    QStringList options = {"Возведение в квадрат (x^2)", "Умножить на 10 (x * 10)"};
    bool ok;
    QString item = QInputDialog::getItem(this, "Выбор функции Map", "Выберите операцию:", options, 0, false, &ok);
    if (!ok) return;

    auto* currentSeq = m_models[currentIndex]->getSequence();
    myLib::Sequence<int>* mappedSeqRaw = nullptr;

    if (item == options[0]) {
        mappedSeqRaw = currentSeq->Map(square);
    } else {
        mappedSeqRaw = currentSeq->Map(multiplyByTen);
    }

    auto* mappedSeq = static_cast<myLib::ArraySequence<int>*>(mappedSeqRaw);
    m_models[currentIndex]->updateModelData(mappedSeq);
}

void SequenceWindow::onApplyWhere() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) {
        return;
    }

    QStringList options = {"Только четные (x % 2 == 0)", "Только положительные (x > 0)"};
    bool ok;
    QString item = QInputDialog::getItem(this, "Выбор фильтра Where", "Выберите условие:", options, 0, false, &ok);
    if (!ok) return;

    auto* currentSeq = m_models[currentIndex]->getSequence();
    myLib::Sequence<int>* filteredSeqRaw = nullptr;

    if (item == options[0]) {
        filteredSeqRaw = currentSeq->Where(isEven);
    } else {
        filteredSeqRaw = currentSeq->Where(isPositive);
    }

    auto* filteredSeq = static_cast<myLib::ArraySequence<int>*>(filteredSeqRaw);
    m_models[currentIndex]->updateModelData(filteredSeq);
}

void SequenceWindow::onApplyReduce() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) {
        return;
    }

    auto* currentSeq = m_models[currentIndex]->getSequence();
    if (currentSeq->GetLength() == 0) {
        QMessageBox::warning(this, "Внимание", "Последовательность пуста.");
        return;
    }

    QStringList options = {"Сумма элементов", "Произведение элементов"};
    bool ok;
    QString item = QInputDialog::getItem(this, "Выбор операции Reduce", "Выберите функцию свёртки:", options, 0, false, &ok);
    if (!ok) {
        return;
    }

    myLib::Sequence<int>* reducedSeqRaw = nullptr;
    QString opName;

    if (item == options[0]) {
        reducedSeqRaw = currentSeq->Reduce(sumReducer, 0);
        opName = "Сумма элементов";
    } else {
        reducedSeqRaw = currentSeq->Reduce(productReducer, 1);
        opName = "Произведение элементов";
    }
    
    int result = reducedSeqRaw->Get(0);
    delete reducedSeqRaw;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Операция: %1\nРезультат: %2").arg(opName).arg(result));
    msgBox.exec();
}

void SequenceWindow::onConcat() {
    if (ui->tabWidgetSequences->count() < 2) {
        QMessageBox::warning(this, "Внимание", "Для склейки необходимо иметь хотя бы две вкладки.");
        return;
    }

    QStringList tabs;
    for (int i = 0; i < ui->tabWidgetSequences->count(); ++i) {
        tabs << ui->tabWidgetSequences->tabText(i);
    }

    bool ok1, ok2;
    QString tab1 = QInputDialog::getItem(this, "Склеивание", "Выберите первую последовательность:", tabs, 0, false, &ok1);
    if (!ok1) {
        return;
    }

    QString tab2 = QInputDialog::getItem(this, "Склеивание", "Выберите вторую последовательность:", tabs, 0, false, &ok2);
    if (!ok2) {
        return;
    }

    int idx1 = tabs.indexOf(tab1);
    int idx2 = tabs.indexOf(tab2);

    auto* seq1 = m_models[idx1]->getSequence();
    auto* seq2 = m_models[idx2]->getSequence();

    myLib::Sequence<int>* concatResultRaw = seq1->Concat(seq2);
    auto* concatResult = static_cast<myLib::ArraySequence<int>*>(concatResultRaw);

    m_tabCounter++;
    QWidget *tabContent = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabContent);
    QListView *listView = new QListView();
    
    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(90, 90)); 
    listView->setSpacing(8);

    listView->setWrapping(true);
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    SequenceModel* newModel = new SequenceModel(concatResult, this);
    listView->setModel(newModel);
    layout->addWidget(listView);

    int newIndex = ui->tabWidgetSequences->addTab(tabContent, QString("Результат Concat %1").arg(m_tabCounter));
    m_models[newIndex] = newModel;
    m_views[newIndex] = listView;

    ui->tabWidgetSequences->setCurrentIndex(newIndex);
}

void SequenceWindow::onGetFirst() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) {
        return;
    }

    auto* currentSeq = m_models[currentIndex]->getSequence();
    if (currentSeq->GetLength() == 0) {
        QMessageBox::warning(this, "Внимание", "Последовательность пуста.");
        return;
    }

    int firstValue = currentSeq->GetFirst();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат GetFirst");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Первый элемент: %1").arg(firstValue));
    msgBox.exec();
}

void SequenceWindow::onGetLast() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) return;

    auto* currentSeq = m_models[currentIndex]->getSequence();
    if (currentSeq->GetLength() == 0) {
        QMessageBox::warning(this, "Внимание", "Последовательность пуста.");
        return;
    }

    int lastValue = currentSeq->GetLast();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат GetLast");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Последний элемент: %1").arg(lastValue));
    msgBox.exec();
}

void SequenceWindow::onGetLength() {
    int currentIndex = ui->tabWidgetSequences->currentIndex();
    if (currentIndex < 0) return;

    auto* currentSeq = m_models[currentIndex]->getSequence();
    size_t length = currentSeq->GetLength();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат GetLength");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Длина последовательности: %1").arg(length));
    msgBox.exec();
}