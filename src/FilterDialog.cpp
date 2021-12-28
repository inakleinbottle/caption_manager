//
// Created by sam on 16/12/2021.
//

#include "FilterDialog.h"

#include <QSql>

void capman::FilterDialog::accept()
{
    auto ids = tagSelector->selectionModel()->selectedIndexes();

    if (ids.empty()) {
        return QDialog::accept();
    }

    QStringList id_list;

    for (auto id : ids) {
        id_list << model->record(id.row()).value(0).toString();
    }

    QSqlQuery query(db_);
    query.prepare(QString("SELECT DISTINCT imageID FROM image_tags WHERE tagID IN (%1)").arg(id_list.join(", ")));

    if (!query.exec()) {
        qDebug() << "Error occurred" << query.lastError();
        return QDialog::accept();
    }

    QStringList imageIDs;
    while (query.next()) {
        imageIDs.append(query.value(0).toString());
    }

    selectStatement.append(QString("id IN (%1)").arg(imageIDs.join(", ")));

    return QDialog::accept();
}
void capman::FilterDialog::reject()
{
    selectStatement.clear();
    QDialog::reject();
}

void capman::FilterDialog::setupUI()
{
    model->setTable("tags");
    model->select();

    tagSelector->setModel(model);
    tagSelector->setModelColumn(model->fieldIndex("name"));
    tagSelector->setSelectionMode(QAbstractItemView::MultiSelection);

    cancelButton->setText("Cancel");
    acceptButton->setText("Accept");

    connect(cancelButton, &QPushButton::clicked, this, &capman::FilterDialog::reject);
    connect(acceptButton, &QPushButton::clicked, this, &capman::FilterDialog::accept);

    tagSelectLayout->addWidget(tagSelector);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(acceptButton);

    mainLayout->addLayout(tagSelectLayout);
    mainLayout->addLayout(buttonLayout);



}

capman::FilterDialog::FilterDialog(QSqlDatabase& db, QWidget* parent)
    : QDialog(parent), db_(db), selectStatement()
{
    mainLayout = new QVBoxLayout(this);
    tagSelectLayout = new QVBoxLayout;
    buttonLayout = new QHBoxLayout;

    mainLayout->setObjectName("mainLayout");
    tagSelectLayout->setObjectName("tagSelectLayout");
    buttonLayout->setObjectName("buttonLayout");

    tagSelector = new QListView(this);
    acceptButton = new QPushButton(this);
    cancelButton = new QPushButton(this);

    tagSelector->setObjectName("tagSelector");
    acceptButton->setObjectName("acceptButton");
    cancelButton->setObjectName("cancelButton");

    model = new QSqlTableModel(this, db_);


    setupUI();
}

QString capman::FilterDialog::getSelectStatement() const
{
    return selectStatement.join("AND");
}
