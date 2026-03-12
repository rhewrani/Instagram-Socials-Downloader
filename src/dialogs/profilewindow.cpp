#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(Manager *managerRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileWindow)
    , manager(managerRef)
    , profiles(manager->getProfiles())
{
    ui->setupUi(this);
    profilesEdit = profiles;
    Init();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::Init()
{
    
}

void ProfileWindow::pw_setData()
{
    profilesEdit = profiles;
    pw_updateList();
}

void ProfileWindow::pw_updateList()
{
    ui->PW_LIST->clear();
    for (const auto &profile : profilesEdit) {
        ui->PW_LIST->addItem(QString("%1 (%2)").arg(profile.id).arg(profile.username));
    }
}

void ProfileWindow::pw_show()
{
    pw_setData();
    show();
}

void ProfileWindow::on_PW_BTN_ADD_clicked()
{

}

void ProfileWindow::on_PW_BTN_DEL_clicked()
{

}

void ProfileWindow::on_PW_BTN_UP_clicked()
{
    qDebug() << "Up clicked";
    if (ui->PW_LIST->currentRow() == 0) return;

    Instagram::userData temp = profilesEdit[ui->PW_LIST->currentRow()-1];
    profilesEdit[ui->PW_LIST->currentRow()-1] = profilesEdit[ui->PW_LIST->currentRow()];
    profilesEdit[ui->PW_LIST->currentRow()] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(ui->PW_LIST->currentRow()-1);
    
}


void ProfileWindow::on_PW_BTN_DOWN_clicked()
{
    if (ui->PW_LIST->currentRow() == ui->PW_LIST->count() - 1) return;

    Instagram::userData temp = profilesEdit[ui->PW_LIST->currentRow()+1];
    profilesEdit[ui->PW_LIST->currentRow()+1] = profilesEdit[ui->PW_LIST->currentRow()];
    profilesEdit[ui->PW_LIST->currentRow()] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(ui->PW_LIST->currentRow()+1);
}



