#pragma once

#include <QWidget>
#include "ui_PPOCRWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <include/args.h>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <include/args.h>
#include <include/paddleocr.h>
#include <include/paddlestructure.h>
#include <include/utility.h>
 using namespace PaddleOCR;

class PPOCRWidget : public QWidget
{
	Q_OBJECT
   
public:
	PPOCRWidget(QWidget *parent = nullptr);
    void Myocr(std::vector<cv::String> &cv_all_img_names);
    void Myprint_result(const std::vector<OCRPredictResult>& ocr_result);
    void structure(std::vector<cv::String> &cv_all_img_names);
    ~PPOCRWidget();

    void Init();
	void ConncetSightInit();
    void check_params();
    void test1();
    bool haveChange = false;
    void haveChangeFun(QString value);
	void haveChangeFun(int value);
	void haveChangeFun(double value);
    PPOCR* ocr;

private:
	Ui::PPOCRWidgetClass ui;
};
