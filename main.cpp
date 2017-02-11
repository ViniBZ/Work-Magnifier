#include <QApplication>
#include "maincontrol.h"

int main(int argc, char * argv[])
{
    QApplication a(argc,argv);

    QString new_cur_path=QCoreApplication::applicationDirPath();
    new_cur_path=new_cur_path.section("/",0,-2);
    QDir::setCurrent(new_cur_path);

    MainControl m;
    m.show();
    m.start_work();

    return a.exec();
}

