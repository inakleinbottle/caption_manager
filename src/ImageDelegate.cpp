//
// Created by sam on 05/11/2021.
//

#include "ImageDelegate.h"

#include <QGraphicsView>
#include <QDebug>

ImageDelegate::ImageDelegate(int image_column, QObject* parent) : imageColumn(image_column), QStyledItemDelegate(parent)
{
    painter = new QPainter()
}


QImage ImageDelegate::loadImage(const QString& path) const
{
    return QImage(path);
}
void ImageDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    qDebug() << index.column();
    qDebug() << index.data().toString();
    if (index.column() == imageColumn) {
        auto image_view = dynamic_cast<QGraphicsView*>(editor);
        auto image = loadImage(index.data().toString());

    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }


}
