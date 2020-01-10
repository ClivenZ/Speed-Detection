#include "include.h"

#define uchar unsigned  char

#define LCD_RS GpioDataRegs.GPADAT.bit.GPIOA6
//#define LCD_RW GpioDataRegs.GPADAT.bit.GPIO2
#define LCD_EN GpioDataRegs.GPADAT.bit.GPIOA7
#define LCD_DB GpioDataRegs.GPBDAT.all

//函数定义
void Init_Port(void);
void LCD_init(void);                        //初始化函数
void LCD_write_command(uchar command);      //写指令函数
void LCD_write_data(uchar dat);             //写数据函数
void show(char *cont);                      //显示子函数


//显示子函数，cout为显示字符串指针参数
void show(char* cout){
    while(*cout != 0){
        LCD_write_data(*cout);
        DELAY_US(500);
        cout++;
    }
}

//初始化函数
void Init_Port(void){
    EALLOW;

    //配置GPIOA、B为通用IO口
    GpioMuxRegs.GPAMUX.all = 0xFFFF;
    GpioMuxRegs.GPBMUX.all = 0xFFFF;

    //配置IO口为输出状态
    GpioMuxRegs.GPADIR.all = 0xFFFF;
    GpioMuxRegs.GPBDIR.all = 0xFFFF;

    //与系统时钟同步
    GpioMuxRegs.GPAQUAL.all = 0x0000;
    GpioMuxRegs.GPBQUAL.all = 0x0000;

    //输出数据RS和EN清零
    GpioDataRegs.GPADAT.bit.GPIOA6 = 1;
    //GpioDataRegs.GPADAT.bit.GPIO2 = 0;
    GpioDataRegs.GPADAT.bit.GPIOA7 = 0;
    EDIS;
}

//写指令函数
void LCD_write_command(uchar command){
    LCD_EN = 0;
    LCD_RS = 0;//指令
    //LCD_RW = 0;
    LCD_DB = command; //赋值给D0-D7对应的GPIO2-GPIO9
    DELAY_US(500);
    LCD_EN = 1;
    DELAY_US(1000);
    LCD_EN = 0;
}

//LCD写数据函数
void LCD_write_data(uchar dat){
    LCD_EN = 0;
    LCD_RS = 1; //数据
    //LCD_RW = 0;   //写入
    LCD_DB = dat;   //赋值D0~D7 G2~9
    DELAY_US(500);
    LCD_EN = 1;     //允许
    DELAY_US(1000);
    LCD_EN = 0;
}

//LCD1602初始化
void LCD_init(void){
    DELAY_US(15000);//延迟15ms
    LCD_write_command(0x38);//设置8位格式，2行，5x7
    DELAY_US(5000);//延迟5ms
    LCD_write_command(0x38);//设置8位格式，2行，5x7
    DELAY_US(5000);
    LCD_write_command(0x38);//设置8位格式，2行，5x7
    LCD_write_command(0x38);//设置8位格式，2行，5x7
    LCD_write_command(0x08);//关显示，不显示光标，光标不闪烁；
    LCD_write_command(0x01);//清除屏幕显示：数据指针清零，所有显示清零；
    LCD_write_command(0x06);//设定输入方式，增量不移位
    LCD_write_command(0x0c);//整体显示，关光标，不闪烁
}

//main
int main(void){

#if 1
// Copy InitFlash function code and Flash setup code to RAM
  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
// This function must reside in RAM
  InitFlash();
#endif

  // 步骤 1. 初始化系统控制:
  // 设置PLL, WatchDog, 使能外设时钟
  // DSP281x_SysCtrl.c文件中找到.
     InitSysCtrl();

  // 步骤 2. 初始化通用输入输出多路复用器GPIO:
  // 这个函数在DSP281x_Gpio.c源文件中被定义了

  //初始化液晶对应的GPIO引脚；
     Init_Port();

  // 步骤 3. 清除所有中断,初始化中断向量表:
  // 禁止CPU全局中断
     DINT;

  // 初始化PIE控制寄存器到他们的默认状态.
  // 这个默认状态就是禁止PIE中断及清除所有PIE中断标志
  // 这个函数放在DSP281x_PieCtrl.c源文件里
     InitPieCtrl();

  // 禁止CPU中断和清除所有CPU中断标志
     IER = 0x0000; //中断标志寄存器
     IFR = 0x0000; //中断使能寄存器

  //初始化PIE中断向量表，并使其指向中断服务子程序（ISR）
  // 这些中断服务子程序被放在了DSP281x_DefaultIsr.c源文件中
  // 这个函数放在了DSP281x_PieVect.c源文件里面.

     InitPieVectTable();


  // 步骤 4.初始化片内外设:
  // 该函数可以在DSP281x_InitPeripherals.c里找到
  // InitPeripherals();

  // 步骤 5. 用户指定的例程:
    for(;;)
     {
        LCD_init();

          LCD_write_command(0x80);    //第一行数据指针地址
          show(" HELLO WORLD!!!");

          LCD_write_command(0xc0);    //第二行数据指针的地址
          show(" hello world!!!");
        while(1);

     }

}



