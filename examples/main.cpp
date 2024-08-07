#include <iostream>
#include <string>
#include <uboat/uboat.h>

int main() {
    const std::string url("127.0.0.1:4533");
    const std::string user("karl");
    const std::string pass("donitz");
    const std::string client_name("uboat_exmpl");

    uboat::OSClient client(url, user, pass, client_name);

    auto r_err = client.ping();

    auto r = client.authenticate();

    nlohmann::json j_r_err = r_err.value();
    nlohmann::json j_err = r_err.value().error;
    nlohmann::json j_r = r.value();

    std::cout << "ping without auth:\n" << j_r_err.dump(4) << "\n";
    std::cout << j_err.dump(4) << "\n";

    std::cout << "\nauthed:\n" << j_r.dump(4) << "\n";

    return 0;
}
