//
// Created by sam on 31/10/2021.
//

#include <QApplication>
#include <QDebug>
#include <filesystem>

#include "CaptionManager.h"


#define CAPMAN_VERSION "0.0"

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(caption_manager);
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("inakleinbottle");
    QCoreApplication::setApplicationName("Caption Manager");
    QCoreApplication::setApplicationVersion(CAPMAN_VERSION);

    QSettings settings;

    QCommandLineParser parser;
    parser.addOptions({
            {
                    {"d", "database"},
                    QCoreApplication::translate("main", "Use an alternative database file"),
                    "database"
            }
    });

    parser.process(app);

    QString database_path = parser.value("database");
    if (database_path.isEmpty()) {
        if (!settings.contains("database")) {
            auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            path += QDir::separator();
            path += "capman.sqlite";
            qDebug() << "Writing default location to settings file" << path;
            settings.setValue("database", QDir::cleanPath(path));
        }
        database_path = settings.value("database").toString();
    }

    std::filesystem::path p(database_path.toStdString());
    if (!std::filesystem::exists(p)) {
        QDir root;
        qDebug() << "Creating directory" << p.parent_path().c_str();
        root.mkpath(p.parent_path().c_str());
    }


    capman::CaptionManager main_window(database_path);
    main_window.show();
    return app.exec();
}