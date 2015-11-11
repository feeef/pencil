#ifndef PENCILAPP_H
#define PENCILAPP_H

#include <QApplication>
#include "editor.h"

class PencilApp : public QApplication
{
    Q_OBJECT

public:
    PencilApp(int &argv, char **args)
    : QApplication(argv, args) {}

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void setEditor(Editor *editor)
        { mEditor = editor; }

private:
    Editor *mEditor;
};

#endif // PENCILAPP_H
