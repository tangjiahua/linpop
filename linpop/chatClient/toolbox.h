#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <QToolButton>
#include <QWidget>

class ToolBox : public QToolBox
{
public:
    ToolBox();
    void initUI();
    QWidget* initWidget(bool bVIP, const QString &name,
                            const QString &icon, const QString &desc = "");
    void onCurrentChanged(int index);
};

#endif // TOOLBOX_H
