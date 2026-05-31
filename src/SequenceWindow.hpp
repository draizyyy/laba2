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
    void onElementDoubleClicked(const QModelIndex &index);
    void onTabCloseRequested(int index);
    void onAppendElement();
    void onPrependElement();
    void onApplyMap();
    void onApplyWhere();
    void onApplyReduce();
    void onConcat();
    void onGetLength();
    
private:
    Ui::SequenceWindow *ui;
    QMap<QWidget*, SequenceModel*> m_models; 
    QMap<QWidget*, QListView*> m_views;
    size_t m_tabCounter = 0;
};