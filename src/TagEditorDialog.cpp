//
// Created by sam on 21/11/2021.
//

#include "TagEditorDialog.h"
#include <QSqlRelation>
#include <QCompleter>
#include <QMessageBox>

void capman::TagEditorDialog::connectDatabase()
{
    model->setTable("image_tags");
    auto id_index = model->fieldIndex("tagID");

    model->setFilter(QString("imageID=%1").arg(current_image));
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setRelation(id_index, QSqlRelation("tags", "id", "name"));
    model->select();

    tagList->setModel(model);
    tagList->setModelColumn(model->fieldIndex("name"));


}

void capman::TagEditorDialog::setupUI()
{
    {
        auto icon = QIcon::fromTheme("list-add");
        buttonAddTag->setIcon(icon);
        connect(
                buttonAddTag,
                &QPushButton::clicked,
                this,
                &TagEditorDialog::addTag
                );
    }

    {
        auto icon = QIcon::fromTheme("list-remove");
        buttonRemoveTag->setIcon(icon);
        connect(
                buttonRemoveTag,
                &QPushButton::clicked,
                this,
                &TagEditorDialog::removeTag
                );
    }

    {
        auto icon = QIcon::fromTheme("edit-undo");
        buttonRevertTag->setIcon(icon);
        connect(
                buttonRevertTag,
                &QPushButton::clicked,
                this,
                &TagEditorDialog::revertTags
                );
    }

    {
        //auto imageRelModel = model->relationModel(model->fieldIndex("name"));
        //auto nameCol = imageRelModel->fieldIndex("name");
        //tagLineEdit->setCompleter(new QCompleter(imageRelModel, this));
        //tagLineEdit->completer()->setCompletionColumn(nameCol);

    }

    listLayout->addWidget(tagLineEdit);
    listLayout->addWidget(tagList);

    buttonLayout->addWidget(buttonAddTag);
    buttonLayout->addWidget(buttonRemoveTag);
    buttonLayout->addWidget(buttonRevertTag);

    mainLayout->addLayout(listLayout);
    mainLayout->addLayout(buttonLayout);
}

capman::TagEditorDialog::TagEditorDialog(QSqlDatabase& db, int index, QWidget* parent)
    : QDialog(parent), current_image(index), db_(db)
{
    db_.transaction();

    mainLayout = new QHBoxLayout(this);
    listLayout = new QVBoxLayout;
    buttonLayout = new QVBoxLayout;

    mainLayout->setObjectName("mainLayout");
    listLayout->setObjectName("listLayout");
    buttonLayout->setObjectName("buttonLayout");

    tagList = new QListView(this);
    tagLineEdit = new QLineEdit(this);
    buttonAddTag = new QPushButton(this);
    buttonRevertTag = new QPushButton(this);
    buttonRemoveTag = new QPushButton(this);

    tagList->setObjectName("tagList");
    tagLineEdit->setObjectName("tagLineEdit");
    buttonAddTag->setObjectName("buttonAddTag");
    buttonRevertTag->setObjectName("buttonRevertTag");
    buttonRemoveTag->setObjectName("buttonRemoveTag");

    model = new QSqlRelationalTableModel(this, db);
    mapper = new QDataWidgetMapper(this);

    connectDatabase();
    setupUI();
}

void capman::TagEditorDialog::revertTags()
{
    model->revertAll();
}

void capman::TagEditorDialog::addTag()
{
    auto text = tagLineEdit->text();

    if (text.isEmpty()) {
        QMessageBox::warning(this, "Error", "Tag cannot be empty");
        return;
    }

    QSqlQuery query(db_);
    query.prepare("SELECT id FROM tags WHERE name = (?)");
    query.addBindValue( QVariant(text));

    if (!query.exec()) {
        qDebug() << "Query failed" << query.lastError();
    }

    int tagID = -1;
    bool exists = false, is_match = false;
    qDebug() << "Checking tags";
    if (query.next()) {
        exists = true;
        tagID = query.value(0).toInt();
    }
    query.finish();

    if (!exists) {
        qDebug() << "Inserting new tag";

        query.prepare(R"sql(
INSERT INTO tags (name) VALUES (?);
)sql");
        query.addBindValue(text);

        if (!query.exec()) {
            qDebug() << "Failed to insert into tags";
            db_.rollback();
            return;
        }

        tagID = query.lastInsertId().toInt();
    } else {

        query.prepare("select id from image_tags where tagID = ? AND imageID = ?");
        query.addBindValue(QVariant(tagID));
        query.addBindValue(QVariant(current_image));

        if (!query.exec()) {
            qDebug() << "Error checking if tag mapping exists" << query.lastError();
            db_.rollback();
            return;
        }

        if (query.next()) {
            qDebug() << "Mapping already exists";
            return;
        }

    }

    qDebug() << tagID;
    query.clear();

    query.prepare("INSERT INTO image_tags (tagID, imageID) VALUES (?, ?);");
    query.addBindValue(QVariant(tagID));
    query.addBindValue(QVariant(current_image));

    if (!query.exec()) {
        qDebug() << "Error occurred inserting image_tags" << query.lastError();
        db_.rollback();
    }

}

void capman::TagEditorDialog::removeTag()
{

}

capman::TagEditorDialog::~TagEditorDialog()
{
    if (db_.transaction() && !db_.commit()) {
        qDebug() << "Failed to commit" << db_.lastError();
    }
}
