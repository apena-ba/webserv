#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "Cgi.hpp"
#include "Configuration.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"

static std::string _replaceAllOccurence(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

std::string Cgi::process(const HTTPRequestParser &request, const Configuration &config, std::string stdin_content) {
    std::string request_method  = "REQUEST_METHOD=" + request.get("type");
    std::string server_protocol = "SERVER_PROTOCOL=" + request.get("version");
    std::string path_info       = "PATH_INFO=" + request.get("location");

    std::vector<std::string>    env;
    env.push_back(request_method);
    env.push_back(server_protocol);
    env.push_back(path_info);

    std::string                 env_var;
    for (std::map<std::string, std::string>::const_iterator
        it = request.getVals().begin();
        it != request.getVals().end();
        ++it) {
        env_var = _replaceAllOccurence(env_var, "-", "_");
        env.push_back(env_var);
    }

    std::string                 phpcgi = config.cgi;
    std::vector<const char *>   argv;
    argv.push_back(phpcgi.c_str());
    argv.push_back(path_info.c_str());
    argv.push_back(NULL);

    int stdin_pipefd[2];
    int stdout_pipefd[2];
    if (pipe(stdin_pipefd) == -1) {
        std::cerr << "pipe failed\n";
        return "";
    }
    if (pipe(stdout_pipefd) == -1) {
        std::cerr << "pipe failed\n";
        return "";
    }

    pid_t pid;
    pid = fork();

    if (pid == -1) {
        std::cerr << "fork failed\n";
        return "";
    }
    if (pid == 0) {
        char * envp[env.size() + 1];
        for (size_t i = 0; i < env.size(); i++) {
            envp[i] = const_cast<char*>(env[i].c_str());
        }
        envp[env.size()] = NULL;
        close(stdin_pipefd[1]);
        dup2(stdin_pipefd[0], STDIN_FILENO);
        close(stdin_pipefd[0]);

        close(stdout_pipefd[0]);
        dup2(stdout_pipefd[1], STDOUT_FILENO);
        close(stdout_pipefd[1]);

        execve(argv[0], (char * const *)argv.data(), envp);
        std::cerr << "execve failed" << std::endl;
        return "";
    }
    close(stdin_pipefd[0]);
    write(stdin_pipefd[1], stdin_content.c_str(), stdin_content.length());
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
