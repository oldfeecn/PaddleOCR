#include "PaddleDWrapper.h"

PaddleDWrapper::PaddleDWrapper(QObject *parent) : QObject(parent), Q_initializeArgs(nullptr), Q_processArguments(nullptr)
{
}

PaddleDWrapper::~PaddleDWrapper()
{
    if (m_library.isLoaded()) {
        m_library.unload();
    }
}

bool PaddleDWrapper::loadLibrary(const QString &path)
{
    QMutexLocker locker(&m_mutex);  // 确保线程安全

    m_library.setFileName(path);

    if (!m_library.load()) {
        qWarning() << "Failed to load library:" << m_library.errorString();
        return false;
    }
    
    // 获取函数地址
    Q_initializeArgs = (LPFN_InitializeArgs)m_library.resolve("initializeArgs");
    Q_processArguments = (LPFN_ProcessArguments)m_library.resolve("processArguments");

    if (!Q_initializeArgs || !Q_processArguments) {
        qWarning() << "Failed to resolve functions from the DLL";
        return false;
    }

    return true;
}

bool PaddleDWrapper::initializeArgs(const std::vector<std::string>& arg_strings)
{
    if (Q_initializeArgs) {
        return Q_initializeArgs(arg_strings);
    } else {
        qWarning() << "initializeArgs function pointer is null.";
        return false;
    }
}

std::vector<PaddleDetection::ObjectResult> PaddleDWrapper::processArguments(cv::Mat& im)
{
    if (Q_processArguments) {
        return Q_processArguments(im);
    } else {
        qWarning() << "processArguments function pointer is null.";
        return std::vector<PaddleDetection::ObjectResult>();
    }
}