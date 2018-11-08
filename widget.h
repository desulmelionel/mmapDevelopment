#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
   // unsigned readReg(unsigned offset);
    ~Widget();

private slots:
    void on_checkButton_clicked();
    int uio_init();

    void on_checkLionelButton_clicked();

private:
    Ui::Widget *ui;
    int uioFileAddress;             // File address of the device we're opening.
    off_t Axi_PAOffset;             // Page-alligned offset of AXI memory region.
    unsigned *ptr_MappedMem;    // Virtually-mapped hardware memory address.
   // unsigned readReg(unsigned offset);
};

#endif // WIDGET_H
