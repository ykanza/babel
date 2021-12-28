/*
** EPITECH PROJECT, 2021
** babel
** File description:
** TODO: add description
*/
#include "Exception/Error.hpp"

/*
 * Base class of error
 */

Error::Error(const std::string  &message, const std::string &whoami) {
    this->message = message;
    this->type = whoami;
}

const char *Error::what() const noexcept {
    return this->message.c_str();
}

std::string const &Error::getType() const {
    return this->type;
}

/*
 * TorchBearer error class
 */
TorchBearerError::TorchBearerError(const std::string &message, const std::string &whoami) : Error(message, whoami) {}

/*
 * Connecttion error class
 */

ConnectionError::ConnectionError(const std::string &message, const std::string &whoami) : Error(message, whoami) {}

/*
 * Audio error class
 */

AudioError::AudioError(const std::string &message, const std::string &whoami) : Error(message, whoami) {}

/*
 *  Codec class error
 */

CodecError::CodecError(const std::string  &message, std::string const &whoami) : Error(message, whoami) {}