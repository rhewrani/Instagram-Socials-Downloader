#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "../core/manager.h"

class Manager;

namespace Ui {
class Settingswindow;
}

class Settingswindow : public QDialog
{
    Q_OBJECT

public:
    explicit Settingswindow(Manager *managerRef, QWidget *parent = nullptr);
    ~Settingswindow();

    void closeEvent(QCloseEvent *event) override;

    void sw_show();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void signal_updateTextMainWindow();

private slots:
    void on_CKBX_OFE_checkStateChanged(const Qt::CheckState &arg1);

    void on_CKBX_DQ_checkStateChanged(const Qt::CheckState &arg1);

    void on_CKBX_EAC_checkStateChanged(const Qt::CheckState &arg1);

    void on_CMBX_LANG_activated(int index);

    void on_BTN_TDF_OPEN_clicked();

    void on_LN_TGEN_INP_textChanged();

    void on_LN_TGEN_INP2_textChanged();

    void on_BTN_RESET_clicked();

    void on_BTN_SAVE_clicked();

    void on_LN_SSID_textChanged(const QString &arg1);

private:
    Ui::Settingswindow *ui;
    Manager *manager;

    Manager::appSettings &settings;
    Manager::appSettings editSettings;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;

    void Init();
    void InitTitleBar();
    void InitLang();

    void sw_setData();
    void checkVariables();

    bool settingsChanged = false;
    bool restartRequired = false;
    bool saveButtonUsed = false;

    QMap<QString, QString> params = {
        {"user", "LISA"},
        {"username", "lalalalisa_m"},
        {"fullname", "LISA"},
        {"caption", "First Emmys experience with my White lotus family 🪷"},
        {"link", "https://www.instagram.com/lalalalisa_m/p/DOqalquE4cU/"},
        {"likes", "1,234,567"},
        {"comments", "12,345"},
        {"views", "0"},
        {"timestamp", "2024-03-17"},
        {"location", "Los Angeles, California"},
        {"biography", "Lalisa Manobal / LLOUD"},
        {"followers", "102,000,000"},
        {"posts_count", "1,200"}
    };

    QMap<QString, QString> paramsStory = {
        {"user", "LLOUD"},
        {"username", "wearelloud"},
        {"fullname", "LLOUD"},
        {"link", "https://www.instagram.com/stories/wearelloud/"},
        {"biography", "Official account for LLOUD"},
        {"followers", "5,000,000"},
        {"posts_count", "150"}
    };
};

#endif // SETTINGSWINDOW_H
