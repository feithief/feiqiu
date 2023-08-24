#include "bcmmasterframe.h"
#include "application_config.h"
#include <QString>
#include <QPainter>
#include <QPen>
#include "ui_masterframe.h"

static const int color[256][3] = {
    {55 ,252   ,0},{55 ,251   ,0},{55 ,250   ,0},{56 ,248   ,0},{55 ,242   ,0},
    {54 ,237   ,5},{53 ,230  ,11},{50 ,214  ,21},{52 ,228  ,42},{57 ,250  ,65},
    {58 ,253  ,90},{56 ,249 ,106},{55 ,242 ,124},{52 ,231 ,143},{51 ,226 ,151},
    {49 ,215 ,166},{43 ,190   ,0},{58 ,252,0    },{58 ,251   ,0},{57 ,249   ,0},
    {55 ,241   ,1},{55 ,232  ,11},{52 ,223  ,16},{50 ,212  ,27},{54 ,234  ,46},
    {59 ,253  ,74},{58 ,250 ,103},{56 ,241 ,124},{53 ,232 ,141},{51 ,218 ,162},
    {48 ,209 ,175},{47 ,207 ,178},{49 ,189   ,0},{64 ,252   ,0},{63 ,250   ,0},
    {64 ,245   ,1},{64 ,241   ,7},{59 ,231  ,14},{54 ,214  ,24},{56 ,223  ,38},
    {64 ,252  ,69},{62 ,253  ,92},{60 ,244 ,118},{56 ,230 ,143},{52 ,217 ,163},
    {49 ,203 ,182},{47 ,195 ,192},{44 ,182 ,199},{78 ,253   ,0},{80 ,251   ,0},
    {77 ,249   ,2},{72 ,246   ,5},{74, 238  ,17},{66 ,222  ,24},{66 ,228  ,42},
    {72 ,250  ,64},{71 ,252  ,94},{67 ,246 ,112},{62 ,226 ,147},{57 ,218 ,160},
    {53 ,195 ,188},{48 ,173 ,207},{43 ,154 ,227},{41 ,146 ,235},{97 ,253  , 0},
    {94 ,252   ,2},{97 ,251   ,7},{96 ,250  ,15},{88 ,239  ,24},{79 ,222  ,35},
    {87 ,248  ,60},{56 ,169  ,54},{81 ,245 ,109},{75 ,232 ,134},{68 ,214 ,160},
    {61 ,191 ,182},{53 ,163 ,212},{48 ,143 ,235},{30  ,84 ,182},{27  ,78 ,183},
    {118 ,250   ,0},{120 ,250   ,4},{120 ,252  ,11},{120 ,252  ,21},{116 ,247  ,34},
    {103 ,235  ,48},{106 ,254  ,72},{104 ,247  ,99},{95 ,233 ,125},{86 ,216 ,150},
    {76 ,189 ,177},{67 ,164 ,205},{58 ,135 ,236},{35  ,79 ,179},{31  ,66 ,184},
    {26  ,53 ,184},{141 ,242   ,0},{143 ,243   ,5},{147 ,244  ,11},{147 ,249  ,22},
    {144 ,253  ,36},{105 ,164  ,67},{137 ,253  ,77},{124 ,239 ,105},{111 ,221 ,132},
    {99 ,202 ,152},{88 ,176 ,183},{80 ,150 ,210},{66 ,118 ,246},{40  ,64 ,179},
    {34  ,50 ,187},{30  ,40 ,186},{161 ,231   ,1},{171 ,226   ,6},{171 ,228  ,10},
    {172 ,233  ,20},{174 ,238  ,33},{180 ,242  ,56},{173 ,232  ,82},{154 ,215 ,109},
    {139 ,195 ,136},{125 ,173 ,163},{112 ,150 ,191},{95 ,124 ,223},{58  ,69 ,175},
    {50  ,56 ,185},{45  ,46 ,192},{37  ,34 ,183},{181 ,209   ,1},{183, 208   ,5},
    {185 ,209  ,10},{189 ,207  ,17},{191 ,210  ,29},{196 ,206  ,47},{194 ,199  ,74},
    {182 ,190 ,102},{165 ,171 ,134},{150 ,151 ,162},{129 ,129 ,194},{116 ,110 ,219},
    {68  ,62 ,174},{59  ,47 ,179},{50  ,39 ,184},{44  ,32 ,182},{209 ,168   ,2},
    {214 ,159   ,5},{215 ,159   ,9},{215 ,160  ,15},{216 ,160  ,29},{218 ,154  ,46},
    {215 ,150  ,71},{208 ,142  ,96},{193 ,126 ,134},{172 ,115 ,166},{154  ,93 ,199},
    {133  ,78 ,229},{80  ,44 ,176},{70  ,37 ,183},{60 , 31 ,184},{55  ,27 ,184},
    {229 ,128   ,3},{234 ,118   ,6},{235 ,117  ,10},{236 ,115  ,18},{236 ,113  ,31},
    {235 ,113  ,47},{233 ,104  ,73},{172  ,72  ,78},{207  ,84 ,145},{191  ,72 ,175},
    {172  ,61 ,205},{149  ,52 ,233},{94  ,29 ,176},{81  ,26 ,181},{73  ,23 ,181},
    {67  ,21 ,183},{185  ,64   ,2},{184  ,60   ,5},{183  ,57   ,8},{252  ,76  ,19},
    {181,  55  ,21},{251  ,73  ,48},{247  ,70  ,71},{239  ,64 ,103},{226  ,57 ,135},
    {209  ,51 ,168},{187  ,42 ,201},{169  ,36 ,225},{110  ,21 ,172},{99  ,19 ,179},
    {90  ,17 ,178},{88  ,16 ,181},{244  ,63   ,3},{213  ,44   ,5},{214  ,43   ,8},
    {229  ,48  ,14},{235  ,46  ,26},{185  ,37  ,32},{193  ,37  ,51},{182  ,34  ,69},
    {237  ,44 ,125},{222  ,38 ,157},{208  ,33 ,181},{191  ,29 ,205},{182  ,28 ,216},
    {168  ,26 ,233},{165  ,24 ,238},{159  ,23 ,245},{181  ,29   ,1},{179 , 28   ,3},
    {176  ,26   ,5},{186  ,29   ,9},{195  ,31  ,17},{214  ,34  ,31},{185  ,31  ,42},
    {181  ,30  ,55},{248,  40 ,105},{239  ,37 ,129},{233  ,35 ,142},{218  ,31 ,168},
    {208  ,29 ,184},{205  ,28 ,188},{197  ,27 ,200},{197,  25 ,201},{163  ,21   ,1},
    {163  ,21   ,2},{165  ,22   ,3},{169  ,23   ,7},{180  ,26  ,14},{196  ,29  ,24},
    {233 , 37  ,43},{183  ,29  ,47},{183  ,29  ,62},{249  ,38 ,103},{176  ,26  ,89},
    {239  ,34 ,131},{236  ,33 ,138},{231  ,31 ,148},{231  ,31 ,148},{231  ,30 ,149},
    {154  ,18   ,0},{157  ,19   ,1},{160  ,20   ,3},{166  ,22   ,6},{177  ,25  ,13},
    {192  ,28  ,23},{225  ,35  ,40},{175  ,27  ,40},{180  ,29  ,52},{181  ,28  ,64},
    {178  ,27  ,73},{177  ,26  ,78},{180  ,26  ,83},{246  ,34 ,116},{177  ,24  ,82},
    {247  ,34 ,114},
};


static const QString QFrameBasicQSS = "QFrame{\
                                        border:2px solid #0fbacd;\
                                        border-radius: 15px;\
                                        background-color:rgba(0, 0, 0, 0.7);\
                                        }";


static const QString PushButtonEnabledQSS = "QPushButton{\
                                                border:2px solid #0fbacd;\
                                                border-top-right-radius: 15px;\
                                                border-bottom-left-radius: 15px;\
                                                color:rgb(255,251,240);\
                                                font-size:22px;\
                                                background:rgba(29, 165, 219, 0.3);\
                                                }";
static const QString PushButtonDisabledQSS = "QPushButton{\
                                                border:0px;\
                                                border-top-right-radius: 15px;\
                                                border-bottom-left-radius: 15px;\
                                                color:rgba(255,251,240,0.5);\
                                                font-size:22px;\
                                                background:rgba(29, 165, 219, 0.1);\
                                                }";



BCMMasterFrame::BCMMasterFrame(QWidget *parent):
  QWidget(parent),
  ui(new Ui::MasterFrame)
{
  colorX = 0;
  colorY = 0;
  updateSianglImmediate = true;

  linScheduler = AmbientLinScheduler::getInstance(this);

  product_check_is_on = false;//new add
  TenColor_Mode_is_on = false;//new add
  change_color_Timer = new QTimer ( this );//new add
  connect(change_color_Timer, SIGNAL(timeout()), this, SLOT(change_RGBW_color()));//new add


  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();


  ui->setupUi(this);

  for(uint16_t i = 0;i<256;i++) //修改color按钮的颜色和色表一致
  {
      QString str = QString("QPushButton{\
                             border:2px solid #0fbacd;\
                             border-top-right-radius: 15px;\
                             border-bottom-left-radius: 15px;\
                             color:rgb(%1   ,%2 ,%3 );\
                             ont-size:22px;\
                             background:rgba(29, 165, 219, 0.3);\
                             }")
                            .arg(QString::number(color[i][0],10))
                            .arg(QString::number(color[i][1],10))
                            .arg(QString::number(color[i][2],10));
      QPushButton *button = this->findChild<QPushButton *>(QString("pushButtonColor%1").arg(QString::number(i+1)));
      button->setStyleSheet(str);
  }

  ui->frameCIE->installEventFilter(this);

  groupPaused = false;
  colorPaused = false;

#if 0
  this->ui->pushButtonPauseG->setText(tr("Pause Group"));
  this->ui->pushButtonPauseC->setText(tr("Pause Color"));

  connect(ui->pushButtonPauseG, SIGNAL(clicked()), this, SLOT(groupPauseClicked()));
  connect(ui->pushButtonPauseC, SIGNAL(clicked()), this, SLOT(colorPauseClicked()));
#endif

#if 0//slide control
  /* close the display of  the ScrollBar*/
  ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  ui->scrollArea->setStyleSheet("background:transparent");
  ui->scrollAreaWidgetContents->setStyleSheet("background:transparent");

  scroller = QScroller::scroller(ui->scrollArea);
  QScroller::ScrollerGestureType gesture = QScroller::LeftMouseButtonGesture;
  scroller->grabGesture(ui->scrollArea, gesture);
#endif

//  ui->groupBox->setStyleSheet("background:transparent");
//  ui->groupBox_2->setStyleSheet("background:transparent");
//  ui->groupBox_3->setStyleSheet("background:transparent");
//  ui->groupBox_4->setStyleSheet("background:transparent");
//  ui->groupBox_5->setStyleSheet("background:transparent");
//  ui->groupBox_6->setStyleSheet("background:transparent");


   ui->groupBox->show();
   ui->groupBox_2->hide();
   ui->groupBox_3->hide();
   ui->groupBox_4->hide();
   ui->groupBox_5->hide();
   ui->groupBox_6->hide();
   ui->groupBox_7->hide(); //new add

  colorMapper = new QSignalMapper((QObject*)this);
  colorMapper->setMapping(ui->pushButtonColor1, 0);
  colorMapper->setMapping(ui->pushButtonColor2, 1);
  colorMapper->setMapping(ui->pushButtonColor3, 2);
  colorMapper->setMapping(ui->pushButtonColor4, 3);
  colorMapper->setMapping(ui->pushButtonColor5, 4);
  colorMapper->setMapping(ui->pushButtonColor6, 5);
  colorMapper->setMapping(ui->pushButtonColor7, 6);
  colorMapper->setMapping(ui->pushButtonColor8, 7);
  colorMapper->setMapping(ui->pushButtonColor9, 8);
  colorMapper->setMapping(ui->pushButtonColor10, 9);
  colorMapper->setMapping(ui->pushButtonColor11, 10);
  colorMapper->setMapping(ui->pushButtonColor12, 11);
  colorMapper->setMapping(ui->pushButtonColor13, 12);
  colorMapper->setMapping(ui->pushButtonColor14, 13);
  colorMapper->setMapping(ui->pushButtonColor15, 14);
  colorMapper->setMapping(ui->pushButtonColor16, 15);
  colorMapper->setMapping(ui->pushButtonColor17, 16);
  colorMapper->setMapping(ui->pushButtonColor18, 17);
  colorMapper->setMapping(ui->pushButtonColor19, 18);
  colorMapper->setMapping(ui->pushButtonColor20, 19);
  colorMapper->setMapping(ui->pushButtonColor21, 20);
  colorMapper->setMapping(ui->pushButtonColor22, 21);
  colorMapper->setMapping(ui->pushButtonColor23, 22);
  colorMapper->setMapping(ui->pushButtonColor24, 23);
  colorMapper->setMapping(ui->pushButtonColor25, 24);
  colorMapper->setMapping(ui->pushButtonColor26, 25);
  colorMapper->setMapping(ui->pushButtonColor27, 26);
  colorMapper->setMapping(ui->pushButtonColor28, 27);
  colorMapper->setMapping(ui->pushButtonColor29, 28);
  colorMapper->setMapping(ui->pushButtonColor30, 29);
  colorMapper->setMapping(ui->pushButtonColor31, 30);
  colorMapper->setMapping(ui->pushButtonColor32, 31);
  colorMapper->setMapping(ui->pushButtonColor33, 32);
  colorMapper->setMapping(ui->pushButtonColor34, 33);
  colorMapper->setMapping(ui->pushButtonColor35, 34);
  colorMapper->setMapping(ui->pushButtonColor36, 35);
  colorMapper->setMapping(ui->pushButtonColor37, 36);
  colorMapper->setMapping(ui->pushButtonColor38, 37);
  colorMapper->setMapping(ui->pushButtonColor39, 38);
  colorMapper->setMapping(ui->pushButtonColor40, 39);
  colorMapper->setMapping(ui->pushButtonColor41, 40);
  colorMapper->setMapping(ui->pushButtonColor42, 41);
  colorMapper->setMapping(ui->pushButtonColor43, 42);
  colorMapper->setMapping(ui->pushButtonColor44, 43);
  colorMapper->setMapping(ui->pushButtonColor45, 44);
  colorMapper->setMapping(ui->pushButtonColor46, 45);
  colorMapper->setMapping(ui->pushButtonColor47, 46);
  colorMapper->setMapping(ui->pushButtonColor48, 47);
  colorMapper->setMapping(ui->pushButtonColor49, 48);
  colorMapper->setMapping(ui->pushButtonColor50, 49);
  colorMapper->setMapping(ui->pushButtonColor51, 50);
  colorMapper->setMapping(ui->pushButtonColor52, 51);
  colorMapper->setMapping(ui->pushButtonColor53, 52);
  colorMapper->setMapping(ui->pushButtonColor54, 53);
  colorMapper->setMapping(ui->pushButtonColor55, 54);
  colorMapper->setMapping(ui->pushButtonColor56, 55);
  colorMapper->setMapping(ui->pushButtonColor57, 56);
  colorMapper->setMapping(ui->pushButtonColor58, 57);
  colorMapper->setMapping(ui->pushButtonColor59, 58);
  colorMapper->setMapping(ui->pushButtonColor60, 59);
  colorMapper->setMapping(ui->pushButtonColor61, 60);
  colorMapper->setMapping(ui->pushButtonColor62, 61);
  colorMapper->setMapping(ui->pushButtonColor63, 62);
  colorMapper->setMapping(ui->pushButtonColor64, 63);
  colorMapper->setMapping(ui->pushButtonColor65, 64);
  colorMapper->setMapping(ui->pushButtonColor66, 65);
  colorMapper->setMapping(ui->pushButtonColor67, 66);
  colorMapper->setMapping(ui->pushButtonColor68, 67);
  colorMapper->setMapping(ui->pushButtonColor69, 68);
  colorMapper->setMapping(ui->pushButtonColor70, 69);
  colorMapper->setMapping(ui->pushButtonColor71, 70);
  colorMapper->setMapping(ui->pushButtonColor72, 71);
  colorMapper->setMapping(ui->pushButtonColor73, 72);
  colorMapper->setMapping(ui->pushButtonColor74, 73);
  colorMapper->setMapping(ui->pushButtonColor75, 74);
  colorMapper->setMapping(ui->pushButtonColor76, 75);
  colorMapper->setMapping(ui->pushButtonColor77, 76);
  colorMapper->setMapping(ui->pushButtonColor78, 77);
  colorMapper->setMapping(ui->pushButtonColor79, 78);
  colorMapper->setMapping(ui->pushButtonColor80, 79);
  colorMapper->setMapping(ui->pushButtonColor81, 80);
  colorMapper->setMapping(ui->pushButtonColor82, 81);
  colorMapper->setMapping(ui->pushButtonColor83, 82);
  colorMapper->setMapping(ui->pushButtonColor84, 83);
  colorMapper->setMapping(ui->pushButtonColor85, 84);
  colorMapper->setMapping(ui->pushButtonColor86, 85);
  colorMapper->setMapping(ui->pushButtonColor87, 86);
  colorMapper->setMapping(ui->pushButtonColor88, 87);
  colorMapper->setMapping(ui->pushButtonColor89, 88);
  colorMapper->setMapping(ui->pushButtonColor90, 89);
  colorMapper->setMapping(ui->pushButtonColor91, 90);
  colorMapper->setMapping(ui->pushButtonColor92, 91);
  colorMapper->setMapping(ui->pushButtonColor93, 92);
  colorMapper->setMapping(ui->pushButtonColor94, 93);
  colorMapper->setMapping(ui->pushButtonColor95, 94);
  colorMapper->setMapping(ui->pushButtonColor96, 95);
  colorMapper->setMapping(ui->pushButtonColor97, 96);
  colorMapper->setMapping(ui->pushButtonColor98, 97);
  colorMapper->setMapping(ui->pushButtonColor99, 98);

  colorMapper->setMapping(ui->pushButtonColor100, 99);
  colorMapper->setMapping(ui->pushButtonColor101, 100);
  colorMapper->setMapping(ui->pushButtonColor102, 101);
  colorMapper->setMapping(ui->pushButtonColor103, 102);
  colorMapper->setMapping(ui->pushButtonColor104, 103);
  colorMapper->setMapping(ui->pushButtonColor105, 104);
  colorMapper->setMapping(ui->pushButtonColor106, 105);
  colorMapper->setMapping(ui->pushButtonColor107, 106);
  colorMapper->setMapping(ui->pushButtonColor108, 107);
  colorMapper->setMapping(ui->pushButtonColor109, 108);
  colorMapper->setMapping(ui->pushButtonColor110, 109);
  colorMapper->setMapping(ui->pushButtonColor111, 110);
  colorMapper->setMapping(ui->pushButtonColor112, 111);
  colorMapper->setMapping(ui->pushButtonColor113, 112);
  colorMapper->setMapping(ui->pushButtonColor114, 113);
  colorMapper->setMapping(ui->pushButtonColor115, 114);
  colorMapper->setMapping(ui->pushButtonColor116, 115);
  colorMapper->setMapping(ui->pushButtonColor117, 116);
  colorMapper->setMapping(ui->pushButtonColor118, 117);
  colorMapper->setMapping(ui->pushButtonColor119, 118);
  colorMapper->setMapping(ui->pushButtonColor120, 119);
  colorMapper->setMapping(ui->pushButtonColor121, 120);
  colorMapper->setMapping(ui->pushButtonColor122, 121);
  colorMapper->setMapping(ui->pushButtonColor123, 122);
  colorMapper->setMapping(ui->pushButtonColor124, 123);
  colorMapper->setMapping(ui->pushButtonColor125, 124);
  colorMapper->setMapping(ui->pushButtonColor126, 125);
  colorMapper->setMapping(ui->pushButtonColor127, 126);
  colorMapper->setMapping(ui->pushButtonColor128, 127);
  colorMapper->setMapping(ui->pushButtonColor129, 128);
  colorMapper->setMapping(ui->pushButtonColor130, 129);
  colorMapper->setMapping(ui->pushButtonColor131, 130);
  colorMapper->setMapping(ui->pushButtonColor132, 131);
  colorMapper->setMapping(ui->pushButtonColor133, 132);
  colorMapper->setMapping(ui->pushButtonColor134, 133);
  colorMapper->setMapping(ui->pushButtonColor135, 134);
  colorMapper->setMapping(ui->pushButtonColor136, 135);
  colorMapper->setMapping(ui->pushButtonColor137, 136);
  colorMapper->setMapping(ui->pushButtonColor138, 137);
  colorMapper->setMapping(ui->pushButtonColor139, 138);
  colorMapper->setMapping(ui->pushButtonColor140, 139);
  colorMapper->setMapping(ui->pushButtonColor141, 140);
  colorMapper->setMapping(ui->pushButtonColor142, 141);
  colorMapper->setMapping(ui->pushButtonColor143, 142);
  colorMapper->setMapping(ui->pushButtonColor144, 143);
  colorMapper->setMapping(ui->pushButtonColor145, 144);
  colorMapper->setMapping(ui->pushButtonColor146, 145);
  colorMapper->setMapping(ui->pushButtonColor147, 146);
  colorMapper->setMapping(ui->pushButtonColor148, 147);
  colorMapper->setMapping(ui->pushButtonColor149, 148);
  colorMapper->setMapping(ui->pushButtonColor150, 149);
  colorMapper->setMapping(ui->pushButtonColor151, 150);
  colorMapper->setMapping(ui->pushButtonColor152, 151);
  colorMapper->setMapping(ui->pushButtonColor153, 152);
  colorMapper->setMapping(ui->pushButtonColor154, 153);
  colorMapper->setMapping(ui->pushButtonColor155, 154);
  colorMapper->setMapping(ui->pushButtonColor156, 155);
  colorMapper->setMapping(ui->pushButtonColor157, 156);
  colorMapper->setMapping(ui->pushButtonColor158, 157);
  colorMapper->setMapping(ui->pushButtonColor159, 158);
  colorMapper->setMapping(ui->pushButtonColor160, 159);
  colorMapper->setMapping(ui->pushButtonColor161, 160);
  colorMapper->setMapping(ui->pushButtonColor162, 161);
  colorMapper->setMapping(ui->pushButtonColor163, 162);
  colorMapper->setMapping(ui->pushButtonColor164, 163);
  colorMapper->setMapping(ui->pushButtonColor165, 164);
  colorMapper->setMapping(ui->pushButtonColor166, 165);
  colorMapper->setMapping(ui->pushButtonColor167, 166);
  colorMapper->setMapping(ui->pushButtonColor168, 167);
  colorMapper->setMapping(ui->pushButtonColor169, 168);
  colorMapper->setMapping(ui->pushButtonColor170, 169);
  colorMapper->setMapping(ui->pushButtonColor171, 170);
  colorMapper->setMapping(ui->pushButtonColor172, 171);
  colorMapper->setMapping(ui->pushButtonColor173, 172);
  colorMapper->setMapping(ui->pushButtonColor174, 173);
  colorMapper->setMapping(ui->pushButtonColor175, 174);
  colorMapper->setMapping(ui->pushButtonColor176, 175);
  colorMapper->setMapping(ui->pushButtonColor177, 176);
  colorMapper->setMapping(ui->pushButtonColor178, 177);
  colorMapper->setMapping(ui->pushButtonColor179, 178);
  colorMapper->setMapping(ui->pushButtonColor180, 179);
  colorMapper->setMapping(ui->pushButtonColor181, 180);
  colorMapper->setMapping(ui->pushButtonColor182, 181);
  colorMapper->setMapping(ui->pushButtonColor183, 182);
  colorMapper->setMapping(ui->pushButtonColor184, 183);
  colorMapper->setMapping(ui->pushButtonColor185, 184);
  colorMapper->setMapping(ui->pushButtonColor186, 185);
  colorMapper->setMapping(ui->pushButtonColor187, 186);
  colorMapper->setMapping(ui->pushButtonColor188, 187);
  colorMapper->setMapping(ui->pushButtonColor189, 188);
  colorMapper->setMapping(ui->pushButtonColor190, 189);
  colorMapper->setMapping(ui->pushButtonColor191, 190);
  colorMapper->setMapping(ui->pushButtonColor192, 191);
  colorMapper->setMapping(ui->pushButtonColor193, 192);
  colorMapper->setMapping(ui->pushButtonColor194, 193);
  colorMapper->setMapping(ui->pushButtonColor195, 194);
  colorMapper->setMapping(ui->pushButtonColor196, 195);
  colorMapper->setMapping(ui->pushButtonColor197, 196);
  colorMapper->setMapping(ui->pushButtonColor198, 197);
  colorMapper->setMapping(ui->pushButtonColor199, 198);
  colorMapper->setMapping(ui->pushButtonColor200, 199);

  colorMapper->setMapping(ui->pushButtonColor201, 200);
  colorMapper->setMapping(ui->pushButtonColor202, 201);
  colorMapper->setMapping(ui->pushButtonColor203, 202);
  colorMapper->setMapping(ui->pushButtonColor204, 203);
  colorMapper->setMapping(ui->pushButtonColor205, 204);
  colorMapper->setMapping(ui->pushButtonColor206, 205);
  colorMapper->setMapping(ui->pushButtonColor207, 206);
  colorMapper->setMapping(ui->pushButtonColor208, 207);
  colorMapper->setMapping(ui->pushButtonColor209, 208);
  colorMapper->setMapping(ui->pushButtonColor210, 209);
  colorMapper->setMapping(ui->pushButtonColor211, 210);
  colorMapper->setMapping(ui->pushButtonColor212, 211);
  colorMapper->setMapping(ui->pushButtonColor213, 212);
  colorMapper->setMapping(ui->pushButtonColor214, 213);
  colorMapper->setMapping(ui->pushButtonColor215, 214);
  colorMapper->setMapping(ui->pushButtonColor216, 215);
  colorMapper->setMapping(ui->pushButtonColor217, 216);
  colorMapper->setMapping(ui->pushButtonColor218, 217);
  colorMapper->setMapping(ui->pushButtonColor219, 218);
  colorMapper->setMapping(ui->pushButtonColor220, 219);
  colorMapper->setMapping(ui->pushButtonColor221, 220);
  colorMapper->setMapping(ui->pushButtonColor222, 221);
  colorMapper->setMapping(ui->pushButtonColor223, 222);
  colorMapper->setMapping(ui->pushButtonColor224, 223);
  colorMapper->setMapping(ui->pushButtonColor225, 224);
  colorMapper->setMapping(ui->pushButtonColor226, 225);
  colorMapper->setMapping(ui->pushButtonColor227, 226);
  colorMapper->setMapping(ui->pushButtonColor228, 227);
  colorMapper->setMapping(ui->pushButtonColor229, 228);
  colorMapper->setMapping(ui->pushButtonColor230, 229);
  colorMapper->setMapping(ui->pushButtonColor231, 230);
  colorMapper->setMapping(ui->pushButtonColor232, 231);
  colorMapper->setMapping(ui->pushButtonColor233, 232);
  colorMapper->setMapping(ui->pushButtonColor234, 233);
  colorMapper->setMapping(ui->pushButtonColor235, 234);
  colorMapper->setMapping(ui->pushButtonColor236, 235);
  colorMapper->setMapping(ui->pushButtonColor237, 236);
  colorMapper->setMapping(ui->pushButtonColor238, 237);
  colorMapper->setMapping(ui->pushButtonColor239, 238);
  colorMapper->setMapping(ui->pushButtonColor240, 239);
  colorMapper->setMapping(ui->pushButtonColor241, 240);
  colorMapper->setMapping(ui->pushButtonColor242, 241);
  colorMapper->setMapping(ui->pushButtonColor243, 242);
  colorMapper->setMapping(ui->pushButtonColor244, 243);
  colorMapper->setMapping(ui->pushButtonColor245, 244);
  colorMapper->setMapping(ui->pushButtonColor246, 245);
  colorMapper->setMapping(ui->pushButtonColor247, 246);
  colorMapper->setMapping(ui->pushButtonColor248, 247);
  colorMapper->setMapping(ui->pushButtonColor249, 248);
  colorMapper->setMapping(ui->pushButtonColor250, 249);
  colorMapper->setMapping(ui->pushButtonColor251, 250);
  colorMapper->setMapping(ui->pushButtonColor252, 251);
  colorMapper->setMapping(ui->pushButtonColor253, 252);
  colorMapper->setMapping(ui->pushButtonColor254, 253);
  colorMapper->setMapping(ui->pushButtonColor255, 254);
  colorMapper->setMapping(ui->pushButtonColor256, 255);

  colorMapper->setMapping(ui->pushButtonColor1_3, 101);//new add
  colorMapper->setMapping(ui->pushButtonColor2_3, 240);
  colorMapper->setMapping(ui->pushButtonColor3_3, 0);
  colorMapper->setMapping(ui->pushButtonColor4_3, 111);
  colorMapper->setMapping(ui->pushButtonColor5_3, 253);
  colorMapper->setMapping(ui->pushButtonColor6_3, 159);
  colorMapper->setMapping(ui->pushButtonColor7_3, 39);
  colorMapper->setMapping(ui->pushButtonColor8_3, 15);
  colorMapper->setMapping(ui->pushButtonColor9_3, 96);
  colorMapper->setMapping(ui->pushButtonColor10_3, 160);

  connect(ui->pushButtonColor1_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor2_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor3_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor4_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor5_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor6_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor7_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor8_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor9_3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor10_3, SIGNAL(clicked()), colorMapper, SLOT(map()));//new add

  connect(ui->pushButtonColor1, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor2, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor3, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor4, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor5, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor6, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor7, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor8, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor9, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor10, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor11, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor12, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor13, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor14, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor15, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor16, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor17, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor18, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor19, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor20, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor21, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor22, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor23, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor24, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor25, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor26, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor27, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor28, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor29, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor30, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor31, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor32, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor33, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor34, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor35, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor36, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor37, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor38, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor39, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor40, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor41, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor42, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor43, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor44, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor45, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor46, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor47, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor48, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor49, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor50, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor51, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor52, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor53, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor54, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor55, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor56, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor57, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor58, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor59, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor60, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor61, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor62, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor63, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor64, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor65, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor66, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor67, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor68, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor69, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor70, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor71, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor72, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor73, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor74, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor75, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor76, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor77, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor78, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor79, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor80, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor81, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor82, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor83, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor84, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor85, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor86, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor87, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor88, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor89, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor90, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor91, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor92, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor93, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor94, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor95, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor96, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor97, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor98, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor99, SIGNAL(clicked()), colorMapper, SLOT(map()));

  connect(ui->pushButtonColor100, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor101, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor102, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor103, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor104, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor105, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor106, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor107, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor108, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor109, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor110, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor111, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor112, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor113, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor114, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor115, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor116, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor117, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor118, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor119, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor120, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor121, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor122, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor123, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor124, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor125, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor126, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor127, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor128, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor129, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor130, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor131, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor132, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor133, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor134, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor135, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor136, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor137, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor138, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor139, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor140, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor141, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor142, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor143, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor144, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor145, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor146, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor147, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor148, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor149, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor150, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor151, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor152, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor153, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor154, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor155, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor156, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor157, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor158, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor159, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor160, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor161, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor162, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor163, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor164, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor165, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor166, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor167, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor168, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor169, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor170, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor171, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor172, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor173, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor174, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor175, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor176, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor177, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor178, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor179, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor180, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor181, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor182, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor183, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor184, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor185, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor186, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor187, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor188, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor189, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor190, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor191, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor192, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor193, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor194, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor195, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor196, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor197, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor198, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor199, SIGNAL(clicked()), colorMapper, SLOT(map()));

  connect(ui->pushButtonColor200, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor201, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor202, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor203, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor204, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor205, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor206, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor207, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor208, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor209, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor210, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor211, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor212, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor213, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor214, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor215, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor216, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor217, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor218, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor219, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor220, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor221, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor222, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor223, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor224, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor225, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor226, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor227, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor228, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor229, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor230, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor231, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor232, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor233, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor234, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor235, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor236, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor237, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor238, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor239, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor240, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor241, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor242, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor243, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor244, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor245, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor246, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor247, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor248, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor249, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor250, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor251, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor252, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor253, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor254, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor255, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(ui->pushButtonColor256, SIGNAL(clicked()), colorMapper, SLOT(map()));
  connect(colorMapper, SIGNAL(mapped(int)), this, SLOT(changeColor(int)));

  connect(ui->pushButtonAccept, SIGNAL(clicked()), this , SLOT(Accept()));
  connect(ui->pushButtonCancel, SIGNAL(clicked()), this , SLOT(Cancel()));

  connect(ui->spinBoxAdress, SIGNAL(valueChanged(int)), ui->horizontalSliderAdress, SLOT(setValue(int)));
  connect(ui->spinBoxAdress, SIGNAL(valueChanged(int)), this, SLOT(changeAddr(int)));
  connect(ui->horizontalSliderAdress, SIGNAL(valueChanged(int)), ui->spinBoxAdress, SLOT(setValue(int)));
  ui->spinBoxAdress->installEventFilter(this);

  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), ui->verticalSliderRed, SLOT(setValue(int)));
  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), this, SLOT(changeR(int)));
  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderRed, SIGNAL(valueChanged(int)), ui->spinBoxRed, SLOT(setValue(int)));
  ui->spinBoxRed->installEventFilter(this);

  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), ui->verticalSliderGreen, SLOT(setValue(int)));
  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), this, SLOT(changeG(int)));
  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderGreen, SIGNAL(valueChanged(int)), ui->spinBoxGreen, SLOT(setValue(int)));
  ui->spinBoxGreen->installEventFilter(this);

  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), ui->verticalSliderBlue, SLOT(setValue(int)));
  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), this, SLOT(changeB(int)));
  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderBlue, SIGNAL(valueChanged(int)), ui->spinBoxBlue, SLOT(setValue(int)));
  ui->spinBoxBlue->installEventFilter(this);

  connect(ui->spinBoxIntensity, SIGNAL(valueChanged(int)), ui->verticalSliderIntensity, SLOT(setValue(int)));
  connect(ui->spinBoxIntensity, SIGNAL(valueChanged(int)), this, SLOT(changeI(int)));
  connect(ui->verticalSliderIntensity, SIGNAL(valueChanged(int)), ui->spinBoxIntensity, SLOT(setValue(int)));
  ui->spinBoxIntensity->installEventFilter(this);

  connect(ui->spinBoxDimRamp, SIGNAL(valueChanged(int)), ui->horizontalSliderDimRamp, SLOT(setValue(int)));
  connect(ui->spinBoxDimRamp, SIGNAL(valueChanged(int)), this, SLOT(changeDimRamp(int)));
  connect(ui->horizontalSliderDimRamp, SIGNAL(valueChanged(int)), ui->spinBoxDimRamp, SLOT(setValue(int)));
  ui->spinBoxDimRamp->installEventFilter(this);
#if 0
  connect(ui->pushButtonGroup, SIGNAL(clicked()), this, SLOT(setGroup()));
  connect(ui->pushButtonSingle, SIGNAL(clicked()), this, SLOT(setSingle()));

  connect(ui->pushButtonAbsolute, SIGNAL(clicked()), this, SLOT(setDimmingAbsolute()));
  connect(ui->pushButtonRelative, SIGNAL(clicked()), this, SLOT(setDimmingRelative()));

  connect(ui->pushButtonNormal, SIGNAL(clicked()), this, SLOT(setRunModeNormal()));
  connect(ui->pushButtonCrash, SIGNAL(clicked()), this, SLOT(setRunModeCrash()));
  connect(ui->pushButtonShowroom, SIGNAL(clicked()), this, SLOT(setRunModeShowroom()));
  connect(ui->pushButtonActuator, SIGNAL(clicked()), this, SLOT(setRunModeActuator()));

  connect(ui->pushButtonInvalid, SIGNAL(clicked()), this, SLOT(setInvalid()));
  connect(ui->pushButtonValid, SIGNAL(clicked()), this, SLOT(setValid()));
#endif
  connect(ui->pushButtonSyncMode, SIGNAL(clicked()), this, SLOT(syncModeChanged()));

  ui->pushButtonSyncMode->setText("Sync Signal\nImmediately");
  ui->pushButtonSyncMode->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonAccept->setStyleSheet(PushButtonDisabledQSS);

  connect(ui->pushButtoncheck_mode, SIGNAL(clicked()), this, SLOT(CheckModeChanged()));//new add
  ui->pushButtoncheck_mode->setStyleSheet(PushButtonEnabledQSS);

  connect(ui->pushButton10color_mode, SIGNAL(clicked()), this, SLOT(TenColorModeChanged()));//new add
  ui->pushButton10color_mode->setStyleSheet(PushButtonEnabledQSS);

  this->init();
  this->hide();

  keys = new KeyBoard(this);
  keys->hide();
}

void BCMMasterFrame::init()
{
  struct BCMSignal currentSig = linScheduler->getBCMSignal();
  masterSignals = currentSig;
  oldSignals = currentSig;
#if 0
  //setSingle();
  ui->pushButtonGroup->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonSingle->setStyleSheet(PushButtonDisabledQSS);

  ui->pushButtonAbsolute->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonRelative->setStyleSheet(PushButtonDisabledQSS);

  ui->pushButtonValid->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonInvalid->setStyleSheet(PushButtonDisabledQSS);

  ui->pushButtonNormal->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonCrash->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonShowroom->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonActuator->setStyleSheet(PushButtonDisabledQSS);
//  this->setDimmingAbsolute();
//  setValid();
//  setRunModeNormal();
//  setDimmingAbsolute();
#endif
#if 0
  if (masterSignals.usingGroupAdress == true)
    setGroup();
  else
    setSingle();

  if (masterSignals.dimmingType == ESlaveDimmingModeAbsolute)
    setDimmingAbsolute();
  else
    setDimmingRelative();

  if (masterSignals.validMarker == true)
    setValid();
  else
    setInvalid();

  switch (masterSignals.mode)
  {
    case ESlaveModeNormal:
      setRunModeNormal();
    break;
    case ESlaveModeCrash:
      setRunModeCrash();
    break;
    case ESlaveModeShowroom:
      setRunModeShowroom();
    break;
    case ESlaveModeActuator:
      setRunModeActuator();
    break;
  }
#endif
  ui->spinBoxAdress->setValue(masterSignals.address);
  ui->spinBoxRed->setValue(masterSignals.R);
  ui->spinBoxGreen->setValue(masterSignals.G);
  ui->spinBoxBlue->setValue(masterSignals.B);
  ui->spinBoxIntensity->setValue(masterSignals.intensity);
  ui->spinBoxDimRamp->setValue(masterSignals.dimmingRamp);
}

static uint8_t count = 0;
static uint8_t check_color = 0;//new add
void BCMMasterFrame::change_RGBW_color(void)
{

    if(this->linScheduler->least_one_online == true)
    {

        updateSianglImmediate = false;
        if(check_color % 4 == 0)//red
        {
            ui->spinBoxRed->setValue(255);
            ui->spinBoxGreen->setValue(0);
            ui->spinBoxBlue->setValue(0);

        }
        else if(check_color % 4 == 1)//green
        {
            ui->spinBoxRed->setValue(0);
            ui->spinBoxGreen->setValue(255);
            ui->spinBoxBlue->setValue(0);

        }
        else if(check_color % 4 == 2)//blue
        {
            ui->spinBoxRed->setValue(0);
            ui->spinBoxGreen->setValue(0);
            ui->spinBoxBlue->setValue(255);

        }
        else //white
        {

            ui->spinBoxBlue->setValue(67);
            ui->spinBoxGreen->setValue(164);
            ui->spinBoxRed->setValue(105);

        }
        updateSianglImmediate = true;
        if (updateSianglImmediate == true)
          updateSignals();

        count++;
        count %= 15;
        if(count % 15 == 0 && check_color < 3)
        {
            check_color++;
        }
    }
    else
    {
        count = 0;
        check_color = 0;
        ui->spinBoxRed->setValue(255);
        ui->spinBoxGreen->setValue(0);
        ui->spinBoxBlue->setValue(0);
    }
}

static const double  DoubleRedx = 0.6911;//0.69;
static const double  DoubleRedy = 0.3019;//0.31;
static const double  DoubleRedz = (1 - DoubleRedx - DoubleRedy);
static const double  DoubleRedY = 1.40;//1.4;

static const double  DoubleGreenx = 0.2059;//0.17;
static const double  DoubleGreeny = 0.6753;//0.72;
static const double  DoubleGreenz = (1 - DoubleGreenx - DoubleGreeny);
static const double  DoubleGreenY = 2.40;//2.2;

static const double  DoubleBluex = 0.1552;//0.15;
static const double  DoubleBluey = 0.0572;//0.03;
static const double  DoubleBluez = (1 - DoubleBluex - DoubleBluey);
static const double  DoubleBlueY = 0.5;

void BCMMasterFrame::drawCIE()
{
  int r,g,b;
  QString content;

  double m1 = 0, m2 = 0, m3 = 0, sum_m = 0;
  double x1m1, x2m2, x3m3, sum_xm;
  double y1m1, y2m2, y3m3, sum_ym;
  double mY1, mY2, mY3;

  r = ui->spinBoxRed->value();
  g = ui->spinBoxGreen->value();
  b = ui->spinBoxBlue->value();

  if ((r == 0) && (g == 0) && (b == 0))
  {
    colorX = 0;
    colorY = 0;
  }
  else
  {
    mY1 = DoubleRedY*(double)r;
    mY2 = DoubleGreenY*(double)g;
    mY3 = DoubleBlueY*(double)b;

    m1 = mY1 / DoubleRedy;
    m2 = mY2 / DoubleGreeny;
    m3 = mY3 / DoubleBluey;

    sum_m = m1 + m2 + m3;

    x1m1 = m1*DoubleRedx;
    x2m2 = m2*DoubleGreenx;
    x3m3 = m3*DoubleBluex;

    sum_xm = x1m1 + x2m2 + x3m3;

    y1m1 = m1*DoubleRedy;
    y2m2 = m2*DoubleGreeny;
    y3m3 = m3*DoubleBluey;

    sum_ym = y1m1 + y2m2 + y3m3;

    colorX = sum_xm / sum_m;
    colorY = sum_ym / sum_m;
  }

  content = "X: ";
  content.append(QString::number(colorX, 'f', 4));
  ui->labelX->setText(content);
  content = "Y: ";
  content.append(QString::number(colorY, 'f', 4));
  ui->labelY->setText(content);

  ui->frameCIE->repaint();
}

bool BCMMasterFrame::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == ui->frameCIE && event->type() == QEvent::Paint)
  {
    QPainter painter(ui->frameCIE);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawEllipse(18 + (int)(colorX / 0.003053) - 4, 301 - (int)(colorY/0.003053) - 4, 8, 8);
  }

  if (event->type() == QEvent::FocusIn)
  {
    if (watched->inherits("QSpinBox") == true)
      showInputKeyBoard(true);
  }

//  if (event->type() == QEvent::FocusOut)
//  {
//    if (watched->inherits("QSpinBox") == true)
//      showInputKeyBoard(false);
//  }

  return QWidget::eventFilter(watched, event);
}


void BCMMasterFrame::updateSignals()
{
  linScheduler->setBCMSignal(&masterSignals);
}
#if 0
void BCMMasterFrame::setGroup()
{
  ui->pushButtonGroup->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonSingle->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.usingGroupAdress = true;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setSingle()
{
  ui->pushButtonGroup->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonSingle->setStyleSheet(PushButtonEnabledQSS);
  //masterSignals.usingGroupAdress = false;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setDimmingAbsolute()
{
  ui->pushButtonAbsolute->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonRelative->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.dimmingType = ESlaveDimmingModeAbsolute;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setDimmingRelative()
{
  ui->pushButtonAbsolute->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonRelative->setStyleSheet(PushButtonEnabledQSS);
  //masterSignals.dimmingType = ESlaveDimmingModeRelative;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setValid()
{
  ui->pushButtonValid->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonInvalid->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.validMarker = true;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setInvalid()
{
  ui->pushButtonValid->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonInvalid->setStyleSheet(PushButtonEnabledQSS);
  //masterSignals.validMarker = false;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setRunModeNormal()
{
  ui->pushButtonNormal->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonCrash->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonShowroom->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonActuator->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.mode = ESlaveModeNormal;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setRunModeCrash()
{
  ui->pushButtonNormal->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonCrash->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonShowroom->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonActuator->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.mode = ESlaveModeCrash;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setRunModeShowroom()
{
  ui->pushButtonNormal->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonCrash->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonShowroom->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonActuator->setStyleSheet(PushButtonDisabledQSS);
  //masterSignals.mode = ESlaveModeShowroom;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::setRunModeActuator()
{
  ui->pushButtonNormal->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonCrash->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonShowroom->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonActuator->setStyleSheet(PushButtonEnabledQSS);
  //masterSignals.mode = ESlaveModeActuator;
  if (updateSianglImmediate == true)
    updateSignals();
}
#endif
void BCMMasterFrame::syncModeChanged()
{
  if (updateSianglImmediate == true)
  {
    updateSianglImmediate = false;
    ui->pushButtonSyncMode->setText("Sync Signal\nManually");
    ui->pushButtonAccept->setStyleSheet(PushButtonEnabledQSS);
  }
  else
  {
    updateSianglImmediate = true;
    ui->pushButtonSyncMode->setText("Sync Signal\nImmediately");
    ui->pushButtonAccept->setStyleSheet(PushButtonDisabledQSS);
  }
}

void BCMMasterFrame::CheckModeChanged() //new add
{
  if (product_check_is_on == true)
  {
    product_check_is_on = false;
    ui->pushButtoncheck_mode->setText("check mode\noff");
    change_color_Timer->stop();
    count = 0;
    check_color = 0;
  }
  else
  {
    product_check_is_on = true;
    ui->pushButtoncheck_mode->setText("check mode\non");
    change_color_Timer->start(100);
  }
}

void BCMMasterFrame::TenColorModeChanged() //new add
{
    if (TenColor_Mode_is_on == true)
    {
      TenColor_Mode_is_on = false;
      ui->pushButton10color_mode->setText("10 color\noff");
      ui->groupBox->show();
      ui->groupBox_2->hide();
      ui->groupBox_3->hide();
      ui->groupBox_4->hide();
      ui->groupBox_5->hide();
      ui->groupBox_6->hide();
      ui->groupBox_7->hide();

    }
    else
    {
      TenColor_Mode_is_on = true;
      ui->pushButton10color_mode->setText("10 color\non");
      ui->groupBox->hide();
      ui->groupBox_2->hide();
      ui->groupBox_3->hide();
      ui->groupBox_4->hide();
      ui->groupBox_5->hide();
      ui->groupBox_6->hide();
      ui->groupBox_7->show();
    }

}

void BCMMasterFrame::changeAddr(int value)
{
  masterSignals.address = (uint16_t)value;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::changeR(int r)
{
  masterSignals.R = (uint8_t)r;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::changeG(int g)
{
  masterSignals.G = (uint8_t)g;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::changeB(int b)
{
  masterSignals.B = (uint8_t)b;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::changeI(int i)
{
  masterSignals.intensity = (uint8_t)i;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::changeDimRamp(int dimramp)
{
  masterSignals.dimmingRamp = (uint8_t)dimramp;
  if (updateSianglImmediate == true)
    updateSignals();
}

void BCMMasterFrame::Accept()
{
  if (updateSianglImmediate == false)
    updateSignals();
}

void BCMMasterFrame::Cancel()
{
  this->hide();
}

void BCMMasterFrame::showInputKeyBoard(bool show)
{
  QWidget *widget = QApplication::focusWidget();
  if (show == true)
  {
    if (widget->y() > this->height()/2)
      keys->setGeometry(68,0, 1230, 353);
    else
      keys->setGeometry(68,this->height() - 353 - 5, 1230, 353);
    keys->setCurrentObject(widget);
    keys->show();
  }
  else
    keys->hide();
}
#if 0
static const int color[14][3] = {
  {32 ,255,200},
  {175,255,64 },
  {255,0  ,3  },
  {255,88 ,4  },
  {255,0  ,0  },
  {0  ,255,0  },
  {0  ,0  ,255},
  {255,255,255},
  {255,198,0  },
  {149,255,3  },
  {19 ,255,49 },
  {3  ,86 ,255},
  {26 ,71 ,255},
  {255,8  ,42 },
};
#endif


void BCMMasterFrame::changeColor(int i)
{
  ui->spinBoxRed->setValue(color[i][0]);
  ui->spinBoxGreen->setValue(color[i][1]);
  ui->spinBoxBlue->setValue(color[i][2]);
  if (updateSianglImmediate == true)
    updateSignals();
}
#if 0
void BCMMasterFrame::groupPauseClicked()
{
  groupPaused = !groupPaused;
  if (groupPaused)
  {
    this->ui->pushButtonPauseG->setText(tr("Resume Group"));
  }
  else
  {
    this->ui->pushButtonPauseG->setText(tr("Pause Group"));
    colorPaused = false;
    this->ui->pushButtonPauseC->setText(tr("Pause Color"));
  }

  linScheduler->colorCoordinate(colorPaused, groupPaused);
}

void BCMMasterFrame::colorPauseClicked()
{
  colorPaused = !colorPaused;
  if (colorPaused)
  {
    this->ui->pushButtonPauseC->setText(tr("Resume Color"));
  }
  else
  {
    this->ui->pushButtonPauseC->setText(tr("Pause Color"));
  }

  linScheduler->colorCoordinate(colorPaused, groupPaused);
}
#endif

void BCMMasterFrame::on_pushButtonCancel_2_clicked()
{
    index++;
    if (index > 5)
        index = 0;
    switch(index)
    {
      case 0:
        ui->groupBox->show();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 1:
        ui->groupBox->hide();
        ui->groupBox_2->show();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 2:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->show();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 3:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->show();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 4:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->show();
        ui->groupBox_6->hide();
        break;
    case 5:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->show();
        break;
    default:
        break;
    };

}

void BCMMasterFrame::on_pushButtonCancel_3_clicked()
{
    index--;
    if (index < 0)
        index = 5;
    switch(index)
    {
      case 0:
        ui->groupBox->show();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 1:
        ui->groupBox->hide();
        ui->groupBox_2->show();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 2:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->show();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 3:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->show();
        ui->groupBox_5->hide();
        ui->groupBox_6->hide();
        break;
    case 4:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->show();
        ui->groupBox_6->hide();
        break;
    case 5:
        ui->groupBox->hide();
        ui->groupBox_2->hide();
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->groupBox_5->hide();
        ui->groupBox_6->show();
        break;
    default:
        break;
    };
}
