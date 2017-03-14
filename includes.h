#ifndef INCLUDES_H
#define INCLUDES_H

#ifdef WB_DEBUG_MODE
#include <QDebug>
#endif

#include <QPlainTextEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QClipboard>
#include <QApplication>
#include <QPainter>

#include <QCompleter>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextBrowser>
#include <QLineEdit>

#include <QLabel>
#include <QListView>
#include <QAbstractListModel>
#include <QAbstractItemModel>

#include <QColor>
#include <QColorDialog>
#include <QPushButton>
#include <QFormLayout>

#include <QColor>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#include <QTextEdit>
#include <QDockWidget>

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#ifdef Q_OS_MACX
#include <iostream>
#endif

//global definitions

//syntax highlighting colors

#define SYNTAX_COLOR_MAIN QColor( 0, 0, 255 )
#define SYNTAX_COLOR_COMMENT QColor( 0, 150, 0 )
#define SYNTAX_COLOR_STRING QColor( 255, 0, 100 )
#define SYNTAX_COLOR_SECONDARY QColor( 255, 0, 0 )
#define SYNTAX_COLOR_TERTIARY QColor( 255, 100, 0 )

#endif // INCLUDES_H
