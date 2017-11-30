// TODO: implement this in client and server

//    /// @file basedataprocessor.h
//    /// @author Sixu Hu <husixu1@hotmail.com>
//    /// @date 10/2017/2017
//    /// @copyright GNU Public License v3
//    #ifndef BASEDATAPROCESSOR_H
//    #define BASEDATAPROCESSOR_H
//
//    #include <QObject>
//    #include <QThread>
//    #include <QByteArray>
//
//    class BaseDataProcessor : public QObject {
//        Q_OBJECT
//
//    private:
//        // TODO: more threads!
//        /// @brief the thread this object is running on
//        QThread thread;
//
//        /// @brief constructor
//        BaseDataProcessor();
//
//        /// @brief process the data, must override (different in server and client)
//        /// @note the data is guranteed to be complete and queued,
//        /// all the implementor should care is process one picece of messgae
//        virtual void processData(QByteArray) = 0;
//
//    public:
//
//        /// @brief get the singleton instance
//        static BaseDataProcessor &getInstance();
//
//        /// @{
//        /// @brief singleton assurance
//        BaseDataProcessor(const BaseDataProcessor &) = delete;
//        BaseDataProcessor &operator =(const BaseDataProcessor &) = delete;
//        /// @}
//
//        /// @brief de-constructor
//        ~BaseDataProcessor();
//    signals:
//
//    private slots:
//        /// @brief receive and queue the data
//        void dataReceived(QByteArray);
//
//    public slots:
//    };
//
//    #endif // BASEDATAPROCESSOR_H
//
