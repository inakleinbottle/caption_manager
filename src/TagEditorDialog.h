//
// Created by sam on 21/11/2021.
//

#ifndef CAPTION_MANAGER_TAGEDITORDIALOG_H
#define CAPTION_MANAGER_TAGEDITORDIALOG_H

#include <QtCore>
#include <QtSql>
#include <QDialog>
#include <QDataWidgetMapper>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QListView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

namespace capman {


class TagEditorDialog : public QDialog
{
    Q_OBJECT

private slots:

    void addTag();
    void removeTag();
    void revertTags();

public:
    void accept() override;
    void reject() override;

private:

    void connectDatabase();
    void setupUI();

public:
    explicit TagEditorDialog(QSqlDatabase& db, int index, QWidget* parent = nullptr);

    ~TagEditorDialog() override = default;

private:
    QListView* tagList;
    QLineEdit* tagLineEdit;
    QPushButton* buttonAddTag;
    QPushButton* buttonRemoveTag;
    QPushButton* buttonRevertTag;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;

    QVBoxLayout* mainLayout;
    QHBoxLayout* midLayout;
    QVBoxLayout* listLayout;
    QVBoxLayout* buttonLayout;
    QHBoxLayout* bottomLayout;

    QSqlRelationalTableModel* model;
    int current_image;
    QSqlDatabase db_;
};

}

#endif//CAPTION_MANAGER_TAGEDITORDIALOG_H
