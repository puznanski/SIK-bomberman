#include "gui_message.hpp"

GuiMessageMove::GuiMessageMove(UdpBytestream &bytestream) : GuiMessage(GuiMessageType::MoveGui) {
    direction = static_cast<Direction>(bytestream.get_byte());
}

std::shared_ptr<ClientMessage> GuiMessagePlaceBomb::get_client_message() {
    return std::make_shared<ClientMessage>(ClientMessageType::PlaceBomb);
}

std::shared_ptr<ClientMessage> GuiMessagePlaceBlock::get_client_message() {
    return std::make_shared<ClientMessage>(ClientMessageType::PlaceBlock);
}

std::shared_ptr<ClientMessage> GuiMessageMove::get_client_message() {
    return std::make_shared<ClientMessageMove>(direction);
}