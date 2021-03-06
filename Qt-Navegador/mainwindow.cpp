#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    browser_ = new WebBrowser;
    browser_->loadSettings();
    browser_->GoHome();
    //setCentralWidget(browser_);
    // PESTAÑAS
    QTabWidget *tabWidgetMainWindow = new QTabWidget(this);
    tabWidgetMainWindow->setMinimumSize(800,600);
    tabWidgetMainWindow->addTab(browser_, "Pestaña 1");
    tabWidgetMainWindow->addTab(new WebBrowser, "Pestaña 2");
    tabWidgetMainWindow->setCurrentIndex(1);
    tabWidgetMainWindow->show();
    setCentralWidget(tabWidgetMainWindow);

    zoomVar = 1;    // inicialización del zoom

    // inicializamos los menus
    mainMenu_ = new QMenuBar(this);
    setMenuBar(mainMenu_);

    mnuArchivo_ = new QMenu(tr("&Archivo"), this);
    //mainMenu_ -> addMenu(mnuArchivo_);

    mnuEditar_ = new QMenu(tr("&Editar"), this);
    //mainMenu_ -> addMenu(mnuEditar_);

    mnuVer_ = new QMenu(tr("&Ver"), this);
    mainMenu_ -> addMenu(mnuVer_);
    actPlusZoom = new QAction(tr("&Aumentar zoom"), this);
    actPlusZoom -> setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
    mnuVer_->addAction(actPlusZoom);
    connect(actPlusZoom, SIGNAL(triggered()), this, SLOT(setPlusZoom()));

    actMinusZoom_ = new QAction(tr("&Disminuir zoom"), this);
    actMinusZoom_ -> setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
    mnuVer_->addAction(actMinusZoom_);
    connect(actMinusZoom_, SIGNAL(triggered()), this, SLOT(setMinusZoom()));


    mnuMarcadores_ = new QMenu(tr("&Marcadores"), this);
    mainMenu_ -> addMenu(mnuMarcadores_);
    actAddBookmark_ = new QAction(tr("&Añadir a marcadores"), this);
    actAddBookmark_ -> setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_O));
    mnuMarcadores_->addAction(actAddBookmark_);
    connect(actAddBookmark_, SIGNAL(triggered()), this, SLOT(addMarcador()));

    mnuHerramientas_ = new QMenu(tr("&Herramientas"), this);
    mainMenu_ -> addMenu(mnuHerramientas_);
    actSetHomepage_ = new QAction(tr("Establecer como pagina principal"), this);
    mnuHerramientas_->addAction(actSetHomepage_);
    connect(actSetHomepage_, SIGNAL(triggered()), browser_, SLOT(setHomepage()));

    mnuHistorial_ = new QMenu(tr("&Historial"), this);
    mainMenu_ -> addMenu(mnuHistorial_);
    actDeleteHistory_ = new QAction(tr("&Borrar el historial"), this);
    actDeleteHistory_->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    mnuHistorial_->addAction(actDeleteHistory_);
    connect(actDeleteHistory_,  SIGNAL(triggered()), this, SLOT(deleteHistory()));

    showHistory();

    readBookmarkFile();
    for (int i = 0; i < bookmarkList.size(); ++i) {
        QAction* tmp = new QAction(tr(bookmarkList.at(i).toLocal8Bit().constData()), this);
        mnuMarcadores_->addAction(tmp);
        connect(tmp,SIGNAL(triggered()),this,SLOT(PulsarMarcador()));
    }
}

MainWindow::~MainWindow()
{

}


void MainWindow::readBookmarkFile()
{
    QFile file("./../marcadores.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        bookmarkList << line;
    }

    file.close();
}

void MainWindow::addMarcador()
{
    QFile file("./../marcadores.txt");
    file.open(QIODevice::Append);
    QTextStream out(&file);
    QString url = browser_->getAddress();
    out << endl<< url;

    QAction* tmp = new QAction(url, this);
    mnuMarcadores_->addAction(tmp);
    connect(tmp,SIGNAL(triggered()),this,SLOT(PulsarMarcador()));

    file.close();
}

void MainWindow::PulsarMarcador() {
    QAction* act = (QAction*) QObject::sender();
    browser_ -> setAddress(act->text());
}


void MainWindow::showHistory() {
    QFile file("./../historial.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    } else {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QAction* tmp = new QAction(line, this);
            mnuHistorial_->addAction(tmp);
            connect(tmp,SIGNAL(triggered()),this,SLOT(PulsarMarcador()));
        }
        file.close();
    }
}

void MainWindow::deleteHistory() {
    QFile file("./../historial.txt");
    file.open(QIODevice::Truncate | QIODevice::Text | QIODevice::ReadWrite);
    file.close();
}

void MainWindow::setPlusZoom() {
    zoomVar++;
    browser_->web_->setZoomFactor(zoomVar);
}

void MainWindow::setMinusZoom() {
    zoomVar--;
    browser_->web_->setZoomFactor(zoomVar);
}
