#include "server_message_sender.hpp"

#include "../common/types.hpp"

std::shared_ptr<ServerMessage> ServerMessageSender::dequeue() {
    std::unique_lock<std::mutex> lock(mutex);

    while(queue.empty()) {
        condition_variable.wait(lock);
    }

    std::shared_ptr<ServerMessage> message = queue.front();
    queue.pop();

    return message;
}

void ServerMessageSender::enqueue(const std::shared_ptr<ServerMessage>& message) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(message);
    condition_variable.notify_one();
}

[[noreturn]] void ServerMessageSender::send_messages() {
    while (true) {
        ByteList serialized_message = dequeue()->serialize();
        boost::asio::write(*socket, boost::asio::buffer(serialized_message));
    }
}

void ServerMessageSender::set_error() {
    std::lock_guard<std::mutex> lock(error_mutex);
    error = true;
}

bool ServerMessageSender::get_error() const {
    std::lock_guard<std::mutex> lock(error_mutex);
    return error;
}
