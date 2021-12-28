/*
** EPITECH PROJECT, 2021
** babel
** File description:
** TODO: add description
*/
#ifndef BABEL_ERROR_HPP
#define BABEL_ERROR_HPP
#include <string>

class Error : public std::exception {
public:
    explicit Error(const std::string  &message, const std::string &type = "Unknown");

    [[nodiscard]] std::string const &getType() const;
    [[nodiscard]] const char *what() const noexcept override;
    ~Error() override = default;
protected:
    std::string message;
    std::string type;
};

class TorchBearerError : public Error {
public:
    explicit TorchBearerError(std::string const &message, std::string const &type = "Unknown");
    ~TorchBearerError() override = default;
};

class ConnectionError : public Error {
public:
    explicit ConnectionError(std::string const &message, std::string const &type = "Unknown");
    ~ConnectionError() override = default;
};

class AudioError : public Error {
public:
    explicit AudioError(std::string const &message, std::string const &type = "Unknown");
    ~AudioError() override = default;
};

class CodecError : public Error {
public:
    explicit CodecError(const std::string  &message,
    std::string const &type = "Unknown");
    ~CodecError() override = default;
};

#endif //BABEL_ERROR_HPP