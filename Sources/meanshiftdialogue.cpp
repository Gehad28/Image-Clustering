#include "meanshiftdialogue.h"
#include "ui_meanshiftdialogue.h"

MeanShiftDialogue::MeanShiftDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeanShiftDialogue)
{
    ui->setupUi(this);
    ui->spatialBW->setValue(10);
    ui->colorBW->setValue(10);
}

MeanShiftDialogue::~MeanShiftDialogue()
{
    delete ui;
}

void MeanShiftDialogue::on_buttonBox_accepted()
{
    spatialBW = ui->spatialBW->value();
    colorBW = ui->colorBW->value();
}

