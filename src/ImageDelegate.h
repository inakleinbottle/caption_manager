//
// Created by sam on 05/11/2021.
//

// Using https://www.qtcentre.org/threads/18633-Images-QTableview-Delegates as reference
#ifndef CAPTION_MANAGER_IMAGEDELEGATE_H
#define CAPTION_MANAGER_IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QSize>
#include <QImage>
#include <QVariant>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class ImageDelegate : public QStyledItemDelegate
{
public:

    ImageDelegate(int image_column, QObject* parent = nullptr);

public:
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

private:
    int imageColumn;
};

#endif//CAPTION_MANAGER_IMAGEDELEGATE_H
