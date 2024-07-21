cmake_minimum_required(VERSION 3.10)

project(MyQtProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# ���� Windows SDK �� MDK �Ŀ�Ͱ���·��
set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Windows Kits/10")
set(WINDOWS_SDK_VERSION "10.0.19041.0")  # ���滻Ϊʵ�ʰ�װ�� SDK �汾

# ����·��
include_directories(
    "${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/ucrt"
    "${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/shared"
    "${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/um"
    "${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/winrt"
)

# ��·��
link_directories(
    "${WINDOWS_SDK_PATH}/Lib/${WINDOWS_SDK_VERSION}/ucrt/x64"
    "${WINDOWS_SDK_PATH}/Lib/${WINDOWS_SDK_VERSION}/um/x64"
)

add_executable(MyQtProject
    main.cpp
    DeviceManager.cpp
    
)

target_include_directories(MyQtProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ���ӿ�
target_link_libraries(MyQtProject
    Qt5::Widgets
    cfgmgr32
    setupapi
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    swdevice 
)
