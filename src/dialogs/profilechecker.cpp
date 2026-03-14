#include "profilechecker.h"
#include "ui_profilechecker.h"

ProfileChecker::ProfileChecker(Manager *managerRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileChecker)
    , manager(managerRef)
{
    ui->setupUi(this);

    Init();
}

ProfileChecker::~ProfileChecker()
{
    delete ui;
}

void ProfileChecker::Init()
{
    setupUI();
}

void ProfileChecker::setupUI()
{
    ui->LBL_NFND->hide();
    ui->LBL_FND->hide();
    ui->LBL_USER_PFP->hide();
    ui->LN_OUTP_ID->hide();
    ui->LN_OUTP_USER->hide();
    ui->LN_OUTP_NAME->hide();
}

void ProfileChecker::pc_show()
{
    show();
}

void ProfileChecker::pc_clearData()
{
    ui->LN_INPT_ID->clear();
    ui->LN_INPT_USER->clear();
    ui->LN_OUTP_ID->clear();
    ui->LN_OUTP_USER->clear();
    ui->LN_OUTP_NAME->clear();

    ui->LBL_NFND->hide();
    ui->LBL_FND->hide();
    ui->LBL_USER_PFP->hide();
    ui->LN_OUTP_ID->hide();
    ui->LN_OUTP_USER->hide();
    ui->LN_OUTP_NAME->hide();
}

void ProfileChecker::on_BTN_CHECK_ID_clicked()
{
    const QString id = ui->LN_INPT_ID->text();
    if (id.isEmpty()) return;

    // when checking if the user exists: already add the user to the profiles list if found (but not in the json! just editProfiles)
    // so that we dont have to all again when the user presses on save
    // and if the user decides to not to keep the found user, just remove it from the profiles list

}

void ProfileChecker::on_BTN_CHECK_USER_clicked()
{
    const QString username = ui->LN_INPT_USER->text();
    if (username.isEmpty()) return;

    // when checking if the user exists: already add the user to the profiles list if found (but not in the json! just editProfiles)
    // so that we dont have to all again when the user presses on save
    // and if the user decides to not to keep the found user, just remove it from the profiles list
    
}

