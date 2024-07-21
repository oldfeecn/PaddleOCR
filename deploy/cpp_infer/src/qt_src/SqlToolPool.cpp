#include "SqlToolPool.h"
#include <QDebug>
#include "JsonTool.h"

QMutex SqlToolPool::mutex;
QWaitCondition SqlToolPool::waitConnection;
SqlToolPool* SqlToolPool::instance = NULL;

SqlToolPool::SqlToolPool() {
    // �������ݿ����ӵ���Щ��Ϣ��ʵ�ʿ�����ʱ����Ҫͨ����ȡ�����ļ��õ���
    // ����Ϊ����ʾ��������д�����˴����
    //bool wirtejson = JsonTool::wirteJson("setting.json");
    //bool readjson= JsonTool::readJson("setting.json");

    hostName = "eTB5NEExSHdFdzVKbUs4WA==";
    databaseName = "d2RuQkUybmFwZFc9";
    username = "eU1XNEQydmFwZFc9";
    password = "dWRmQnZ2dmdvaGpCckxUVHcxaTBBMm5RcWhUcHJ2cTg=";
    databaseType = "dmZxV3cxcVdyMFc9";
    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";
    maxWaitTime = 1000;
    waitInterval = 200;
    maxConnectionCount = 5;
}

SqlToolPool& SqlToolPool::getInstance() {
    if (NULL == instance) {
        QMutexLocker locker(&mutex);
        if (NULL == instance) {
            instance = new SqlToolPool();
        }
    }

    return *instance;
}
SqlToolPool::~SqlToolPool()
{
    // �������ӳص�ʱ��ɾ�����е�����
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

}
void SqlToolPool::release() {
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

QSqlDatabase SqlToolPool::openConnection() {
    //QThread::currentThreadId();
    SqlToolPool& pool = SqlToolPool::getInstance();
    QString connectionName;
    QMutexLocker locker(&mutex);
    // �Ѵ���������
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    // ��������Ѿ����꣬�ȴ� waitInterval ���뿴���Ƿ��п������ӣ���ȴ� maxWaitTime ����
    for (int i = 0;
        i < pool.maxWaitTime
        && pool.unusedConnectionNames.size() == 0
        && connectionCount == pool.maxConnectionCount;
        i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);
        // ���¼����Ѵ���������
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }
    qDebug() << "unusedConnectionNames." << pool.unusedConnectionNames.size() << endl;;
    if (pool.unusedConnectionNames.size() > 0) {
        // ���Ѿ����յ����ӣ���������
        connectionName = pool.unusedConnectionNames.dequeue();
    }
    else if (connectionCount < pool.maxConnectionCount) {
        // û���Ѿ����յ����ӣ�����û�дﵽ������������򴴽��µ�����
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
        //connectionName = QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));;
    }
    else {
        // �Ѿ��ﵽ���������
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }
    qDebug() << connectionName << endl;;
    // ��������
    QSqlDatabase db = pool.createConnection(connectionName);
    // ��Ч�����Ӳŷ��� usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
    }
    return db;
}

void SqlToolPool::closeConnection(QSqlDatabase connection) {
    SqlToolPool& pool = SqlToolPool::getInstance();
    QString connectionName = connection.connectionName();
    // ��������Ǵ��������ӣ��� used ��ɾ�������� unused ��
    if (pool.usedConnectionNames.contains(connectionName)
        ) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}



QSqlDatabase SqlToolPool::createConnection(const QString& connectionName) {
    // �����Ѿ��������ˣ������������������´���
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);
        if (testOnBorrow) {
            // ��������ǰ�������ݿ⣬������ӶϿ������½�������
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);
            if (query.lastError().type() != QSqlError::NoError || !db1.open()) {
                qDebug() << "testOnBorrow datatabase error:" << db1.lastError().text();
                //return QSqlDatabase();
                //����������ݿ����ӳ����ͷ���һ���µ����ݿ�����
            }
            else {
                return db1;
            }

        }
    }
   // ����һ���µ�����
    QSqlDatabase db = QSqlDatabase::addDatabase(JsonTool::myEncryptUncrypt(databaseType, "oldfee"), connectionName);
    db.setHostName(JsonTool::myEncryptUncrypt(hostName, "oldfee"));
    db.setDatabaseName(JsonTool::myEncryptUncrypt(databaseName, "oldfee"));
    db.setUserName(JsonTool::myEncryptUncrypt(username, "oldfee"));
    db.setPassword(JsonTool::myEncryptUncrypt(password, "oldfee"));
    qDebug() << QString(JsonTool::myEncryptUncrypt(databaseType, "oldfee"));
    qDebug() << QString(JsonTool::myEncryptUncrypt(hostName, "oldfee"));
    qDebug() << QString(JsonTool::myEncryptUncrypt(databaseName, "oldfee"));
    qDebug() << QString(JsonTool::myEncryptUncrypt(username, "oldfee"));
    qDebug() << QString(JsonTool::myEncryptUncrypt(password, "oldfee"));
    //QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
    // db.setHostName("localhost");
    // db.setDatabaseName("cfsl");
    // db.setUserName("postgres");
    // db.setPassword("iaiib.com");

    //  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    // db.setHostName("localhost");
    // db.setPort(3306);
    // db.setDatabaseName("cfsl");
    // db.setUserName("root");
    // db.setPassword("CFSLoldfee.cn@345");
   
    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}


void SqlToolPool::test() {
    // 1. �����ݿ����ӳ���ȡ������
    QSqlDatabase db = SqlToolPool::openConnection();
    // 2. ʹ�����Ӳ�ѯ���ݿ�
    QSqlQuery query(db);
    query.exec("SELECT * FROM dnf_info where id=1");
    while (query.next()) {
        qDebug() << query.value("qq").toString();
    }
    // 3. ����ʹ�������Ҫ�ͷŻ����ݿ����ӳ�
    SqlToolPool::closeConnection(db);
    //���������ʱ��
    SqlToolPool::release(); // 4. �ͷ����ݿ�����

}

bool SqlToolPool::insertData(const QString sqlStr)
{
    // 1. �����ݿ����ӳ���ȡ������
    QSqlDatabase db = SqlToolPool::openConnection();
    // 2. ʹ�����Ӳ�ѯ���ݿ�
    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }
    // 3. ����ʹ�������Ҫ�ͷŻ����ݿ����ӳ�
    SqlToolPool::closeConnection(db);
    return true;
}
bool SqlToolPool::updateData(const QString sqlStr)
{
    QSqlDatabase db = SqlToolPool::openConnection();
    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }
    // 3. ����ʹ�������Ҫ�ͷŻ����ݿ����ӳ�
    SqlToolPool::closeConnection(db);
    return true;
}
bool SqlToolPool::selectData(const QString sqlStr, QList<QMap<QString, QVariant>>& valuesList)
{
    QSqlDatabase db = SqlToolPool::openConnection();
    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }
    for (int r = 0; sql_query.next(); r++)
    {
        QSqlRecord rec = sql_query.record();
        QMap<QString, QVariant> valueMap;
        valueMap.clear();
        for (int c = 0; c < rec.count(); c++)
        {
            valueMap[rec.fieldName(c)] = rec.value(c);
        }
        valuesList.append(valueMap);
    }
    // 3. ����ʹ�������Ҫ�ͷŻ����ݿ����ӳ�
    SqlToolPool::closeConnection(db);
    return true;
}

bool SqlToolPool::deleteData(const QString sqlStr)
{
    QSqlDatabase db = SqlToolPool::openConnection();
    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }
    // 3. ����ʹ�������Ҫ�ͷŻ����ݿ����ӳ�
    SqlToolPool::closeConnection(db);
    return true;
}
bool SqlToolPool::initDatabase()
{
    QSqlDatabase db = SqlToolPool::openConnection();
    if (db.open())
    {
        QSqlQuery sql_query(db);

        //������loginInfo
        //qInfo("[SQL] %s", qPrintable(SQL_CREATE_LOGININFO));
       /* if (!sql_query.exec(SQL_CREATE_LOGININFO))
        {
            qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
            return false;
        }*/
        //������userInfo
        //qInfo("[SQL] %s", qPrintable(SQL_CREATE_USERINFO));
        /*if (!sql_query.exec(SQL_CREATE_USERINFO))
        {
            qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
            return false;
        }*/
        return true;
    }
    else
    {
        qWarning("Init database failed,database is not open.");
        return false;
    }

}
