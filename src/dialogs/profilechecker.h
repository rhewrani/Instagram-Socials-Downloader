#ifndef PROFILECHECKER_H
#define PROFILECHECKER_H

#include <QDialog>
#include "../core/manager.h"

namespace Ui {
class ProfileChecker;
}

/**
 * @brief The ProfileChecker class provides a UI to validate and add new Instagram profiles.
 * 
 * It takes a username, fetches its public data using the Instagram API, and 
 * shows the result before the user commits to adding it to their bookmarks.
 */
class ProfileChecker : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileChecker(Manager *managerRef, QList<Instagram::userData> &profilesEditRef, QWidget *parent = nullptr);
    ~ProfileChecker();

    //  @brief Shows the checker dialog and clears any previous state.
    void pc_show();

    //  @brief Callback triggered when the Instagram API returns profile info.
    void on_receivedProfileInfo(Instagram::userData *user);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void signal_updateProfileList();

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
