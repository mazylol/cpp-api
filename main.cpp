#include <pistache/endpoint.h>
#include <pistache/router.h>

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "Hello, World\n");
    }
};

void helloHandler(const Rest::Request& request, Http::ResponseWriter response) {
    auto name = request.param(":name").as<std::string>();

    response.send(Http::Code::Ok, "Hello " + name);
}

Rest::Router registerRoutes() {
    Rest::Router router;

    Rest::Routes::Get(router, "/hello/:name", Rest::Routes::bind(&helloHandler));

    return router;
}

int main() {
    Address addr(Ipv4::any(), 9080);

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    //server.setHandler(Http::make_handler<HelloHandler>());
    server.setHandler(registerRoutes().handler());

    server.serve();
}