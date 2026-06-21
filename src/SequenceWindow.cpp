#include "SequenceWindow.hpp"
#include "ui_SequenceWindow.h"
#include "SequenceModel.hpp"
#include "sequences/sequence.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/sequence_operations.hpp"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

int square(int x) { return x * x; }
int multiplyByTen(int x) { return x * 10; }
bool isEven(int x) { return x % 2 == 0; }
bool isPositive(int x) { return x > 0; }
int sumReducer(int acc, int x) { return acc + x; }
int productReducer(int acc, int x) { return acc * x; }

SequenceWindow::SequenceWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SequenceWindow) {
    ui->setupUi(this);

    connect(ui->tabWidgetSequences, &QTabWidget::tabCloseRequested, this, &SequenceWindow::onTabCloseRequested);
    connect(ui->pushButtonAddTab, &QPushButton::clicked, this, &SequenceWindow::onAddTab);
    connect(ui->pushButtonPrepend, &QPushButton::clicked, this, &SequenceWindow::onPrependElement);
    connect(ui->pushButtonAppend, &QPushButton::clicked, this, &SequenceWindow::onAppendElement);
    connect(ui->pushButtonMap, &QPushButton::clicked, this, &SequenceWindow::onApplyMap);
    connect(ui->pushButtonWhere, &QPushButton::clicked, this, &SequenceWindow::onApplyWhere);
    connect(ui->pushButtonReduce, &QPushButton::clicked, this, &SequenceWindow::onApplyReduce);
    connect(ui->pushButtonConcat, &QPushButton::clicked, this, &SequenceWindow::onConcat);
    connect(ui->pushButtonGetLength, &QPushButton::clicked, this, &SequenceWindow::onGetLength);
}

SequenceWindow::~SequenceWindow() {
    for (auto model : m_models) {
        delete model;
    }
    delete ui;
}

void SequenceWindow::onAddTab() {
    int actualNum = 1;

    while (true) {
        bool numberTaken = false;

        for (int i = 0; i < ui->tabWidgetSequences->count(); i++) {
            QString tabName = ui->tabWidgetSequences->tabText(i);

            if (tabName.endsWith(" " + QString::number(actualNum))) {
                numberTaken = true;
                break;
            }
        }

        if (!numberTaken) {
            break;
        }

        actualNum++;
    }

    QStringList sequenceTypes = {"ArraySequence", "ListSequence"};

    bool ok;
    QString selectedType = QInputDialog::getItem(this, "Тип последовательности", "Выберите тип последовательности:", sequenceTypes, 
        0, false, &ok);

    if (!ok) {
        return;
    }

    myLib::Sequence<int>* newSeq = nullptr;

    if (selectedType == "ArraySequence") {
        newSeq = new myLib::MutableArraySequence<int>();
    } else {
        newSeq = new myLib::ListSequence<int>();
    }

    QWidget *tabContent = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabContent);
    QListView *listView = new QListView();

    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(90, 90));
    listView->setSpacing(8);
    listView->setWrapping(true);
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    SequenceModel* newModel = new SequenceModel(newSeq, this);
    listView->setModel(newModel);

    layout->addWidget(listView);

    connect(listView, &QListView::doubleClicked, this, &SequenceWindow::onElementDoubleClicked);

    ui->tabWidgetSequences->addTab(
        tabContent,
        QString("%1 %2").arg(selectedType).arg(actualNum)
    );

    m_models[tabContent] = newModel;
    ui->tabWidgetSequences->setCurrentWidget(tabContent);
}

void SequenceWindow::onTabCloseRequested(int index) {
    QWidget* tabContent = ui->tabWidgetSequences->widget(index);
    if (!tabContent) {
        return;
    }

    ui->tabWidgetSequences->removeTab(index);

    SequenceModel* model = m_models.take(tabContent);

    delete model;
    delete tabContent;
}

void SequenceWindow::onAppendElement() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    bool ok;
    int val = QInputDialog::getInt(this, "Вставка в конец", "Введите число:", 0, -99999, 99999, 1, &ok);
    if (!ok) {
        return;
    }

    m_models[currentTab]->appendElement(val);
}

void SequenceWindow::onPrependElement() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    bool ok;
    int val = QInputDialog::getInt(this, "Вставка в начало", "Введите число:", 0, -99999, 99999, 1, &ok);
    if (!ok) {
        return;
    }

    m_models[currentTab]->prependElement(val);
}

void SequenceWindow::onApplyMap() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    QStringList options = {"Возведение в квадрат (x^2)", "Умножить на 10 (x * 10)"};
    bool ok;
    QString item = QInputDialog::getItem(this, "Выбор функции Map", "Выберите операцию:", options, 0, false, &ok);
    if (!ok) {
        return;
    }

    auto* currentSeq = m_models[currentTab]->getSequence();
    myLib::Sequence<int>* mappedSeq = nullptr;

    if (item == options[0]) {
        mappedSeq = myLib::Map<int>(currentSeq, square);
    } else {
        mappedSeq = myLib::Map<int>(currentSeq, multiplyByTen);
    }

    m_models[currentTab]->updateModelData(mappedSeq);
}
    

void SequenceWindow::onApplyWhere() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    QStringList options = {"Только четные (x % 2 == 0)", "Только положительные (x > 0)"};
    bool ok;
    QString item = QInputDialog::getItem(this, "Выбор фильтра Where", "Выберите условие:", options, 0, false, &ok);
    if (!ok) {
        return;
    }

    auto* currentSeq = m_models[currentTab]->getSequence();
    myLib::Sequence<int>* filteredSeq = nullptr;

    if (item == options[0]) {
        filteredSeq = myLib::Where<int>(currentSeq, isEven);
    } else {
        filteredSeq = myLib::Where<int>(currentSeq, isPositive);
    }

    m_models[currentTab]->updateModelData(filteredSeq);
}

void SequenceWindow::onApplyReduce() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    auto* currentSeq = m_models[currentTab]->getSequence();
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

    int result;
    QString opName;

    if (item == options[0]) {
        result = myLib::Reduce<int>(currentSeq, sumReducer, 0);
        opName = "Сумма элементов";
    } else {
        result = myLib::Reduce<int>(currentSeq, productReducer, 1);
        opName = "Произведение элементов";
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Операция: %1\nРезультат: %2").arg(opName).arg(result));
    msgBox.exec();
}

void SequenceWindow::onConcat() {
    if (ui->tabWidgetSequences->count() < 2) {
        QMessageBox::warning(this, "Ошибка", "Для склейки необходимо иметь хотя бы две последовательности.");
        return;
    }

    QStringList tabs;
    for (int i = 0; i < ui->tabWidgetSequences->count(); i++) {
        tabs << ui->tabWidgetSequences->tabText(i);
    }

    bool ok1 = false;
    bool ok2 = false;

    QString tab1 = QInputDialog::getItem(this, "Склеивание", "Выберите первую последовательность:", tabs, 0, false, &ok1);

    if (!ok1) {
        return;
    }

    QString tab2 = QInputDialog::getItem(this, "Склеивание", "Выберите вторую последовательность:", tabs, 0, false, &ok2);

    if (!ok2) {
        return;
    }

    int index1 = tabs.indexOf(tab1);
    int index2 = tabs.indexOf(tab2);

    QWidget* w1 = ui->tabWidgetSequences->widget(index1);
    QWidget* w2 = ui->tabWidgetSequences->widget(index2);

    if (!w1 || !w2 || !m_models.contains(w1) || !m_models.contains(w2)) {
        return;
    }

    Sequence<int>* seq1 = m_models[w1]->getSequence();
    Sequence<int>* seq2 = m_models[w2]->getSequence();

    Sequence<int>* concatResult = nullptr;

    try {
        concatResult = seq1->Concat(seq2);
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        return;
    }
    catch (...) {
        QMessageBox::warning(this, "Ошибка", "Неизвестная ошибка при склеивании последовательностей.");
        return;
    }

    if (!concatResult) {
        QMessageBox::warning(this, "Ошибка", "Склеивание выполнено неверно.");
        return;
    }

    m_tabCounter++;

    QWidget* tabContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tabContent);
    QListView* listView = new QListView();

    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(90, 90));
    listView->setSpacing(8);
    listView->setWrapping(true);
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    SequenceModel* newModel = new SequenceModel(concatResult, this);

    listView->setModel(newModel);
    layout->addWidget(listView);

    connect(listView, &QListView::doubleClicked, this, &SequenceWindow::onElementDoubleClicked);

    ui->tabWidgetSequences->addTab(tabContent, QString("Результат Concat %1").arg(m_tabCounter));

    m_models[tabContent] = newModel;

    ui->tabWidgetSequences->setCurrentWidget(tabContent);
}

void SequenceWindow::onElementDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    int row = index.row();
    int currentVal = m_models[currentTab]->getSequence()->Get(row);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Действие с элементом");
    msgBox.setText(QString("Элемент в позиции %1: %2").arg(row).arg(currentVal));

    QPushButton* editButton = msgBox.addButton("Изменить", QMessageBox::AcceptRole);
    QPushButton* deleteButton = msgBox.addButton("Удалить", QMessageBox::DestructiveRole);
    QPushButton* cancelButton = msgBox.addButton("Отмена", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == cancelButton) {
        return;
    }

    if (msgBox.clickedButton() == deleteButton) {
        m_models[currentTab]->deleteElement(row);
        return;
    }

    if (msgBox.clickedButton() == editButton) {
        bool ok;
        int newVal = QInputDialog::getInt(this, "Редактирование элемента", QString("Изменение элемента в позиции %1:").arg(row),
            currentVal, -99999, 99999, 1, &ok);

        if (!ok) {
            return;
        }

        m_models[currentTab]->setElement(row, newVal);
    }
}

void SequenceWindow::onGetLength() {
    QWidget* currentTab = ui->tabWidgetSequences->currentWidget();
    if (!currentTab || !m_models.contains(currentTab)) {
        return;
    }

    auto* currentSeq = m_models[currentTab]->getSequence();
    size_t length = currentSeq->GetLength();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Результат GetLength");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setText(QString("Длина последовательности: %1").arg(length));
    msgBox.exec();
}