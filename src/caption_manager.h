//
// Created by sam on 02/11/2021.
//

#ifndef CAPTION_MANAGER_CAPTION_MANAGER_H
#define CAPTION_MANAGER_CAPTION_MANAGER_H

#include <QMainWindow>
#include <QDataWidgetMapper>

#include "ImageDelegate.h"
#include "database_manager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class caption_manager;
}
QT_END_NAMESPACE

class caption_manager : public QMainWindow
{
    Q_OBJECT

private slots:

    void onChangeItem();

private:

    void setupModel();

public:
    explicit caption_manager(QString dbpath, QWidget* parent = nullptr);
    ~caption_manager() override;

private:
    Ui::caption_manager* ui;
    database_manager dbm;
    QSqlTableModel *model;
    QDataWidgetMapper* mapper;

};

#endif//CAPTION_MANAGER_CAPTION_MANAGER_H
