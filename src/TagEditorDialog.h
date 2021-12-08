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

private:

    void connectDatabase();
    void setupUI();

public:
    explicit TagEditorDialog(QSqlDatabase& db, int index, QWidget* parent = nullptr);

    ~TagEditorDialog() override;

private:
    QListView* tagList;
    QLineEdit* tagLineEdit;
    QPushButton* buttonAddTag;
    QPushButton* buttonRemoveTag;
    QPushButton* buttonRevertTag;


    QHBoxLayout* mainLayout;
    QVBoxLayout* listLayout;
    QVBoxLayout* buttonLayout;

    QSqlRelationalTableModel* model;
    QDataWidgetMapper* mapper;
    int current_image;
    QSqlDatabase db_;
};

}

#endif//CAPTION_MANAGER_TAGEDITORDIALOG_H
