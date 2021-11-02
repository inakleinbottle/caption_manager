//
// Created by sam on 02/11/2021.
//

#ifndef CAPTION_MANAGER_CAPTION_MANAGER_H
#define CAPTION_MANAGER_CAPTION_MANAGER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class caption_manager;
}
QT_END_NAMESPACE

class caption_manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit caption_manager(QWidget* parent = nullptr);
    ~caption_manager() override;

private:
    Ui::caption_manager* ui;
};

#endif//CAPTION_MANAGER_CAPTION_MANAGER_H
