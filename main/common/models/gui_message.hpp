#ifndef ROBOTS_CLIENT_GUI_MESSAGE_HPP
#define ROBOTS_CLIENT_GUI_MESSAGE_HPP

#include <memory>

#include "../types.hpp"
#include "../networking/udp_bytestream.hpp"
#include "client_message.hpp"

class GuiMessage {
public:
    virtual ~GuiMessage() = default;
    explicit GuiMessage(GuiMessageType gui_message_type) : gui_message_type(gui_message_type) {}

    GuiMessageType gui_message_type;

    virtual std::shared_ptr<ClientMessage> get_client_message() = 0;
};

class GuiMessagePlaceBomb : public GuiMessage {
public:
    GuiMessagePlaceBomb() : GuiMessage(GuiMessageType::PlaceBombGui) {}

    std::shared_ptr<ClientMessage> get_client_message() override;
};

class GuiMessagePlaceBlock : public GuiMessage {
public:
    GuiMessagePlaceBlock() : GuiMessage(GuiMessageType::PlaceBlockGui) {}

    std::shared_ptr<ClientMessage> get_client_message() override;
};

class GuiMessageMove : public GuiMessage {
public:
    explicit GuiMessageMove(Direction direction) : GuiMessage(GuiMessageType::MoveGui), direction(direction) {}
    explicit GuiMessageMove(UdpBytestream& bytestream);

private:
    Direction direction;

public:
    std::shared_ptr<ClientMessage> get_client_message() override;
};

#endif //ROBOTS_CLIENT_GUI_MESSAGE_HPP
