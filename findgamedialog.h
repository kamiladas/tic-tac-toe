#ifndef FINDGAMEDIALOG_H
#define FINDGAMEDIALOG_H

#include <QDialog>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class FindGameDialog; }
QT_END_NAMESPACE

class FindGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindGameDialog(QWidget *parent = nullptr);
    ~FindGameDialog();

    QListWidget* getHostListWidget() const;

private:
    Ui::FindGameDialog *ui;
};

#endif // FINDGAMEDIALOG_H

