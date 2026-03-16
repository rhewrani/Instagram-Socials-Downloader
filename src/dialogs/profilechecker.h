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
    explicit ProfileChecker(Manager *managerRef, QList<Instagram::userData> &profilesEditRef, QWidget *parent = nullptr);
    ~ProfileChecker();

    void pc_show();

    void on_receivedProfileInfo(Instagram::userData *user);

    // TO-DO: 
    // 1. Prevent usage of profilewindow if profilechecker is opened (maybe not?)
    // 2. Prevent adding the same user twice (just check if the user id is already in the list)
    // 3. Remove discord stuff

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_BTN_CHECK_USER_clicked();

    void on_BTN_SAVE_clicked();

    void on_BTN_CNCL_clicked();

private:
    Ui::ProfileChecker *ui;
    Manager *manager;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;

    Instagram::userData *editUser = nullptr;
    QList<Instagram::userData> &profilesEdit;

    void Init();
    void InitTitleBar();
    void InitLang();
    void setupUI();

    void pc_clearData();
    bool pc_checkUserExists(const QString &id);
    void pc_helperSetPfpImageFromURL(QLabel *target, QString &url, QString &id, int dimX, int dimY);
};

#endif // PROFILECHECKER_H
