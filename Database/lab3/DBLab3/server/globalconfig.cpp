#include "globalconfig.h"

namespace Config {
const QString dataBaseName("db_lab3");
const QString databaseUserName("dblab");
const QString databasePassword("dblabdblab");

const quint32 dataBaseMaxQueuedRequest = 10;

const QString messagePattern(
        "%{if-debug}\033[94m%{endif}"
        "%{if-info}\033[92m%{endif}"
        "%{if-warning}\033[93m%{endif}"
        "%{if-critical}\033[91m%{endif}"
        "%{if-fatal}\033[95m%{endif}"
        "\033[01m[%{type}]\033[0m\033[01m"
        "[%{time yyyy-MM-dd hh:mm:ss.zzz}]"
        "[%{file}:%{line}]"
        "[%{threadid}]\033[0m "
        "%{message}");

}
