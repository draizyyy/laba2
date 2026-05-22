#include "SequenceWindow.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp" 
#include "exceptions.hpp"
#include <sstream>
#include <format>
#include <utils/functions.hpp>

using namespace myLib;

SequenceWindow::SequenceWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), seq1(nullptr), seq2(nullptr), current_seq(nullptr) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &SequenceWindow::onCreateSequence);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SequenceWindow::onInsertEnd);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &SequenceWindow::onInsertStart);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &SequenceWindow::onInsertByIndex);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &SequenceWindow::onGetByIndex);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &SequenceWindow::onGetFirst);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &SequenceWindow::onGetLast);
    connect(ui->pushButton_14, &QPushButton::clicked, this, &SequenceWindow::onGetLength);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &SequenceWindow::onApplyMap);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &SequenceWindow::onApplyWhere);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &SequenceWindow::onApplyReduce);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &SequenceWindow::onDisplaySequence);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &SequenceWindow::onGetSubsequence);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &SequenceWindow::onSelectSequence);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &SequenceWindow::onConcat);
    connect(ui->pushButton_16, &QPushButton::clicked, this, &SequenceWindow::onClearSequence); // <--- НОВОЕ
}

SequenceWindow::~SequenceWindow() {
    delete seq1;
    delete seq2;
    delete ui;
}

void SequenceWindow::onCreateSequence() {
    QString s = ui->comboBox->currentText();
    Sequence<int>* new_seq = nullptr;

    if (s == "На динамическом списке") {
        new_seq = new ArraySequence<int>();
    } else if (s == "На связном списке") {
        new_seq = new ListSequence<int>();
    }

    if (!new_seq) {
        print(QString("Неверный тип последовательности!"));
        return;
    }

    if (!seq1) {
        seq1 = new_seq;
        current_seq = seq1;
        ui->comboBox_3->addItem(QString::fromStdString(std::format("Последовательность 1 ({})", s.toStdString())));
        ui->comboBox_3->setCurrentIndex(0);
    } else if (!seq2) {
        seq2 = new_seq;
        current_seq = seq2;
        ui->comboBox_3->addItem(QString::fromStdString(std::format("Последовательность 2 ({})", s.toStdString())));
        ui->comboBox_3->setCurrentIndex(1);
    } else {
        print(QString("Можно создать максимум две последовательности!"));
        delete new_seq;
        return;
    }

    print(QString("Создано: ") + s);
}

QString tryCatch(std::function<void()> operation) {
    try {
        operation();
        return "";
    }
    catch (InvalidInputException& e) {
        return QString::fromStdString(std::format("Ошибка ввода: {}", e.what()));
    }
    catch (IndexOutOfRangeException& e) {
        return QString::fromStdString(std::format("Ошибка индекса: {}", e.what()));
    }
    catch (EmptyCollectionException& e) {
        return QString::fromStdString(std::format("Ошибка: {}", e.what()));
    }
    catch (const std::bad_alloc&) {
        return QString("Ошибка: Последовательности разных типов!");
    }
    catch (const std::exception& e) {
        return QString::fromStdString(std::format("Неизвестная ошибка: {}", e.what()));
    }
}

void SequenceWindow::print(const std::string& s) {
    ui->textEdit_2->setPlainText(QString::fromStdString(s));
}

void SequenceWindow::print(QString s) {
    ui->textEdit_2->setPlainText(s);
}

QString SequenceWindow::getElem() {
    return ui->lineEdit->text();
}

QString SequenceWindow::getIndex() {
    return ui->lineEdit_2->text();
}

size_t SequenceWindow::FromString(const std::string& s) {
    if (s.empty()) {
        throw InvalidInputException("Строка ввода пустая");
    }
    
    std::istringstream iss(s);
    int value;
    iss >> value;

    if (iss.fail()) {
        throw InvalidInputException("Неверный формат ввода");
    }

    if (value < 0) {
        throw InvalidInputException("Значение не может быть отрицательным");
    }
    
    return value;
}

void SequenceWindow::onInsertEnd() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    print(QString("Вставка в конец: " + getElem()));
    QString error = tryCatch([this]() {
        current_seq->Append(current_seq->FromString(getElem().toStdString()));
        print(current_seq->ToString());
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onInsertStart() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    print("Вставка в начало: " + getElem());
    QString error = tryCatch([this]() {
        current_seq->Prepend(current_seq->FromString(getElem().toStdString()));
        print(current_seq->ToString());
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onInsertByIndex() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    print("Вставка по индексу " + getIndex() + ": " + getElem());
    QString error = tryCatch([this]() {
        current_seq->InsertAt(current_seq->FromString(getElem().toStdString()), FromString(getIndex().toStdString()));
        print(current_seq->ToString());
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onGetByIndex() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    QString error = tryCatch([this]() {
        std::string s = getIndex().toStdString();
        print("Элемент по индексу " + s + ": " + current_seq->ToString(current_seq->Get(FromString(s))));
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onGetFirst() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    QString error = tryCatch([this]() {
        print("Первый элемент: " + current_seq->ToString(current_seq->GetFirst()));
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onGetLast() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    QString error = tryCatch([this]() {
        print("Последний элемент получен: " + current_seq->ToString(current_seq->GetLast()));
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onGetSubsequence() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    QString error = tryCatch([this]() {
        size_t start = FromString(ui->lineEdit_3->text().toStdString());
        size_t end = FromString(ui->lineEdit_4->text().toStdString());
        
        Sequence<int>* sub_seq = current_seq->GetSubsequence(start, end);
        print(std::format("Подпоследовательность: {}", sub_seq->ToString()));
        delete sub_seq;
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onSelectSequence() {
    int index = ui->comboBox_3->currentIndex();
    if (index == 0 && seq1) {
        current_seq = seq1;
        print(QString("Текущая: Последовательность 1"));
    } else if (index == 1 && seq2) {
        current_seq = seq2;
        print(QString("Текущая: Последовательность 2"));
    }
}

void SequenceWindow::onConcat() {
    if (!seq1 || !seq2) {
        print(QString("Для склейки необходимо создать обе последовательности!"));
        return;
    }
    QString error = tryCatch([this]() {
        Sequence<int>* concat_seq = seq1->Concat(seq2);
        print(std::format("Результат склейки: {}", concat_seq->ToString()));
        delete concat_seq;
    });
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onDisplaySequence() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }
    print(std::format("Полный вывод последовательности: {}", current_seq->ToString()));
}

void SequenceWindow::onGetLength() {
    if (!current_seq) { 
        print(QString("Сначала создайте и выберите последовательность!")); 
        return; 
    }
    print(std::format("Размер: {}", current_seq->GetLength()));
}

void SequenceWindow::onApplyMap() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }

    QString choice = ui->comboBox_4->currentText();
    int (*mapFunc)(int) = nullptr;

    if (choice == "Прибавить 1") {
        mapFunc = [](int x) { return x + 1; };
    } else if (choice == "Возвести в квадрат") {
        mapFunc = [](int x) { return x * x; };
    } else if (choice == "Сделать отрицательным") {
        mapFunc = [](int x) { return -x; };
    }

    if (!mapFunc) {
        print(QString("Неизвестная функция Map!"));
        return;
    }

    QString error = tryCatch([this, mapFunc]() {
        Sequence<int>* mapped_seq = Map(current_seq, mapFunc);
        
        print(std::format("Результат Map: {}", mapped_seq->ToString()));
        delete mapped_seq;
    });
    
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onApplyWhere() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }

    QString choice = ui->comboBox_5->currentText();
    bool (*predicate)(int) = nullptr;

    if (choice == "Только чётные") {
        predicate = [](int x) { return x % 2 == 0; };
    } else if (choice == "Только положительные") {
        predicate = [](int x) { return x > 0; };
    } else if (choice == "Кратные 3") {
        predicate = [](int x) { return x % 3 == 0; };
    }

    if (!predicate) {
        print(QString("Неизвестный предикат Where!"));
        return;
    }

    QString error = tryCatch([this, predicate]() {
        Sequence<int>* filtered_seq = myLib::Where(current_seq, predicate);
        
        print(std::format("Результат Where: {}", filtered_seq->ToString()));
        delete filtered_seq;
    });
    
    if (!error.isEmpty()) {
        print(error);
    }
}

void SequenceWindow::onApplyReduce() {
    if (!current_seq) {
        print(QString("Сначала создайте и выберите последовательность!"));
        return;
    }

    QString choice = ui->comboBox_6->currentText();
    int (*reduceFunc)(int, int) = nullptr;
    int value = 0;

    if (choice == "Сумма элементов") {
        reduceFunc = [](int acc, int x) { return acc + x; };
        value = 0;
    } else if (choice == "Произведение элементов") {
        reduceFunc = [](int acc, int x) { return acc * x; };
        value = 1;
    }

    if (!reduceFunc) {
        print(QString("Неизвестная функция Reduce!"));
        return;
    }

    QString error = tryCatch([this, reduceFunc, value]() {
        Sequence<int>* reduced_seq = myLib::Reduce(current_seq, reduceFunc, value);
        
        print(std::format("Результат Reduce: {}", reduced_seq->ToString()));
        delete reduced_seq;
    });
    
    if (!error.isEmpty()) {
        print(error);
    }
}


void SequenceWindow::onClearSequence() {
    if (!current_seq) {
        print(QString("Нет выбранной последовательности для удаления!"));
        return;
    }

    if (current_seq == seq1) {
        delete seq1;
        seq1 = nullptr;
    } else if (current_seq == seq2) {
        delete seq2;
        delete seq2; 
        seq2 = nullptr;
    }
    current_seq = nullptr;

    ui->comboBox_3->clear();

    if (seq1) {
        ui->comboBox_3->addItem(QString("Последовательность 1"));
    }
    if (seq2) {
        ui->comboBox_3->addItem(QString("Последовательность 2"));
    }

    if (seq1) {
        current_seq = seq1;
        ui->comboBox_3->setCurrentIndex(0);
        print(QString("Последовательность удалена. Текущая: Последовательность 1"));
    } else if (seq2) {
        current_seq = seq2;
        ui->comboBox_3->setCurrentIndex(0);
        print(QString("Последовательность удалена. Текущая: Последовательность 2"));
    } else {
        print(QString("Все последовательности удалены."));
    }
}