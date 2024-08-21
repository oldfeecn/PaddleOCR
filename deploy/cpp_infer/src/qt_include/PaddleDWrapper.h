#ifndef PaddleDWrapper_H
#define PaddleDWrapper_H

#include <QObject>
#include <QLibrary>
#include <QMutex>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>  // 包含 OpenCV 库
#include <QDebug>
namespace PaddleDetection {

struct ObjectResult {
    // 检测到的物体的矩形坐标：左、右、上、下
    std::vector<int> rect;
    // 检测到的物体的类别 ID
    int class_id;
    // 检测到的物体的标签
    std::string label;
    // 检测到的物体的置信度
    float confidence;
    // 检测到的物体的掩码
    std::vector<int> mask;
};
}
class PaddleDWrapper : public QObject
{
    Q_OBJECT

public:
    explicit PaddleDWrapper(QObject *parent = nullptr);
    ~PaddleDWrapper();

    bool loadLibrary(const QString &path);  // 加载 DLL
    bool initializeArgs(const std::vector<std::string>& arg_strings);  // 初始化变量
    std::vector<PaddleDetection::ObjectResult> processArguments(cv::Mat& im);  // 处理图像

private:
    QLibrary m_library;
    QMutex m_mutex;  // 线程安全

    // 函数指针类型定义
    typedef bool (*LPFN_InitializeArgs)(const std::vector<std::string>&);
    typedef std::vector<PaddleDetection::ObjectResult> (*LPFN_ProcessArguments)(cv::Mat&);

    // 函数指针成员变量
    LPFN_InitializeArgs Q_initializeArgs;
    LPFN_ProcessArguments Q_processArguments;
};

#endif // PaddleDWrapper_H
