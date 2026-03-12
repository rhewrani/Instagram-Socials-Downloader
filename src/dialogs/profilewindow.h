#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>
#include "../core/manager.h"

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWindow(Manager *managerRef, QWidget *parent = nullptr);
    ~ProfileWindow();
    
    void pw_show();

private slots:
    void on_PW_BTN_UP_clicked();

    void on_PW_BTN_DOWN_clicked();

    void on_PW_BTN_ADD_clicked();

    void on_PW_BTN_DEL_clicked();

private:
    Ui::ProfileWindow *ui;
    Manager *manager;

    QList<Instagram::userData> profiles;
    QList<Instagram::userData> profilesEdit;

    void Init();
    void pw_setData();
    void pw_updateList();

    bool dataChanged = false;

};

#endif // PROFILEWINDOW_H
