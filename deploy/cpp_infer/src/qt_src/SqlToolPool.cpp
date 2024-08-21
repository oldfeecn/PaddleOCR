#include "SqlToolPool.h"
#include <QDebug>
#include "JsonTool.h"

QMutex SqlToolPool::mutex;
QWaitCondition SqlToolPool::waitConnection;
SqlToolPool* SqlToolPool::instance = nullptr;

SqlToolPool::SqlToolPool() {
    // 创建数据库连接的这些信息在实际开发中都需要通过读取配置文件得到，
    // 这里为了演示方便所以写死在了代码里。
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
    static QMutex initMutex;
    QMutexLocker locker(&initMutex);
    if (instance == nullptr) {
        instance = new SqlToolPool();
    }
    return *instance;
}

SqlToolPool::~SqlToolPool() {
    // 销毁连接池的时候删除所有的连接
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
    instance = nullptr;
}

QSqlDatabase SqlToolPool::openConnection() {
    SqlToolPool& pool = SqlToolPool::getInstance();
    QString connectionName;
    QMutexLocker locker(&mutex);

    // 已创建连接数
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for (int i = 0; i < pool.maxWaitTime && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount; i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);
        // 重新计算已创建连接数
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if (pool.unusedConnectionNames.size() > 0) {
        // 有已经回收的连接，复用它们
        connectionName = pool.unusedConnectionNames.dequeue();
    } else if (connectionCount < pool.maxConnectionCount) {
        // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);

    } else {
        // 已经达到最大连接数
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    qDebug() << connectionName;

    // 创建连接
    QSqlDatabase db = pool.createConnection(connectionName);
    // 有效的连接才放入 usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
    } else {
        qDebug() << "Failed to open database connection.";
    }
    return db;
}

void SqlToolPool::closeConnection(QSqlDatabase connection) {
    SqlToolPool& pool = SqlToolPool::getInstance();
    QString connectionName = connection.connectionName();
    // 如果是我们创建的连接，从 used 里删除，放入 unused 里
    if (pool.usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

QSqlDatabase SqlToolPool::createConnection(const QString& connectionName) {
    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);
        if (testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);
            if (query.lastError().type() != QSqlError::NoError || !db1.open()) {
                qDebug() << "testOnBorrow database error:" << db1.lastError().text();
            } else {
                return db1;
            }
        }
    }

    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(JsonTool::myEncryptUncrypt(databaseType, "oldfee"), connectionName);
    db.setHostName(JsonTool::myEncryptUncrypt(hostName, "oldfee"));
    db.setDatabaseName(JsonTool::myEncryptUncrypt(databaseName, "oldfee"));
    db.setUserName(JsonTool::myEncryptUncrypt(username, "oldfee"));
    db.setPassword(JsonTool::myEncryptUncrypt(password, "oldfee"));

    if (!db.open()) {
        qDebug() << "Open database error:" << db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}

bool SqlToolPool::insertDataWithQmap(const QMap<QString, QVariant>& data) {
    
    // Get table name
    QString tableName = data.value("tableName").toString();
    if (tableName.isEmpty()) {
        qWarning() << "Table name is missing.";
        return false;
    }
    QSqlDatabase db = SqlToolPool::openConnection();
    if (!db.isOpen()) {
        qWarning("[SQL] Database is not open.");
        return false;
    }

    // Prepare SQL statement
    QStringList columns;
    QStringList placeholders;
    QSqlQuery sql_query(db);

    QStringList keys = data.keys();
    for (const QString& key : keys) {
        if (key != "tableName") {  // Skip the tableName key
            columns << key;
            placeholders << QString(":%1").arg(key);  // Use :key format
        }
    }

    QString sqlStr = QString("INSERT INTO `%1` (%2) VALUES (%3);")
                        .arg(tableName)
                        .arg(columns.join(", "))
                        .arg(placeholders.join(", "));

    sql_query.prepare(sqlStr);

    // Bind values
    for (const QString& key : keys) {
        if (key != "tableName") {
            sql_query.bindValue(QString(":%1").arg(key), data.value(key));
        }
    }

    // Execute query
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec()) {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }
    SqlToolPool::closeConnection(db);
    return true;
}
bool SqlToolPool::insertData(const QString sqlStr) {
    QSqlDatabase db = SqlToolPool::openConnection();
    if (!db.isOpen()) {
        qWarning("[SQL] Database is not open.");
        return false;
    }

    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr)) {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }

    SqlToolPool::closeConnection(db);
    return true;
}

bool SqlToolPool::updateData(const QString sqlStr) {
    QSqlDatabase db = SqlToolPool::openConnection();
    if (!db.isOpen()) {
        qWarning("[SQL] Database is not open.");
        return false;
    }

    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr)) {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }

    SqlToolPool::closeConnection(db);
    return true;
}

bool SqlToolPool::selectData(const QString sqlStr, QList<QMap<QString, QVariant>>& valuesList) {
    QSqlDatabase db = SqlToolPool::openConnection();
    if (!db.isOpen()) {
        qWarning("[SQL] Database is not open.");
        return false;
    }

    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr)) {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }

    while (sql_query.next()) {
        QSqlRecord rec = sql_query.record();
        QMap<QString, QVariant> valueMap;
        for (int c = 0; c < rec.count(); c++) {
            valueMap[rec.fieldName(c)] = rec.value(c);
        }
        valuesList.append(valueMap);
    }

    SqlToolPool::closeConnection(db);
    return true;
}

bool SqlToolPool::deleteData(const QString sqlStr) {
    QSqlDatabase db = SqlToolPool::openConnection();
    if (!db.isOpen()) {
        qWarning("[SQL] Database is not open.");
        return false;
    }

    QSqlQuery sql_query(db);
    qInfo("[SQL] %s", qPrintable(sqlStr));
    if (!sql_query.exec(sqlStr)) {
        qWarning("[SQL] %s", qPrintable(sql_query.lastError().text()));
        return false;
    }

    SqlToolPool::closeConnection(db);
    return true;
}
