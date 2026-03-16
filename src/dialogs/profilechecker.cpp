#include "profilechecker.h"
#include "ui_profilechecker.h"

ProfileChecker::ProfileChecker(Manager *managerRef, QList<Instagram::userData> &profilesEditRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileChecker)
    , manager(managerRef)
    , profilesEdit(profilesEditRef)
{
    ui->setupUi(this);

    Init();
}

ProfileChecker::~ProfileChecker()
{
    delete ui;
}

bool ProfileChecker::eventFilter(QObject *obj, QEvent *event)
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

void ProfileChecker::Init()
{
    InitTitleBar();
    InitLang();
    setupUI();
}

void ProfileChecker::InitTitleBar() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: #1a1a1a; color: white;");

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    QLabel *titleLabel = new QLabel(_("PRFL_LOOK"));
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

    connect(closeBtn, &QPushButton::clicked, this, &ProfileChecker::close);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(ui->checkerwidget);
    setLayout(mainLayout);
}

void ProfileChecker::InitLang() {
    ui->BTN_CHECK_USER->setText(_("PRFL_CHCK"));
    ui->BTN_SAVE->setText(_("BTN_SAVE"));
    ui->BTN_CNCL->setText(_("BTN_CAN"));
    ui->LBL_FND->setText(_("PRFL_FND"));
    ui->LBL_NFND->setText(_("PRFL_NFND"));
    ui->LN_OUTP_ID->setText(_("PRFL_ID"));
    ui->LN_OUTP_USER->setText(_("PRFL_USER"));
    ui->LN_OUTP_NAME->setText(_("PRFL_NAME"));
    ui->LN_INPT_USER->setPlaceholderText(_("PRFL_PLCH"));
}

void ProfileChecker::setupUI()
{
    ui->LBL_NFND->hide();
    ui->LBL_FND->hide();
    ui->LBL_USER_PFP->hide();
    ui->LN_OUTP_ID->hide();
    ui->LN_OUTP_USER->hide();
    ui->LN_OUTP_NAME->hide();
    ui->line->hide();

}

void ProfileChecker::pc_show()
{
    show();
    pc_clearData();
}

void ProfileChecker::pc_clearData()
{
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
    ui->line->hide();

    ui->BTN_SAVE->setHidden(true);
    editUser = nullptr;
}

bool ProfileChecker::pc_checkUserExists(const QString &id) {
    for (const auto &profile : profilesEdit) {
        if (profile.id == id) return true;
    }
    return false;
}

void ProfileChecker::on_BTN_CHECK_USER_clicked()
{
    const QString username = ui->LN_INPT_USER->text();
    if (username.isEmpty()) return;

    ui->BTN_SAVE->setHidden(true);
    manager->instagram_GET_userInfo(username, true);

}

void ProfileChecker::on_receivedProfileInfo(Instagram::userData *user)
{
    if (user == nullptr) {
        ui->LBL_NFND->show();
        return;
    }

    ui->BTN_SAVE->setHidden(false);

    pc_helperSetPfpImageFromURL(ui->LBL_USER_PFP, user->profilePicUrl, user->id, 150, 150);
    ui->LN_OUTP_ID->setText(user->id);
    ui->LN_OUTP_USER->setText(user->username);
    ui->LN_OUTP_NAME->setText(user->fullname);
    
    ui->LBL_FND->show();
    ui->LBL_USER_PFP->show();
    ui->LN_OUTP_ID->show();
    ui->LN_OUTP_USER->show();
    ui->LN_OUTP_NAME->show();
    ui->line->show();

    editUser = user;
}

void ProfileChecker::pc_helperSetPfpImageFromURL(QLabel *target, QString &url, QString &id, int dimX, int dimY)
{
    manager->loadPixmap(url, id, dimX, dimY,
                        [this, id, target, dimX](const QPixmap &pixmap) {
        QPixmap rounded(pixmap.size());
        rounded.fill(Qt::transparent);

        QPainter painter(&rounded);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(pixmap.rect());
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pixmap);

        target->setPixmap(rounded);
        target->setScaledContents(true);
        target->setStyleSheet(QString(
                                  "border-radius: %1px;"
                                  "border: 2px solid #0c0c0c;"
                                  "background-color: lightgray;"
                                  "background-position: center;"
                                  "background-repeat: no-repeat;"
                                  ).arg(dimX/2));
    });
}

void ProfileChecker::on_BTN_SAVE_clicked()
{
    if (pc_checkUserExists(editUser->id)) {
         Logger::instance()->warning(_(""));
        return;
    }
    profilesEdit.append(*editUser);
    close();
}


void ProfileChecker::on_BTN_CNCL_clicked()
{
    close();
}

