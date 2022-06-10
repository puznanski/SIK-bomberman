#ifndef ROBOTS_SERVER_MESSAGE_SENDER_HPP
#define ROBOTS_SERVER_MESSAGE_SENDER_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>
#include <boost/asio.hpp>
#include <utility>

#include "../common/models/server_message.hpp"

class ServerMessageSender {
public:
    explicit ServerMessageSender(std::shared_ptr<boost::asio::ip::tcp::socket> socket) : socket(socket) {};

private:
    std::queue<std::shared_ptr<ServerMessage>> queue;
    mutable std::mutex mutex;
    std::condition_variable condition_variable;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;

    bool error = false;
    mutable std::mutex error_mutex;

    std::shared_ptr<ServerMessage> dequeue();

public:
    void enqueue(const std::shared_ptr<ServerMessage>& message);
    [[noreturn]] void send_messages();

    void set_error();
    bool get_error() const;
};

#endif //ROBOTS_SERVER_MESSAGE_SENDER_HPP
