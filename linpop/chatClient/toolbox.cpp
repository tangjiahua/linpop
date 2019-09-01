#include "toolbox.h"
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QToolBox>

QWidget* initWidget(bool bVIP, const QString &name,
                        const QString &icon, const QString &desc = "");
void onCurrentChanged(int index);

ToolBox::ToolBox()
{
    initUI();  //初始化UI
    connect(this, &QToolBox::currentChanged, this, &ToolBox::onCurrentChanged);
}


void initUI() {
        struct User {
            bool bVIP;  // 会员
            QString strName;  // 名字
            QString strIcon;  // 图像
            QString strDesc;  // 个人说明
        } user[3][5] =
        {
            {
                {true, QStringLiteral("香香公主"), ":/QQ/1", QStringLiteral("金庸笔下的第一美女")},
                {true, QStringLiteral("小龙女"), ":/QQ/2", QStringLiteral("玉女心经")},
                {true, QStringLiteral("王语嫣"), ":/QQ/3", QStringLiteral("熟读各派武学秘笈")},
                {false, QStringLiteral("赵敏"), ":/QQ/4", QStringLiteral("大元第一美人")},
                {false, QStringLiteral("周芷若"), ":/QQ/5", QStringLiteral("光复汉家河山，光大峨嵋")}
            },

            {
                {true, QStringLiteral("萧峰"), ":/QQ/6", QStringLiteral("丐帮帮主 - 智勇双全、胆略过人、豪迈飒爽")},
                {true, QStringLiteral("令狐冲"), ":/QQ/8", QStringLiteral("独孤九剑")},
                {true, QStringLiteral("杨过"), ":/QQ/9", QStringLiteral("黯然销魂掌")},
                {false, QStringLiteral("郭靖"), ":/QQ/7", QStringLiteral("降龙十八掌")},
                {false, QStringLiteral("胡一刀"), ":/QQ/10", QStringLiteral("辽东大侠")}
            },

            {
                {true, QStringLiteral("金轮法王"), ":/QQ/11", QStringLiteral("龙象般若功")},
                {true, QStringLiteral("丁春秋"), ":/QQ/13", QStringLiteral("星宿老仙，法力无边")},
                {false, QStringLiteral("裘千仞"), ":/QQ/12", QStringLiteral("铁掌水上飘")},
                {false, QStringLiteral("成昆"), ":/QQ/14", QStringLiteral("混元霹雳手")},
                {false, QStringLiteral("李莫愁"), ":/QQ/15", QStringLiteral("冰魄银针，杀人如麻")}
            }
        };

        QStringList groupList;
        groupList << QStringLiteral("大美女") << QStringLiteral("大英雄") << QStringLiteral("大恶人");

        // 初始化列表
        for (int i = 0; i < sizeof(user)/sizeof(user[0]); i++)
        {
            QGroupBox *pGroupBox = new QGroupBox();
            QVBoxLayout *pLayout = new QVBoxLayout(pGroupBox);

            // 添加好友
            for (int j = 0; j < sizeof(user[0])/sizeof(user[0][0]); ++j)
            {
                QWidget *pWidget = initWidget(user[i][j].bVIP, user[i][j].strName,
                                              user[i][j].strIcon, user[i][j].strDesc);
                pLayout->addWidget(pWidget);
            }
            pLayout->addStretch();
            pLayout->setSpacing(10);
            pLayout->setContentsMargins(10, 10, 10, 10);

            // 添加分组
            //addItem(pGroupBox, groupList.at(i));


        }
    }

QWidget* initWidget(bool bVIP, const QString &name,
                        const QString &icon, const QString &desc = "") {
        QWidget *pWidget = new QWidget(this);
        QLabel *pPixmapLabel = new QLabel(this);
        QLabel *pNameLabel = new QLabel(this);
        QLabel *pDescLabel = new QLabel(this);

        // 图像 名称 描述
        pPixmapLabel->setPixmap(QPixmap(icon));
        pNameLabel->setText(name);
        pDescLabel->setText(desc);

        // VIP名字为红色；否则，白色
        pNameLabel->setStyleSheet(QString("color: %1;").arg(bVIP ? "rgb(240, 30, 40)" : "white"));
        pDescLabel->setStyleSheet("color: gray;");

        QVBoxLayout *pVLayout = new QVBoxLayout();
        pVLayout->addStretch();
        pVLayout->addWidget(pNameLabel);
        pVLayout->addWidget(pDescLabel);
        pVLayout->addStretch();
        pVLayout->setSpacing(5);
        pVLayout->setContentsMargins(0, 0, 0, 0);

        QHBoxLayout *pHLayout = new QHBoxLayout();
        pHLayout->addWidget(pPixmapLabel);
        pHLayout->addLayout(pVLayout);
        pHLayout->addStretch();
        pHLayout->setContentsMargins(0, 0, 0, 0);

        pWidget->setLayout(pHLayout);

        return pWidget;
    }

void onCurrentChanged(int index) {
        QString strGroup = itemText(index);
        qDebug() << strGroup;
    }
