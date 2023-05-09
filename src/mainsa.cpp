/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/02 19:02:25 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

int main(int argc, char **argv, char **env) {
    try {
        std::string request = "GET /index.php HTTP/1.1\n"
                              "Host: example.com\n"
                              "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0\n"
                              "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\n"
                              "Accept-Language: en-US,en;q=0.5\n"
                              "Accept-Encoding: gzip, deflate, br\n"
                              "Connection: keep-alive\n"
                              "Upgrade-Insecure-Requests: 1";

        std::string script_path = "/script.php";

        std::vector<const char*> envp;
        envp.push_back("CONTENT_LENGTH=17");
        envp.push_back("REQUEST_METHOD=POST");
        std::string file_name = "SCRIPT_FILENAME=" + script_path;
        envp.push_back(file_name.c_str());
        envp.push_back("SCRIPT_NAME=/script.php");
        std::vector<const char*> arg;
        arg.push_back("/usr/bin/php-cgi");
        arg.push_back("-q");
        int stdin_pipe[2];
        int stdout_pipe[2];
        pipe(stdin_pipe);
        pipe(stdout_pipe);
        const char* request_body = "Bonjour le monde !";
        write(stdin_pipe[1], request_body, strlen(request_body));
        pid_t pid = fork();
        if (pid == 0) {
            close(stdin_pipe[1]);
            close(stdout_pipe[0]);
            dup2(stdin_pipe[0], STDIN_FILENO);
            dup2(stdout_pipe[1], STDOUT_FILENO);
            execve(arg[0], (char *const *)arg.data(), (char *const *)envp.data());
            exit (1);
        }
        int status;
        waitpid(pid, &status, 0);
        close (stdin_pipe[0]);
        char buffer[1];
        std::string response;
        for (int i = 0; read(stdout_pipe[0], buffer, 1) > 0; i++) {
            std::cout << "ok";
        }
        std::cout << response << std::endl;
        exit (1);
        ConfigurationParser parser;
        std::vector<Configuration> configs = parser.parse("clean_conf");
        std::cout << Configuration::getExtension("hello.html") << std::endl;
        Cluster cluster(configs);
        cluster.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
