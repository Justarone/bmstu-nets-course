template <typename Executer>
bool MessageProcessor<Executer>::operator()(const std::vector<char>& data, const int fd, const std::unordered_set<int> & all_fds) {
    const std::string s(data.begin(), data.end());
    auto message = Message(s);
    switch (message.type) {
        case Message::Type::addChar:
            return processAddChar(message, fd, all_fds);
        case Message::Type::getAll:
            return processGetAll(fd);
        default:
            return false;
    }
    return false;
}

template <typename Executer>
bool MessageProcessor<Executer>::processAddChar(const Message & msg, const int fd, std::unordered_set<int> all_fds) {
    auto m = AddCharMessage(msg.data);
    mstate.setCurpos(m.pos);
    auto res = processor.process(imstate, mstate, m.data);

    std::string packet;
    if (res == CursesChProcessor::ActionType::Exec) {
        packet = msg.buildPacket();
    } else {
        auto command = mstate.getData();
        auto res = executer.execute(command);

        res.insert(res.begin(), "> " + command);
        for (auto & s : res)
            imstate.insertLine(s);

        auto diffMessage = DiffMessage(res);
        auto message = diffMessage.buildMessage();
        packet = message.buildPacket();
    }

    broadcastPacket(packet, all_fds);
    return true;
}

template <typename Executer>
bool MessageProcessor<Executer>::processGetAll(const int fd) {
    auto message = GetAllMessage(imstate, mstate);
    auto msg = message.buildMessage();
    auto packet = msg.buildPacket();
    if (send(fd, packet.data(), packet.size(), 0) == -1)
        std::cout << "Can't send data to client\n";
    return true;
}
