#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>
#include "profilechecker.h"

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWindow(Manager *managerRef, QWidget *parent = nullptr);
    ~ProfileWindow();
    
    void closeEvent(QCloseEvent *event) override;

    void pw_show();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_BTN_UP_clicked();

    void on_BTN_DOWN_clicked();

    void on_BTN_ADD_clicked();

    void on_BTN_DEL_clicked();

signals:
    void signal_updateProfileList(const QList<Instagram::userData> &profilesEdit);

private:
    Ui::ProfileWindow *ui;
    Manager *manager;
    ProfileChecker *profileChecker;

    QList<Instagram::userData> &profiles;
    QList<Instagram::userData> profilesEdit;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;


    void Init();
    void InitTitleBar();
    void InitLang();

    void pw_setData();
    void pw_updateList();
    void pw_checkDataChanged();

    bool dataChanged = false;

};

#endif // PROFILEWINDOW_H
