//
// Created by sam on 02/11/2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_caption_manager.h" resolved

#include "caption_manager.h"
#include "ui_caption_manager.h"

caption_manager::caption_manager(QWidget* parent) : QMainWindow(parent), ui(new Ui::caption_manager)
{
    ui->setupUi(this);
}

caption_manager::~caption_manager()
{
    delete ui;
}
