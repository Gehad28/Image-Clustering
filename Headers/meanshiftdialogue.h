#ifndef MEANSHIFTDIALOGUE_H
#define MEANSHIFTDIALOGUE_H

#include <QDialog>

namespace Ui {
class MeanShiftDialogue;
}

class MeanShiftDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit MeanShiftDialogue(QWidget *parent = nullptr);
    ~MeanShiftDialogue();
    int spatialBW;
    int colorBW;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MeanShiftDialogue *ui;
};

#endif // MEANSHIFTDIALOGUE_H
