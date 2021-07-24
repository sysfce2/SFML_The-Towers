#include "PacketHandler.hpp"


void PacketHandler::init(NetworkManager* _manager, World* _world, ParticleHandler* _pHandler, EntityHandler* _eHandler){
	m_networkManager = _manager;
	m_world = _world;
	m_particleHandler = _pHandler;
	m_entityHandler = _eHandler;
}

void PacketHandler::handlePackets(){
	sf::Packet packet;
	while(m_networkManager->receiveGameUpdatePacket(packet)){
		// Getting packet operation code
		uint8_t code;
		packet >> code;
		// Doing different operations based on the packet code
		if(code == 1){ // A Player has Disconnected
			uint8_t id;
			packet >> id;
			std::cout << "Player: " + std::to_string(id) + " has disconnected" << std::endl;
			m_entityHandler->removeEntity(id);
		} else if (code == 2){ // Block Update
			int x = 0;
			int y = 0;
			int z = 0;
			uint8_t b = 0;
			packet >> x >> y >> z >> b;
			if (!b) {
				m_particleHandler->placeParticlesAroundBlock(x, y, z);
			}
			m_world->setBlock(x, y, z, b);
		}
	}
	while(m_networkManager->receiveEntityUpdatePacket(packet)){
		math::vec3 position;
		float pitch, yaw;
		uint8_t remoteID;
		packet >> remoteID >> position.x >> position.y >> position.z >> pitch >> yaw;
		m_entityHandler->updateEntity(remoteID, position, pitch, yaw);
	}
}