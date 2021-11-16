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

        scene->clear();
        auto path = index.data().toString();
        if (path.isEmpty()) {
            return;
        }

        QPixmap map(path);


        if (!map.isNull()) {
            auto* item = new QGraphicsPixmapItem(map.scaled(image_view->width(), image_view->height(), Qt::KeepAspectRatio));

            scene->addItem(item);
            image_view->show();
        }
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }


}
void ImageDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Don't change the image.
    if (index.column() != imageColumn) {
        QStyledItemDelegate::setModelData(editor, model, index);
    }

}
