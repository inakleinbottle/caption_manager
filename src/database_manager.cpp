//
// Created by sam on 02/11/2021.
//

#include "database_manager.h"


inline bool is_images_initialised(QSqlDatabase& db)
{
    return db.tables().contains("images");
}

database_manager::database_manager(const QString& path)
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        throw std::runtime_error("Could not open database path");
    }

    if (!is_images_initialised(m_db)) {
        qDebug() << "Database is not initialised, creating tables";
        QSqlQuery query;
        query.exec("CREATE TABLE images (id integer primary key, name text, image_uri path, caption text)");
    } else {
        qDebug() << "Database is already initialised";
    }
}

database_manager::~database_manager()
{
    m_db.close();
}

bool database_manager::add_image_entry(const QString& name, const QString& path, const QString& caption)
{
    return false;
}
bool database_manager::del_image_entry(const QString& name)
{
    return false;
}
database_manager::image_entry database_manager::get_image_entry(const QString& name)
{
    return database_manager::image_entry();
}
database_manager::image_entry database_manager::get_image_entry(int id)
{
    return database_manager::image_entry();
}
