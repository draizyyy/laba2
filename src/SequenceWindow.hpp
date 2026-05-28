#pragma once
#include <QMainWindow>
#include <QListView>
#include <QMap>

namespace Ui { class SequenceWindow; }
class SequenceModel;

class SequenceWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SequenceWindow(QWidget *parent = nullptr);
    ~SequenceWindow() override;

private slots:
    void onAddTab();
    void onAppendElement();
    void onPrependElement();
    void onApplyMap();
    void onApplyWhere();
    void onApplyReduce();
    void onConcat();

private:
    Ui::SequenceWindow *ui;
    QMap<int, SequenceModel*> m_models;
    QMap<int, QListView*> m_views;
    size_t m_tabCounter = 0;
};