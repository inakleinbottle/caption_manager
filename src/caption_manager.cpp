//
// Created by sam on 02/11/2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_caption_manager.h" resolved

#include "caption_manager.h"
#include "ui_caption_manager.h"


caption_manager::caption_manager(QString dbpath, QWidget* parent) : QMainWindow(parent), ui(new Ui::caption_manager), dbm(dbpath)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->imagePreview->setScene(scene);

    model = new QSqlTableModel(this);
    model->setTable("images");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();


    ui->imageList->setModel(model);
    ui->imageList->setModelColumn(model->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new ImageDelegate(model->fieldIndex("image_uri"), this));

    mapper->addMapping(ui->imageDescription, model->fieldIndex("caption"));
    mapper->addMapping(ui->imageTitle, model->fieldIndex("name"));
    mapper->addMapping(ui->imagePreview, model->fieldIndex("image_uri"));

    mapper->toFirst();

    connect(ui->imageList->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex);


    connect(ui->actionSave, &QAction::triggered, this, &caption_manager::saveItem);

}

caption_manager::~caption_manager()
{
    delete ui;
}


void caption_manager::onChangeItem()
{
}

void caption_manager::saveItem() const
{
    qDebug() << "Save action";
    model->submitAll();

}
