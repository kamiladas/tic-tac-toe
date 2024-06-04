#include "findgamedialog.h"
#include "ui_findgamedialog.h"

FindGameDialog::FindGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindGameDialog)
{
    ui->setupUi(this);
}

FindGameDialog::~FindGameDialog()
{
    delete ui;
}

QListWidget* FindGameDialog::getHostListWidget() const
{
    return ui->hostListWidget;
}

