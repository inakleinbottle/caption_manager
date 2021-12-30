//
// Created by sam on 14/11/2021.
//

#ifndef CAPTION_MANAGER_CAPTIONMANAGER_H
#define CAPTION_MANAGER_CAPTIONMANAGER_H

#include <QtCore>
#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QGraphicsScene>
#include <QSqlDatabase>
#include <QtSql>
#include <QDebug>

#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>


#include "TagEditorDialog.h"
#include "ImageDelegate.h"


namespace capman {

class CaptionManager : public QMainWindow
{
    Q_OBJECT

private slots:

    void newEntry();
    void saveEntry();
    void deleteEntry();
    void filterEntries();
    void editTags();
    void updateImage();
    void exportDocument();

    void about();
    void exit();

protected:
    void resizeEvent(QResizeEvent* event) override;

public:

    explicit CaptionManager(QString dbpath, QWidget* parent=nullptr);
    ~CaptionManager() override;


private:

    void setupMainWindow();
    void createActions();
    void createMenuBar();
    void createUI();
    void setupDatabase(const QString& dbpath);
    void connectDatabase();

private:

    QSqlDatabase database;
    QSqlTableModel* model;
    QDataWidgetMapper* mapper;
    QGraphicsScene* scene;

    // UI elements
    QWidget* centralWidget;
    QHBoxLayout* mainLayout;
    QVBoxLayout* listLayout;
    QHBoxLayout* listButtonsLayout;
    QVBoxLayout* detailsLayout;
    QHBoxLayout* metadataLayout;
    QHBoxLayout* detailButtonsLayout;

    QListView* imageListView;
    QGraphicsView* imageView;
    QDateTimeEdit* imageDateEdit;
    QLineEdit* imageTitleEdit;
    QTextEdit* imageCaptionEdit;
    QPushButton* buttonAddImage;
    QPushButton* buttonFilterImageList;
    QPushButton* buttonSaveImage;
    QPushButton* buttonEditImageTags;

    QMenuBar* menuBar;
    QMenu* menuFile;
    QMenu* menuHelp;

    // Actions
    QAction* actionAbout;
    QAction* actionAddImage;
    QAction* actionSaveImage;
    QAction* actionDeleteImage;
    QAction* actionExport;
    QAction* actionExit;

};

}
#endif//CAPTION_MANAGER_CAPTIONMANAGER_H
