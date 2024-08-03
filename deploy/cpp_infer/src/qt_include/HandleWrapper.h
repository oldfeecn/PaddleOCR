#include <windows.h>
#include <iostream>

class HandleWrapper {
public:
    explicit HandleWrapper(HANDLE handle = NULL) : m_handle(handle) {}

    ~HandleWrapper() {
        close();
    }

    // 禁用复制构造和复制赋值
    HandleWrapper(const HandleWrapper&) = delete;
    HandleWrapper& operator=(const HandleWrapper&) = delete;

    // 启用移动构造和移动赋值
    HandleWrapper(HandleWrapper&& other) noexcept : m_handle(other.m_handle) {
        other.m_handle = NULL;
    }

    HandleWrapper& operator=(HandleWrapper&& other) noexcept {
        if (this != &other) {
            close();
            m_handle = other.m_handle;
            other.m_handle = NULL;
        }
        return *this;
    }

    void setHandle(HANDLE handle) {
        close();
        m_handle = handle;
    }

    HANDLE getHandle() const {
        return m_handle;
    }

    void close() {
        if (m_handle != NULL && m_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_handle);
            std::cout << "Handle closed.\n";
            m_handle = NULL;
        }
    }

private:
    HANDLE m_handle;
};


