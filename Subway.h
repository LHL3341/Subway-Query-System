#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Subway.h"
#include"Graph.h"

class Subway : public QMainWindow
{
	Q_OBJECT

public:
	Subway(QWidget* parent = nullptr);
	~Subway();
	void Check();

private:
	Graph* subway;
	QStatusBar* stBar;
	Ui::SubwayClass ui;
	QComboBox* start;
	QComboBox* end;
	QComboBox* opt;
	QLabel* label;
	QLabel* label1;
	QLabel* status;

	void Updatecombo();
private slots:						//槽函数

	void Query();
	void AddNode();
	void AddAdj();
	void DelNode();
	void DelPath();
	void close(QCloseEvent* event);
	void ShowMap();
	void PrintCurMap();
	void Help();
	void AboutMe();
	void Recover();
	void GetLineSpeed();
	void AutoScroll();
};
