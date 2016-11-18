#include "trackdelegate.h" 
#include <QTextOption>
#include <QTimeEdit>
#include <QPainter>
 
TrackDelegate::TrackDelegate(int durationColumn, QObject *parent) 
        : QStyledItemDelegate(parent) 
{ 
        this->durationColumn = durationColumn; 
} 
 
void TrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{ 
        if (index.column() == durationColumn) 
        { 
                int secs = index.model()->data(index, Qt::DisplayRole).toInt(); 
                QString text = QString("%1:%2").arg(secs / 60, 2, 10, QChar('0')).arg(secs % 60, 2, 10, QChar('0')); 
                QTextOption o(Qt::AlignRight | Qt::AlignVCenter); 
                painter->drawText(option.rect, text, o); 
        } 
        else 
        { 
                QStyledItemDelegate::paint(painter, option, index); 
        } 
} 
 
QWidget *TrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{ 
        if (index.column() == durationColumn) 
        { 
                QTimeEdit *timeEdit = new QTimeEdit(parent); 
                timeEdit->setDisplayFormat("mm:ss"); 
                connect(timeEdit, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor())); 
                return timeEdit; 
        } 
        else 
        { 
                return QStyledItemDelegate::createEditor(parent, option, index); 
        } 
} 
 
void TrackDelegate::commitAndCloseEditor() 
{ 
        QTimeEdit *editor = qobject_cast<QTimeEdit *>(sender()); 
        emit commitData(editor); 
        emit closeEditor(editor); 
} 
 
void TrackDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const 
{ 
        if (index.column() == durationColumn) 
        { 
                int secs = index.model()->data(index, Qt::DisplayRole).toInt(); 
                QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor); 
                timeEdit->setTime(QTime(0, secs / 60, secs % 60)); 
        } 
        else
        { 
                QStyledItemDelegate::setEditorData(editor, index); 
        } 
} 
 
void TrackDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const 
{ 
        if (index.column() == durationColumn) 
        { 
                QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor); 
                QTime time = timeEdit->time(); 
                int secs = (time.minute() * 60) + time.second(); 
                model->setData(index, secs); 
        } 
        else 
        { 
                QStyledItemDelegate::setModelData(editor, model, index); 
        } 
} 
