#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <QString>

// 定义事件类型枚举
enum class KeyMessage {
    KeyboardMessage  = 6,
    LeftButton       = 1,
    RightButton      = 2,
    MiddleButton     = 4,
    MoveMessage      = 5,
    MouseWhell      =6
};

// 定义事件状态枚举
enum class EventStatus {
    Pressed = 1,
    Released = 2,
    MouseWhellSta=-1
};

// 定义事件结构体
struct EventInfo {
    KeyMessage type;     // 事件类型
    EventStatus status; // 事件状态
    int x=0;
    int y=0;
    int key=0;
    int delay=0;          // 延时

     QString toString() const {
        return QString("%1,%2,%3,%4,%5,%6;")
                .arg(static_cast<int>(type))
                .arg(static_cast<int>(status))
                .arg(x)
                .arg(y)
                .arg(key)
                .arg(delay);
    }
};

#endif // EVENTINFO_H