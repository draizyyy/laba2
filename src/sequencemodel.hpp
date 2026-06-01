#pragma once

#include <QAbstractListModel>
#include <sequences/sequence.hpp>
#include <sequences/array_sequence.hpp>
#include <sequences/list_sequence.hpp>

class SequenceModel : public QAbstractListModel {
    Q_OBJECT

private:
    myLib::Sequence<int>* m_seq;

public:
    SequenceModel(myLib::Sequence<int>* seq, QObject *parent = nullptr)
        : QAbstractListModel(parent), m_seq(seq) {}

    ~SequenceModel() override {
        delete m_seq;
    }

    myLib::Sequence<int>* getSequence() {
        return m_seq;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid() || !m_seq) {
            return 0;
        }

        return static_cast<int>(m_seq->GetLength());
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole || !m_seq) {
            return QVariant();
        }

        size_t targetIndex = static_cast<size_t>(index.row());

        if (targetIndex >= m_seq->GetLength()) {
            return QVariant();
        }

        return m_seq->Get(targetIndex);
    }

    void setElement(int row, int value) {
        if (!m_seq || row < 0 || row >= static_cast<int>(m_seq->GetLength())) {
            return;
        }

        m_seq->Set(row, value);

        QModelIndex idx = createIndex(row, 0);
        emit dataChanged(idx, idx, {Qt::DisplayRole});
    }

    void appendElement(int value) {
        if (!m_seq) {
            return;
        }

        int row = rowCount();

        beginInsertRows(QModelIndex(), row, row);
        m_seq->Append(value);
        endInsertRows();
    }

    void prependElement(int value) {
        if (!m_seq) {
            return;
        }

        beginInsertRows(QModelIndex(), 0, 0);
        m_seq->Prepend(value);
        endInsertRows();
    }

    void deleteElement(int row) {
        if (!m_seq || row < 0 || row >= static_cast<int>(m_seq->GetLength())) {
            return;
        }

        beginRemoveRows(QModelIndex(), row, row);
        m_seq->DeleteAt(row);
        endRemoveRows();
    }

    void updateModelData(myLib::Sequence<int>* newSeq) {
        beginResetModel();

        delete m_seq;
        m_seq = newSeq;

        endResetModel();
    }

    void clearAll() {
        if (!m_seq || m_seq->GetLength() == 0) {
            return;
        }

        beginResetModel();

        delete m_seq;
        m_seq = new myLib::MutableArraySequence<int>();

        endResetModel();
    }
};