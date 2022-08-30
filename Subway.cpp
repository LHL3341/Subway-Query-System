#include "Subway.h"

Subway::Subway(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowIcon(QIcon(":/Subway/resource/icon.bmp"));
	this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	this->setStyleSheet("QMenuBar{background-color:rgb(231,240,252);}\
                         QStatusBar{background-color:rgb(165,240,255);}");
	this->setWindowTitle("北京地铁线路查询系统 —— by LHL");
	subway = new Graph;
	start = new QComboBox(this);
	end = new QComboBox(this);
	opt = new QComboBox(this);
	label = new QLabel("***  Made by LHL   2022-8-25  ***", this);

	start->move(QPoint(370, 160));
	end->move(QPoint(370, 250));
	opt->move(QPoint(690, 185));

	start->setEditable(true);
	QCompleter* pCompleter1 = new QCompleter(start->model());
	pCompleter1->setFilterMode(Qt::MatchContains);
	start->setCompleter(pCompleter1);
	start->resize(100, 30);

	end->setEditable(true);
	QCompleter* pCompleter2 = new QCompleter(end->model());
	pCompleter2->setFilterMode(Qt::MatchContains);
	end->setCompleter(pCompleter2);
	end->resize(100, 30);

	start->setStyleSheet("QComboBox {background-color: #6a8f95;border: 0px solid gray; color:#fbf8e7;font: normal normal 18px \"STXingkai\";}\
                             QComboBox:hover{background-color: #386972;}\
                             QComboBox QAbstractItemView {outline: 0px solid gray;border: 1px solid #fdfdfd;color: green;background-color: #c4d2d5;selection-background-color: lightgreen;}\
                             QComboBox QAbstractItemView::item{height:33px;color: #000000;}\
                             QComboBox QAbstractItemView::item:selected { color: #fbf8e7;background-color: #386972;}\
                             QComboBox QAbstractItemView::item:hover {color: #fbf8e7;background-color: #386972;}\
                             QComboBox::drop-down {subcontrol-origin: padding; subcontrol-position: top right;height:33px;width:15px;border-left-width: 0px;}\
                             QComboBox::drop-down:hover {background: #386972;}");
	end->setStyleSheet("QComboBox {background-color: #6a8f95;border: 0px solid gray; color:#fbf8e7;font: normal normal 18px \"STXingkai\";}\
                             QComboBox:hover{background-color: #386972;}\
                             QComboBox QAbstractItemView {outline: 0px solid gray;border: 1px solid #fdfdfd;color: green;background-color: #c4d2d5;selection-background-color: lightgreen;}\
                             QComboBox QAbstractItemView::item{height:33px;color: #000000;}\
                             QComboBox QAbstractItemView::item:selected { color: #fbf8e7;background-color: #386972;}\
                             QComboBox QAbstractItemView::item:hover {color: #fbf8e7;background-color: #386972;}\
                             QComboBox::drop-down {subcontrol-origin: padding; subcontrol-position: top right;height:33px;width:15px;border-left-width: 0px;}\
                             QComboBox::drop-down:hover {background: #386972;}");

	opt->setStyleSheet("QComboBox {background-color: #6a8f95;border: 0px solid gray; color:#fbf8e7;font: normal normal 18px \"STXingkai\";}\
                             QComboBox:hover{background-color: #386972;}\
                             QComboBox QAbstractItemView {outline: 0px solid gray;border: 1px solid #fdfdfd;color: green;background-color: #c4d2d5;selection-background-color: lightgreen;}\
                             QComboBox QAbstractItemView::item{height:33px;color: #000000;}\
                             QComboBox QAbstractItemView::item:selected { color: #fbf8e7;background-color: #386972;}\
                             QComboBox QAbstractItemView::item:hover {color: #fbf8e7;background-color: #386972;}\
                             QComboBox::drop-down {subcontrol-origin: padding; subcontrol-position: top right;height:33px;width:15px;border-left-width: 0px;}\
                             QComboBox::drop-down:hover {background: #386972;}");

	//	ui.pushButton->setStyleSheet("QpushButton{border-style:outset;\
	//background-color: rgb(255, 255, 244);\
	//border-width:2px;\
	//border-radius:15px;\
	//padding:6px;\
	//color: rgb(0, 85, 127);}");
		//ui.pushButton->setStyleSheet("QPushButton:hover{ background - color: aqua;}");
	//	ui.pushButton->setStyleSheet("QPushButton:hover{color: red; border - color: green; background - color: aqua;}\
	//QPushButton:pressed{color: green;border - color: blueviolet;background - color: black;}\
	//QPushButton:disabled{color: blue;border - color: brown;background - color: aqua;}");

	opt->addItem("路程最短");
	opt->addItem("用时最短");
	opt->addItem("最少换乘");
	Updatecombo();

	stBar = statusBar();
	this->setStatusBar(stBar);
	stBar->addPermanentWidget(label);

	ui.textBrowser->document()->setMaximumBlockCount(50);

	connect(ui.addnode, &QAction::triggered, this, &Subway::AddNode);
	connect(ui.addpath, &QAction::triggered, this, &Subway::AddAdj);
	connect(ui.pushButton, &QPushButton::clicked, this, &Subway::Query);
	connect(ui.showmap, &QAction::triggered, this, &Subway::ShowMap);
	connect(ui.delnode, &QAction::triggered, this, &Subway::DelNode);
	connect(ui.delpath, &QAction::triggered, this, &Subway::DelPath);
	connect(ui.help, &QAction::triggered, this, &Subway::Help);
	connect(ui.recover, &QAction::triggered, this, &Subway::Recover);
	connect(ui.about, &QAction::triggered, this, &Subway::AboutMe);
	connect(ui.printmap, &QAction::triggered, this, &Subway::PrintCurMap);
	connect(ui.getlinespeed, &QAction::triggered, this, &Subway::GetLineSpeed);
	connect(ui.textBrowser, SIGNAL(cursorPositionChanged()), this, SLOT(AutoScroll()));
}

Subway::~Subway()
{
	delete subway;
}

void Subway::Updatecombo()
{
	start->clear();
	end->clear();
	start->addItems(subway->GetNodes());
	end->addItems(subway->GetNodes());
}

void Subway::Check()
{
	if (subway->Check())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(NULL, "导入默认配置", "当前站点信息过少，是否导入默认数据?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (reply == QMessageBox::Yes)
		{
			if (subway->Recover())
			{
				QMessageBox::information(this, tr("提示"), tr("已成功导入"));
				stBar->showMessage("已导入默认配置", 3000);
				Updatecombo();
			}
			else
			{
				QMessageBox::critical(this, tr("提示"), tr("导入失败"));
				stBar->showMessage("获取默认配置失败，检查是否损坏！", 3000);
			}
		}
	}
}

void Subway::AddNode()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("新增站点"), tr("请输入站点名称"), QLineEdit::Normal, 0, &ok);
	if (text == "")
		return;
	if (subway->AddNode(text.toStdWString()))
	{
		Updatecombo();
		stBar->showMessage("已添加站点", 2000);
	}
	else
	{
		stBar->showMessage("添加失败,站点已存在", 2000);
	}
}

void Subway::AddAdj()
{
	QDialog dialog(this);
	QFormLayout form(&dialog);
	dialog.setWindowTitle("新增路线");
	// Value1
	QString value1 = QString("起点: ");
	QComboBox* combobox1 = new QComboBox(&dialog);
	form.addRow(value1, combobox1);
	// Value2
	QString value2 = QString("终点: ");
	QComboBox* combobox2 = new QComboBox(&dialog);
	form.addRow(value2, combobox2);
	QString value3 = QString("线路: ");
	QComboBox* combobox3 = new QComboBox(&dialog);
	form.addRow(value3, combobox3);
	QString value4 = QString("距离: ");
	QSpinBox* spinbox = new QSpinBox(&dialog);
	spinbox->setRange(0, 10000);
	form.addRow(value4, spinbox);
	combobox1->addItems(subway->GetNodes());
	combobox2->addItems(subway->GetNodes());
	combobox3->addItems(subway->GetLines());

	combobox1->setEditable(true);
	QCompleter* pCompleter3 = new QCompleter(end->model());
	pCompleter3->setFilterMode(Qt::MatchContains);
	combobox1->setCompleter(pCompleter3);
	combobox1->resize(100, 30);

	combobox2->setEditable(true);
	QCompleter* pCompleter4 = new QCompleter(end->model());
	pCompleter4->setFilterMode(Qt::MatchContains);
	combobox2->setCompleter(pCompleter4);
	combobox2->resize(100, 30);

	//QCompleter *pcomp = new QCompleter(combobox1->model(),this);
	//combobox1->setCompleter(pcomp);
	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	if (dialog.exec() == QDialog::Accepted) {
		int flag1 = 0;
		int flag2 = 0;
		QStringList list(subway->GetNodes());
		for (QString& str : list)
		{
			if (combobox1->currentText() == str)
			{
				flag1 = 1;
				break;
			}
		}
		for (QString& str : list)
		{
			if (combobox2->currentText() == str)
			{
				flag2 = 1;
				break;
			}
		}
		if (!flag1 || !flag2)
		{
			stBar->showMessage("站点不存在，请重新选择", 2000);
			return;
		}
		if (subway->AddEdge(combobox1->currentText().toStdWString(), combobox2->currentText().toStdWString(),
			spinbox->value(), combobox3->currentText().toStdWString()))
		{
			stBar->showMessage("已添加路线", 2000);
			return;
		}
		stBar->showMessage("路线已存在，请先删除", 3000);
	}
}

void Subway::DelNode()
{
	QDialog dialog(this);
	QFormLayout form(&dialog);
	dialog.setWindowTitle("删除站点");
	QString value1 = QString("请选择站点: ");
	QComboBox* combobox1 = new QComboBox(&dialog);
	form.addRow(value1, combobox1);
	combobox1->addItems(subway->GetNodes());
	combobox1->setEditable(true);
	QCompleter* pCompleter3 = new QCompleter(end->model());
	pCompleter3->setFilterMode(Qt::MatchContains);
	combobox1->setCompleter(pCompleter3);
	combobox1->resize(100, 30);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	
	if (dialog.exec() == QDialog::Accepted) {
		int flag = 0;
		QStringList list(subway->GetNodes());

		for (QString& str : list)
		{
			if (combobox1->currentText() == str)
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			stBar->showMessage("站点不存在，请重新选择", 2000);
			return;
		}
		subway->DelNode(combobox1->currentText().toStdWString());
		Updatecombo();
		stBar->showMessage("已删除站点" + combobox1->currentText(), 2000);
	}
}

void Subway::DelPath()
{
	QDialog dialog(this);
	QFormLayout form(&dialog);
	dialog.setWindowTitle("删除路线");
	QString value1 = QString("起点站: ");
	QComboBox* combobox1 = new QComboBox(&dialog);
	form.addRow(value1, combobox1);
	QString value2 = QString("终点站: ");
	QComboBox* combobox2 = new QComboBox(&dialog);
	form.addRow(value2, combobox2);

	combobox1->setEditable(true);
	QCompleter* pCompleter3 = new QCompleter(end->model());
	pCompleter3->setFilterMode(Qt::MatchContains);
	combobox1->setCompleter(pCompleter3);
	combobox1->resize(100, 30);

	combobox2->setEditable(true);
	QCompleter* pCompleter4 = new QCompleter(end->model());
	pCompleter4->setFilterMode(Qt::MatchContains);
	combobox2->setCompleter(pCompleter4);
	combobox2->resize(100, 30);

	combobox1->addItems(subway->GetNodes());
	combobox2->addItems(subway->GetNodes());

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


	
	if (dialog.exec() == QDialog::Accepted) {

		int flag1 = 0;
		int flag2 = 0;
		QStringList list(subway->GetNodes());
		for (QString& str : list)
		{
			if (combobox1->currentText() == str)
			{
				flag1 = 1;
				break;
			}
		}
		for (QString& str : list)
		{
			if (combobox2->currentText() == str)
			{
				flag2 = 1;
				break;
			}
		}
		if (!flag1 || !flag2)
		{
			stBar->showMessage("站点不存在，请重新选择", 2000);
			return;
		}
		subway->DelEdge(combobox1->currentText().toStdWString(), combobox2->currentText().toStdWString());
		stBar->showMessage("已删除: " + combobox1->currentText() + "==>" + combobox2->currentText(), 2000);
	}
}

void Subway::close(QCloseEvent* event)
{
}

void Subway::ShowMap()
{
	QMessageBox customMsgBox;
	customMsgBox.setWindowTitle(tr("北京地铁线路图"));
	//QPushButton* yesBtn = customMsgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
	QPixmap pixmap(":/Subway/resource/lines.png");
	customMsgBox.resize(1050, 800);
	pixmap = pixmap.scaled(customMsgBox.size());
	customMsgBox.setIconPixmap(pixmap);
	//customMsgBox.setScaledContents(True);
	customMsgBox.exec();//显示此消息框
	return;
}

void Subway::PrintCurMap()
{
	//Py_Initialize();
	//PyRun_SimpleString("print('Hello')");
	//Py_Finalize();
	//system("pause");
	stBar->showMessage("功能尚未开发，敬请期待");
}

void Subway::Help()
{
	QString helptxt("快捷键:\
	添加站点: Ctrl+S\n\
	添加路线: Ctrl+P\n\
	删除站点: Ctrl+D\n\
	删除路线: Ctrl+R\n\
	查看当前地图:Ctrl+M\n\
	恢复默认配置:Alt+R\n\
	查看帮助:Ctrl+H\n\n\
数据文件保存路径: \\data\n\
请不要删除\\.default下的数据，否则将无法恢复。\n\
若要以txt文件导入，请选择ANSI(即GB2312)格式。\n\n\
更多帮助请看ReadMe.txt\
");
	QMessageBox msgbox;
	msgbox.resize(500, 500);
	msgbox.about(this, "帮助", helptxt);
}

void Subway::AboutMe()
{
	QString str("欢迎使用北京地铁线路查询系统！\n\
我是LHL，欢迎大家到我的家乡赛博理塘来玩！\t \n\n\
我的QQ:    1162820139\n\n\
email:     lhl3341@bupt.edu.cn\n\n\
github:    https://github.com/LHL3341\n\n\
若有站点或线路遗漏，敬请随时联系我");
	QMessageBox msgbox;
	msgbox.resize(500, 500);
	msgbox.about(this, "关于LHL", str);
}

void Subway::Recover()
{
	if (subway->Recover())
	{
		QMessageBox::information(this, tr("提示"), tr("已成功导入"));
		stBar->showMessage("已恢复默认配置", 5000);
		Updatecombo();
	}
	else
	{
		QMessageBox::critical(this, tr("错误"), tr("导入失败"));
		stBar->showMessage("获取默认配置失败，检查是否损坏！", 5000);
	}
}

void Subway::GetLineSpeed()
{
	QString helptxt("\
1号线/八通线 : 75 km/h \n\
2号线 : 80 km/h	\n\
4号线/大兴线 : 80 km/h\n\
5号线 : 80 km/h\n\
6号线 : 100 km/h\n\
7号线 : 80 km/h\n\
8号线 : 80 km/h\n\
9号线 : 80 km/h\n\
10号线 : 80 km/h\n\
11号线 : 100 km/h\n\
13号线 : 80 km/h\n\
14号线 : 80 km/h\n\
15号线 : 100 km/h\n\
16号线 : 80 km/h\n\
17号线 : 100 km/h\n\
19号线 : 120 km/h\n\
亦庄线 : 80 km/h\n\
房山线 : 100 km/h\n\
燕房线 : 80 km/h\n\
昌平线 : 100 km/h\n\
S1线 : 100 km/h \n\
西郊线 : 70 km/h\n\
亦庄T1线 : 70 km/h\n\
大兴机场线 : 160 km/h\n\
首都机场线 : 110 km/h\n\n\
数据来源 : 百度百科\
");
	QMessageBox msgbox;
	//msgbox.resize(500, 500);
	msgbox.about(this, "线路最高时速", helptxt);
}

void Subway::AutoScroll()
{
	QTextCursor cursor = ui.textBrowser -> textCursor();
	cursor.movePosition(QTextCursor::End);
	ui.textBrowser ->setTextCursor(cursor);
}

void Subway::Query()
{
	if (subway->GetNodes().isEmpty())
	{
		stBar->showMessage("当前站点列表为空，请先添加站点", 3000);
		return;
	}
	if (start->currentText() == end->currentText())
	{
		stBar->showMessage("起点与终点相同,请重新选择", 3000);
		return;
	}
	QStringList list = subway->GetPath(start->currentText().toStdWString(), end->currentText().toStdWString(), opt->currentText());
	int size = list.size();
	QString str;
	str += start->currentText().toStdWString();
	if (list.isEmpty())
	{
		ui.textBrowser->insertPlainText("此路不通\n");
		stBar->showMessage("查询完毕", 2000);
		return;
	}
	for (int i = size - 1; i > 3; i--)
	{
		str += "==>";
		str += list[i];
	}
	str += "\n\n\t";
	str += "共" + QString::number(size - 4) + "站";
	str += "\t";
	str += list[3];
	str += "\t";
	str += list[2];
	str += "\t";
	str += list[1];
	stBar->showMessage("查询完毕", 2000);
	ui.textBrowser->insertPlainText("=================================================================================\n" +
		start->currentText() + "到" + end->currentText()
		+ opt->currentText() + "的路线是:\n\n" + list[0] + "==>" + str + "\n");
}