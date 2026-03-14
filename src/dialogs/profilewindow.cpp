#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(Manager *managerRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileWindow)
    , manager(managerRef)
    , profiles(manager->getProfiles())
{
    ui->setupUi(this);
    profileChecker = new ProfileChecker(manager, this);
    profilesEdit = profiles;
    Init();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::closeEvent(QCloseEvent *event)
{
    pw_checkDataChanged();
    if (dataChanged) {
        if (manager->saveProfiles(profilesEdit)) {
            emit signal_updateProfileList(profilesEdit);
            event->accept();
        } else {
            event->ignore();
        };
    } else {
        event->accept();
    }
}

void ProfileWindow::Init()
{
    
}

void ProfileWindow::pw_setData()
{
    profilesEdit = manager->getProfiles();
    pw_updateList();
}

void ProfileWindow::pw_show()
{
    pw_setData();
    show();
}

void ProfileWindow::pw_updateList()
{
    ui->PW_LIST->clear();
    for (const auto &profile : profilesEdit) {
        ui->PW_LIST->addItem(QString("%1 (%2)").arg(profile.id).arg(profile.username));
    }
}

void ProfileWindow::pw_checkDataChanged()
{
    if (profiles.size() != profilesEdit.size()) {
        dataChanged = true;
        return;
    }

    for (int i = 0; i < profiles.size(); ++i) {
        if (profiles[i].id != profilesEdit[i].id) {
            dataChanged = true;
            return;
        }
    }

    dataChanged = false;
}

void ProfileWindow::on_PW_BTN_ADD_clicked()
{
    if (profileChecker->isVisible()) {
        profileChecker->raise();
        profileChecker->activateWindow();
    } else {
        profileChecker->pc_show();
    }
}

void ProfileWindow::on_PW_BTN_DEL_clicked()
{
    if (ui->PW_LIST->currentRow() == -1) return;
    profilesEdit.removeAt(ui->PW_LIST->currentRow());
    pw_updateList();
}

void ProfileWindow::on_PW_BTN_UP_clicked()
{
    int currentRow = ui->PW_LIST->currentRow();
    if (currentRow <= 0) return;

    Instagram::userData temp = profilesEdit[currentRow-1];
    profilesEdit[currentRow-1] = profilesEdit[currentRow];
    profilesEdit[currentRow] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(currentRow-1);
}


void ProfileWindow::on_PW_BTN_DOWN_clicked()
{
    int currentRow = ui->PW_LIST->currentRow();
    if (currentRow == ui->PW_LIST->count() - 1 || currentRow == -1) return;

    Instagram::userData temp = profilesEdit[currentRow+1];
    profilesEdit[currentRow+1] = profilesEdit[currentRow];
    profilesEdit[currentRow] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(currentRow+1);
}