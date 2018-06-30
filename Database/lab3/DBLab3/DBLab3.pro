TEMPLATE = subdirs

include(DBLab3_common.pri)
DESTDIR = $$BUILDDIR

# library
SUBDIRS += qhttp
SUBDIRS += protocol
SUBDIRS += sqlparser
SUBDIRS += client

# qhttp support
qhttp.file = libs/qhttp/qhttp.pro
# sql parser support
sqlparser.file = libs/sqlparser/sqlparser.pro

# http comminicate protocol
protocol.file = protocol/DBLab3_protocol.pro

test {
    # server test
    SUBDIRS += server_test
    server_test.file = server_test/DBLab3_server_test.pro
    servet_test.depends = qhttp protocol sqlparser
} else {
    # server
    SUBDIRS += server
    server.file = server/DBLab3_server.pro
    server.depends = qhttp protocol sqlparser
}

client.file = client/DBLab3_client.pro
client.depends = qhttp protocol
