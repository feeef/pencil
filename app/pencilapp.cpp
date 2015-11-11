#include <QtWidgets>
#include "pencilapp.h"


bool PencilApp::event(QEvent *event)
{
    // Couldn't manage to make TabletEnterProximity to be called
    // with my Cintiq 18SX nor my Bamboo under Fedora 23.
    //
    // It should be used to determine that we are using a Tablet
    // before the mouse press event.
    //
    // This code is left here but has no incidence on the rest of
    // application.
    //
    if (event->type() == QEvent::TabletEnterProximity ||
        event->type() == QEvent::TabletLeaveProximity) {
        mEditor->setTabletDevice(
            static_cast<QTabletEvent *>(event)->device());
        return true;
    }
    return QApplication::event(event);
}
