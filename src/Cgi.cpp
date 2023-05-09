#include "Cgi.hpp"
#include "Configuration.hpp"
#include "Cluster.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

std::string Cgi::_findScript(const std::string & path){
    return path.substr(path.find_last_of('/') + 1);
}

std::string Cgi::process(const HTTPRequestParser &request, const Configuration &config) {
    std::string phpcgi = config.php_cgi;
    std::string script = _findScript(request.get("location"));
    std::vector<const char *> env;
    std::string redirect_status = "REDIRECT_STATUS=200";
    std::string request_method = "REQUEST_METHOD=" + request.get("type");
    std::string script_filename = "SCRIPT_FILENAME=" + config.root + request.get("location");
    std::string script_name = "SCRIPT_NAME=" + request.get("location");
    std::string content_type = "CONTENT_TYPE=" + request.get("content-type");
    std::string content_length = "CONTENT_LENGTH=" + request.get("content-length");
    std::string document_root = "DOCUMENT_ROOT=" + config.root;
    std::string query_string = "QUERY_STRING=" + request.get("query-string");
    //TODO: QUERY_STRING

    env.push_back(redirect_status.c_str());
    env.push_back(request_method.c_str());
    env.push_back(script_filename.c_str());
    env.push_back(script_name.c_str());
    env.push_back(content_type.c_str());
    env.push_back(content_length.c_str());
    env.push_back(document_root.c_str());
    env.push_back(NULL);

    std::vector<const char *> argv;
    argv.push_back(phpcgi.c_str());
    argv.push_back("-q");
    argv.push_back(NULL);
    int stdin_pipefd[2];
    int stdout_pipefd[2];
    pid_t pid;

    if (pipe(stdin_pipefd) == -1) {
        std::cerr << "pipe failed\n";
        return "";
    }

    if (pipe(stdout_pipefd) == -1) {
        std::cerr << "pipe failed\n";
        return "";
    }

    pid = fork();

    if (pid == -1) {
        std::cerr << "fork failed\n";
        return "";
    }
    if (pid == 0) {
        close(stdin_pipefd[1]);
        dup2(stdin_pipefd[0], STDIN_FILENO);
        close(stdin_pipefd[0]);

        close(stdout_pipefd[0]);
        dup2(stdout_pipefd[1], STDOUT_FILENO);
        close(stdout_pipefd[1]);

        execve(argv[0], (char * const *)argv.data(), (char * const*)env.data());
        std::cerr << "execve failed" << std::endl;
        std::cerr << "code d'erreur: " << errno << std::endl;
        std::cerr << "erreur: " << strerror(errno) << std::endl;
        std::cout << "script_name: " << script_name << std::endl;
        std::cout << "script_name: " << script_filename << std::endl;
        return "";
    } else {
        close(stdin_pipefd[0]);
        std::string body = request.get("body");
        write(stdin_pipefd[1], body.c_str(), body.length());
        close(stdin_pipefd[1]);
        close(stdout_pipefd[1]);
        char buffer[1];
        std::string response;
        for (int i = 0; read(stdout_pipefd[0], buffer, 1) > 0; i++) {
            response += buffer[0];
        }
        close(stdout_pipefd[0]);
        wait(NULL);
        return response;
    }

}
