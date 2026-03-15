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

    void on_receivedProfileInfo(Instagram::userData *user, QList<Instagram::userData> *profilesEdit);

    // TO-DO: 
    // 1. Prevent usage of profilewindow if profilechecker is opened (maybe not?)
    // 2. Investigate issue when you sometimes cant swap back in profilewindow / not being saved?
    // 3. Check what happens when you delete a selected user
    // 4. Prevent adding the same user twice (just check if the user id is already in the list)

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_BTN_CHECK_USER_clicked();

private:
    Ui::ProfileChecker *ui;
    Manager *manager;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;

    void Init();
    void InitTitleBar();
    void InitLang(); // TODO
    void setupUI();

    void pc_clearData();
    void pc_helperSetPfpImageFromURL(QLabel *target, QString &url, QString &id, int dimX, int dimY);
};

#endif // PROFILECHECKER_H
