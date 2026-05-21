#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <cstdint>
#include "ui_sequencewindow.h"
#include <sequences/sequence.hpp>

using namespace myLib;

class SequenceWindow : public QMainWindow {
    Q_OBJECT

private:
    Sequence<int>* seq1{};
    Sequence<int>* seq2{};
    Sequence<int>* current_seq{};

    size_t FromString(const std::string& s);

public:
    explicit SequenceWindow(QWidget *parent = nullptr);
    ~SequenceWindow();

private slots:
    void onCreateSequence();
    void onInsertEnd();
    void onInsertStart();
    void onInsertByIndex();
    void onGetByIndex();
    void onGetFirst();
    void onGetLast();
    void onGetLength();
    void onDisplaySequence();
    void onGetSubsequence();
    void onApplyMap();
    void onApplyWhere();
    void onApplyReduce();
    void onSelectSequence();
    void onConcat();
    void onClearSequence();

    void print(const std::string& s);
    void print(const QString s);
    QString getElem();
    QString getIndex();

private:
    Ui::MainWindow *ui;  
};