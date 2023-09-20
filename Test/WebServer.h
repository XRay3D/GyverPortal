#pragma once

#include <DNSServer.h>
#include <QCoreApplication>
#include <WiFi.h>

#include <httpserver/httplistener.h>

using namespace stefanfrings;

enum {
    CONTENT_LENGTH_UNKNOWN,
};

enum HTTPMethod {
    HTTP_ANY,
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_PATCH,
    HTTP_DELETE,
    HTTP_OPTIONS
};
enum HTTPUploadStatus {
    UPLOAD_FILE_START,
    UPLOAD_FILE_WRITE,
    UPLOAD_FILE_END,
    UPLOAD_FILE_ABORTED
};
enum HTTPClientStatus {
    HC_NONE,
    HC_WAIT_READ,
    HC_WAIT_CLOSE
};

struct IPAddress : String {
    IPAddress() { qWarning(""); }

    template <typename T>
    IPAddress(T) { }
    IPAddress(uint8_t, uint8_t, uint8_t, uint8_t) { }
};

struct Client {
    Client() { qWarning(""); }
    void stop() { qWarning(""); };
    IPAddress remoteIP() {
        // qWarning("");
        return {};
    }
};

struct HTTPUpload {
    String name;
    String filename;
    char* buf;
    int currentSize;
    int status;
};

struct WebServer {

    class RequestHandler : public HttpRequestHandler {
        // Q_OBJECT
        Q_DISABLE_COPY(RequestHandler)

        WebServer& server;

    public:
        RequestHandler(WebServer& server, QObject* parent = nullptr)
            : HttpRequestHandler(parent)
            , server{server} {
            // qWarning("RequestHandler: created");
        }

        ~RequestHandler() {
            // qWarning("RequestHandler: deleted");
        }

        void service(HttpRequest& request, HttpResponse& response) {
            QByteArray path = request.getPath();
            // qWarning("Conroller: path=%s", path.data());
            //  server.request = &request;
            server.response = &response;
            server.parameterMap = request.getParameterMap();
            server.uri_ = path;
            for(auto&& [key, func]: server.map) {
                if(key == path) {
                    func();
                    return;
                }
            }
            server.map["onNotFound"]();

            //            // Set a response header
            //            response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");

            //            // Return a simple HTML document
            //            response.write("<html><body>Hello World!</body></html>", true);

            //            //qWarning("RequestHandler: finished request");
        }
    } requestHandler{*this};
    ///////////////////
    static HTTPUpload httpUpload;

    HttpResponse* response;
    // HttpRequest* request;

    std::unordered_map<QByteArray, std::function<void(void)>> map;
    MultiMap<QByteArray, QByteArray> parameterMap;
    String uri_;
    HttpListener* httpListener;
    void
    begin(uint16_t port) {
        // qWarning() << port;

        // Collect hardcoded configarion settings
        QSettings* settings = new QSettings(qApp);
        settings->setValue("host", "localhost"); //"192.168.0.100");
        settings->setValue("port", port);
        settings->setValue("minThreads", 4);
        settings->setValue("maxThreads", 100);
        settings->setValue("cleanupInterval", 60000);
        settings->setValue("readTimeout", 60000);
        settings->setValue("maxRequestSize", 16000);
        settings->setValue("maxMultiPartSize", 10000000);
        // settings->setValue("sslKeyFile","ssl/my.key");
        // settings->setValue("sslCertFile","ssl/my.cert");

        // Configure and start the TCP listener
        httpListener = new HttpListener(settings, &requestHandler, qApp);
    }

    void stop() {
        // qWarning("");
    }

    void handleClient() { /*//qWarning("");*/
        if(!httpListener->isListening())
            httpListener->listen();
    }

    void sendContent(String content) {
        // //qWarning() << content;
        response->write(QByteArray{content});
    }

    void sendContent_P(String content) {
        // //qWarning() << content;
        response->write(QByteArray{content});
    }

    void send(int code, String contentType, String content) {
        // //qWarning() << code << contentType << content;
        response->setStatus(code, QByteArray{contentType});
        response->write(QByteArray{content});
    }

    void send_P(int code, String contentType, String content, int len) {
        // //qWarning() << code << contentType << content << len;
        response->setStatus(code, QByteArray{contentType});
        response->write(QByteArray{content});
    }

    void send(int code, String s) {
        // qWarning() << code << s;
        response->setStatus(code, QByteArray{s});
    }

    void send(int code) {
        // qWarning() << code;
        response->setStatus(code);
    }

    void sendHeader(String name, String value) {
        // qWarning() << name << value;
        response->setHeader(QByteArray{name}, QByteArray{value});
    }

    void setContentLength(int i) {
        // qWarning() << i;
    }

    Client client() {
        // qWarning("");
        return {};
    }

    typedef std::function<void(void)> THandlerFunction;
    void on(const String& uri, THandlerFunction handler) {
        // qWarning("");
    }
    void on(const String& uri, HTTPMethod method, THandlerFunction fn) {
        // qWarning("");
    }
    void on(const String& uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn) {
        // qWarning("");
    }
    void onNotFound(THandlerFunction func) {
        // qWarning("");
        map["onNotFound"] = func;
    }

    String uri() const {
        // qWarning("");
        return uri_;
    }

    HTTPUpload& upload() {
        // qWarning("");
        return httpUpload;
    }

    int args() const {
        //  qWarning("");
        return static_cast<int>(parameterMap.data_.size());
    }

    String arg(String name) const {
        //  qWarning() << name;
        return parameterMap.value(QByteArray{name});
    }

    String arg(int i) const {
        String arg;
        try {
            arg = parameterMap.value(i);
        } catch(...) {
        }
        //  qWarning() << i << arg;
        return arg;
    }

    bool hasArg(int i) const {
        //  qWarning() << i;
        return parameterMap.data_.size() > i;
    }

    bool hasArg(String name) const {
        //  qWarning() << name;
        return parameterMap.contains(QByteArray{name});
    }

    String argName(int i) const {
        String argName;
        try {
            argName = parameterMap.data_.at(i).first;
        } catch(...) {
        }
        // qWarning() << i << argName;
        return argName;
    }

    bool authenticate(String s1, String s2) {
        // qWarning() << s1 << s2;
        return {};
    }

    void requestAuthentication() {
        // qWarning("");
    }

    //
};
