#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include "ui_sequencewindow.h"
#include <sequences/sequence.hpp>

using namespace myLib;

class SequenceWindow : public QMainWindow {
    Q_OBJECT

private:
    Sequence<int>* seq;
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
    void onDisplaySequence();
    void onApplyMap();
    void onApplyWhere();
    void onApplyReduce();
private:
    Ui::MainWindow *ui;  
};

