#pragma once
#include <QAbstractListModel>
#include <sequences/sequence.hpp>
#include <sequences/array_sequence.hpp>

class SequenceModel : public QAbstractListModel {
    Q_OBJECT
private:
    myLib::ArraySequence<int>* m_seq;

public:
    explicit SequenceModel(myLib::ArraySequence<int>* seq, QObject *parent = nullptr)
        : QAbstractListModel(parent), m_seq(seq) {}

    ~SequenceModel() override {
        delete m_seq;
    }

    myLib::ArraySequence<int>* getSequence() const {
        return m_seq;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid() || !m_seq) return 0;
        return static_cast<int>(m_seq->GetLength());
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole) return QVariant();
        
        size_t targetIndex = static_cast<size_t>(index.row());
        if (targetIndex >= m_seq->GetLength()) return QVariant();
        
        return m_seq->Get(targetIndex);
    }

    void appendElement(int value) {
        int row = rowCount();
        beginInsertRows(QModelIndex(), row, row);
        m_seq->Append(value);
        endInsertRows();
    }

    void prependElement(int value) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_seq->Prepend(value);
        endInsertRows();
    }

    void updateModelData(myLib::ArraySequence<int>* newSeq) {
        beginResetModel();
        delete m_seq;
        m_seq = newSeq;
        endResetModel();
    }

    void clearAll() {
        if (m_seq->GetLength() == 0) return;
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        delete m_seq;
        m_seq = new myLib::MutableArraySequence<int>();
        endRemoveRows();
    }
};