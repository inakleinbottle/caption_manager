//
// Created by sam on 05/11/2021.
//

#include "ImageDelegate.h"

#include <QGraphicsView>

#include <QPixmap>
#include <QDebug>

ImageDelegate::ImageDelegate(int image_column, QObject* parent)
    : imageColumn(image_column), QStyledItemDelegate(parent)
{
}


void ImageDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

    if (index.column() == imageColumn) {
        auto image_view = dynamic_cast<QGraphicsView*>(editor);
        auto* scene = image_view->scene();

        auto path = index.data().toString();

        QPixmap map(path);

        if (!map.isNull()) {
            scene->clear();
            auto* item = new QGraphicsPixmapItem(map.scaled(image_view->width(), image_view->height(), Qt::KeepAspectRatio));

            qDebug() << "Width: " << item->pixmap().width();
            qDebug() << "Height: " << item->pixmap().height();

            scene->addItem(item);
            image_view->show();
        }
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }


}
