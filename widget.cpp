#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
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
#include <QTime>

#define AXI_BASE_ADDRESS    0x43C00000
#define AXI_OFFSET          0x0
#define AXI_SIZE            0x1000
#define REG0_OFFSET         0
#define REG1_OFFSET         4
#define REG2_OFFSET         8
#define REG3_OFFSET         12
#define DEVICE_FILENAME     "/dev/uio0"

int uioFileAddress;             // File address of the device we're opening.
off_t Axi_PAOffset;             // Page-alligned offset of AXI memory region.
unsigned *ptr_MappedMem;             // Virtually-mapped hardware memory address.

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
unsigned readReg(unsigned offset) {
    return ptr_MappedMem[offset/sizeof(*ptr_MappedMem)];
}

int Widget::uio_init()
{
    /* Open the file.
     * Really, this gets the virtual memmory addess, which will then be used
     * in mmap().  Therefore, the file can be opened as read-only.
     */
    //uioFileAddress = open(DEVICE_FILENAME, O_RDONLY);
    uioFileAddress = open(DEVICE_FILENAME, O_RDWR);

    /* Test for successful file open. */
    if(uioFileAddress == -1) {
        qDebug()<<"File could not be open";
        // TODO: exit program or throw error.
    }
    else
        qDebug()<<"File Open with no ERRORS";

    /* Find page-alligned address of desired memory. */
    Axi_PAOffset = AXI_BASE_ADDRESS & ~(sysconf(_SC_PAGE_SIZE) - 1);

    /* Obtain space in virtual memory linked to hardwere memory. */
    size_t mmaplen = (int)AXI_BASE_ADDRESS + (int)AXI_SIZE - Axi_PAOffset;

    qDebug("Mapping length: %d, fileAddress:%d axi_offset: %lu",
           mmaplen, uioFileAddress, 0);

    ptr_MappedMem = (unsigned*)mmap(nullptr, mmaplen,
                               PROT_READ | PROT_WRITE, MAP_SHARED,
                               uioFileAddress, 0);

    if(ptr_MappedMem == MAP_FAILED) {
         qDebug()<<"mmap has FAILED";
         uioFileAddress = close();
    }
    else
        qDebug()<<"mmap SUCCESFULL";

    return 0;
}

void Widget::on_checkButton_clicked()
{
    uio_init();

   qDebug("Reg0:%u",ptr_MappedMem[0]);
    //qDebug()<< *ptrreg1Value;

    qDebug(" Reg0:%u \tReg1:%u \tReg2:%u \tReg3:%u",
           readReg(0),
           readReg(REG1_OFFSET),
           readReg(REG2_OFFSET),
           readReg(REG3_OFFSET));

    /*  int *ptr;
     *  int value;
     *  ptr = &value; */

    /*for (unsigned int loop_cntr = 1; loop_cntr <= 10; ++loop_cntr)
    {
        qDebug("%u:\tReg0:%u \tReg1:%u \tReg2:%u \tReg3:%u",
               loop_cntr,
               (int)(ptr_MappedMem[REG0_OFFSET]),
               (int)(ptr_MappedMem+REG1_OFFSET),
               (int)(ptr_MappedMem+REG2_OFFSET),
               (int)(ptr_MappedMem+REG3_OFFSET));
    }*/

}

void Widget::on_checkLionelButton_clicked()
{
    uio_init();
    int reg0Value = ptr_MappedMem[0];
    int reg1Value = ptr_MappedMem[1];
    int reg2Value = ptr_MappedMem[2];
    int reg3Value = ptr_MappedMem[3];

    qDebug(" Reg0:%u \tReg1:%u \tReg2:%u \tReg3:%u",
           reg0Value,
           reg1Value,
           reg2Value,
           reg3Value);


   qDebug("assigned value is: %d", ptr_MappedMem[0]);
}
