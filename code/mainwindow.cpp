#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QPainter>
#include <QPen>
#include <QSize>
#include <QSpinBox>

void clear(int x , int y);

static int tangram_type[23][35]={{0}};
static int type_select = 0;
static int mouse_x = 0;
static int mouse_y = 0;
static int guide_line = 0; //2exist 0not
static int frame_x = 0;
static int frame_y = 0;
static int length_x = 20;
static int length_y = 11;
static int longer = 3;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //设置基本标题
    setWindowTitle(QStringLiteral("Tangram 我才不叫七巧板！"));
    setWindowIcon(QIcon("://tangram_2x196.png"));

    this->centralWidget()->setMouseTracking(true); //主界面鼠标追踪
    this->setMouseTracking(true);

    ui->type->setVisible(false); //测试用Label

    resize();
    iconm();
    user();

    for(int i=0 ; i < 35 ; i++){for(int j=0 ; j < length_y*2 ; j++){ tangram_type[j][i] = 0; }} //清空

    setPalette(QPalette(Qt::white)); //背景画布
    setAutoFillBackground(true);
    setMinimumSize((length_x+longer) * 60 , (length_y*2+1) * 30);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e_mouse)
{   //当前位置和当前格位
    ui->mposition->setText("(" + QString::number(e_mouse->x()) + "," + QString::number(e_mouse->y()) + ")");
    mouse_x = e_mouse->x(); mouse_y = e_mouse->y();
    frame_x = e_mouse->x()/60; frame_y = e_mouse->y()/30; //水平方向60一格 竖直方向30一格
    ui->mframe->setText("(" + QString::number(frame_x) + "," + QString::number(frame_y) + ")");
    ui->type->setText(QString::number(tangram_type[frame_y][frame_x]));
}

void MainWindow::mousePressEvent(QMouseEvent *e_mouse)
{
    if(type_select != 0){ //替换或添加 清除将被占用的格子 再占用
        clear(frame_x , frame_y);
        clear(frame_x , frame_y + 1);

        tangram_type[frame_y][frame_x] = type_select;
        tangram_type[frame_y + 1][frame_x] = 11;
    }
    else{ clear(frame_x , frame_y); } //清除当前被占用的格子

    count(); //计数
    update(); //重画
}

void MainWindow::iconm()
{   //工具栏选项中的图标设置
    ui->left_up->setIcon(QIcon("://icon/left_up.ico"));
    ui->left_up->setIconSize(QSize(48 , 48));
    ui->left_up->setText("");

    ui->right_up->setIcon(QIcon("://icon/right_up.ico"));
    ui->right_up->setIconSize(QSize(48 , 48));
    ui->right_up->setText("");

    ui->left_down->setIcon(QIcon("://icon/left_down.ico"));
    ui->left_down->setIconSize(QSize(48 , 48));
    ui->left_down->setText("");

    ui->right_down->setIcon(QIcon("://icon/right_down.ico"));
    ui->right_down->setIconSize(QSize(48 , 48));
    ui->right_down->setText("");

    ui->square->setIcon(QIcon("://icon/square.ico"));
    ui->square->setIconSize(QSize(48 , 48));
    ui->square->setText("");

    ui->empty_square->setIcon(QIcon("://icon/empty_square.ico"));
    ui->empty_square->setIconSize(QSize(48 , 48));
    ui->empty_square->setText("");
}

void MainWindow::user()
{
    ui->delete_2->setChecked(true);

    QButtonGroup* type_select_group = new QButtonGroup(ui->dockWidget);
    type_select_group->addButton(ui->left_up,1);
    type_select_group->addButton(ui->right_up,2);
    type_select_group->addButton(ui->left_down,3);
    type_select_group->addButton(ui->right_down,4);
    type_select_group->addButton(ui->square,5);
    type_select_group->addButton(ui->empty_square,6);
    type_select_group->addButton(ui->delete_2,0);

    connect( type_select_group , QOverload<int>::of(&QButtonGroup::buttonPressed),
                  [=](int i) //选择的种类返回值
                 {
                    type_select = i;
                  });

    connect( ui->guideline , QOverload<int>::of(&QCheckBox::stateChanged),
          [=](int state){ //是否显示网格的返回值
                guide_line = state ;
                update();
            });

    connect( ui->alldelete , &QPushButton::pressed ,this ,
             [=]() //全部清空
            {
                for(int i=0 ; i < 35 ; i++){for(int j=0 ; j < length_y*2 ; j++){tangram_type[j][i] = 0;}}
                count();
                update();
            });

    connect( ui->moveup , &QPushButton::pressed ,this ,
             [=]() //up
            {
                for(int i=0 ; i < (length_x+longer) ; i++){for(int j=0 ; j < length_y*2; j++){
                        tangram_type[j][i] = tangram_type[j + 1][i];}}
                    //tangram_type[22][i] = 0;}
                count();
                update();
            });

    connect( ui->movedown , &QPushButton::pressed ,this ,
             [=]() //dowm
            {
                for(int i=0 ; i < (length_x+longer) ; i++){for(int j=length_y*2 - 1; j > 0 ; j--){
                        tangram_type[j][i] = tangram_type[j - 1][i];}
                    tangram_type[0][i] = 0;}
                count();
                update();
            });

    connect( ui->moveleft , &QPushButton::pressed ,this ,
             [=]() //left
            {
                for(int i=0 ; i < (length_x+longer) ; i++){for(int j=0 ; j < length_y*2 ; j++){
                        tangram_type[j][i] = tangram_type[j][i + 1];}}
                count();
                update();
            });

    connect( ui->moveright , &QPushButton::pressed ,this ,
             [=]() //right
            {
                for(int i=(length_x+longer) - 1 ; i > 0 ; i--){for(int j=0 ; j < length_y*2 ; j++){
                        tangram_type[j][i] = tangram_type[j][i - 1];}}
                for(int j=0 ; j < length_y*2 ; j++){
                    tangram_type[j][0] = 0;}
                count();
                update();
            });

}

void MainWindow::resize()
{
    connect(ui->spinBox_weigth, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int d){
                longer = d - 20;
                setMinimumSize((length_x+longer) * 60 , (length_y*2+1) * 30);
                update();
            });
}

void MainWindow::count()
{   //计数功能 计数累加并显示所需的方块数
    int c[7] = {0};
    for(int i=0 ; i < (length_x+longer) ; i++){ for(int j=0 ; j < length_y*2 ; j++){
            if(tangram_type[j][i] != 0 && tangram_type[j][i] < 10)c[0]++;
            for(int k=1 ; k <= 6 ; k++){if(tangram_type[j][i] == k)c[k]++; }
        }}

    ui->label_left_up->setText(QString::number(c[1]));
    ui->label_right_up->setText(QString::number(c[2]));
    ui->label_left_down->setText(QString::number(c[3]));
    ui->label_right_down->setText(QString::number(c[4]));
    ui->label_square->setText(QString::number(c[5]));
    ui->label_empty_square->setText(QString::number(c[6]));
    ui->label_total->setText(QString::number(c[0]));
}

void clear(int x , int y)
{   //清空所传递格子的可能被占用的格子的数字
    if(tangram_type[y][x] == 11){
        tangram_type[y][x] = 0;
        tangram_type[y - 1][x] = 0;
    }
    else if(tangram_type[y][x] != 0){
        tangram_type[y][x] = 0;
        tangram_type[y + 1][x] = 0;
    }
}

void MainWindow::paintEvent(QPaintEvent *d_event)
{
    QPainter guide_painter(this);
    QColor gray(0xC0,0xC0,0xC0); //0xC0,0xC0,0xC0
    QPen pen(gray);
    pen.setWidth(1);
    guide_painter.setPen(pen);

    if(guide_line == 2){ //画出参考线
        for(int i = 0 ; i <= (length_y*2+1) ; i++){ guide_painter.drawLine(0 , i*30 , (length_x+longer)*60 , i*30);}
        for(int i = 0 ; i <= (length_x+longer) ; i++){ guide_painter.drawLine(i*60 , 0 , i*60 , (length_y*2+1)*30);}
    }

    QPainter tangram_painter(this);
    pen.setColor(Qt::black);    //画笔颜色
    tangram_painter.setBrush(Qt::black); //填充颜色
    tangram_painter.setPen(pen);         //选择画笔
    Qt::FillRule fillrule = Qt::OddEvenFill; //背景填充方式

    for(int i=0 ; i < (length_x+longer) ; i++){  //画出当前位置的图形
        for(int j=0 ; j < length_y*2 ; j++){
            const QPoint triangle_1[3]={QPoint(i*60,j*30),QPoint(i*60,j*30+59),QPoint(i*60+59,j*30)};
            const QPoint triangle_2[3]={QPoint(i*60,j*30),QPoint(i*60+59,j*30+59),QPoint(i*60+59,j*30)};
            const QPoint triangle_3[3]={QPoint(i*60,j*30),QPoint(i*60,j*30+59),QPoint(i*60+59,j*30+59)};
            const QPoint triangle_4[3]={QPoint(i*60,j*30+59),QPoint(i*60+59,j*30+59),QPoint(i*60+59,j*30)};
            const QPoint square[4]={QPoint(i*60,j*30),QPoint(i*60,j*30+59),QPoint(i*60+59,j*30+59),QPoint(i*60+59,j*30)};
            const QPoint square_2[4]={QPoint(i*60,j*30),QPoint(i*60,j*30+59),QPoint(i*60+59,j*30+59),QPoint(i*60+59,j*30)};

            if(tangram_type[j][i] == 1){tangram_painter.drawPolygon(triangle_1,3,fillrule);}
            else if(tangram_type[j][i] == 2){tangram_painter.drawPolygon(triangle_2,3,fillrule);}
            else if(tangram_type[j][i] == 3){tangram_painter.drawPolygon(triangle_3,3,fillrule);}
            else if(tangram_type[j][i] == 4){tangram_painter.drawPolygon(triangle_4,3,fillrule);}
            else if(tangram_type[j][i] == 5){tangram_painter.drawPolygon(square,4,fillrule);}
            else if(tangram_type[j][i] == 6){
                tangram_painter.drawPolygon(square_2,4,fillrule);
                tangram_painter.setBrush(Qt::white);
                tangram_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform , true);
                tangram_painter.drawEllipse(i*60+2 , j*30+2 , 56 , 56);
                tangram_painter.setBrush(Qt::black);
                tangram_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform , false);}
        }
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}
















