#include "KeyMouseWidget.h"

KeyMouseWidget::KeyMouseWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	buttonList = this->findChildren<QPushButton*>();
	for (size_t i = 0; i < buttonList.size(); i++)
	{
		buttonList[i]->setEnabled(false);
	}
	ui.dll_button->setEnabled(true);
	InitConnect();

}

void KeyMouseWidget::InitKeyMouseThread() {
	if (keyMouseThread != nullptr) {
		return;
	}
	keyMouseThread = new QThread();
	objKeyMouseThread = new KeyAndMouseThread(NULL);
	objKeyMouseThread->moveToThread(keyMouseThread);
	connect(keyMouseThread, &QThread::finished, objKeyMouseThread, &QObject::deleteLater);
	qRegisterMetaType<QVariantMap>("QVariantMap&");
	connect(this, &KeyMouseWidget::sendStartKMThreadState,
		objKeyMouseThread, &KeyAndMouseThread::getStartKMThreadState, Qt::QueuedConnection);
	/* connect(objKeyMouseThread, &KeyAndMouseThread::sendMsg, this, &GameTool::getMessage, Qt::QueuedConnection);*/
	keyMouseThread->start();


}

KeyMouseWidget::~KeyMouseWidget()
{
	if (box_handle) {
		mSDKWrapper.M_Close(box_handle);
	}
	if (keyMouseThread != nullptr) {
		emit sendStartKMThreadState(2);
		objKeyMouseThread->deleteLater();
		keyMouseThread->quit();
		delete objKeyMouseThread;
	}

}

QString KeyMouseWidget::uncharToQstring(unsigned char* id, int len)
{
	QString temp, msg;
	int j = 0;

	while (j < len)
	{
		temp = QString("%1").arg((int)id[j], 2, 16, QLatin1Char('0'));
		msg.append(temp);
		msg.append(" ");
		j++;
	}

	return msg;
}

void KeyMouseWidget::initBox()
{
	if (box_handle != NULL) {
		for (size_t i = 0; i < buttonList.size(); i++)
		{
			buttonList[i]->setEnabled(true);
		}
		ui.dll_button->setEnabled(false);
		ui.M_Open_VidPid_btn->setEnabled(false);
		ui.M_ScanAndOpen_btn->setEnabled(false);
		ui.M_Open_btn->setEnabled(false);
		ui.log_textEdit->appendText("==============================");
		//获取设备序列号
		/*DWORD xuliehaoLen=256;
		unsigned char xuliehao=256;
		int  t1 = getDevSn(box_handle, &xuliehaoLen, &xuliehao);
		if (t1 != 0) {
			ui.log_textEdit->appendText(QStringLiteral("获取序列号失败!!"));
		}
		else {
			ui.M_GetDevSn_edit->setText(QStringLiteral("%1   长度=%2").arg(uncharToQstring(xuliehao, xuliehaoLen)).arg(int(xuliehaoLen)));
		}*/
		// 读取当前绝对移动功能的状态 返回 : 1 : 打开; 0:关闭; 其他: 错误	
		int M_GetAbsCfg_ret = mSDKWrapper.M_GetAbsCfg(box_handle);
		if (M_GetAbsCfg_ret == 1) {
			ui.M_GetAbsCfg_state_cbox->setChecked(true);
		}
		else {
			ui.M_GetAbsCfg_state_cbox->setChecked(false);
		}


	}
	else {
		ui.log_textEdit->appendText(QStringLiteral("盒子未初始化!!"));
	}
	ui.M_GetVidPid_btn->clicked();
}

void KeyMouseWidget::InitConnect() {
	connect(ui.M_Open_VidPid_btn, &QPushButton::clicked, this, [this]() {
		if (box_handle != NULL) {
			mSDKWrapper.M_Close(box_handle);
			box_handle = NULL;
		}
		bool ok;
		ui.log_textEdit->appendText(QStringLiteral("VID = %1, pid = %2").arg(ui.vid_edit->text().toInt(&ok, 16)).arg(ui.pid_edit->text().toInt(&ok, 16)));
		box_handle = mSDKWrapper.M_Open_VidPid(ui.vid_edit->text().toInt(&ok, 16), ui.pid_edit->text().toInt(&ok, 16));
		if (box_handle == NULL) {
			ui.log_textEdit->appendText(QStringLiteral("//使用mSDKWrapper.M_Open_VidPid接口可以打开指定vid pid的单头盒子或者双头盒子的主控端。必须保证电脑上只插有一个这种盒子。"));
			ui.log_textEdit->appendText(QStringLiteral("打开盒子失败!"));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("打开VID_PID!"));
			mSDKWrapper.M_MoveTo(box_handle, 100, 100);
			ui.log_textEdit->appendText(QStringLiteral("移动位置 (100,100)"));
			ui.M_Open_VidPid_btn->setEnabled(false);
			ui.M_ScanAndOpen_btn->setEnabled(false);
			ui.M_Open_btn->setEnabled(false);
			ui.M_Close_btn->setEnabled(true);
			initBox();
		}
		});
	connect(ui.M_Open_btn, &QPushButton::clicked, this, [this]() {
		QString str = ui.box_num->currentText();
		box_handle = mSDKWrapper.M_Open(str.toInt());

		if (box_handle == NULL) {
			ui.log_textEdit->appendText(QStringLiteral("mSDKWrapper.M_Open打开盒子失败!"));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("mSDKWrapper.M_Open()打开端口:%1").arg(str));
			mSDKWrapper.M_MoveTo(box_handle, 100, 100);
			ui.log_textEdit->appendText(QStringLiteral("移动位置 (100,100)"));
			ui.M_Open_VidPid_btn->setEnabled(false);
			ui.M_Open_btn->setEnabled(false);
			ui.M_ScanAndOpen_btn->setEnabled(false);
			ui.M_Close_btn->setEnabled(true);
			initBox();
		}
		});
	connect(ui.M_ScanAndOpen_btn, &QPushButton::clicked, this, [this]() {
		if (box_handle != NULL) {
			mSDKWrapper.M_Close(box_handle);
			box_handle = NULL;
		}
		box_handle = mSDKWrapper.M_ScanAndOpen();
		if (box_handle == NULL) {
			ui.log_textEdit->appendText(QStringLiteral("//使用M_ScanAndOpen接口扫描硬件列表，打开扫描到的第一个单头或者双头盒子"));
			ui.log_textEdit->appendText(QStringLiteral("打开盒子失败!"));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("扫描硬件列表M_ScanAndOpen!"));
			mSDKWrapper.M_MoveTo(box_handle, 100, 100);
			ui.log_textEdit->appendText(QStringLiteral("移动位置 (100,100)"));
			ui.M_Open_VidPid_btn->setEnabled(false);
			ui.M_ScanAndOpen_btn->setEnabled(false);
			ui.M_Open_btn->setEnabled(false);
			ui.M_Close_btn->setEnabled(true);
			initBox();
		}
		});
	connect(ui.M_Close_btn, &QPushButton::clicked, this, [this]() {
		if (box_handle != NULL) {
			mSDKWrapper.M_Close(box_handle);
		    ui.log_textEdit->appendText(QStringLiteral("关闭盒子!"));
			box_handle = NULL;
			for (size_t i = 0; i < buttonList.size(); i++)
			{
				buttonList[i]->setEnabled(false);
			}
			ui.M_Open_VidPid_btn->setEnabled(true);
			ui.M_Open_btn->setEnabled(true);
			ui.M_ScanAndOpen_btn->setEnabled(true);
		}
		else {
			//ui.log_textEdit->setText(QStringLiteral("请先打开端口!"));
		}
		});
	connect(ui.dll_button, &QPushButton::clicked, this, [this]() {
		QString dllName = ui.dll_lineEdit->text();
		int ret = mSDKWrapper.loadLibrary(dllName);//msdk.dll
		ui.log_textEdit->appendText(QStringLiteral("加载DLL返回值为%1").arg(1));
		ui.M_Open_VidPid_btn->setEnabled(true);
		ui.M_Open_btn->setEnabled(true);
		ui.M_ScanAndOpen_btn->setEnabled(true);
		ui.dll_button->setEnabled(false);
		});
	//UDP对象初始化
	STATICVar->m_receiver = new QUdpSocket(this);
	STATICVar->m_receiver->bind(45454, QUdpSocket::ShareAddress);
	connect(STATICVar->m_receiver, &QUdpSocket::readyRead,
		[this]() {
			//udp对象回调事件------程序处于接收消息状态
		  //  qDebug() << "STATICVar->msgRadioState"<<STATICVar->msgRadioState;
			QByteArray datagram; //拥于存放接收的数据报
			//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
			datagram.resize(STATICVar->m_receiver->pendingDatagramSize());
			//接收数据报，将其存放到datagram中
			STATICVar->m_receiver->readDatagram(datagram.data(), datagram.size());
			/*if (3 == STATICVar->msgRadioState) {
				ui.keyMouse_edit->appendText(datagram);
				return;
			}*/
			if (2 == STATICVar->msgRadioState && logKeyMouse == 0) {
				QString arrs = datagram;
				QStringList arr = arrs.split(':');
				if (arr[0] == "mouse") {

				}
				else if (arr[0] == "key") {
					//qDebug() << arr[1].toInt() << "," << arr[3].toInt();
					//按键抬起
					if (arr[3].toInt() == 128) {
						hookKeyStr.append(STATICVar->hookKeyToKeyStr(arr[1].toInt())).append(",");
						//qDebug() << STATICVar->hookKeyToKeyStr(arr[1].toInt());
						ui.log_textEdit->appendText(hookKeyStr);
					}
					//ui.KeyCode_edit->appendText(datagram);
				}
				// Sleep(arr[4].toInt());
			}
			else if (2 == STATICVar->msgRadioState && logKeyMouse == 1) {
				ui.log_textEdit->appendText(datagram);
			}

		});
	//写入用户数据
	connect(ui.M_SetUserData_btn, &QPushButton::clicked, this, [this]() {
		if (ui.ucp_UserData_edit->text().length() <= 256) {
			mSDKWrapper.M_SetUserData(box_handle, ui.ucp_UserData_edit->text().length(), (unsigned char*)ui.ucp_UserData_edit->text().toLatin1().data());
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("用户数据不能超过256字节!!"));
		}
		});
	//验证用户数据
	connect(ui.M_VerifyUserData_btn, &QPushButton::clicked, this, [this]() {
		if (ui.ucp_UserData_edit->text().length() <= 256) {
			mSDKWrapper.M_VerifyUserData(box_handle, ui.ucp_UserData_edit->text().length(), (unsigned char*)ui.ucp_UserData_edit->text().toLatin1().data());
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据不能超过256字节!!"));
		}
		});

	//盒子是否可修改
	connect(ui.M_ChkSupportMdy_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_ChkSupportMdy(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("%1").arg(ret));
		if (ret == 1) {
			ui.log_textEdit->appendText(QStringLiteral("盒子可修改"));
			ui.M_ChkSupportMdy_btn->setEnabled(true);
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("盒子不可修改"));
			ui.M_ChkSupportMdy_btn->setEnabled(false);
		}
		});
	//读取设备名称
	connect(ui.M_GetProdStr_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("该函数地址无效"));
		ui.M_GetProdStr_btn->setEnabled(false);
		});
	//设置设备名称
	connect(ui.M_SetProdStr_btn, &QPushButton::clicked, this, [this]() {
		QString zhukongName = ui.ucp_boxName_edit_1->text();
		QString beikongName = ui.ucp_boxName_edit_2->text();
		//设置主控
		int retzhu = mSDKWrapper.M_SetProdStr(box_handle, 0,
			zhukongName.length(), (unsigned char*)(zhukongName.toLatin1().data()));
		//设置被控
		int retbei = mSDKWrapper.M_SetProdStr(box_handle, 1,
			zhukongName.length(), (unsigned char*)(beikongName.toLatin1().data()));
		ui.log_textEdit->appendText(QStringLiteral("设置主控返回值为:%1").arg(retzhu));
		ui.log_textEdit->appendText(QStringLiteral("设置被控返回值为:%1").arg(retbei));
		if (retzhu == 0 && retbei == 0) {
			ui.log_textEdit->appendText(QStringLiteral("设置主备名称成功"));
		};
		});
	//是否支持加密功能, 返回值1=支持，0=不支持；-1，失败
	connect(ui.M_ChkSupportEnc_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_ChkSupportEnc(box_handle);
		if (ret == 1) {
			ui.log_textEdit->appendText(QStringLiteral("盒子支持加密"));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("盒子不支持加密"));
		}
		});
	//检测密码状态; 返回值: <0 错误；255=无密码; 0~16:密码剩余有效次数
	connect(ui.M_ChkPwStat_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_ChkPwStat(box_handle);
		if (ret < 0) {
			ui.log_textEdit->appendText(QStringLiteral("密码错误"));
		}
		else if (ret == 255) {
			ui.log_textEdit->appendText(QStringLiteral("无密码"));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("密码剩余次数:%1").arg(ret));
		}
		});
	//设置新密码
	connect(ui.M_SetPw_btn, &QPushButton::clicked, this, [this]() {
		QString pwd = ui.upc_pwd_edit->text();
		int ret = mSDKWrapper.M_SetPw(box_handle, pwd.length(), 16, (unsigned char*)(pwd.toLatin1().data()));
		ui.log_textEdit->appendText(QStringLiteral("设置密码返回值:%1").arg(ret));
		});
	//验证密码
	connect(ui.M_VerifyPw_btn, &QPushButton::clicked, this, [this]() {
		QString pwd = ui.upc_pwd_edit->text();
		int ret = mSDKWrapper.M_VerifyPw(box_handle, pwd.length(), (unsigned char*)(pwd.toLatin1().data()));
		ui.log_textEdit->appendText(QStringLiteral("设置密码返回值:%1").arg(ret));
		});
	//写入数据
	connect(ui.M_WrEncData_btn, &QPushButton::clicked, this, [this]() {
		QString pwd = ui.ucp_data_edit->text();
		int ret = mSDKWrapper.M_WrEncData(box_handle, pwd.length(), (unsigned char*)(pwd.toLatin1().data()));
		ui.log_textEdit->appendText(QStringLiteral("写入数据返回值:%1").arg(ret));
		});
	//读取数据
	connect(ui.M_RdEncData_btn, &QPushButton::clicked, this, [this]() {
		QString pwd = ui.ucp_data_edit->text();
		unsigned char ucp_data[1000];
		int tem = 1000;
		int ret = mSDKWrapper.M_RdEncData(box_handle, tem, ucp_data);
		ui.log_textEdit->appendText(QStringLiteral("读取数据返回值为:%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("读取数据值为:%1").arg(QString(QLatin1String((char*)ucp_data))));
		});
	//DLL内部参数恢复默认值
	connect(ui.M_InitParam_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_InitParam(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("内部参数恢复默认值返回值为:%1").arg(ret));
		});
	//DLL内部参数恢复默认值
	connect(ui.M_InitParam_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_InitParam(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("内部参数恢复默认值返回值为:%1").arg(ret));
		});
	//读取VID/PID; IdIndex=1/2/3/4 分别对应 主控端Vid, 主控端Pid, 被控端Vid, 被控端Pid
	connect(ui.M_GetVidPid_btn, &QPushButton::clicked, this, [this]() {
		int zhuVID = mSDKWrapper.M_GetVidPid(box_handle, 1);
		int zhuPID = mSDKWrapper.M_GetVidPid(box_handle, 2);
		int beiVID = mSDKWrapper.M_GetVidPid(box_handle, 3);
		int beiPID = mSDKWrapper.M_GetVidPid(box_handle, 4);
		ui.M_GetVid_edit1->setText(QString::number(zhuVID, 16).right(4));
		ui.M_GetVid_edit2->setText(QString::number(zhuPID, 16).right(4));
		ui.M_GetVid_edit3->setText(QString::number(beiVID, 16).right(4));
		ui.M_GetVid_edit4->setText(QString::number(beiPID, 16).right(4));
		ui.log_textEdit->appendText(QStringLiteral("主控端Vid:%1").arg(QString::number(zhuVID, 16).right(4)));
		ui.log_textEdit->appendText(QStringLiteral("主控端Pid:%1").arg(QString::number(zhuPID, 16).right(4)));;
		ui.log_textEdit->appendText(QStringLiteral("被控端Vid:%1").arg(QString::number(beiVID, 16).right(4)));
		ui.log_textEdit->appendText(QStringLiteral("被控端Pid:%1").arg(QString::number(beiPID, 16).right(4)));
		});

	//重置VID/PID
	connect(ui.M_ResetVidPid_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_ResetVidPid(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("重置VID/PID返回值为:%1").arg(ret));
		});
	//修改VID/PID
	connect(ui.M_SetNewVidPid_btn, &QPushButton::clicked, this, [this]() {
		QString id1 = ui.M_GetVid_edit1->text();
		QString id2 = ui.M_GetVid_edit2->text();
		QString id3 = ui.M_GetVid_edit3->text();
		QString id4 = ui.M_GetVid_edit4->text();
		bool ok;
		int ret = mSDKWrapper.M_SetNewVidPid(box_handle,
			id1.left(2).toInt(&ok, 16),
			id2.left(2).toInt(&ok, 16),
			id3.left(2).toInt(&ok, 16),
			id4.left(2).toInt(&ok, 16)
		);
		ui.log_textEdit->appendText(QStringLiteral("修改VID/PID返回值为:%1").arg(ret));
		});
	//写入用户数据
	connect(ui.M_SetUserData_btn, &QPushButton::clicked, this, [this]() {
		QString str = ui.ucp_UserData_edit->text();
		unsigned char* UserData = (unsigned char*)(str.toLatin1().data());
		int ret = mSDKWrapper.M_SetUserData(box_handle, sizeof(UserData), UserData);
		if (ret < 0) {
			ui.log_textEdit->appendText(QStringLiteral("写入用户数据=错误,返回值为:%1").arg(ret));
		}
		else if (ret == 0) {
			ui.log_textEdit->appendText(QStringLiteral("写入用户数据=成功,返回值为:%1").arg(ret));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("写入用户数据=其它,返回值为:%1").arg(ret));
		}
		});
	//验证用户数据
	connect(ui.M_VerifyPw_btn, &QPushButton::clicked, this, [this]() {
		QString str = ui.ucp_UserData_edit->text();
		unsigned char* UserData = (unsigned char*)(str.toLatin1().data());
		int ret = mSDKWrapper.M_VerifyUserData(box_handle, sizeof(UserData), UserData);
		if (ret < 0) {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=错误,返回值为:%1").arg(ret));
		}
		else if (ret == 0) {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=成功,返回值为:%1").arg(ret));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=其它,返回值为:%1").arg(ret));
		}
		});
	//读取用户数据
	connect(ui.M_VerifyUserData_btn, &QPushButton::clicked, this, [this]() {
		QString str = ui.ucp_UserData_edit->text();
		unsigned char* UserData = (unsigned char*)(str.toLatin1().data());
		int ret = mSDKWrapper.M_VerifyUserData(box_handle, sizeof(UserData), UserData);
		if (ret < 0) {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=错误,返回值为:%1").arg(ret));
		}
		else if (ret == 0) {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=成功,返回值为:%1").arg(ret));
		}
		else {
			ui.log_textEdit->appendText(QStringLiteral("验证用户数据=其它,返回值为:%1").arg(ret));
		}
		});
	//修改设备内部默认参数
	void (QComboBox:: * valuesChageString)(const QString&) = &QComboBox::currentIndexChanged;
	connect(ui.M_SetParam_cbox, valuesChageString, [this](const QString& s) {
		if (s == QString::fromLocal8Bit(("单击按键0"))) {
			ui.dll_key_min_2->setText("50");
			ui.dll_key_max_2->setText("80");
		}
		else if (s == QString::fromLocal8Bit(("多个按键2"))) {
			ui.dll_key_min_2->setText("150");
			ui.dll_key_max_2->setText("600");
		}
		else if (s == QString::fromLocal8Bit(("单击鼠标8"))) {
			ui.dll_key_min_2->setText("50");
			ui.dll_key_max_2->setText("80");
		}
		else if (s == QString::fromLocal8Bit(("多次单击击鼠标10"))) {
			ui.dll_key_min_2->setText("500");
			ui.dll_key_max_2->setText("900");
		}
		else if (s == QString::fromLocal8Bit(("双击鼠标左键12"))) {
			ui.dll_key_min_2->setText("60");
			ui.dll_key_max_2->setText("110");
		}
		else if (s == QString::fromLocal8Bit(("多次上级鼠标左键14"))) {
			ui.dll_key_min_2->setText("500");
			ui.dll_key_max_2->setText("800");
		}
		else if (s == QString::fromLocal8Bit(("鼠标移动轨迹=20"))) {
			ui.dll_key_min_2->setText("10");
			ui.dll_key_max_2->setText("20");
		}
		ui.log_textEdit->appendText(QStringLiteral("修改设备内部默认参数:%1").arg(s));

		});
	//读取用户数据
	connect(ui.M_SetParam_btn, &QPushButton::clicked, this, [this]() {
		int ret;
		QString s = ui.M_SetParam_cbox->currentText();
		if (s == QString::fromLocal8Bit(("单击按键0"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("多个按键2"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("单击鼠标8"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("多次单击击鼠标10"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("双击鼠标左键12"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("多次上级鼠标左键14"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		else if (s == QString::fromLocal8Bit(("鼠标移动轨迹=20"))) {
			ret = mSDKWrapper.M_SetParam(box_handle, 0, ui.dll_key_min_2->text().toInt(), ui.dll_key_max_2->text().toInt());
		}
		ui.log_textEdit->appendText(QStringLiteral("修改设备内部默认参数:%1").arg(ret));
		});
	//键鼠消息监控
	connect(ui.onKeyMouseInfo_btn, &QPushButton::clicked, this, [this]() {
		if (keyMouseThread == nullptr) {
			InitKeyMouseThread();
		}
		emit sendStartKMThreadState(1);
		STATICVar->msgRadioState = 2;
		ui.log_textEdit->appendText(QStringLiteral("开启键鼠消息监控"));
		logKeyMouse = 1;
		});
	//卸载键鼠消息监控
	connect(ui.noKeyMouseInfo_btn, &QPushButton::clicked, this, [this]() {
		if (keyMouseThread == nullptr) {
			InitKeyMouseThread();
		}
		emit sendStartKMThreadState(2);
		STATICVar->msgRadioState = 3;
		ui.log_textEdit->appendText(QStringLiteral("关闭键鼠消息监控"));
		logKeyMouse = 0;
		});
	//监控转化按键编辑框
	/*void (QLineEdit::* KeyCode_edit_textChanged)(const QString&) = &QLineEdit::textChanged;
	connect(ui.KeyCode_edit, &QLineEdit::editingFinished, this, [this]() {

		ui.log_textEdit->appendText(QStringLiteral("所有按键监控结束:editingFinished"));
		});*/

	void (KeyMouseQLineEdit:: * KeyCode_edit_KeyMouseQLineEditMsg)(int msg) = &KeyMouseQLineEdit::KeyMouseQLineEditMsg;
	connect(ui.KeyCode_edit, KeyCode_edit_KeyMouseQLineEditMsg, this, [this](int msg) {
		//获得焦点
		if (msg == 4) {
			if (keyMouseThread == nullptr) {
				InitKeyMouseThread();
			}
			STATICVar->msgRadioState = 2;
			emit sendStartKMThreadState(1);
			logKeyMouse = 0;
			//qDebug() << QStringLiteral("获得焦点");
		}//失去焦点
		else if (msg == 5) {
			if (keyMouseThread == nullptr) {
				InitKeyMouseThread();
			}
			emit sendStartKMThreadState(2);
			STATICVar->msgRadioState = 3;
			//qDebug() << QStringLiteral("失去焦点");
			ui.KeyCode_edit->setText(hookKeyStr);
			hookKeyStr = "";
			logKeyMouse = 1;
		}
		});
	//按下A键
	connect(ui.M_KeyDown_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		//按下a键
		int anjian = STATICVar->KeyToUsbKeyCode("a");
		int ret = mSDKWrapper.M_KeyPress(box_handle, anjian, 1);
		//读取按键状态
		int ret1 = mSDKWrapper.M_KeyState(box_handle, anjian);
		//返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
		ui.log_textEdit->appendText(QStringLiteral("返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)"));
		ui.log_textEdit->appendText(QStringLiteral("按下某键,返回值=%1").arg(ret1));

		});
	/**=============接口采用的HidKeyCode是USB键盘的KeyCode，不是windows的KeyCode==========**/
	//单击按键
	connect(ui.M_KeyPress_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		QString keyArrs = ui.KeyCode_edit->text();
		QStringList arr = keyArrs.split(',');
		for (size_t i = 0; i < arr.length(); i++)
		{
			if (arr[i] != "") {
				int anjian = STATICVar->KeyToUsbKeyCode(arr[i]);
				if (anjian != 0) {
					int ret = mSDKWrapper.M_KeyPress(box_handle, anjian, 1);
					ui.log_textEdit->appendText(QStringLiteral("按键: %1   返回值= %2").arg(anjian).arg(ret));
				}
				else {
					ui.log_textEdit->appendText(QStringLiteral("不支持该按键:%1").arg(arr[i]));
				}
			}
		}
		ui.log_textEdit->appendText(QStringLiteral("执行完毕,避免bug,抬起所有按键"));
		//避免bug,抬起所有按键;
		mSDKWrapper.M_ReleaseAllKey(box_handle);
		});

	//a键位状态
	connect(ui.M_KeyState_btn, &QPushButton::clicked, this, [this]() {
		int anjian = STATICVar->KeyToUsbKeyCode("a");
		int ret1 = mSDKWrapper.M_KeyState(box_handle, anjian);
		//返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
		ui.log_textEdit->appendText(QStringLiteral("返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)"));
		ui.log_textEdit->appendText(QStringLiteral("a键状态,返回值=%1").arg(ret1));
		});
	//抬起某键
	connect(ui.M_KeyUp_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		//按下a键
		int anjian = STATICVar->KeyToUsbKeyCode("a");
		mSDKWrapper.M_KeyUp(box_handle, anjian);
		//读取按键状态
		int ret1 = mSDKWrapper.M_KeyState(box_handle, anjian);
		ui.log_textEdit->appendText(QStringLiteral("抬起某键,返回值=%1").arg(ret1));
		ui.log_textEdit->appendText(QStringLiteral("执行完毕,避免bug,抬起所有按键"));

		});
	/**===============是windows的KeyCode===================**/
	//单击按键
	connect(ui.M_KeyPress2_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		QString keyArrs = ui.KeyCode_edit->text();
		QStringList arr = keyArrs.split(',');
		for (size_t i = 0; i < arr.length(); i++)
		{
			if (arr[i] != "") {
				int anjian = STATICVar->KeyToWinKeyCode(arr[i]);
				if (anjian != 0) {
					int ret = mSDKWrapper.M_KeyPress2(box_handle, anjian, 1);
					ui.log_textEdit->appendText(QStringLiteral("按键: %1   返回值= %2").arg(anjian).arg(ret));
				}
				else {
					ui.log_textEdit->appendText(QStringLiteral("不支持该按键:%1").arg(arr[i]));
				}
			}
		}
		ui.log_textEdit->appendText(QStringLiteral("执行完毕,避免bug,抬起所有按键"));
		//避免bug,抬起所有按键;
		mSDKWrapper.M_ReleaseAllKey(box_handle);
		});
	//按下a键
	connect(ui.M_KeyDown2_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		//按下a键
		int anjian = STATICVar->KeyToWinKeyCode("a");
		int ret = mSDKWrapper.M_KeyPress2(box_handle, anjian, 1);
		//读取按键状态
		int ret1 = mSDKWrapper.M_KeyState2(box_handle, anjian);
		//返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
		ui.log_textEdit->appendText(QStringLiteral("返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)"));
		ui.log_textEdit->appendText(QStringLiteral("按下某键,返回值=%1").arg(ret1));
		});
	//抬起a键
	connect(ui.M_KeyUp2_btn, &QPushButton::clicked, this, [this]() {

		ui.keyMouse_edit->setFocus();
		//按下a键
		int anjian = STATICVar->KeyToWinKeyCode("a");
		mSDKWrapper.M_KeyUp2(box_handle, anjian);
		//读取按键状态
		int ret1 = mSDKWrapper.M_KeyState2(box_handle, anjian);
		ui.log_textEdit->appendText(QStringLiteral("抬起某键,返回值=%1").arg(ret1));
		ui.log_textEdit->appendText(QStringLiteral("执行完毕,避免bug,抬起所有按键"));
		});
	//按键状态
	connect(ui.M_KeyState2_btn, &QPushButton::clicked, this, [this]() {
		int anjian = STATICVar->KeyToWinKeyCode("a");
		//int ret = keyPress2(box_handle, anjian, 1);
		int ret1 = mSDKWrapper.M_KeyState2(box_handle, anjian);
		//返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
		ui.log_textEdit->appendText(QStringLiteral("返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)"));
		ui.log_textEdit->appendText(QStringLiteral("a键状态,返回值=%1").arg(ret1));
		});
	//抬起所有按键
	connect(ui.M_ReleaseAllKey_btn, &QPushButton::clicked, this, [this]() {
		mSDKWrapper.M_ReleaseAllKey(box_handle);
		});
	//中文(GBK编码)英文混合
	connect(ui.M_KeyInputStringGBK_btn, &QPushButton::clicked, this, [this]() {
		QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
		QTextCodec::setCodecForLocale(utf8);
		//QTextCodec::setCodecForCStrings(utf8);
		QTextCodec* gbk = QTextCodec::codecForName("gbk");

		ui.keyMouse_edit->setFocus();
		_sleep(1000);
		QString str1 = "char与语言";
		QString strUnicode = utf8->toUnicode(str1.toLocal8Bit().data());
		QByteArray gb_bytes = gbk->fromUnicode(strUnicode);
		char* p = gb_bytes.data(); //获取其char *	
		mSDKWrapper.M_KeyInputStringGBK(box_handle, p, sizeof(p));
		});
	//中文(Unicode编码)英文
	connect(ui.M_KeyInputStringUnicode_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		_sleep(1000);
		QString str1 = "char与语言";
		QByteArray byte = str1.toUtf8();
		char* strchar = byte.data();
		mSDKWrapper.M_KeyInputStringUnicode(box_handle, strchar, sizeof(strchar));
		});
	//ASCII字符串
	connect(ui.M_KeyInputString_btn, &QPushButton::clicked, this, [this]() {
		ui.keyMouse_edit->setFocus();
		_sleep(1000);
		QString input_string = "char与语言";
		QByteArray array = input_string.toLatin1();

		for (int i = 0; i < array.size(); ++i)
		{
			int a1 = (int)(array.at(i));
			QString a = QString("%1").arg(a1);
			mSDKWrapper.M_KeyInputString(box_handle, a.toLocal8Bit().data(), 1);
		}

		});
	//读取小键盘NumLock灯的状态 //返回 0:灭；1:亮；-1: 失败
	connect(ui.M_NumLockLedState_btn, &QPushButton::clicked, this, [this]() {
		int ret1 = mSDKWrapper.M_NumLockLedState(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("读取小键盘NumLock灯的状态 //返回 0:灭；1:亮；-1: 失败,返回值=%1").arg(ret1));
		});
	//读取CapsLock灯的状态 //返回 0:灭；1:亮；-1: 失败
	connect(ui.M_CapsLockLedState_btn, &QPushButton::clicked, this, [this]() {
		int ret1 = mSDKWrapper.M_CapsLockLedState(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("读取CapsLock灯的状态 //返回 0:灭；1:亮；-1: 失败,返回值=%1").arg(ret1));
		});
	//读取ScrollLockLed灯的状态 //返回 0:灭；1:亮；-1: 失败
	connect(ui.M_ScrollLockLedState_btn, &QPushButton::clicked, this, [this]() {
		int ret1 = mSDKWrapper.M_ScrollLockLedState(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("读取ScrollLock灯的状态 //返回 0:灭；1:亮；-1: 失败,返回值=%1").arg(ret1));
		});
	//左键单击 
	connect(ui.M_LeftClick_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		int ret = mSDKWrapper.M_LeftClick(box_handle, 1);
		ui.log_textEdit->appendText(QStringLiteral("左键单击 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("左键单击 结束"));
		});
	//左键双击 
	connect(ui.M_LeftDoubleClick_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		ui.log_textEdit->appendText(QStringLiteral("左键双击 开始"));
		int ret = mSDKWrapper.M_LeftDoubleClick(box_handle, 1);
		ui.log_textEdit->appendText(QStringLiteral("左键双击 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("左键双击 结束"));
		});
	//按下左键不弹起 
	connect(ui.M_LeftDown_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		ui.log_textEdit->appendText(QStringLiteral("按下左键不弹起 开始"));
		int ret = mSDKWrapper.M_LeftDown(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("按下左键不弹起 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("按下左键不弹起 结束"));
		ui.M_LeftUp_btn->clicked();
		});
	//弹起左键 
	connect(ui.M_LeftUp_btn, &QPushButton::clicked, this, [this]() {

		ui.log_textEdit->appendText(QStringLiteral("弹起左键 开始"));
		int ret = mSDKWrapper.M_LeftUp(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("弹起左键 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("弹起左键 结束"));
		});
	//右键单击 
	connect(ui.M_RightClick_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		ui.log_textEdit->appendText(QStringLiteral("右键单击 开始"));
		int ret = mSDKWrapper.M_RightClick(box_handle, 1);
		ui.log_textEdit->appendText(QStringLiteral("右键单击 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("右键单击 结束"));
		});
	//按下右键不弹起 
	connect(ui.M_RightDown_btn, &QPushButton::clicked, this, [this]() {

		ui.log_textEdit->appendText(QStringLiteral("按下右键不弹起 开始"));
		int ret = mSDKWrapper.M_RightDown(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("按下右键不弹起 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("按下右键不弹起 结束"));

		ui.M_RightUp_btn->clicked();
		});
	//弹起右键 
	connect(ui.M_RightUp_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("弹起右键 开始"));
		int ret = mSDKWrapper.M_RightUp(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("弹起右键 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("弹起右键 结束"));
		});
	//中键单击 
	connect(ui.M_MiddleClick_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		ui.log_textEdit->appendText(QStringLiteral("中键单击 开始"));
		int ret = mSDKWrapper.M_MiddleClick(box_handle, 1);
		ui.log_textEdit->appendText(QStringLiteral("中键单击 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("中键单击 结束"));
		});
	//按下中键不弹起 
	connect(ui.M_MiddleDown_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("左键单击 开始;鼠标移动到(0,0)"));
		ui.M_ResetMousePos_btn->clicked();
		ui.log_textEdit->appendText(QStringLiteral("按下中键不弹起 开始"));
		int ret = mSDKWrapper.M_MiddleDown(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("按下中键不弹起 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("按下中键不弹起 结束"));
		ui.M_MiddleUp_btn->clicked();
		});
	//弹起中键 
	connect(ui.M_MiddleUp_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("弹起中键 开始"));
		int ret = mSDKWrapper.M_MiddleUp(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("弹起中键 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("弹起中键 结束"));
		});
	//弹起鼠标的所有按键
	connect(ui.M_ReleaseAllMouse_btn, &QPushButton::clicked, this, [this]() {
		ui.log_textEdit->appendText(QStringLiteral("弹起鼠标的所有按键 开始"));
		int ret = mSDKWrapper.M_ReleaseAllMouse(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("弹起鼠标的所有按键 返回 0: 成功；-1: 失败,返回值=%1").arg(ret));
		ui.log_textEdit->appendText(QStringLiteral("弹起鼠标的所有按键 结束"));
		});
	//读取鼠标左中右键状态   //MouseKeyCode: 1=左键 2=右键 3=中键  //返回 0: 弹起状态；1:按下状态；-1: 失败
	connect(ui.M_MouseKeyState_btn, &QPushButton::clicked, this, [this]() {
		int mouseKeycode=0;
		mSDKWrapper.M_MouseKeyState(box_handle, mouseKeycode);
		ui.log_textEdit->appendText(QStringLiteral("读取鼠标左中右键状态   //MouseKeyCode: 1=左键 2=右键 3=中键  //返回 0: 弹起状态；1:按下状态；-1: 失败,返回值=%1").arg(mouseKeycode));
		});
	//滚动鼠标滚轮;  Nbr: 滚动量,  为正,向上滚动；为负, 向下滚动;
	connect(ui.M_MouseWheel_1, &QPushButton::clicked, this, [this]() {
		//向上
		int ret = mSDKWrapper.M_MouseWheel(box_handle, 1);
		ui.log_textEdit->appendText(QStringLiteral("滚动鼠标滚轮;  Nbr: 滚动量,  为正,向上滚动；为负, 向下滚动,返回值=%1").arg(ret));

		});
	connect(ui.M_MouseWheel_2, &QPushButton::clicked, this, [this]() {
		//向下
		int ret = mSDKWrapper.M_MouseWheel(box_handle, -1);
		ui.log_textEdit->appendText(QStringLiteral("滚动鼠标滚轮;  Nbr: 滚动量,  为正,向上滚动；为负, 向下滚动,返回值=%1").arg(ret));

		});
	//将鼠标移动到原点(0,0) 
	connect(ui.M_ResetMousePos_btn, &QPushButton::clicked, this, [this]() {
		int ret = mSDKWrapper.M_ResetMousePos(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("将鼠标移动到原点 ,返回值=%1").arg(ret));
		});
	//相对移动
	connect(ui.M_MoveR_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.x_edit->text().toInt();
		int y = ui.y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveR(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("相对移动 ,返回值=%1").arg(ret));
		});
	//指定坐标移动
	connect(ui.M_MoveTo_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.x_edit->text().toInt();
		int y = ui.y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveTo(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("指定坐标移动 ,返回值=%1").arg(ret));
		});
	//读取当前鼠标所在坐标  返回坐标在x、y中
	connect(ui.M_GetCurrMousePos_btn, &QPushButton::clicked, this, [this]() {
		int x;
		int y;
		int ret = mSDKWrapper.M_GetCurrMousePos(box_handle, &x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	//返回当前鼠标坐标X值
	connect(ui.M_GetCurrMousePosX_btn, &QPushButton::clicked, this, [this]() {
		int x = mSDKWrapper.M_GetCurrMousePosX(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在X标  ,返回值=%1").arg(x));
		});
	//返回当前鼠标坐标Y值
	connect(ui.M_GetCurrMousePosY_btn, &QPushButton::clicked, this, [this]() {
		int y = mSDKWrapper.M_GetCurrMousePosY(box_handle);
		ui.log_textEdit->appendText(QStringLiteral("返回当前鼠标坐标Y值 ,返回值=%1").arg(y));
		});

	/*以下接口仅适用主控机和被控机是同一台电脑的使用方式(单头模块；双头模块的两个USB头都连接到同一台电脑)*/
	//指定坐标
	connect(ui.M_MoveR2_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.inOne_x_edit->text().toInt();
		int y = ui.inOne_y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveR2(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("指定坐标 ,返回值=%1").arg(ret));
		int ret1 = mSDKWrapper.M_GetCurrMousePos2(&x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	connect(ui.M_MoveTo2_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.inOne_x_edit->text().toInt();
		int y = ui.inOne_y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveTo2(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("指定坐标 ,返回值=%1").arg(ret));
		int ret1 = mSDKWrapper.M_GetCurrMousePos2(&x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	connect(ui.M_GetCurrMousePos2_btn, &QPushButton::clicked, this, [this]() {
		int x;
		int y;
		int ret1 = mSDKWrapper.M_GetCurrMousePos2(&x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	//以下接口将使用绝对移动功能。该功能目前还不能支持安卓

	connect(ui.M_ResolutionUsed_btn, &QPushButton::clicked, this, [this]() {
		int ret1 = mSDKWrapper.M_ResolutionUsed(box_handle ,STATICVar->screen_width, STATICVar->screen_height);
		if (ret1 == -10) {ui.log_textEdit->appendText(QStringLiteral("盒子不支持绝对移动"));}
		else if (ret1 == 0) {ui.log_textEdit->appendText(QStringLiteral("盒子支持绝对移动"));}	
		});
	connect(ui.M_MoveTo3_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.inOne_x_edit->text().toInt();
		int y = ui.inOne_y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveTo3(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("指定移动,返回值=%1").arg(ret)); 
		int ret1 = mSDKWrapper.M_GetCurrMousePos(box_handle ,&x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	connect(ui.M_MoveTo3_D_btn, &QPushButton::clicked, this, [this]() {
		int x = ui.inOne_x_edit->text().toInt();
		int y = ui.inOne_y_edit->text().toInt();
		int ret = mSDKWrapper.M_MoveTo3_D(box_handle, x, y);
		ui.log_textEdit->appendText(QStringLiteral("无轨迹移动,返回值=%1").arg(ret));
		int ret1 = mSDKWrapper.M_GetCurrMousePos(box_handle, &x, &y);
		ui.log_textEdit->appendText(QStringLiteral("读取当前鼠标所在坐标  返回坐标在x、y中 ,返回值=(%1,%2)").arg(x).arg(y));
		});
	connect(ui.M_Delay_btn, &QPushButton::clicked, this, [this]() {
		int time = ui.time_edit->text().toInt();
		int ret1 = mSDKWrapper.M_Delay(time);
		ui.log_textEdit->appendText(QStringLiteral("设置延时,返回值=%1").arg(ret1));
		});
	connect(ui.M_DelayRandom_btn, &QPushButton::clicked, this, [this]() {
		int minTime = ui.Min_time_edit->text().toInt();
		int maxTime= ui.Max_time_edit->text().toInt();
		int ret1 = mSDKWrapper.M_DelayRandom(minTime, maxTime);
		ui.log_textEdit->appendText(QStringLiteral("设置随机延时,返回值=%1").arg(ret1));
		});
	connect(ui.M_RandDomNbr_btn, &QPushButton::clicked, this, [this]() {
		int minTime = ui.Min_time_edit->text().toInt();
		int maxTime = ui.Max_time_edit->text().toInt();
		int ret1 = mSDKWrapper.M_RandDomNbr(minTime, maxTime);
		ui.log_textEdit->appendText(QStringLiteral("在最小最大值之间取随机数,返回值=%1").arg(ret1));
		});
}
