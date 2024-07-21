#pragma once
#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "Databasesql.h"
class SqlToolPool
{
public:
    ~SqlToolPool();
    static void release(); // 关闭所有的数据库连接
    static QSqlDatabase openConnection();                 // 获取数据库连接
    static void closeConnection(QSqlDatabase connection);
    static void test();
    // 释放数据库连接回连接池
    static bool insertData(const QString sqlStr);
    static bool updateData(const QString sqlStr);
    static bool selectData(const QString sqlStr, QList<QMap<QString, QVariant>>& valuesList);
    static bool deleteData(const QString sqlStr);
    static  bool initDatabase();

private:
    static SqlToolPool& getInstance();
    SqlToolPool();
    SqlToolPool(const SqlToolPool& other);
    SqlToolPool& operator=(const SqlToolPool& other);
    QSqlDatabase createConnection(const QString& connectionName); // 创建数据库连接
    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名 
   // QQueue<QMap<QString, int>> usedConnectionNames;   // 已使用的数据库连接名定时关闭
   // QQueue<QMap<QString, int>> unusedConnectionNames; // 未使用的数据库连接名定时关闭  
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL
    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数

    static QWaitCondition waitConnection;
    static QMutex mutex;
    static SqlToolPool* instance;

};

