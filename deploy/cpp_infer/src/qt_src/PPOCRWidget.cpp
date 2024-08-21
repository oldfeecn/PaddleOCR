#include "PPOCRWidget.h"

PPOCRWidget::PPOCRWidget(QWidget *parent)
	: QWidget(parent),
	  m_timer(new QTimer(this)) // 初始化 QTimer 对象
{

	ui.setupUi(this);
	setAutoFillBackground(false);
	ui.tabWidget->setAutoFillBackground(false);
	setStyleSheet("background-color:#FFFFFF");
	
	ConncetSightInit();
	recImgsBtnConnectFun();
	Init();
}

void PPOCRWidget::recImgsBtnConnectFun()
{

	connect(ui.recImgsBtn, &QPushButton::clicked,
			this, [this]()
			{ 				
				m_timer->start(); // 启动定时器
				qDebug() << "Timer started"; });
	connect(ui.stopRecImgsBtn, &QPushButton::clicked,
			this, [this]()
			{ 				
				m_timer->stop(); // 停止定时器
   				 qDebug() << "Timer stopped"; });
	// 设置定时器间隔，假设为 1000 毫秒（1 秒）
	m_timer->setInterval(1000);

	// 连接定时器的 timeout 信号到一个 Lambda 表达式
	connect(m_timer, &QTimer::timeout, this, [this]()
			{
				// 定时器超时后要执行的代码
				 startTimer(); });
}
void PPOCRWidget::startTimer()
{

	QList<QMap<QString, QVariant>> valuesList;
	bool success = SqlToolPool::selectData(QString("SELECT id, pathName FROM dnf_jietu WHERE recstatus = 0 ORDER BY id LIMIT 10;"), valuesList);
	if (success && !valuesList.isEmpty())
	{

		for (const QMap<QString, QVariant> &row : valuesList)
		{
			ui.logTextEdit->clear();
			auto start = std::chrono::system_clock::now();
			int id = row.value("id").toInt();
			QString pathName = row.value("pathName").toString();
			QString srcpathName = QString("%1%2").arg("D:\\img\\").arg(pathName);
			FLAGS_image_dir = srcpathName.toStdString();
			std::vector<cv::String> cv_all_img_names;
			cv::glob(FLAGS_image_dir, cv_all_img_names);
			ui.logTextEdit->appendText(srcpathName);
			ui.logTextEdit->appendText(QString("total images num: %1").arg(cv_all_img_names.size()));
			if (cv_all_img_names.size() == 0)
			{
				if (SqlToolPool::updateData(QString("update dnf_jietu set recstatus =%1 where id = %2;").arg(1).arg(id)))
				{
					ui.logTextEdit->appendPlainText("update no img file");
				}
			}
			else
			{
				Myocrs(cv_all_img_names, id);
			}
			auto end = std::chrono::system_clock::now();
			auto duration =
				std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			double time = double(duration.count()) *
						  std::chrono::microseconds::period::num /
						  std::chrono::microseconds::period::den;
			ui.logTextEdit->appendPlainText(QString("共耗时： %1 ").arg(time));
		}
	}
	else
	{
		ui.logTextEdit->appendText("No records found or query failed");
	}
}
void PPOCRWidget::Myocrs(std::vector<cv::String> &cv_all_img_names, int id)
{
	if (!ocr)
	{
		ocr = new PPOCR();
	};
	if (FLAGS_benchmark)
	{
		ocr->reset_timer();
	}
	std::vector<cv::Mat> img_list;
	std::vector<cv::String> img_names;
	for (int i = 0; i < cv_all_img_names.size(); ++i)
	{
		cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
		if (!img.data)
		{
			std::cerr << "[ERROR] image read failed! image path: "
					  << cv_all_img_names[i] << std::endl;
			continue;
		}
		img_list.push_back(img);
		img_names.push_back(cv_all_img_names[i]);
	}

	std::vector<std::vector<OCRPredictResult>> ocr_results =
		ocr->ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);

	for (int i = 0; i < img_names.size(); ++i)
	{
		std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
		Utility::print_result(ocr_results[i]);
		Myprint_results(ocr_results[i], id);
		if (FLAGS_visualize && FLAGS_det)
		{
			std::string file_name = Utility::basename(img_names[i]);
			cv::Mat srcimg = img_list[i];
			Utility::VisualizeBboxes(srcimg, ocr_results[i],
									 FLAGS_output + "/" + file_name);
		}
	}
	if (FLAGS_benchmark)
	{
		ocr->benchmark_log(cv_all_img_names.size());
	}
}

void PPOCRWidget::Myocr(std::vector<cv::String> &cv_all_img_names)
{
	if (!ocr)
	{
		ocr = new PPOCR();
	};
	if (FLAGS_benchmark)
	{
		ocr->reset_timer();
	}
	std::vector<cv::Mat> img_list;
	std::vector<cv::String> img_names;
	for (int i = 0; i < cv_all_img_names.size(); ++i)
	{
		cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
		if (!img.data)
		{
			std::cerr << "[ERROR] image read failed! image path: "
					  << cv_all_img_names[i] << std::endl;
			continue;
		}
		img_list.push_back(img);
		img_names.push_back(cv_all_img_names[i]);
	}

	std::vector<std::vector<OCRPredictResult>> ocr_results =
		ocr->ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);

	for (int i = 0; i < img_names.size(); ++i)
	{
		std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
		Utility::print_result(ocr_results[i]);
		Myprint_result(ocr_results[i]);
		if (FLAGS_visualize && FLAGS_det)
		{
			std::string file_name = Utility::basename(img_names[i]);
			cv::Mat srcimg = img_list[i];
			Utility::VisualizeBboxes(srcimg, ocr_results[i],
									 FLAGS_output + "/" + file_name);
		}
	}
	if (FLAGS_benchmark)
	{
		ocr->benchmark_log(cv_all_img_names.size());
	}
}
void PPOCRWidget::Myprint_result(const std::vector<OCRPredictResult> &ocr_result)
{

	QString detText;
	ui.logTextEdit->appendText(QString("共识别到 %1 个目标：").arg(ocr_result.size()));
	for (int i = 0; i < ocr_result.size(); i++)
	{
		detText = "";
		detText.append(QString("%1 . rect( ").arg(i));
		// det
		std::vector<std::vector<int>> boxes = ocr_result[i].box;
		if (boxes.size() > 0)
		{
			// printf("%d %d %d %d  ", boxes[i][0][0], boxes[i][0][1], boxes[i][2][0], boxes[i][2][1]);     //左上 右下
			detText.append(QString("%1,%2,%3,%4 ){ ").arg(boxes[0][0]).arg(boxes[0][1]).arg(boxes[2][0]).arg(boxes[2][1]));
			// detText.append(QString("[ %1,%2 ]").arg(boxes[0][0]).arg(boxes[0][1]));
		}
		// rec
		if (ocr_result[i].score != -1.0)
		{
			detText.append(ocr_result[i].text.c_str());
			detText.append(" }");
			detText.append(QString(" (%1)").arg(ocr_result[i].score));
		}
		ui.logTextEdit->appendPlainText(detText);
		// cls
		if (ocr_result[i].cls_label != -1)
		{
			// std::cout << "cls label: " << ocr_result[i].cls_label
			//          << " cls score: " << ocr_result[i].cls_score;
		}
	}
}
void PPOCRWidget::Myprint_results(const std::vector<OCRPredictResult> &ocr_result, int id)
{

	QString detText;
	// QString sqlText = QString("insert into `dnf_recimg` (jietu_id,rect1,rect2,rect3,rect4,text,num) values (%1,%2,%3,%4,%5,'%6',%7) ;");
	ui.logTextEdit->appendText(QString("共识别到 %1 个目标：").arg(ocr_result.size()));
	for (int i = 0; i < ocr_result.size(); i++)
	{
		//QString sqlText = QString("insert into `dnf_recimg` (jietu_id,rect1,rect2,rect3,rect4,text,num) values (%1,%2,%3,%4,%5,'%6',%7) ;").arg(id);
		QMap<QString, QVariant> date;
		date["tableName"]="dnf_recimg";
		date["jietu_id"]=id;
		// qDebug()<<sqlText;
		detText = "";
		detText.append(QString("%1 . rect( ").arg(i));
		// det
		std::vector<std::vector<int>> boxes = ocr_result[i].box;
		if (boxes.size() > 0)
		{
			// printf("%d %d %d %d  ", boxes[i][0][0], boxes[i][0][1], boxes[i][2][0], boxes[i][2][1]);     //左上 右下
			detText.append(QString("%1,%2,%3,%4 ){ ").arg(boxes[0][0]).arg(boxes[0][1]).arg(boxes[2][0]).arg(boxes[2][1]));
			date["rect1"]=boxes[0][0];
			date["rect2"]=boxes[0][1];
			date["rect3"]=boxes[2][0];
			date["rect4"]=boxes[2][1];
			
			// detText.append(QString("[ %1,%2 ]").arg(boxes[0][0]).arg(boxes[0][1]));
		}
		// rec
		if (ocr_result[i].score != -1.0)
		{
			detText.append(ocr_result[i].text.c_str());
			
			date["text"]=ocr_result[i].text.c_str();
			detText.append(" }");
			date["num"]=ocr_result[i].score;
			detText.append(QString(" (%1)").arg(ocr_result[i].score));
			//sqlText = sqlText.arg(ocr_result[i].score);
			if (SqlToolPool::insertDataWithQmap(date))
			{
				//ui.logTextEdit->appendPlainText();
			}
			else
			{
				ui.logTextEdit->appendPlainText("insert error");
			}
		}

		// cls
		if (ocr_result[i].cls_label != -1)
		{
			// std::cout << "cls label: " << ocr_result[i].cls_label
			//          << " cls score: " << ocr_result[i].cls_score;
		}
	}

	if (SqlToolPool::updateData(QString("update dnf_jietu set recstatus =%1 where id = %2;").arg(1).arg(id)))
	{
		ui.logTextEdit->appendPlainText("update succes");
	}
}
void PPOCRWidget::structure(std::vector<cv::String> &cv_all_img_names)
{
	PaddleOCR::PaddleStructure engine;

	if (FLAGS_benchmark)
	{
		engine.reset_timer();
	}

	for (int i = 0; i < cv_all_img_names.size(); i++)
	{
		std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
		cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
		if (!img.data)
		{
			std::cerr << "[ERROR] image read failed! image path: "
					  << cv_all_img_names[i] << std::endl;
			continue;
		}

		std::vector<StructurePredictResult> structure_results = engine.structure(
			img, FLAGS_layout, FLAGS_table, FLAGS_det && FLAGS_rec);

		for (int j = 0; j < structure_results.size(); j++)
		{
			std::cout << j << "\ttype: " << structure_results[j].type
					  << ", region: [";
			std::cout << structure_results[j].box[0] << ","
					  << structure_results[j].box[1] << ","
					  << structure_results[j].box[2] << ","
					  << structure_results[j].box[3] << "], score: ";
			std::cout << structure_results[j].confidence << ", res: ";

			if (structure_results[j].type == "table")
			{
				std::cout << structure_results[j].html << std::endl;
				if (structure_results[j].cell_box.size() > 0 && FLAGS_visualize)
				{
					std::string file_name = Utility::basename(cv_all_img_names[i]);

					Utility::VisualizeBboxes(img, structure_results[j],
											 FLAGS_output + "/" + std::to_string(j) +
												 "_" + file_name);
				}
			}
			else
			{
				std::cout << "count of ocr result is : "
						  << structure_results[j].text_res.size() << std::endl;
				if (structure_results[j].text_res.size() > 0)
				{
					std::cout << "********** print ocr result "
							  << "**********" << std::endl;
					// Utility::print_result(structure_results[j].text_res);
					Myprint_result(structure_results[j].text_res);
					std::cout << "********** end print ocr result "
							  << "**********" << std::endl;
				}
			}
		}
	}
	if (FLAGS_benchmark)
	{
		engine.benchmark_log(cv_all_img_names.size());
	}
}

PPOCRWidget::~PPOCRWidget()
{
	if (!ocr)
	{
		delete ocr;
	};
}
void PPOCRWidget::Init()
{
	
	try
	{
	FLAGS_use_gpu = ui.checkBox_use_gpu->isChecked();
	FLAGS_use_tensorrt = ui.checkBox_use_tensorrt->isChecked();
	FLAGS_gpu_id = ui.spinBox_gpu_id->value();
	FLAGS_gpu_mem = ui.lineEdit_gpu_mem->text().toInt();
	FLAGS_cpu_threads = ui.cpu_threads->value();
	FLAGS_enable_mkldnn = ui.checkBox_enable_mkldnn->isChecked();
	FLAGS_precision = ui.comboBox_precision->currentText().toStdString();
	FLAGS_benchmark = ui.checkBox_benchmark->isChecked();
	FLAGS_output = ui.lineEdit_output->text().toStdString();
	//FLAGS_image_dir = ui.lineEdit_image_dir->text().toStdString();
	FLAGS_type = ui.type->currentText().toStdString();
	FLAGS_det_model_dir = ui.det_model_dir->text().toStdString();
	FLAGS_limit_type = ui.comboBox_limit_type->currentText().toStdString();
	FLAGS_limit_side_len = ui.limit_side_len->text().toInt();
	FLAGS_det_db_thresh = ui.det_db_thresh->text().toDouble();
	FLAGS_det_db_box_thresh = ui.det_db_box_thresh->text().toDouble();
	FLAGS_det_db_unclip_ratio = ui.det_db_unclip_ratio->text().toDouble();
	FLAGS_use_dilation = ui.use_dilation->isChecked();
	FLAGS_det_db_score_mode = ui.det_db_score_mode->currentText().toStdString();
	FLAGS_visualize = ui.visualize->isChecked();
	FLAGS_use_angle_cls = ui.use_angle_cls->isChecked();
	FLAGS_cls_model_dir = ui.cls_model_dir->text().toStdString();
	FLAGS_cls_thresh = ui.cls_thresh->value();
	FLAGS_cls_batch_num = ui.cls_batch_num->value();
	FLAGS_rec_model_dir = ui.rec_model_dir->text().toStdString();
	FLAGS_rec_batch_num = ui.rec_batch_num->value();
	FLAGS_rec_char_dict_path = ui.rec_char_dict_path->text().toStdString();
	FLAGS_rec_img_h = ui.rec_img_h->text().toInt();
	FLAGS_rec_img_w = ui.rec_img_w->text().toInt();
	FLAGS_layout_model_dir = ui.layout_model_dir->text().toStdString();
	FLAGS_layout_dict_path = ui.layout_dict_path->text().toStdString();
	FLAGS_layout_score_threshold = ui.layout_score_threshold->value();
	FLAGS_layout_nms_threshold = ui.layout_nms_threshold->value();
	FLAGS_table_model_dir = ui.table_model_dir->text().toStdString();
	FLAGS_table_max_len = ui.table_max_len->text().toInt();
	FLAGS_table_batch_num = ui.table_batch_num->text().toInt();
	FLAGS_merge_no_span_structure = ui.merge_no_span_structure->isChecked();
	FLAGS_table_char_dict_path = ui.table_char_dict_path->text().toStdString();
	FLAGS_det = ui.det->isChecked();
	FLAGS_rec = ui.rec->isChecked();
	FLAGS_cls = ui.cls->isChecked();
	FLAGS_table = ui.table->isChecked();
	FLAGS_layout = ui.layout->isChecked();
	// 打印值
		qDebug() << "FLAGS_use_gpu:" << FLAGS_use_gpu;
		qDebug() << "FLAGS_use_tensorrt:" << FLAGS_use_tensorrt;
		qDebug() << "FLAGS_gpu_id:" << FLAGS_gpu_id;
		qDebug() << "FLAGS_gpu_mem:" << FLAGS_gpu_mem;
		qDebug() << "FLAGS_cpu_threads:" << FLAGS_cpu_threads;
		qDebug() << "FLAGS_enable_mkldnn:" << FLAGS_enable_mkldnn;
		qDebug() << "FLAGS_precision:" << QString::fromStdString(FLAGS_precision);
		qDebug() << "FLAGS_benchmark:" << FLAGS_benchmark;
		qDebug() << "FLAGS_output:" << QString::fromStdString(FLAGS_output);
		qDebug() << "FLAGS_image_dir:" << QString::fromStdString(FLAGS_image_dir);
		qDebug() << "FLAGS_type:" << QString::fromStdString(FLAGS_type);
		qDebug() << "FLAGS_det_model_dir:" << QString::fromStdString(FLAGS_det_model_dir);
		qDebug() << "FLAGS_limit_type:" << QString::fromStdString(FLAGS_limit_type);
		qDebug() << "FLAGS_limit_side_len:" << FLAGS_limit_side_len;
		qDebug() << "FLAGS_det_db_thresh:" << FLAGS_det_db_thresh;
		qDebug() << "FLAGS_det_db_box_thresh:" << FLAGS_det_db_box_thresh;
		qDebug() << "FLAGS_det_db_unclip_ratio:" << FLAGS_det_db_unclip_ratio;
		qDebug() << "FLAGS_use_dilation:" << FLAGS_use_dilation;
		qDebug() << "FLAGS_det_db_score_mode:" << QString::fromStdString(FLAGS_det_db_score_mode);
		qDebug() << "FLAGS_visualize:" << FLAGS_visualize;
		qDebug() << "FLAGS_use_angle_cls:" << FLAGS_use_angle_cls;
		qDebug() << "FLAGS_cls_model_dir:" << QString::fromStdString(FLAGS_cls_model_dir);
		qDebug() << "FLAGS_cls_thresh:" << FLAGS_cls_thresh;
		qDebug() << "FLAGS_cls_batch_num:" << FLAGS_cls_batch_num;
		qDebug() << "FLAGS_rec_model_dir:" << QString::fromStdString(FLAGS_rec_model_dir);
		qDebug() << "FLAGS_rec_batch_num:" << FLAGS_rec_batch_num;
		qDebug() << "FLAGS_rec_char_dict_path:" << QString::fromStdString(FLAGS_rec_char_dict_path);
		qDebug() << "FLAGS_rec_img_h:" << FLAGS_rec_img_h;
		qDebug() << "FLAGS_rec_img_w:" << FLAGS_rec_img_w;
		qDebug() << "FLAGS_layout_model_dir:" << QString::fromStdString(FLAGS_layout_model_dir);
		qDebug() << "FLAGS_layout_dict_path:" << QString::fromStdString(FLAGS_layout_dict_path);
		qDebug() << "FLAGS_layout_score_threshold:" << FLAGS_layout_score_threshold;
		qDebug() << "FLAGS_layout_nms_threshold:" << FLAGS_layout_nms_threshold;
		qDebug() << "FLAGS_table_model_dir:" << QString::fromStdString(FLAGS_table_model_dir);
		qDebug() << "FLAGS_table_max_len:" << FLAGS_table_max_len;
		qDebug() << "FLAGS_table_batch_num:" << FLAGS_table_batch_num;
		qDebug() << "FLAGS_merge_no_span_structure:" << FLAGS_merge_no_span_structure;
		qDebug() << "FLAGS_table_char_dict_path:" << QString::fromStdString(FLAGS_table_char_dict_path);
		qDebug() << "FLAGS_det:" << FLAGS_det;
		qDebug() << "FLAGS_rec:" << FLAGS_rec;
		qDebug() << "FLAGS_cls:" << FLAGS_cls;
		qDebug() << "FLAGS_table:" << FLAGS_table;
		qDebug() << "FLAGS_layout:" << FLAGS_layout;
	if(ocr){
		ocr->reInit();
	}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	
}

void PPOCRWidget::ConncetSightInit()
{
	connect(ui.checkBox_use_gpu, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	connect(ui.checkBox_use_tensorrt, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	connect(ui.checkBox_enable_mkldnn, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	connect(ui.checkBox_benchmark, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	connect(ui.comboBox_precision, &QComboBox::textActivated,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	connect(ui.spinBox_gpu_id, &QSpinBox::textChanged,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	connect(ui.cpu_threads, &QSpinBox::textChanged,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	connect(ui.lineEdit_gpu_mem, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception &)
				{
					haveChangeFun("init text to int error");
				} });
	connect(ui.type, &QComboBox::textActivated,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	// 输出文件路径
	connect(ui.lineEdit_output, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
			haveChangeFun("select outpue dir");
			haveChangeFun(textA); });
	connect(ui.pushButton_5, &QPushButton::clicked,
			this, [this](bool checked)
			{
			QString directory = QFileDialog::getExistingDirectory(
			this, 
			tr("Open Directory"), 
			ui.lineEdit_output->text(), 
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
		);
		if (!directory.isEmpty()) {
		// 用户选择了一个目录，directory 变量包含了目录的路径
		// 在这里处理选择的目录
			//haveChangeFun(directory);
			ui.lineEdit_output->setText(directory);
		} });
	// 打开图片列表
	connect(ui.lineEdit_image_dir, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select outpue dir");
				haveChangeFun(textA); });
	connect(ui.pushButton_select_img, &QPushButton::clicked,
			this, [this](bool checked)
			{
				QString directory = QFileDialog::getExistingDirectory(
					this,
					tr("Open Directory"),
					ui.lineEdit_image_dir->text(),
					QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
				);
				if (!directory.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					FLAGS_image_dir=directory.toStdString();
					test1();
					ui.lineEdit_image_dir->setText(directory);

				} });
	connect(ui.oneimg_lineEdit, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select oneimg_lineEdit dir");
				haveChangeFun(textA); });
	// oneImg
	connect(ui.oneimg_btn, &QPushButton::clicked,
			this, [this](bool checked)
			{
				// ... 在你的函数中使用以下代码
				QString fileName = QFileDialog::getOpenFileName(
					this, // 父窗口指针，可以是任何QWidget的子类的对象指针
					"Open File", // 标题
					ui.oneimg_lineEdit->text(), // 起始目录
					"Images (*.png *.xpm *.jpg *.bmp);" // 文件过滤器
					//All Files (*.*);;;Images (*.png *.xpm *.jpg);;Documents (*.doc *.odt)
				);
				if (!fileName.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					FLAGS_image_dir=fileName.toStdString();
					test1();
					ui.oneimg_lineEdit->setText(fileName);
					
					
				} });
	// det检测
	connect(ui.det, &QCheckBox::stateChanged,
			this, [this](int textA)
			{
				if (textA) {
					ui.type->setCurrentIndex(0);
					ui.layout->setChecked(false);
					ui.table->setChecked(false);
				}
				haveChangeFun(textA); });
	connect(ui.rec, &QCheckBox::stateChanged,
			this, [this](int textA)
			{
				if (textA) {
					ui.type->setCurrentIndex(0);
					ui.layout->setChecked(false);
					ui.table->setChecked(false);
				}
				haveChangeFun(textA); });
	connect(ui.cls, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	connect(ui.layout, &QCheckBox::stateChanged,
			this, [this](int textA)
			{
				if (textA)
				{
					ui.type->setCurrentIndex(1);
					ui.det->setChecked(false);
					ui.rec->setChecked(false);
				}

				haveChangeFun(textA); });
	connect(ui.table, &QCheckBox::stateChanged,
			this, [this](int textA)
			{
				if (textA ) {
					ui.type->setCurrentIndex(1);
					ui.det->setChecked(false);
					ui.rec->setChecked(false);
				}
				haveChangeFun(textA); });
	connect(ui.visualize, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); 
				FLAGS_visualize=true;
			});
	// det Tab
	connect(ui.det_db_thresh, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_thresh value error exception");
					ui.det_db_thresh->setText("0.3");
				} });
	connect(ui.det_db_box_thresh, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_box_thresh value error exception");
					ui.det_db_box_thresh->setText("0.6");
				} });
	connect(ui.limit_side_len, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("limit_side_len value error exception");
					ui.limit_side_len->setText("960");
				} });
	connect(ui.det_db_unclip_ratio, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_unclip_ratio value error exception");
					ui.det_db_unclip_ratio->setText("1.5");
				} });
	// 选择检测模型
	connect(ui.det_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select det_model_dir dir");
				haveChangeFun(textA); });
	connect(ui.det_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked)
			{
				QString directory = QFileDialog::getExistingDirectory(
					this,
					tr("Open Directory"),
					ui.det_model_dir->text(),
					QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
				);
				if (!directory.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					ui.det_model_dir->setText(directory);
				} });
	// rec  Tab
	connect(ui.cls_batch_num, &QSpinBox::textChanged,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	// rec  Tab
	/*connect(ui.cls_thresh, &QDoubleSpinBox::textChanged,
	this, [this](QString textA) {
		haveChangeFun(textA);
	});*/
	connect(ui.cls_thresh, &QDoubleSpinBox::textChanged,
			this, [this](QString textA)
			{ haveChangeFun(textA); });
	connect(ui.use_angle_cls, &QCheckBox::stateChanged,
			this, [this](int textA)
			{ haveChangeFun(textA); });
	// 选择分类模型
	connect(ui.cls_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select cls_model_dir dir");
				haveChangeFun(textA); });
	connect(ui.cls_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked)
			{
				QString directory = QFileDialog::getExistingDirectory(
					this,
					tr("Open Directory"),
					ui.cls_model_dir->text(),
					QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
				);
				if (!directory.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					ui.cls_model_dir->setText(directory);
				} });
	// rec识别
	connect(ui.rec_img_w, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("rec_img_w value error exception");
					ui.limit_side_len->setText("960");
				} });
	connect(ui.rec_img_h, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("rec_img_w value error exception");
					ui.limit_side_len->setText("960");
				} });
	connect(ui.rec_batch_num, &QSpinBox::textChanged,
			this, [this](QString textA)
			{ haveChangeFun(textA); });

	connect(ui.rec_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select cls_model_dir dir");
				haveChangeFun(textA); });
	connect(ui.rec_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked)
			{
				QString directory = QFileDialog::getExistingDirectory(
					this,
					tr("Open Directory"),
					ui.rec_model_dir->text(),
					QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
				);
				if (!directory.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					ui.rec_model_dir->setText(directory);
				} });
	// 选择识别文字库文件
	connect(ui.rec_char_dict_path, &QLineEdit::textChanged,
			this, [this](QString textA)
			{
				haveChangeFun("select rec_char_dict_path file");
				haveChangeFun(textA); });

	connect(ui.rec_char_dict_path_btn, &QPushButton::clicked,
			this, [this](bool checked)
			{			
				// ... 在你的函数中使用以下代码
				QString fileName = QFileDialog::getOpenFileName(
					this, // 父窗口指针，可以是任何QWidget的子类的对象指针
					"Open File", // 标题
					ui.rec_char_dict_path->text(), // 起始目录
					"Text Files (*.txt);" // 文件过滤器
					//All Files (*.*);;;Images (*.png *.xpm *.jpg);;Documents (*.doc *.odt)
				);
				if (!fileName.isEmpty()) {
					// 用户选择了一个目录，directory 变量包含了目录的路径
					// 在这里处理选择的目录
						//haveChangeFun(directory);
					ui.rec_model_dir->setText(fileName);
				} });
}
void PPOCRWidget::check_params()
{
	if (FLAGS_det)
	{
		if (FLAGS_det_model_dir.empty() || FLAGS_image_dir.empty())
		{
			std::cout << "Usage[det]: ./ppocr "
						 "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
					  << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
			exit(1);
		}
	}
	if (FLAGS_rec)
	{
		std::cout
			<< "In PP-OCRv3, rec_image_shape parameter defaults to '3, 48, 320',"
			   "if you are using recognition model with PP-OCRv2 or an older "
			   "version, "
			   "please set --rec_image_shape='3,32,320"
			<< std::endl;
		if (FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty())
		{
			std::cout << "Usage[rec]: ./ppocr "
						 "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
					  << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
			exit(1);
		}
	}
	if (FLAGS_cls && FLAGS_use_angle_cls)
	{
		if (FLAGS_cls_model_dir.empty() || FLAGS_image_dir.empty())
		{
			std::cout << "Usage[cls]: ./ppocr "
					  << "--cls_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
					  << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
			exit(1);
		}
	}
	if (FLAGS_table)
	{
		if (FLAGS_table_model_dir.empty() || FLAGS_det_model_dir.empty() ||
			FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty())
		{
			std::cout << "Usage[table]: ./ppocr "
					  << "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
					  << "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
					  << "--table_model_dir=/PATH/TO/TABLE_INFERENCE_MODEL/ "
					  << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
			exit(1);
		}
	}
	if (FLAGS_layout)
	{
		if (FLAGS_layout_model_dir.empty() || FLAGS_image_dir.empty())
		{
			std::cout << "Usage[layout]: ./ppocr "
					  << "--layout_model_dir=/PATH/TO/LAYOUT_INFERENCE_MODEL/ "
					  << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
			exit(1);
		}
	}
	if (FLAGS_precision != "fp32" && FLAGS_precision != "fp16" &&
		FLAGS_precision != "int8")
	{
		std::cout << "precison should be 'fp32'(default), 'fp16' or 'int8'. "
				  << std::endl;
		exit(1);
	}
}
void PPOCRWidget::test1()
{
	ui.logTextEdit->clear();
	
	if (haveChange)
	{
		 try
		 {
			Init();
			haveChange=false;
		 }
		 catch(const std::exception& e)
		 {
			qDebug()<<e.what();
		 }
		 
	}
	
	auto start = std::chrono::system_clock::now();
	check_params();
	
	if (!Utility::PathExists(FLAGS_image_dir))
	{
		std::cerr << "[ERROR] image path not exist! image_dir: " << FLAGS_image_dir
				  << std::endl;
		exit(1);
	}
	std::vector<cv::String> cv_all_img_names;
	cv::glob(FLAGS_image_dir, cv_all_img_names);
	std::cout << "total images num: " << cv_all_img_names.size() << std::endl;

	if (!Utility::PathExists(FLAGS_output))
	{
		Utility::CreateDir(FLAGS_output);
	}
	if (FLAGS_type == "ocr")
	{
		Myocr(cv_all_img_names);
	}
	else if (FLAGS_type == "structure")
	{
		structure(cv_all_img_names);
	}
	else
	{
		std::cout << "only value in ['ocr','structure'] is supported" << std::endl;
	}
	auto end = std::chrono::system_clock::now();
	auto duration =
		std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double time = double(duration.count()) *
				  std::chrono::microseconds::period::num /
				  std::chrono::microseconds::period::den;
	ui.logTextEdit->appendPlainText(QString("共耗时： %1 ").arg(time));
}
void PPOCRWidget::haveChangeFun(QString value)
{
	haveChange = true;
	ui.logTextEdit->appendText(QString("%1").arg(value));
}

void PPOCRWidget::haveChangeFun(int value)
{
	haveChange = true;
	ui.logTextEdit->appendText(QString("%1").arg(value));
}

void PPOCRWidget::haveChangeFun(double value)
{
	haveChange = true;
	ui.logTextEdit->appendText(QString("%1").arg(value));
}
