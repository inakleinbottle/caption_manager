//
// Created by sam on 16/12/2021.
//

#include "FilterDialog.h"

void capman::FilterDialog::accept()
{
    QDialog::accept();
}
void capman::FilterDialog::reject()
{
    QDialog::reject();
}

void capman::FilterDialog::setupUI()
{
    model->setTable("tags");
    model->select();

    tagSelector->setModel(model);
    tagSelector->setModelColumn(model->fieldIndex("name"));

    tagSelectLayout->addWidget(tagSelector);

    mainLayout->addLayout(tagSelectLayout);

}

capman::FilterDialog::FilterDialog(QSqlDatabase& db, QWidget* parent)
    : QDialog(parent), db_(db), selectStatement()
{
    mainLayout = new QHBoxLayout(this);
    tagSelectLayout = new QVBoxLayout;

    mainLayout->setObjectName("mainLayout");
    tagSelectLayout->setObjectName("tagSelectLayout");

    tagSelector = new QListView(this);

    tagSelector->setObjectName("tagSelector");


    model = new QSqlTableModel(this, db_);


    setupUI();
}

QString capman::FilterDialog::getSelectStatement() const
{
    return selectStatement;
}
