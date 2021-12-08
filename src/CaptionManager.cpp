//
// Created by sam on 14/11/2021.
//

#include "CaptionManager.h"
#include <QIcon>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMessageBox>


void capman::CaptionManager::newEntry()
{
    qDebug() << "New entry";

    QString selectedPath;
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::PicturesLocation);
        selectedPath = QFileDialog::getOpenFileName(this, "Select an Image", path, "Images (*.png *.jpg)");
        if (selectedPath.isEmpty()) {
            qDebug() << "No file selected";
            return;
        }
    }

    QFileInfo info(selectedPath);
    QDateTime creationDate = info.birthTime();
    QString fileName = info.baseName();

    QSqlRecord newRecord = model->record();
    newRecord.setValue("image_uri", QVariant(info.absoluteFilePath()));
    newRecord.setValue("name", QVariant(fileName));
    newRecord.setValue("created", QVariant(creationDate));
    newRecord.setNull("caption");

    if (!model->insertRecord(-1, newRecord)) {
        qDebug() << "Failed to insert new record";
    } else {
        qDebug() << "Inserted new record" << fileName << ' ' << creationDate;
    }

    mapper->toLast();
}

void capman::CaptionManager::saveEntry()
{
    qDebug() << "Save entry";
    if (!mapper->submit()) {  // Force submit of the current widgets
        qDebug() << "No changes submitted";
    }
    if (model->isDirty()) {
        qDebug() << "Submitting changes";
        if (!model->submitAll()) {
            qDebug() << "Failed to submit changes";
        }
    }
}

void capman::CaptionManager::deleteEntry()
{
    auto confirmed = QMessageBox::question(this, "Confirm delete",
                          "You are about to delete the entry for this image.\n"
                          "This will not delete the image itself.\n"
                          "Are you sure you wish to proceed?"
                          );

    if (confirmed == QMessageBox::StandardButton::Ok) {
        model->removeRow(mapper->currentIndex());
        model->submitAll();
    }
}

void capman::CaptionManager::about()
{
    QMessageBox::about(this, "About", "Image caption manager thing.");
}

void capman::CaptionManager::exit()
{
    QCoreApplication::quit();
}

void capman::CaptionManager::filterEntries()
{
    qDebug() << "Filtering entries";
}

void capman::CaptionManager::editTags()
{
    qDebug() << "Add tags";
    auto current_id = model->record(mapper->currentIndex()).field("id").value().toInt();

    TagEditorDialog dialog(database, current_id, this);
    dialog.setModal(true);
    dialog.exec();


}

void capman::CaptionManager::updateImage()
{

}

capman::CaptionManager::CaptionManager(QString dbpath, QWidget* parent)
    : QMainWindow(parent)
{
    setObjectName("mainWindow");
    database = QSqlDatabase::addDatabase("QSQLITE");
    setupDatabase(dbpath);

    model = new QSqlTableModel(this, database);
    mapper = new QDataWidgetMapper(this);
    scene = new QGraphicsScene(this);

    centralWidget = new QWidget(this);
    mainLayout = new QHBoxLayout(centralWidget);
    listLayout = new QVBoxLayout;
    listButtonsLayout = new QHBoxLayout;
    detailsLayout = new QVBoxLayout;
    metadataLayout = new QHBoxLayout;
    detailButtonsLayout = new QHBoxLayout;

    centralWidget->setObjectName("centralWidget");
    mainLayout->setObjectName("mainLayout");
    listLayout->setObjectName("listLayout");
    listButtonsLayout->setObjectName("listButtonsLayout");
    detailsLayout->setObjectName("detailsLayout");
    metadataLayout->setObjectName("metadataLayout");
    detailButtonsLayout->setObjectName("detailButtonsLayout");

    imageListView = new QListView(centralWidget);
    imageView = new QGraphicsView(centralWidget);
    imageTitleEdit = new QLineEdit(centralWidget);
    imageDateEdit = new QDateTimeEdit(centralWidget);
    imageCaptionEdit = new QTextEdit(centralWidget);

    buttonAddImage = new QPushButton(centralWidget);
    buttonFilterImageList = new QPushButton(centralWidget);
    buttonSaveImage = new QPushButton(centralWidget);
    buttonEditImageTags = new QPushButton(centralWidget);

    imageListView->setObjectName("imageListView");
    imageView->setObjectName("imageView");
    imageTitleEdit->setObjectName("ImageTitleEdit");
    imageDateEdit->setObjectName("imageDateEdit");
    imageCaptionEdit->setObjectName("imageCaptionEdit");
    buttonAddImage->setObjectName("buttonAddImage");
    buttonFilterImageList->setObjectName("buttonFilterImageList");
    buttonSaveImage->setObjectName("buttonSaveImage");
    buttonEditImageTags->setObjectName("buttonEditImageTags");

    menuBar = new QMenuBar(this);
    menuFile = menuBar->addMenu("&File");
    menuHelp = menuBar->addMenu("&Help");

    menuBar->setObjectName("menuBar");
    menuFile->setObjectName("menuFile");
    menuHelp->setObjectName("menuHelp");

    actionAbout = new QAction(this);
    actionAddImage = new QAction(this);
    actionSaveImage = new QAction(this);
    actionDeleteImage = new QAction(this);
    actionExport = new QAction(this);
    actionExit = new QAction(this);

    actionAbout->setObjectName("actionAbout");
    actionAddImage->setObjectName("actionAddImage");
    actionSaveImage->setObjectName("actionSaveImage");
    actionDeleteImage->setObjectName("actionDeleteImage");
    actionExport->setObjectName("actionExport");
    actionExit->setObjectName("actionExit");


    createActions();
    setupMainWindow();
    createMenuBar();
    createUI();
    connectDatabase();

}

capman::CaptionManager::~CaptionManager()
{

    if (database.open()) {
        database.close();
    }

}

void capman::CaptionManager::setupMainWindow()
{
    if (objectName().isEmpty()) {
        setObjectName("Caption Manager");
    }

    resize(800, 600);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(800, 600));

}

void capman::CaptionManager::createActions()
{

    actionAbout->setObjectName("actionAbout");
    actionAddImage->setObjectName("actionAddImage");
    actionSaveImage->setObjectName("actionSaveImage");
    actionDeleteImage->setObjectName("actionDeleteImage");

    {
        actionAddImage->setShortcuts(QKeySequence::New);
        actionAddImage->setIcon(QIcon::fromTheme("document-new"));
        actionAddImage->setText("Add");
        connect(actionAddImage, &QAction::triggered, this, &CaptionManager::newEntry);
    }

    {
        actionSaveImage->setShortcuts(QKeySequence::Save);
        actionSaveImage->setIcon(QIcon::fromTheme("document-save"));
        actionSaveImage->setText("Save");
        connect(actionSaveImage, &QAction::triggered, this, &CaptionManager::saveEntry);
    }

    {
        actionDeleteImage->setIcon(QIcon::fromTheme("document-delete"));
        actionDeleteImage->setText("Delete");
        connect(actionDeleteImage, &QAction::triggered, this, &CaptionManager::deleteEntry);
    }

    {
        actionExit->setText("Exit");
        actionExit->setIcon(QIcon::fromTheme("application-exit"));
        connect(actionExit, &QAction::triggered, this, &CaptionManager::exit);
    }

    {
        actionAbout->setText("About");
        actionAbout->setIcon(QIcon::fromTheme("help-about"));
        connect(actionAbout, &QAction::triggered, this, &CaptionManager::about);
    }

}

void capman::CaptionManager::createMenuBar()
{
    menuFile->addAction(actionAddImage);
    menuFile->addAction(actionSaveImage);
    menuFile->addAction(actionDeleteImage);
    menuFile->addSeparator();

    menuHelp->addAction(actionAbout);

    setMenuBar(menuBar);

}

void capman::CaptionManager::createUI()
{

    imageDateEdit->setCalendarPopup(true);

    imageTitleEdit->setPlaceholderText("Image title");


    {
        auto icon = QIcon::fromTheme("list-add");
        buttonAddImage->setIcon(icon);
        connect(buttonAddImage, &QPushButton::clicked, this, &CaptionManager::newEntry);
    }

    {
        auto icon = QIcon::fromTheme("document-save");
        buttonSaveImage->setIcon(icon);
        connect(buttonSaveImage, &QPushButton::clicked, this, &CaptionManager::saveEntry);
    }

    {
        auto icon = QIcon::fromTheme("edit-find");
        buttonFilterImageList->setIcon(icon);
        connect(buttonFilterImageList, &QPushButton::clicked, this, &CaptionManager::filterEntries);
    }

    {
        buttonEditImageTags->setText("Edit tags");
        connect(buttonEditImageTags, &QPushButton::clicked, this, &CaptionManager::editTags);
    }

    {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(imageListView->sizePolicy().hasHeightForWidth());
        imageListView->setSizePolicy(sizePolicy);
        imageListView->setMinimumWidth(150);
    }

    {
        imageCaptionEdit->setPlaceholderText("Add text here...");
    }


    listLayout->addWidget(imageListView);
    listLayout->addLayout(listButtonsLayout);
    listButtonsLayout->addWidget(buttonFilterImageList);
    listButtonsLayout->addWidget(buttonAddImage);

    metadataLayout->addWidget(imageTitleEdit);
    metadataLayout->addWidget(imageDateEdit);
    detailsLayout->addLayout(metadataLayout);
    detailsLayout->addWidget(imageView);
    detailsLayout->addWidget(imageCaptionEdit);
    detailsLayout->addLayout(detailButtonsLayout);
    detailButtonsLayout->addWidget(buttonEditImageTags);
    detailButtonsLayout->addWidget(buttonSaveImage);

    mainLayout->addLayout(listLayout);
    mainLayout->addLayout(detailsLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

}

void capman::CaptionManager::setupDatabase(const QString& dbpath)
{
    database.setDatabaseName(dbpath);

    if (!database.open()) {
        throw std::runtime_error("could not open database");
    }

    if (!database.tables().contains("images")) {
        qDebug() << "Database not initialised, creating new tables";

        QSqlQuery query(database);

        auto exec_query = [&query](auto t) {
            if (!query.exec(t)) {
                qDebug() << query.lastError();
                throw std::runtime_error("Failed to create database");
            }
        };

        exec_query(R"sql(
create table tags
(
    id integer not null
        constraint tags_pk
            primary key autoincrement,
    name text not null
)
)sql");

        exec_query(R"sql(
create unique index tags_id_uindex
    on tags (id)
)sql");

        exec_query(R"sql(
create unique index tags_name_uindex
    on tags (name)
)sql");

        exec_query(R"sql(
create table images
(
    id        integer not null
        constraint images_pk
            primary key autoincrement,
    name      text    not null,
    image_uri text,
    caption   text,
    created   timestamp
)
)sql");

        exec_query(R"sql(
create unique index images_id_uindex
    on images (id)
)sql");

        exec_query(R"sql(
create table image_tags
(
    id      integer not null
        constraint image_tags_pk
            primary key autoincrement,
    tagID   integer not null
        constraint image_tags_tags_id_fk
            references tags
            on update cascade on delete cascade
            deferrable initially deferred,
    imageID integer not null
        constraint image_tags_images_id_fk
            references images
)
)sql");

        exec_query(R"sql(
create unique index image_tags_id_uindex
    on image_tags (id)
)sql");




    }
}

void capman::CaptionManager::connectDatabase()
{
    imageView->setScene(scene);

    model->setTable("images");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    imageListView->setModel(model);
    imageListView->setModelColumn(model->fieldIndex("name"));

    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new ImageDelegate(model->fieldIndex("image_uri"), this));
    mapper->addMapping(imageCaptionEdit, model->fieldIndex("caption"));
    mapper->addMapping(imageTitleEdit, model->fieldIndex("name"));
    mapper->addMapping(imageView, model->fieldIndex("image_uri"));
    mapper->addMapping(imageDateEdit, model->fieldIndex("created"));

    mapper->toFirst();

    connect(
            imageListView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex
            );
}


void capman::CaptionManager::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    mapper->setCurrentModelIndex(imageListView->currentIndex());
}
