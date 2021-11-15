//
// Created by sam on 14/11/2021.
//

#include "CaptionManager.h"
#include <QIcon>

void capman::CaptionManager::newEntry()
{
    qDebug() << "New entry";
}

void capman::CaptionManager::saveEntry()
{
    qDebug() << "Save entry";
    mapper->submit();  // Force submit of the current widgets
    if (model->isDirty()) {
        qDebug() << "Submitting changes";
        if (!model->submitAll()) {
            qDebug() << "Failed to submit changes";
        }
    }
}

void capman::CaptionManager::deleteEntry()
{
    qDebug() << "Delete entry";
}

void capman::CaptionManager::about()
{
    qDebug() << "about";
}

void capman::CaptionManager::exit()
{
    QCoreApplication::quit();
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

    imageListView->setObjectName("imageListView");
    imageView->setObjectName("imageView");
    imageTitleEdit->setObjectName("ImageTitleEdit");
    imageDateEdit->setObjectName("imageDateEdit");
    imageCaptionEdit->setObjectName("imageCaptionEdit");

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
        connect(actionExit, &QAction::triggered, this, &CaptionManager::exit);
    }

    {
        actionAbout->setText("About");
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

    metadataLayout->addWidget(imageTitleEdit);
    metadataLayout->addWidget(imageDateEdit);
    detailsLayout->addLayout(metadataLayout);
    detailsLayout->addWidget(imageView);
    detailsLayout->addWidget(imageCaptionEdit);
    detailsLayout->addLayout(detailButtonsLayout);

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

        QSqlQuery query;
        query.exec("CREATE TABLE images (id integer primary key, name text, image_uri path, caption text)");
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
