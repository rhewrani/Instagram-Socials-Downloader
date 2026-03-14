#ifndef PROFILECHECKER_H
#define PROFILECHECKER_H

#include <QDialog>
#include "../core/manager.h"

namespace Ui {
class ProfileChecker;
}

class ProfileChecker : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileChecker(Manager *managerRef, QWidget *parent = nullptr);
    ~ProfileChecker();

    void pc_show();

private slots:
    void on_BTN_CHECK_ID_clicked();

    void on_BTN_CHECK_USER_clicked();

private:
    Ui::ProfileChecker *ui;
    Manager *manager;

    void Init();
    void setupUI();

    void pc_clearData();
};

#endif // PROFILECHECKER_H
