//
// Created by sam on 31/10/2021.
//

#include <QApplication>
#include <QDebug>

#include "caption_manager.h"
#include "database_manager.h"

#define CAPMAN_VERSION "0.0"

int main(int argc, char* argv[])
{
    //Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Caption Manager");
    QCoreApplication::setApplicationVersion(CAPMAN_VERSION);

    database_manager dbm("test.sqlite");


    caption_manager main_window;
    main_window.show();
    return app.exec();

}