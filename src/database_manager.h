//
// Created by sam on 02/11/2021.
//

#ifndef CAPTION_MANAGER_DATABASE_MANAGER_H
#define CAPTION_MANAGER_DATABASE_MANAGER_H

#include <QtCore>
#include <QtDebug>
#include <QtSql>


/*
 * I'm very much using the following as a tutorial for this:
 * http://katecpp.github.io/sqlite-with-qt/
 * I think this is a pretty good model to follow.
 *
 */

class database_manager
{

public:

    struct image_entry {
        int id;
        QString name;
        QString path;
        QString caption;
    };

    explicit database_manager(const QString& path);

    ~database_manager();

    bool add_image_entry(const QString& name, const QString& path, const QString& caption);
    bool del_image_entry(const QString& name);
    image_entry get_image_entry(const QString& name);
    image_entry get_image_entry(int id);


private:

    QSqlDatabase m_db;
};

#endif//CAPTION_MANAGER_DATABASE_MANAGER_H
