#include "PPOCRWidget.h"
#include <include/args.h>

PPOCRWidget::PPOCRWidget(QWidget *parent)
	: QWidget(parent)
{
	
	ui.setupUi(this);
	setAutoFillBackground(false);
	ui.tabWidget->setAutoFillBackground(false);
	setStyleSheet("background-color:#FFFFFF");
	Init();
	ConncetSightInit();
}

PPOCRWidget::~PPOCRWidget()
{}
 void PPOCRWidget::Init()
{	
	 FLAGS_use_gpu = ui.checkBox_use_gpu->checkState();
	 FLAGS_use_tensorrt = ui.checkBox_use_tensorrt->checkState();
	 FLAGS_gpu_id = ui.spinBox_gpu_id->value();
	 FLAGS_gpu_mem = ui.lineEdit_gpu_mem->text().toInt();
	 FLAGS_cpu_threads = ui.cpu_threads->value();
	 FLAGS_enable_mkldnn = ui.checkBox_enable_mkldnn->checkState();
	 FLAGS_precision = ui.comboBox_precision->currentText().toStdString();
	 FLAGS_benchmark = ui.checkBox_benchmark->checkState();
	 FLAGS_output = ui.lineEdit_output->text().toStdString();
	 FLAGS_image_dir = ui.lineEdit_image_dir -> text().toStdString();
	 FLAGS_type = ui.type->currentText().toStdString();
	 FLAGS_det_model_dir = ui.det_model_dir->text().toStdString();
	 FLAGS_limit_type = ui.comboBox_limit_type->currentText().toStdString();
	 FLAGS_limit_side_len = ui.limit_side_len->text().toInt();
	 FLAGS_det_db_thresh = ui.det_db_thresh->text().toDouble();
	 FLAGS_det_db_box_thresh = ui.det_db_box_thresh->text().toDouble();
	 FLAGS_det_db_unclip_ratio = ui.det_db_unclip_ratio->text().toDouble();
	 FLAGS_use_dilation = ui.use_dilation->checkState();
	 FLAGS_det_db_score_mode = ui.det_db_score_mode->currentText().toStdString();
	 FLAGS_visualize = ui.visualize->checkState();
	 FLAGS_use_angle_cls = ui.use_angle_cls->checkState();
	 FLAGS_cls_model_dir = ui.cls_model_dir->text().toStdString();
	 FLAGS_cls_thresh = ui.cls_thresh->value();
	 FLAGS_cls_batch_num =ui.cls_batch_num->value();
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
	 FLAGS_merge_no_span_structure = ui.merge_no_span_structure->checkState();
	 FLAGS_table_char_dict_path = ui.table_char_dict_path->text().toStdString();
	 FLAGS_det = ui.det->checkState();
	 FLAGS_rec = ui.rec->checkState();
	 FLAGS_cls = ui.cls->checkState();
	 FLAGS_table = ui.table->checkState();
	 FLAGS_layout = ui.layout->checkState();
 }

 void PPOCRWidget::ConncetSightInit()
 {
	 connect(ui.checkBox_use_gpu, &QCheckBox::stateChanged,
		 this, [this](int textA) {
			 haveChangeFun(textA);
		 });
	 connect(ui.checkBox_use_tensorrt, &QCheckBox::stateChanged,
		 this, [this](int textA) {
			 haveChangeFun(textA);
		 });
	 connect(ui.checkBox_enable_mkldnn, &QCheckBox::stateChanged,
		 this, [this](int textA) {
			 haveChangeFun(textA);
		 });
	 connect(ui.checkBox_benchmark, &QCheckBox::stateChanged,
		 this, [this](int textA) {
			 haveChangeFun(textA);
		 });
	connect(ui.comboBox_precision, &QComboBox::textActivated,
		 this, [this](QString textA) {
			haveChangeFun(textA);
		 });
	connect(ui.spinBox_gpu_id, &QSpinBox::textChanged,
		 this, [this](QString textA) {
			haveChangeFun(textA);
		 });  
	connect(ui.cpu_threads, &QSpinBox::textChanged,
		this, [this](QString textA) {
			haveChangeFun(textA);
		});
	connect(ui.lineEdit_gpu_mem, &QLineEdit::textChanged,
		this, [this](QString textA) {
			try
			{
				haveChangeFun(textA.toInt());
			}
			catch (const std::exception&)
			{
				haveChangeFun("init text to int error");
			}
			
		});
	connect(ui.type, &QComboBox::textActivated,
		this, [this](QString textA) {
			haveChangeFun(textA);
		});
	//输出文件路径
	connect(ui.lineEdit_output, &QLineEdit::textChanged,
		this, [this](QString textA) {
			haveChangeFun("select outpue dir");
			haveChangeFun(textA);
		});	
		connect(ui.pushButton_5, &QPushButton::clicked,
		this, [this](bool checked ) {
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
		}		
		});	
		//打开图片列表
		connect(ui.lineEdit_image_dir, &QLineEdit::textChanged,
			this, [this](QString textA) {
				haveChangeFun("select outpue dir");
				haveChangeFun(textA);
			});
		connect(ui.pushButton_select_img, &QPushButton::clicked,
			this, [this](bool checked) {
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
					ui.lineEdit_image_dir->setText(directory);
				}
			});
		//det检测
		connect(ui.det, &QCheckBox::stateChanged,
			this, [this](int textA) {
				if (textA) {
					ui.type->setCurrentIndex(0);
					ui.layout->setChecked(false);
					ui.table->setChecked(false);
				}
				haveChangeFun(textA);
			});
		connect(ui.rec, &QCheckBox::stateChanged,
			this, [this](int textA) {
				if (textA) {
					ui.type->setCurrentIndex(0);
					ui.layout->setChecked(false);
					ui.table->setChecked(false);
				}
				haveChangeFun(textA);
			});
		connect(ui.cls, &QCheckBox::stateChanged,
			this, [this](int textA) {
				haveChangeFun(textA);
			});
		connect(ui.layout, &QCheckBox::stateChanged,
			this, [this](int textA) {
				
				if (textA) {
					ui.type->setCurrentIndex(1);
					ui.det->setChecked(false);
					ui.rec->setChecked(false);
				}
				
				haveChangeFun(textA);
				
			});
		connect(ui.table, &QCheckBox::stateChanged,
			this, [this](int textA) {
				if (textA ) {
					ui.type->setCurrentIndex(1);
					ui.det->setChecked(false);
					ui.rec->setChecked(false);
				}
				haveChangeFun(textA);
			});
		connect(ui.visualize, &QCheckBox::stateChanged,
			this, [this](int textA) {		
				haveChangeFun(textA);
			});
		//det Tab
		connect(ui.det_db_thresh, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_thresh value error exception");
					ui.det_db_thresh->setText("0.3");
				}
			});
		connect(ui.det_db_box_thresh, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_box_thresh value error exception");
					ui.det_db_box_thresh->setText("0.6");
				}
			});
		connect(ui.limit_side_len, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("limit_side_len value error exception");
					ui.limit_side_len->setText("960");
				}
			});
		connect(ui.det_db_unclip_ratio, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toDouble());
				}
				catch (const std::exception&)
				{
					haveChangeFun("det_db_unclip_ratio value error exception");
					ui.det_db_unclip_ratio->setText("1.5");
				}
			});
			//选择检测模型
				connect(ui.det_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA) {
				haveChangeFun("select det_model_dir dir");
				haveChangeFun(textA);
			});
		connect(ui.det_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked) {
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
				}
			});
		//rec  Tab 
		connect(ui.cls_batch_num, &QSpinBox::textChanged,
		this, [this](QString textA) {
			haveChangeFun(textA);
		});
		//rec  Tab 
		connect(ui.cls_thresh, &QSpinBox::textChanged,
		this, [this](QString textA) {
			haveChangeFun(textA);
		});
		connect(ui.cls_thresh, &QDoubleSpinBox::textChanged,
		this, [this](int textA) {
			haveChangeFun(textA);
		});
		connect(ui.use_angle_cls, &QCheckBox::stateChanged,
		this, [this](int textA) {
			haveChangeFun(textA);
		});
		//选择分类模型
		connect(ui.cls_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA) {
				haveChangeFun("select cls_model_dir dir");
				haveChangeFun(textA);
			});
		connect(ui.cls_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked) {
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
				}
			});
		//rec识别
		connect(ui.rec_img_w, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("rec_img_w value error exception");
					ui.limit_side_len->setText("960");
				}
			});
			connect(ui.rec_img_h, &QLineEdit::textChanged,
			this, [this](QString textA) {
				try
				{
					haveChangeFun(textA.toInt());
				}
				catch (const std::exception&)
				{
					haveChangeFun("rec_img_w value error exception");
					ui.limit_side_len->setText("960");
				}
			});
		connect(ui.rec_batch_num, &QSpinBox::textChanged,
		 this, [this](QString textA) {
			haveChangeFun(textA);
		 });  
		 connect(ui.rec_batch_num, &QSpinBox::textChanged,
		 this, [this](QString textA) {
			haveChangeFun(textA);
		 });  
		 		connect(ui.rec_model_dir, &QLineEdit::textChanged,
			this, [this](QString textA) {
				haveChangeFun("select cls_model_dir dir");
				haveChangeFun(textA);
			});
		connect(ui.rec_model_dir_btn, &QPushButton::clicked,
			this, [this](bool checked) {
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
				}
			});
		//选择识别文字库文件

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

