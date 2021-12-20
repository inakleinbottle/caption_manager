//
// Created by sam on 16/12/2021.
//

#ifndef CAPTION_MANAGER_FILTERDIALOG_H
#define CAPTION_MANAGER_FILTERDIALOG_H

#include <QtCore>
#include <QtSql>
#include <QDialog>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListView>

namespace capman {


class FilterDialog : public QDialog
{
    Q_OBJECT
private slots:

public:

    void accept() override;
    void reject() override;

private:

    void setupUI();

public:

    QString getSelectStatement() const;

    explicit FilterDialog(QSqlDatabase & db, QWidget* parent=nullptr);
    ~FilterDialog() override = default;

private:
    QHBoxLayout* mainLayout;
    QVBoxLayout* tagSelectLayout;

    QListView* tagSelector;

    QSqlTableModel* model;
    QSqlDatabase db_;
    QString selectStatement;
};





} // namespace capman



#endif//CAPTION_MANAGER_FILTERDIALOG_H
