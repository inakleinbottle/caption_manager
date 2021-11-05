//
// Created by sam on 05/11/2021.
//

// Using https://www.qtcentre.org/threads/18633-Images-QTableview-Delegates as reference
#ifndef CAPTION_MANAGER_IMAGEDELEGATE_H
#define CAPTION_MANAGER_IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QSize>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QVariant>
#include <QPainter>

class ImageDelegate : public QStyledItemDelegate
{
public:

    ImageDelegate(int image_column, QObject* parent = nullptr);

private:

    QImage loadImage(const QString& path) const;

public:
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

private:
    int imageColumn;
    QPainter* painter;
};

#endif//CAPTION_MANAGER_IMAGEDELEGATE_H
