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
    static void release(); // �ر����е����ݿ�����
    static QSqlDatabase openConnection();                 // ��ȡ���ݿ�����
    static void closeConnection(QSqlDatabase connection);
    static void test();
    // �ͷ����ݿ����ӻ����ӳ�
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
    QSqlDatabase createConnection(const QString& connectionName); // �������ݿ�����
    QQueue<QString> usedConnectionNames;   // ��ʹ�õ����ݿ�������
    QQueue<QString> unusedConnectionNames; // δʹ�õ����ݿ������� 
   // QQueue<QMap<QString, int>> usedConnectionNames;   // ��ʹ�õ����ݿ���������ʱ�ر�
   // QQueue<QMap<QString, int>> unusedConnectionNames; // δʹ�õ����ݿ���������ʱ�ر�  
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    bool    testOnBorrow;    // ȡ�����ӵ�ʱ����֤�����Ƿ���Ч
    QString testOnBorrowSql; // ���Է������ݿ�� SQL
    int maxWaitTime;  // ��ȡ�������ȴ�ʱ��
    int waitInterval; // ���Ի�ȡ����ʱ�ȴ����ʱ��
    int maxConnectionCount; // ���������

    static QWaitCondition waitConnection;
    static QMutex mutex;
    static SqlToolPool* instance;

};

