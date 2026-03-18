#include "profilewindow.h"
#include "profilechecker.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(Manager *managerRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileWindow)
    , manager(managerRef)
    , profiles(manager->getProfiles())
{
    ui->setupUi(this);
    profileChecker = new ProfileChecker(manager, profilesEdit, this);
    profilesEdit = profiles;
    Init();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

// Overrides close event to save changes if the profile list was modified.
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

bool ProfileWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == titleBar) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                m_isDragging = true;
                m_dragStartPosition = me->globalPosition().toPoint() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if (m_isDragging && (me->buttons() & Qt::LeftButton)) {
                move(me->globalPosition().toPoint() - m_dragStartPosition);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            m_isDragging = false;
        }
    }
    return QDialog::eventFilter(obj, event);
}

void ProfileWindow::Init()
{
    InitTitleBar();
    InitLang();

    connect(manager->getInstagram(), &Instagram::signal_profileCheckerReceivedInfo,
            profileChecker, [this](Instagram::userData *user) {
                profileChecker->on_receivedProfileInfo(user);
            });

    connect(profileChecker, &ProfileChecker::signal_updateProfileList, this, &ProfileWindow::pw_updateList);
}

void ProfileWindow::InitTitleBar() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: #1a1a1a; color: white;");

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    QLabel *titleLabel = new QLabel(_("PRFL_TTL"));
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold;");

    QPushButton *closeBtn = new QPushButton("×");
    closeBtn->setFixedSize(32, 24);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; border: none; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #e81123; color: white; }"
        "QPushButton:pressed { background-color: #bf0f1d; }"
        );

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeBtn);

    titleBar->installEventFilter(this);

    connect(closeBtn, &QPushButton::clicked, this, &ProfileWindow::close);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(ui->profilewidget);
    setLayout(mainLayout);
}

// Initializes language-specific texts for UI elements.
void ProfileWindow::InitLang() {
    ui->LBL_TTL->setText(_("PRFL_TTL"));
    ui->LBL_INFO->setText(_("PRFL_INFO"));
}

// Populates the internal edit list with current profiles and updates the view.
void ProfileWindow::pw_setData()
{
    profilesEdit = profiles;
    pw_updateList();
}

// Shows the profile window and updates its data.
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

void ProfileWindow::on_BTN_ADD_clicked()
{
    if (profileChecker->isVisible()) {
        profileChecker->raise();
        profileChecker->activateWindow();
    } else {
        profileChecker->pc_show();
    }
}

void ProfileWindow::on_BTN_DEL_clicked()
{
    if (ui->PW_LIST->currentRow() == -1) return;
    profilesEdit.removeAt(ui->PW_LIST->currentRow());
    pw_updateList();
}

void ProfileWindow::on_BTN_UP_clicked()
{
    int currentRow = ui->PW_LIST->currentRow();
    if (currentRow <= 0) return;

    Instagram::userData temp = profilesEdit[currentRow-1];
    profilesEdit[currentRow-1] = profilesEdit[currentRow];
    profilesEdit[currentRow] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(currentRow-1);
}


void ProfileWindow::on_BTN_DOWN_clicked()
{
    int currentRow = ui->PW_LIST->currentRow();
    if (currentRow == ui->PW_LIST->count() - 1 || currentRow == -1) return;

    Instagram::userData temp = profilesEdit[currentRow+1];
    profilesEdit[currentRow+1] = profilesEdit[currentRow];
    profilesEdit[currentRow] = temp;
    pw_updateList();
    ui->PW_LIST->setCurrentRow(currentRow+1);
}
