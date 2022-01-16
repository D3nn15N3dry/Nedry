#include "Main.h"

class CloseHandler : public QObject{
private:
    bool flag;
    QString buf;
protected:
    bool eventFilter(QObject* obj, QEvent* event){
        if(obj->isWidgetType()) {
            switch(event->type()){
                case QEvent::Close:{
                    if(flag){
                        event->ignore();
                        return true;
                    }
                    break;
                }
                case QEvent::KeyPress:{
                    if(((QKeyEvent*)event)->key() == Qt::Key_F4 && (((QKeyEvent*)event)->modifiers() == Qt::AltModifier)){
                        flag = true;
                    }else if(((QKeyEvent*)event)->key() == Qt::Key_Tab && (((QKeyEvent*)event)->modifiers() == Qt::AltModifier)){
                            event->ignore();
                            return true;
                    }else if(((QKeyEvent*)event)->key() == Qt::Key_Return){
                        if(buf == NEDRY_PASSWORD)
                            qApp->quit();
                        buf.clear();
                    }else{
                        buf.append(((QKeyEvent*)event)->key());
                    }
                    break;
                }
                case QEvent::MouseButtonPress:
                case QEvent::MouseButtonRelease:
                case QEvent::MouseMove:
                case QEvent::Leave:
                    //flag = false;
                default: break;
            }
        }
        return QObject::eventFilter(obj,event);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int n = qApp->screens().length();
    CloseHandler h;
    for(int i = 0; i < n; i++){
        QRect screenres = QApplication::desktop()->screenGeometry(i);
        QWidget *widget = new QWidget();
        widget->installEventFilter(&h);
        widget->move(QPoint(screenres.x(), screenres.y()));
        widget->resize(screenres.width(), screenres.height());
        widget->showFullScreen();
        QVBoxLayout *main_layout = new QVBoxLayout();
        widget->setLayout(main_layout);

        QHBoxLayout *qhbl01 = new QHBoxLayout();
        main_layout->addLayout(qhbl01);
#ifdef SAFEMODE
        QPushButton *but = new QPushButton("SAFEMODE EXIT BUTTON\nCAN BE REMOVED IN MAIN.H");
        qhbl01->addWidget(but);
        QObject::connect(but, &QPushButton::clicked, [=](){qApp->quit();});
#endif
        qhbl01->addStretch();

        main_layout->addStretch();
        QHBoxLayout *qhbl02 = new QHBoxLayout();
        main_layout->addLayout(qhbl02);
        QLabel *label02 = new QLabel();
        QMovie *movie = new QMovie(":/res/nedry.gif");
        label02->setMovie(movie);
        movie->start();
        movie->setScaledSize(QSize(640,480));
        qhbl02->addStretch();
        qhbl02->addWidget(label02);
        qhbl02->addStretch();

        main_layout->addStretch();
        if(!i){
            widget->grabKeyboard();
        }
    }

    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/res/ahahahh.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer *player = new QMediaPlayer();
    player->setPlaylist(playlist);
    player->setVolume(50);
    player->play();
    return a.exec();
}
