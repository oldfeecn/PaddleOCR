#include "PPOCRWidget.h"
#include <include/args.h>

PPOCRWidget::PPOCRWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
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